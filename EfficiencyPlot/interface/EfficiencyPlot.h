#ifndef EFFICIENCYPLOT_H
#define EFFICIENCYPLOT_H

#include "TriggerEfficiency/EfficiencyPlot/interface/TrgPlotMgr.h"
/*******************************************************************************
*   Global function
*******************************************************************************/
extern TrgPlotMgr& PlotMgr( const std::string& = "" );

/*******************************************************************************
*   PlotEfficiency
*******************************************************************************/
extern void                            PlotEff();

extern void                            MergeData();
extern void                            MergeMC();
extern void                            CleanSample();
extern void                            InitSetting(const std::string&, const std::string&);

extern TH1*                            HistTH1( const std::string&, const std::string& );
extern TH2*                            HistTH2( const std::string&, const std::string& );
extern TGraphAsymmErrors*              HistTGraph( const std::string&, const std::string& );
extern TEfficiency*                    HistTEff( const std::string&, const std::string& );

/*******************************************************************************
*   PlotEfficiency
*******************************************************************************/
extern void DrawEff();
extern void DrawPt(const std::string&);
extern void DrawEta(const std::string&);
extern void SetHistTitle(TH1*, const string&, const string&);
extern void SetBottomHist(TGraphAsymmErrors*);
extern void SetHist(
        TGraphAsymmErrors*,
        Color_t,
        int
        );
/******************************************************************************
*   PlotEfficiency
*******************************************************************************/
extern double ErrorProp(
        const double&,
        const double&,
        const double&,
        const double&
        );

extern TH1* MakeScale(TEfficiency*, TEfficiency*);
extern void  PlotSF();
/*******************************************************************************
*   PlotEfficiency
*******************************************************************************/
extern void FitsCheck();
extern void DrawZmass(const std::string&);
#endif
