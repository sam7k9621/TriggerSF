#ifndef TRGPLOTMGR_H
#define TRGPLOTMGR_H

#include "ManagerUtils/ArgHelper/interface/Common.hpp"
#include "ManagerUtils/ArgHelper/interface/Parsermgr.hpp"
#include "ManagerUtils/ArgHelper/interface/Pathmgr.hpp"
#include "ManagerUtils/ArgHelper/interface/Readmgr.hpp"
#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"

#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "TH2.h"

#include <string>
#include <vector>

class TrgPlotMgr : public dra::Pathmgr,
                   public dra::Readmgr,
                   public dra::Parsermgr {

    public:

        TrgPlotMgr( const std::string& );
        ~TrgPlotMgr();

        std::string     GetResultsName( const std::string&, const std::string& );
        /*******************************************************************************/
        TH1*           GetTH1( const std::string&, const std::string& );
        void           AddTH1( const std::string&, TH1* );
        void           SetColor();
        
        /*******************************************************************************/
        TGraphAsymmErrors* GetTGraph( const std::string&, const std::string& );
        void               AddTGraph( const std::string&, TGraphAsymmErrors* );
        
        /*******************************************************************************/
        TH2*           GetTH2( const std::string&, const std::string& );
        void           AddTH2( const std::string&, TH2* );

        /*******************************************************************************/
        TEfficiency*   GetTEff( const std::string&, const std::string& );
        void           AddTEff( const std::string&, TEfficiency* );


    private:

        std::vector<HistMgr<TH1> > _TH1mgr;
        std::vector<HistMgr<TH2> > _TH2mgr;
        std::vector<HistMgr<TGraphAsymmErrors> > _TGraphmgr;
        std::vector<HistMgr<TEfficiency> > _TEffmgr;
};

#endif
