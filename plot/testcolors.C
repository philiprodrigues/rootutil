#ifndef __CINT__
#include "plot.h"
#include "../util.h"

#include "TH1.h"
#include "TRandom.h"
#include "TCanvas.h"

void fillHist(TH1* h, double offset)
{
  for(int i=0; i<100000; ++i){
    h->Fill(gRandom->Gaus()+offset);
  }
}

#endif // __CINT__
void testcolors()
{
#ifndef __CINT__
  for(int iPal=0; iPal<6; ++iPal){
    TCanvas* c=new TCanvas(uniq(), "sdlk", 1000, 500);
    c->SetRightMargin(0.02);
    c->SetLeftMargin(0.05);
    vector<int> colours=getColors(iPal);
    for(int iCol=0; iCol<colours.size(); ++iCol){
      const double xmax=100;
      TH1* h=new TH1D(uniq(), "", 1000, -1, xmax);
      fillHist(h, 1+xmax*iCol/colours.size()+0.1*gRandom->Rndm());
      // h->SetLineColor(colours[iCol]);
      h->Draw(iCol==0 ? "hist l" : "hist l same");
    }
    autoColorHists(c, iPal);
    reMax(c);
    c->Print(TString::Format("color-test-palette%d.eps", iPal));
    c->Print(TString::Format("color-test-palette%d.png", iPal));
  }
#endif // __CINT__
}
