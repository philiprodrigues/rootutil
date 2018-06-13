#ifndef UTIL_H
#define UTIL_H

#include "TString.h"
#include "TSystem.h"
#include "TStopwatch.h"

#include <iostream>

#include "sys/stat.h"
#include "sys/ioctl.h"

//======================================================================
template<class T>
T sqr(T x) { return x*x; }

//======================================================================
TString uniq()
{
  static int i=0;
  return TString::Format("uniq%d", i++);
}

//======================================================================
void dumpMemory()
{
  ProcInfo_t p;
  gSystem->GetProcInfo(&p);
  std::cout << (p.fMemResident/1000) << " " << (p.fMemVirtual/1000) << std::endl;
}

//======================================================================
// Show a snazzy progress bar and estimated time remaining.
// Stolen from MINOS NC::Utility::ReportProgress() by Backhouse
void reportProgress(double est_frac, TStopwatch& sw)
{
  // Check if we're outputing to a file. If so don't bother showing off
  // with the progress bar, it won't work.
  struct stat buf;
  fstat(fileno(stdout), &buf);
  const bool isFile = (buf.st_mode & S_IFREG) || (buf.st_mode & S_IFIFO);
 
  const double elapsed = sw.RealTime();
  sw.Start(kFALSE);
 
  if(est_frac < 0) est_frac = 0;
  if(est_frac > 1) est_frac = 1;
 
  const int remain = est_frac ? int(elapsed*(1/est_frac-1)) : 0;
 
  const TString txt = TString::
    Format("%d%% complete, approx %dh%dm%ds remain",
           int(100*est_frac), remain/3600, (remain/60)%60, remain%60);
 
  if(isFile){
    std::cout << txt << std::endl;
  }
  else{
    TString bar;
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);
    const int columns=ws.ws_col;
    const int extra = columns-txt.Length();
    const int before = extra/2;
    const int after = extra-before;
 
    string plain_bar;
    for(int n = 0; n < before; ++n) plain_bar += " ";
    plain_bar += txt;
    for(int n = 0; n < after; ++n) plain_bar += " ";
 
    const char kColor_Reset[]   = { 0x1B, '[', '0', 'm', 0 };
    const char kColor_BgRed[]   = { 0x1B, '[', '4', '1', 'm', 0 };
    const char kColor_BgGreen[] = { 0x1B, '[', '4', '2', 'm', 0 };
 
    int fracInt = int(est_frac*columns);
    if(fracInt < 0) fracInt = 0;
    if(fracInt > columns) fracInt = columns;
    bar += kColor_BgGreen;
    for(int n = 0; n < columns; ++n){
      if(n >= fracInt) bar += kColor_BgRed;
      bar += plain_bar[n];
    }
    bar += kColor_Reset;
 
    std::cout << bar <<  "\r" << std::flush;
  }
}

#endif
