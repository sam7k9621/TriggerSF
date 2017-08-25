#ifndef HIST_MGR
#define HIST_MGR

#include <map>
#include <memory>
#include <string>
#include "TH1.h"

class HistMgr {

        typedef std::map<std::string, TH1* > ObjContainer;

    public:
        HistMgr() {}
        ~HistMgr() {}
        HistMgr( const HistMgr& )            = delete;
        HistMgr& operator=( const HistMgr& ) = delete;

        void AddObj( TH1* );
        void RemoveObj( const std::string& );
        TH1* GetObj( const std::string& );
        const TH1* GetObj( const std::string& ) const;

    private:
        ObjContainer _objmap;
};


#endif
