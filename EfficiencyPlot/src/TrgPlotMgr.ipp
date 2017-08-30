#include "TriggerEfficiency/EfficiencyPlot/interface/TrgPlotMgr.h"

#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"

#include <iostream>

using namespace std;
using namespace dra;

template<typename T>
TrgPlotMgr<T>::TrgPlotMgr(const string& subdir):
    Pathmgr("TriggerEfficiency", subdir),
    Readmgr(SettingsDir() / "Trigger.json"),
    Parsermgr()
{
}

template<typename T>
TrgPlotMgr<T>::~TrgPlotMgr(){

    for(auto& h : _histmgr){
        h.CleanAll();
    }
}

template<typename T>
void TrgPlotMgr<T>::SetColor(){

    Color_t c[] = {kGreen-6, kAzure-3, kOrange+1, kRed-7, kMagenta+2, kGray+1};

    for(int i=0; i< (int)_histmgr.size();i++){
        _histmgr[i].SetLineColor(c[i]);
    }
}

template<typename T>
void 
TrgPlotMgr<T>::AddPlot(const std::string& tag, T* plot){
    
    bool find_mgr = false;
    for(auto& h : _histmgr){
        if(h.GetTag() == tag){
            h.AddObj(plot);
            find_mgr = true;
            break;
        }
    }

    if(!find_mgr){
        HistMgr<T> hist(tag);
        hist.AddObj(plot);
        _histmgr.emplace_back(hist);
    }
}

template<typename T>
vector<T*> 
TrgPlotMgr<T>::GetHist(const string& name){
    
    vector<T*> histlst;
    for(auto& h : _histmgr){
        histlst.push_back( h.GetObj( name ) ); 
    }

    return histlst;
}

template<typename T>
string 
TrgPlotMgr<T>::GetResultsName( const string& type, const string& prefix ){
    string ans = OptName();

    if( prefix == "" ) {
        ans.erase( ans.begin() );
    }

    return ( ResultsDir() / ( prefix + ans + "." + type ) );
}

