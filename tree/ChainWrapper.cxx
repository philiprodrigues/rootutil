#include "ChainWrapper.h"
#include "TChain.h"
#include "TList.h"
#include "TChainElement.h"
#include "TSystem.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>

#ifndef __CINT__
#include "glob.h"
#endif

ClassImp(ChainWrapper)

ChainWrapper::ChainWrapper(const char* name)
  : TreeWrapper(new TChain(name))
{

  wrappingChain=true;
}

int ChainWrapper::AddFile(const char* filename)
{
  fListOfFiles.push_back(filename);
  return ((TChain*)tree)->Add(filename);
}

//===========================================================================

int ChainWrapper::Add(const char* globStr, bool checkForBadPNFS)
{
  // Urgh, downcast
  TChain* ch=(TChain*)tree;

  glob_t g;
  glob(globStr, 0, 0, &g);
  for(int i=0; i<(int)g.gl_pathc; ++i){
    TString filename(g.gl_pathv[i]);
    if(checkForBadPNFS && filename.BeginsWith("/pnfs")){
      const char* basename=gSystem->BaseName(filename);
      const char* dirname=gSystem->DirName(filename);
      
      std::ifstream fin(TString::Format("%s/.(get)(%s)(locality)", dirname, basename));
      char status[256];
      fin.getline(status, 256);
      // std::cout << "  Status was " << status << std::endl;
      if(strcmp(status, "UNAVAILABLE")==0) continue;
    }
    if(filename.BeginsWith("/pnfs")){
      filename.ReplaceAll("/pnfs/minerva", "root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/minerva");
    }
    ch->Add(filename);
    fListOfFiles.push_back(filename.Data());
  }
  int ret=g.gl_pathc;

  globfree(&g);

  return ret;
}

// //===========================================================================
// void ChainWrapper::RemoveUnavailableFiles()
// {
//   // std::cout << "RemoveUnavailableFiles()" << std::endl;
//   // for(uint i=0; i<fListOfFiles.size(); ++i) std::cout << fListOfFiles[i] << std::endl << std::endl;
//   TObjArray* files=GetChain()->GetListOfFiles();
//   // std::cout << "Before loop, nentries=" << files->GetEntries() << std::endl;
//   TIter next(files);
//   TObject *obj;
//   while((obj = (TObject*)next())){     // iterator skips empty slots
//     TChainElement* elem=(TChainElement*)obj;
//     const char* filename=elem->GetTitle();
//     // std::cout << "  Trying " << filename << std::endl;
//     if(!TString(filename).BeginsWith("/pnfs")) continue;
//     const char* basename=gSystem->BaseName(filename);
//     const char* dirname=gSystem->DirName(filename);

//     std::ifstream fin(TString::Format("%s/.(get)(%s)(locality)", dirname, basename));
//     char status[256];
//     fin.getline(status, 256);
//     // std::cout << "  Status was " << status << std::endl;
//     if(strcmp(status, "UNAVAILABLE")==0){
//       // std::cout << "Removing " << filename << std::endl;
//       // std::cout << "Object has index " << files->IndexOf(obj) << std::endl;
//       std::vector<std::string>::iterator it=std::find(fListOfFiles.begin(), fListOfFiles.end(), std::string(filename));
//       // std::cout << "is it end? " << (it==fListOfFiles.end()) << std::endl;
//       fListOfFiles.erase(it);
//       files->Remove(obj);
//       delete obj;
//     }
//   }
//   // std::cout << "After loop, nentries=" << files->GetEntries() << std::endl;
//   // std::cout << "fListOfFiles.size()=" << fListOfFiles.size() << std::endl << std::endl;
// }
