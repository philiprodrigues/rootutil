// drawWithCutAxis.C: P. Rodrigues, May 2013

// Utilities for persuading ROOT to produce canvases where the y axis
// has a break in it: useful for seeing both a large peak and a small
// tail in the same plot, without using logarithmic axes. A massive
// hack, but I like it. See drawWithCutAxis() for something that
// resembles documentation. Someone more conscientious than me (hint,
// hint) should take this, tidy it up, and put it in PlotUtils or
// something
#ifndef DRAWWITHCUTAXIS_C
#define DRAWWITHCUTAXIS_C
#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TAxis.h"
#include "TGaxis.h"
#include "TBox.h"
#include "TLine.h"
#include "TObject.h"

#include "setStyle.C"

//======================================================================

// Create a canvas with two pads, one above the other, where the lower
// pad takes up a fraction ratioPadFraction of the total canvas height
TCanvas* ratioCanvas(double ratioPadFraction)
{
  TCanvas* c=new TCanvas;
  TPad *pad1 = new TPad("pad1", "foo", 0, 0, 1, 1);
  pad1->SetLeftMargin(0.15);
  pad1->SetNumber(1);
  pad1->SetFillStyle(4000);
  pad1->SetBottomMargin(ratioPadFraction);
  c->cd();
  pad1->Draw();

  TPad *pad2 = new TPad("pad2", "foo", 0, 0, 1, 1);
  pad2->SetNumber(2);
  pad2->SetFillStyle(4000);
  pad2->SetLeftMargin(0.15);
  pad2->SetTopMargin(1-ratioPadFraction);
  c->cd();
  pad2->Draw();

  return c;
}

//======================================================================

// Draw little caps on the y axis where the break is, to show that the
// axis stops there
void drawAxisBreak(double x, double y1, double y2)
{
  double lineLengthInNDC=0.013;
  double lineLengthInUser=lineLengthInNDC*(gPad->GetX2()-gPad->GetX1());
  TLine* l1=new TLine(x-lineLengthInUser, y1, x+lineLengthInUser, y1);
  l1->Draw();
  TLine* l2=new TLine(x-lineLengthInUser, y2, x+lineLengthInUser, y2);
  l2->Draw();
}

//======================================================================

// Add another histogram to the cut axis canvas c (which was created
// by drawWithCutAxis, or manually). The draw option to be used is
// given by "option", and "offset" is for doing funky things with
// multiple cut y axis plots in the same canvas, a la
// make_nubar_shared_axis_plots.C
void addToCutAxis(TPad* c, TObject* h, const char* option, int offset=0)
{
  c->cd(2+offset);
  TObject* objCopy1=h->Clone();
  objCopy1->Draw(option);
  gPad->RedrawAxis();

  c->cd(1+offset);
  TObject* objCopy2=h->Clone();
  objCopy2->Draw(option);
  gPad->RedrawAxis();
}

//======================================================================

// Draw a white box over the actual axis break region, to hide the
// join, and put a dashed line there to make it clear that there is
// funny business going on
void drawAxisBoxes(TPad* c)
{
  c->cd(2);
  // One or more of these causes the pad to update its coordinates
  gPad->Modified(); gPad->Update(); gPad->Draw();

  double xmin=gPad->GetUxmin();
  double xmax=gPad->GetUxmax();
  double maxY1=gPad->GetUymax();
  double minY1=gPad->GetUymin();

  double yrange1=maxY1-minY1;
  double breaky1=maxY1-0.05*yrange1;
  double breaky2=maxY1+0.05*yrange1;
  TBox* box1=new TBox(xmin-0.02*(xmax-xmin), breaky1, xmax+0.02*(xmax-xmin), breaky2);
  box1->SetFillColor(kWhite);
  box1->SetLineColor(kWhite);
  box1->SetFillStyle(1001);
  box1->Draw();
  TBox* box2=(TBox*)box1->Clone();
  box2->SetFillColor(kGray+2);
  box2->SetLineColor(kGray+2);
  box2->SetFillStyle(3244);
  //box2->Draw();

  TLine *l=new TLine(xmin, 0.5*(breaky1+breaky2), xmax, 0.5*(breaky1+breaky2));
  l->SetLineStyle(2);
  l->SetLineWidth(2);
  l->Draw();

  drawAxisBreak(xmin, breaky1, breaky2);
  drawAxisBreak(xmax, breaky1, breaky2);
}

//======================================================================

// Draw histogram h with a break in the y axis. Implementation is, of
// course, a horrible hack, in which the canvas is divided into two
// pads, and copies of the histogram are drawn in each pad. Returns
// the canvas (or pad) in which the histograms have been drawn. To
// draw multiple histograms in the same plot, use something like:
//
//   TH1* h1=new TH1[...]
//   TH1* h2=new TH1[...]
//   TH1* h3=new TH1[...]
//   TCanvas* c=drawWithCutAxis(h1, 0.1, 0, 100, 300, "hist");

//   addToCutAxis(c, h2, "hist same");
//   addToCutAxis(c, h3, "ep same");
//   drawAxisBoxes(c);

//
// Arguments:

// * h: the histogram to draw

// * bottomPadFrac: where the axis break appears on the pad, as a fraction of the pad height

// * minY1: the minimum value of y in the lower pad (ie, below the axis break)

// * maxY1: the maximum value of y in the lower pad (ie, below the axis break)

// * minY2: the minimum value of y in the upper pad (ie, above the
//   axis break). So the y region that's cut out is from maxY1 to
//   minY2. The maximum value of the upper pad is set automatically such
//   that the scales on the upper and lower pads are the same

// * option: the drawing option for the histogram

// * offset: the number of pads to offset by when drawing the
//   histogram. Useful in conjunction with the next option, when trying
//   to draw multiple cut axis plots on the same canvas with a shared y
//   axis (used in the CCQE PRLs). See
//   make_nubar_shared_axis_paper_plots.C for an example

// * c: A canvas with already-created pads on it, for doing funky
//   things as described in the previous argument

// * titleOffset: Set the y axis title offset
TPad* drawWithCutAxis(TH1* h, double bottomPadFrac,
                      double minY1, double maxY1, double minY2,
                      const char* option="hist", int offset=0,
                      TCanvas* c=0, double titleOffset=1.08)
{
  if(!c) c=ratioCanvas(bottomPadFrac);
  TPad* pad2=(TPad*)c->cd(2+offset);
  double pad2Height=1-pad2->GetTopMargin()-pad2->GetBottomMargin();
  TH1* hDrawnBottom=h->DrawCopy(option);
  hDrawnBottom->GetYaxis()->SetRangeUser(minY1, maxY1);
  hDrawnBottom->GetYaxis()->SetTitleSize(0);
  gPad->Modified(); gPad->Update(); gPad->Draw();
  TPad* pad1=(TPad*)c->cd(1+offset);
  double pad1Height=1-pad1->GetTopMargin()-pad1->GetBottomMargin();
  TH1* hDrawnTop=h->DrawCopy(option);
  hDrawnTop->GetYaxis()->SetTitleOffset(titleOffset);
  hDrawnTop->GetXaxis()->SetLabelSize(0);
  hDrawnTop->GetXaxis()->SetTitleSize(0);
  hDrawnTop->GetXaxis()->SetTickLength(0);
  gPad->Modified(); gPad->Update(); gPad->Draw();

  double xmin=gPad->GetUxmin();
  double xmax=gPad->GetUxmax();

  double yrange1=maxY1-minY1;
  double yrange2=yrange1*pad1Height/pad2Height;
  hDrawnTop->GetYaxis()->SetRangeUser(minY2, minY2+yrange2);

  double breaky1=maxY1-0.05*yrange1;
  double breaky2=maxY1+0.05*yrange1;

  hDrawnTop->GetXaxis()->SetTickLength(hDrawnBottom->GetYaxis()->GetTickLength()*bottomPadFrac/(1-bottomPadFrac));

  hDrawnBottom->GetYaxis()->SetNdivisions(505);
  hDrawnTop->GetYaxis()->SetNdivisions(505);

  drawAxisBoxes(c);
  drawAxisBreak(xmin, breaky1, breaky2);
  drawAxisBreak(xmax, breaky1, breaky2);

  return c;
}

#endif
