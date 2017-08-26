#ifndef HIST_MGR
#define HIST_MGR

#include <map>
#include <memory>
#include <string>

template<typename T>
class HistMgr {

        typedef std::map<std::string, T* > ObjContainer;

    public:
        HistMgr() {}
        ~HistMgr() {}
        HistMgr( const HistMgr& )            = delete;
        HistMgr& operator=( const HistMgr& ) = delete;

        void AddObj( T* );
        void RemoveObj( const std::string& );
        T* GetObj( const std::string& );
        const T* GetObj( const std::string& ) const;
        void CleanAll();

    private:
        ObjContainer _objmap;
};

/*******************************************************************************
*   Including template class implementation
*******************************************************************************/
#include "TriggerEfficiency/EfficiencyPlot/src/HistMgr.ipp"

#endif
