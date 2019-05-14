#ifndef CHAINWRAPPER_H
#define CHAINWRAPPER_H

#include "TreeWrapper.h"
#include "TChain.h"

namespace PlotUtils {

    class ChainWrapper : public TreeWrapper
    {
      public:
        
        ChainWrapper(const char* name);
        
        virtual ~ChainWrapper() { delete m_tree; }
        
        int AddPlaylist(const char* file_path);

        int AddFiles(const char* file_path);

        int Add(const std::string file_path);

        TChain* GetChain() { return (TChain*)m_tree; }

        const std::vector<std::string>& GetListOfFiles() const {return fListOfFiles; }

        ClassDef(ChainWrapper, 0);

      private:
        std::vector<std::string> fListOfFiles;

        ChainWrapper() {}
        
    };
    
}
#endif
