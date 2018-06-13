#ifndef CHAINWRAPPER_H
#define CHAINWRAPPER_H

#include "TreeWrapper.h"

#include <vector>
#include <string>

class TChain;

class ChainWrapper : public TreeWrapper
{
public:

  // Default ROOT constructor. Don't use
  ChainWrapper() {};

  ChainWrapper(const char* name);

  virtual ~ChainWrapper() { delete tree; }

  int Add(const char* glob, bool checkForBadPNFS=false);

  // Add a file directly. Need this for URLs where we shouldn't glob
  int AddFile(const char* file);

  TChain* GetChain() { return (TChain*)tree; }

  const std::vector<std::string>& GetListOfFiles() const {return fListOfFiles; }

  // Loop over the chain's list of files and remove those that are on
  // pnfs and considered "UNAVAILABLE" by dcache. This is useful for
  // when a small subset of files are b0rked in dcache and would hang
  // the job, but you just want to iterate over the rest
  // void RemoveUnavailableFiles();

protected:
  std::vector<std::string> fListOfFiles;

  ClassDef(ChainWrapper, 0);
};
#endif
