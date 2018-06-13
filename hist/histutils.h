#ifndef HISTUTILS_H
#define HISTUTILS_H

#include "TH2.h"

#include "../util.h"

//===================================================================
//
// Get the area of the bin by global bin number in a 2D histogram
double getBinArea(TH2* h, int globalBin)
{
  int binx, biny, binz;
  h->GetBinXYZ(globalBin, binx, biny, binz);
  return h->GetXaxis()->GetBinWidth(binx)*h->GetYaxis()->GetBinWidth(biny);
}

//===================================================================
void normalizeColumns(TH2* h2D, bool useBinWidth=false, bool useBinHeight=false)
{
  for(int ix=1; ix<h2D->GetNbinsX()+1; ++ix){
    TH1* proj=h2D->ProjectionY(uniq(), ix, ix);
    proj->SetDirectory(0);
    double integral=proj->Integral();
    double scale=integral!=0 ? 1.0/integral : 1;

    double binWidth=useBinWidth ? h2D->GetXaxis()->GetBinWidth(ix) : 1;
    delete proj;
    for(int iy=1; iy<h2D->GetNbinsY()+1; ++iy){
      double oldBinContent=h2D->GetBinContent(ix, iy);
      double oldBinError=h2D->GetBinError(ix, iy);
      double binHeight=useBinHeight ? h2D->GetXaxis()->GetBinWidth(ix) : 1;

      double binArea=binWidth*binHeight;
      h2D->SetBinContent(ix, iy, oldBinContent*scale/binArea);
      h2D->SetBinError(ix, iy, oldBinError*scale/binArea);
    }
  }
}

//===================================================================
void normalizeRows(TH2* h2D, bool useBinWidth=false, bool useBinHeight=false)
{
  for(int iy=1; iy<h2D->GetNbinsY()+1; ++iy){
    TH1* proj=h2D->ProjectionX(uniq(), iy, iy);
    proj->SetDirectory(0);
    double integral=proj->Integral();
    double scale=integral!=0 ? 1.0/integral : 1;

    double binHeight=useBinHeight ? h2D->GetYaxis()->GetBinWidth(iy) : 1;
    delete proj;
    for(int ix=1; ix<h2D->GetNbinsX()+1; ++ix){
      double oldBinContent=h2D->GetBinContent(ix, iy);
      double oldBinError=h2D->GetBinError(ix, iy);
      double binWidth=useBinWidth ? h2D->GetXaxis()->GetBinWidth(ix) : 1;

      double binArea=binWidth*binHeight;
      h2D->SetBinContent(ix, iy, oldBinContent*scale/binArea);
      h2D->SetBinError(ix, iy, oldBinError*scale/binArea);
    }
  }
}


#endif
