#ifndef EFFICIENCYPLOT_H
#define EFFICIENCYPLOT_H

#include "TriggerEfficiency/EfficiencyPlot/interface/TrgPlotMgr.h"
/*******************************************************************************
*   Global function
*******************************************************************************/

extern TrgPlotMgr<TGraphAsymmErrors>& PlotMgr( const std::string& = "" );

/*******************************************************************************
*   PlotEfficiency
*******************************************************************************/
extern void                            PlotEff();
extern void                            InitSetting();
extern void                            DrawEff();
extern void                            DrawEta( const std::string& );
extern void                            DrawPt( const std::string& );
extern void                            SetHist( TH1*, const std::string&, const std::string& );
extern std::vector<TGraphAsymmErrors*> Hist( const std::string& );

#endif
