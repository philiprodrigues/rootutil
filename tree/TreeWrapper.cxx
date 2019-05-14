#define PR(x) cout << #x " = " << x << "\n";

#include <iostream>
#include <cstdlib>

#include <TChain.h>
#include <TBranchElement.h>
#include <TStreamerInfo.h>
#include <TVirtualCollectionProxy.h>

#include "TreeWrapper.h"

using std::cout;
using std::cerr;
using std::exit;


PlotUtils::TreeWrapper::TreeWrapper(TTree* t)
  : m_tree(t), m_wrappingChain(false)
{
  if(t->IsA()->InheritsFrom(TChain::Class())){
    m_wrappingChain=true;
    t->GetEntries();
    ((TChain*)t)->SetNotify(this);
  }
}

//======================================================================

double PlotUtils::TreeWrapper::GetValue(const char* branchName, Long64_t ientry, int leafVal)
{
  const LeafAndBranch& lab=FindLeafAndBranch(branchName);

  int localEntry;
  if(m_wrappingChain){
    localEntry=((TChain*)m_tree)->LoadTree(ientry);
  }
  else{
    localEntry=ientry;
  }

  // If we're trying to get a value from a variable sized array, we
  // need to make sure that the variable defining the size of the
  // array has been filled. The leafcount member of TLeaf is the leaf
  // that contains this number, so we fill it here. This is done by
  // TLeafI, but not TLeafF. I don't really understand why.
  if(lab.leaf->GetLeafCount())
    lab.leaf->GetLeafCount()->GetBranch()->GetEntry(localEntry);
  
  lab.branch->GetEntry(localEntry);
  return lab.leaf->GetValue(leafVal);
}

//======================================================================

Bool_t PlotUtils::TreeWrapper::Notify()
{
  SetBranchAddresses();
  return kTRUE;
}

