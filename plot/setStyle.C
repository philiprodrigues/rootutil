#ifndef SETSTYLE_C
#define SETSTYLE_C

#include "TColor.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TGaxis.h"

void setRedPalette()
{
  const int NRGBs = 9, NCont = 999;
  gStyle->SetNumberContours(NCont);
  Double_t stops[NRGBs] = { 0.00, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875, 1.000};
  Double_t red[NRGBs]   = { 1.00, 1.00, 0.99, 0.99, 0.98, 0.94, 0.80, 0.65, 0.40 };
  Double_t green[NRGBs] = { 0.96, 0.88, 0.73, 0.57, 0.42, 0.23, 0.09, 0.06, 0.00 };
  Double_t blue[NRGBs]  = { 0.94, 0.82, 0.63, 0.45, 0.29, 0.17, 0.11, 0.08, 0.05 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
}

void setCorrelationPalette()
{
  // A colour palette that goes blue->white->red, useful for
  // correlation matrices
  const int NRGBs = 3, NCont = 999;
  gStyle->SetNumberContours(NCont);
  Double_t stops[NRGBs] = { 0.00, 0.50, 1.00};
  Double_t red[NRGBs]   = { 0.00, 1.00, 1.00};
  Double_t green[NRGBs] = { 0.00, 1.00, 0.00};
  Double_t blue[NRGBs]  = { 1.00, 1.00, 0.00};
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
}

void setStyle()
{
  TH1::AddDirectory(false);
  TH1::SetDefaultSumw2();
  TGaxis::SetMaxDigits(3);

  gStyle->SetTickLength(0.015, "XY");
  gStyle->SetNdivisions(505, "XY");
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.1);

  gStyle->SetTextSize(0.08);
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetTitleSize(0.06,"xyz");
  gStyle->SetTitleOffset(0.8, "Y");

  // TODO: It looks like Laura is using non-bold (43) for labels, but bold (63) for titles
  gStyle->SetLabelFont(43, "XYZ"); // The "3" means text size in pixels, so it's independent of the pad size
  gStyle->SetLegendFont(63);
  gStyle->SetTextFont(63);
  gStyle->SetTitleFont(63, "XYZ");

  gStyle->SetTextSize(25);
  gStyle->SetLabelSize(25,"xyz");
  gStyle->SetTitleSize(30,"xyz");

  setRedPalette();
}

void applyHistStyle(TH1* h)
{
  TAxis* xaxis=h->GetXaxis();
  xaxis->SetLabelFont(gStyle->GetLabelFont());
  xaxis->SetLabelSize(gStyle->GetLabelSize());
  xaxis->SetTitleFont(gStyle->GetTitleFont());
  xaxis->SetTitleSize(gStyle->GetTitleSize());
  xaxis->CenterTitle(true);

  TAxis* yaxis=h->GetYaxis();
  yaxis->SetLabelFont(gStyle->GetLabelFont());
  yaxis->SetLabelSize(gStyle->GetLabelSize());
  yaxis->SetTitleFont(gStyle->GetTitleFont());
  yaxis->SetTitleSize(gStyle->GetTitleSize());

}

void writeTitle(TCanvas* c, bool neutrino)
{
  const char* text=neutrino ? "MINER#nuA #bullet #nu Tracker #rightarrow CCQE" : "MINER#nuA #bullet #bar{#nu} Tracker #rightarrow CCQE";
  TLatex* la=new TLatex(0.5, 1.01-c->GetTopMargin(), text);
  la->SetNDC();
  la->SetTextFont(gStyle->GetTextFont());
  la->SetTextSize(gStyle->GetLabelSize());
  la->SetTextColor(kBlue);
  la->SetTextAlign(21);
  c->cd();
  la->Draw();
}

void writeAreaNorm(TCanvas* c)
{
  TLatex* la=new TLatex(0.99-c->GetRightMargin(), 0.99-c->GetTopMargin(), "Area normalized");
  la->SetNDC();
  la->SetTextFont(gStyle->GetLabelFont());
  la->SetTextSize(gStyle->GetLabelSize());
  la->SetTextAlign(33);
  c->cd();
  la->Draw();
}

#endif // include guard
