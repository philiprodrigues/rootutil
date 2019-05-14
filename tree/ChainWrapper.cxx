#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ChainWrapper.h"
#include "glob.h"

PlotUtils::ChainWrapper::ChainWrapper(const char* name)
  : TreeWrapper(new TChain(name))
{
  m_wrappingChain=true;
}

//===========================================================================

int PlotUtils::ChainWrapper::AddFiles(const char* name)
{
  // Urgh, downcast
  //TChain* ch=(TChain*)tree;
  TChain* ch= dynamic_cast<TChain*>(m_tree);

  glob_t g;
  glob(name, 0, 0, &g);

  if ((int)g.gl_pathc == 0){
    std::cerr << "File not found: " << name << std::endl;
    exit(1);
  }

  for(int i=0; i<(int)g.gl_pathc; ++i){
    const char* filename=g.gl_pathv[i];
    ch->Add(filename);
    fListOfFiles.push_back(filename);
  }
  int ret=g.gl_pathc;

  globfree(&g);

  return ret;
}

//===========================================================================

int PlotUtils::ChainWrapper::AddPlaylist(const char* name)
{
  // Urgh, downcast
  //TChain* ch=(TChain*)tree;
  TChain* ch= dynamic_cast<TChain*>(m_tree);

  std::ifstream input_stream(name);

  if (!input_stream.is_open()) {
      std::cerr << "File not found: " << name << std::endl;
      exit(1);
  }
  
  int nfiles = 0;

  while (!input_stream.eof()) {

      std::string item;
      input_stream >> item;

      if (input_stream.eof()) break;

      std::cout << "\tadding file: " << item << std::endl;
      nfiles += ch->Add(item.c_str());
  }

  return nfiles;
}

//===========================================================================

int PlotUtils::ChainWrapper::Add(std::string name){
  if (name.find(".root") != std::string::npos) 
    return PlotUtils::ChainWrapper::AddFiles(name.c_str());
  else return PlotUtils::ChainWrapper::AddPlaylist(name.c_str());
}
