#include "TriggerEfficiency/EfficiencyPlot/interface/TrgPlotMgr.h"

#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"

#include <iostream>

using namespace std;
using namespace dra;

TrgPlotMgr::TrgPlotMgr(const string& subdir):
    Pathmgr("TriggerEfficiency", subdir),
    Readmgr(SettingsDir() / "Trigger.json"),
    Parsermgr()
{
}

TrgPlotMgr::~TrgPlotMgr()
{
    for(auto& h : _TH1mgr){
        h.CleanAll();
    }

    for(auto& h : _TH2mgr){
        h.CleanAll();
    }

    for(auto& h : _TGraphmgr){
        h.CleanAll();
    }

}

string 
TrgPlotMgr::GetResultsName( const string& type, const string& prefix )
{
    string ans = OptName();

    if( prefix == "" ) {
        ans.erase( ans.begin() );
    }

    return ( ResultsDir() / ( prefix + ans + "." + type ) );
}

/*******************************************************************************/
void 
TrgPlotMgr::AddTH1(const std::string& tag, TH1* plot)
{
    
    bool find_mgr = false;
    for(auto& h : _TH1mgr){
        if(h.GetTag() == tag){
            h.AddObj(plot);
            find_mgr = true;
            break;
        }
    }

    if(!find_mgr){
        HistMgr<TH1> hist(tag);
        hist.AddObj(plot);
        _TH1mgr.emplace_back(hist);
    }
}

TH1*
TrgPlotMgr::GetTH1(const string& tag, const string& name)
{
    
    for(auto& h : _TH1mgr){
        if(h.GetTag() == tag){
            return h.GetObj(name);
        }
    }

    return NULL;

}

void 
TrgPlotMgr::SetColor()
{
    Color_t c[]  = {kGreen-6, kAzure-3, kOrange+1, kRed-7, kMagenta+2, kGray+1};
    int     ms[] = {33, 8, 21, 34, 22, 29};
    for(int i=0; i< (int)_TH1mgr.size();i++){
        _TH1mgr[i].SetLineColor(c[i]);
        _TH1mgr[i].SetMarkerColor(c[i]);
        _TH1mgr[i].SetMarkerStyle(ms[i]);
        _TH1mgr[i].SetLineWidth(2);
        _TH1mgr[i].SetMarkerSize(1);
    }
}

/*******************************************************************************/
void 
TrgPlotMgr::AddTH2(const std::string& tag, TH2* plot)
{
    bool find_mgr = false;
    for(auto& h : _TH2mgr){
        if(h.GetTag() == tag){
            h.AddObj(plot);
            find_mgr = true;
            break;
        }
    }

    if(!find_mgr){
        HistMgr<TH2> hist(tag);
        hist.AddObj(plot);
        _TH2mgr.emplace_back(hist);
    }
}

TH2*
TrgPlotMgr::GetTH2(const string& tag, const string& name)
{
    for(auto& h : _TH2mgr){
        if(h.GetTag() == tag){
            return h.GetObj(name);
        }
    }

    return NULL;
}

/*******************************************************************************/
void 
TrgPlotMgr::AddTGraph(const std::string& tag, TGraphAsymmErrors* plot)
{
    bool find_mgr = false;
    for(auto& h : _TGraphmgr){
        if(h.GetTag() == tag){
            h.AddObj(plot);
            find_mgr = true;
            break;
        }
    }

    if(!find_mgr){
        HistMgr<TGraphAsymmErrors> hist(tag);
        hist.AddObj(plot);
        _TGraphmgr.emplace_back(hist);
    }
}

TGraphAsymmErrors*
TrgPlotMgr::GetTGraph(const string& tag, const string& name)
{    
    for(auto& h : _TGraphmgr){
        if(h.GetTag() == tag){
            return h.GetObj(name);
        }
    }

    return NULL;

}

/*******************************************************************************/
void 
TrgPlotMgr::AddTEff(const std::string& tag, TEfficiency* plot)
{
    bool find_mgr = false;
    for(auto& h : _TEffmgr){
        if(h.GetTag() == tag){
            h.AddObj(plot);
            find_mgr = true;
            break;
        }
    }

    if(!find_mgr){
        HistMgr<TEfficiency> hist(tag);
        hist.AddObj(plot);
        _TEffmgr.emplace_back(hist);
    }
}

TEfficiency*
TrgPlotMgr::GetTEff(const string& tag, const string& name)
{    
    for(auto& h : _TEffmgr){
        if(h.GetTag() == tag){
            return h.GetObj(name);
        }
    }

    return NULL;

}
