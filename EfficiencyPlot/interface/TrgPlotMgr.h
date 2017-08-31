#ifndef TRGPLOTMGR_H
#define TRGPLOTMGR_H

#include "ManagerUtils/ArgHelper/interface/Common.hpp"
#include "ManagerUtils/ArgHelper/interface/Parsermgr.hpp"
#include "ManagerUtils/ArgHelper/interface/Pathmgr.hpp"
#include "ManagerUtils/ArgHelper/interface/Readmgr.hpp"
#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"

#include "TGraphAsymmErrors.h"

#include <string>
#include <vector>

template<typename T>
class TrgPlotMgr : public dra::Pathmgr,
                   public dra::Readmgr,
                   public dra::Parsermgr {

    public:

        TrgPlotMgr( const std::string& );
        ~TrgPlotMgr();

        void            SetColor();
        std::vector<T*> GetHist( const std::string& );
        void            AddPlot( const std::string&, T* );
        std::string     GetResultsName( const std::string&, const std::string& );

    private:

        std::vector<HistMgr<T> > _histmgr;


};

#include "TriggerEfficiency/EfficiencyPlot/src/TrgPlotMgr.ipp"

#endif
