#ifndef HIST_MGR
#define HIST_MGR

#include <map>
#include <memory>
#include "TColor.h"
#include <string>

template<typename T>
class HistMgr {

        typedef std::map<std::string, T* > ObjContainer;

    public:
        HistMgr(const std::string&);
        HistMgr();
        ~HistMgr() {}

        void AddObj( T* );
        void RemoveObj( const std::string& );
        T* GetObj( const std::string& );
        const T* GetObj( const std::string& ) const;
        std::string MakeAliasName(const std::string&);
        void SetLineColor( const Color_t );
        void CleanAll();
        std::string GetTag() const {
            return _tag;
        }

    private:
        ObjContainer _objmap;
        std::string  _tag;
};

/*******************************************************************************
*   Including template class implementation
*******************************************************************************/
#include "TriggerEfficiency/EfficiencyPlot/src/HistMgr.ipp"

#endif
