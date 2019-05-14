#ifndef TREEWRAPPER_H
#define TREEWRAPPER_H

#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"

#include <map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

struct LeafAndBranch
{
  std::string name;
  TLeaf* leaf;
  TBranch* branch;
};

/**
   Typical use:

   TTree *t = (TTree*)somefile.Get("sometree");
   TreeWrapper w(t);
   double energy=w.GetValue("evt.energy", 1);

*/

namespace PlotUtils {

  class TreeWrapper : public TObject {
  public:
    /**
       Create a TreeWrapper for TTree t. TChain derives from TTree so
       can pass a TChain here too. On second thoughts that might not
       work. Who knows?
    */
    TreeWrapper(TTree* t);
        
    virtual ~TreeWrapper() {}
    /**
       Get the value of branch \a branchName in tree entry \a ientry.
       Use non-zero \a leafVal for leaves that are arrays
    */
    virtual double GetValue(const char* branchName, Long64_t ientry, int leafVal=0);
    virtual double GetValue(const string& branchName, Long64_t ientry, int leafVal=0)
    {
      return GetValue(branchName.c_str(), ientry, leafVal);
    }
        
    /**
       Number of entries in the tree
    */
    virtual Long64_t GetEntries() const { return m_tree->GetEntries(); }
        
    virtual TTree* GetTree() const { return m_tree; }
        
    virtual int GetInt(const string& branchName, Long64_t ientry, int leafVal=0)
    {
      return (int)GetValue(branchName, ientry, leafVal);
    }
 
    virtual int GetInt(const char* branchName, Long64_t ientry, int leafVal=0)
    {
      return (int)GetValue(branchName, ientry, leafVal);
    }
        
    virtual bool GetBool(const string& branchName, Long64_t ientry, int leafVal=0)
    {
      return (bool)GetValue(branchName, ientry, leafVal);
    }

    virtual bool GetBool(const char* branchName, Long64_t ientry, int leafVal=0)
    {
      return (bool)GetValue(branchName, ientry, leafVal);
    }

    template<class T>
    T* GetValueArray(const string& branchName, Long64_t ientry, int* N=NULL)
    {
      return GetValueArray<T>(branchName.c_str(), ientry, N);
    }

    template<class T>
    T* GetValueArray(const char* branchName, Long64_t ientry, int* N=NULL)
    {
      GetValue(branchName, ientry);
      const LeafAndBranch& lb=FindLeafAndBranch(branchName);
      if(N) *N=lb.leaf->GetLen();
      return (T*)lb.leaf->GetValuePointer();
    }

    template<class T>
    vector<T> GetValueVector(const string& branchName, Long64_t ientry)
    {
      return GetValueVector<T>(branchName.c_str(), ientry);
    }

    template<class T>
    vector<T> GetValueVector(const char* branchName, Long64_t ientry)
    {
      GetValue(branchName, ientry);
      const LeafAndBranch& lb=FindLeafAndBranch(branchName);
      int N=lb.leaf->GetLen();
      T* d=(T*)lb.leaf->GetValuePointer();
      vector<T> ret(d, d+N);
      return ret;
    }
        
  protected:

    // Find the leaf and branch for the given name. Get it out of our
    // list if we already have it; otherwise, add it
    virtual const LeafAndBranch& FindLeafAndBranch(const char* branchName)
    {
      const LeafAndBranch* lab=0;
      // First look through our existing list
      for (std::vector<LeafAndBranch>::const_iterator it=m_leavesAndBranches.begin();
           it!=m_leavesAndBranches.end(); ++it){
        if(strcmp(it->name.c_str(), branchName)==0){
          lab=&(*it);
          break;
        }
      }
      if(!lab){
        // We don't have the branch in our list yet, so find it in the
        // tree and add to our list
        TBranch* b=m_tree->FindBranch(branchName);
        if (!b) b=m_tree->GetBranch(branchName);
        TLeaf* l=m_tree->FindLeaf(branchName);
        if (!l) l=m_tree->GetLeaf(branchName);
        if (!b) {
          std::cerr << "Can't find branch " << branchName << std::endl;
          exit(1);
        }
        if (!l) {
          std::cerr << "Can't find leaf " << branchName << std::endl;
          exit(1);
        }
        m_tree->SetBranchStatus(branchName, 1);
          
        LeafAndBranch lb;
        lb.name = branchName;
        lb.leaf = l;
        lb.branch = b;
        // Put the new branch at the end of the list. This has the
        // nice effect that branches we access earlier will be at the
        // start of the list, and be found more quickly in the linear
        // sarch at the top of this function. In a typical analysis
        // macro, the branches we call first are the ones we call on
        // all events, so this should be ~optimal, speed-wise
        m_leavesAndBranches.push_back(lb);
        lab=&m_leavesAndBranches.back();
      }
      return *lab;
    }

    /**
       Reset the branch addresses because we're on a new TTree (if wrapping a chain)
    */
    virtual bool SetBranchAddresses()
    {
      for (std::vector<LeafAndBranch>::iterator it=m_leavesAndBranches.begin();
           it!=m_leavesAndBranches.end(); ++it){
        TBranch* b=m_tree->FindBranch(it->name.c_str());
        if (!b) b=m_tree->GetBranch(it->name.c_str());
        TLeaf* l=m_tree->FindLeaf(it->name.c_str());
        if (!l) l=m_tree->GetLeaf(it->name.c_str());
        if (!b) {
          std::cerr << "Can't find branch " << it->name << std::endl;
          return false;
        }
        if (!l) {
          std::cerr << "Can't find leaf " << it->name << std::endl;
          return false;
        }
        m_tree->SetBranchStatus(it->name.c_str(), 1);

        it->leaf = l;
        it->branch = b;
      }
      return true;
    }

    TTree* m_tree;
    // The list of leaves+branches that we've accessed so far. Naively
    // (and in the original implementation of this class), you would
    // want a map here for O(log N) lookup and fast insertion, but it
    // seems to be faster to just store a list and do linear
    // search. Some possible reasons for this:
    //
    // * With a list, we can have GetValue(const char* ...) and avoid
    //   a std::string ctor for every GetValue() call
    // 
    // * Linear search means fewer branch misses(?)
    //
    // * We add entries to the end of the list, which means
    //   earlier-called entries are nearer the beginning. In typical
    //   analysis macros, the earlier-called branches are probably the
    //   ones that are read for the largest number of events, so
    //   putting them near the front of the list is optimal for linear
    //   search
    std::vector<LeafAndBranch> m_leavesAndBranches;
    // Are we wrapping a TChain?
    bool m_wrappingChain;
    /**
       Reset the branch addresses because we're on a new TTree (if wrapping a chain)
    */
    virtual Bool_t Notify();

    ClassDef(TreeWrapper, 0);

    // Stupid default constructor forced by ROOT.
    TreeWrapper() {}


  };

}

#endif
