#ifndef TRGPLOTMGR_H
#define TRGPLOTMGR_H

#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"
#include "ManagerUtils/ArgHelper/interface/Common.hpp"
#include "ManagerUtils/ArgHelper/interface/Parsermgr.hpp"
#include "ManagerUtils/ArgHelper/interface/Pathmgr.hpp"
#include "ManagerUtils/ArgHelper/interface/Readmgr.hpp"

#include "TGraphAsymmErrors.h"

#include <string>
#include <vector>

class TrgPlotMgr : public dra::Pathmgr, public dra::Readmgr, public dra::Parsermgr{

    public:
        TrgPlotMgr(const std::string&);
        ~TrgPlotMgr();

        void initPlot();
        void SetColor();
        std::vector<TGraphAsymmErrors*> GetHist(const std::string&);
        

    private:
        std::vector< HistMgr<TGraphAsymmErrors> > _histmgr;


};

#endif
