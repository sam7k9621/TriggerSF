#ifndef FITSCHECK_H
#define FITSCHECK_H

#include "TriggerEfficiency/EfficiencyPlot/interface/TrgPlotMgr.h"
#include "TH1D.h"

extern TrgPlotMgr<TH1>& FitsMgr(const std::string& = "");
extern void PlotFits();
extern void InitSetting_fit();
extern void DrawFits();
extern void DrawFitsPass(const std::string&);
extern void DrawFitsFail(const std::string&);
extern std::vector<TH1*> HIST(const std::string&);

#endif
