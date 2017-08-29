#include "TriggerEfficiency/EfficiencyPlot/interface/TrgPlotMgr.h"

#include "TH1.h"
#include "TFile.h"
#include "TDirectory.h"

using namespace std;
using namespace dra;

TrgPlotMgr::TrgPlotMgr(const string& subdir):
    Pathmgr("TriggerEfficiency", subdir),
    Readmgr(SettingsDir() / "Trigger.json"),
    Parsermgr()
{
}

TrgPlotMgr::~TrgPlotMgr(){

    for(auto& h : _histmgr){
        h.CleanAll();
    }
}

void TrgPlotMgr::SetColor(){

    Color_t c[] = {kGreen-6, kAzure-3, kOrange+1, kRed-7, kMagenta+2, kGray+1};

    for(int i=0; i< (int)_histmgr.size();i++){
        _histmgr[i].SetLineColor(c[i]);
    }
}

void TrgPlotMgr::initPlot(){

    for(const auto& ver : GetListData<string>("verlst")){
        TFile* file = TFile::Open( GetSingleData<string>(ver).c_str() );
        TDirectory* dir = file->GetDirectory("demo");

        HistMgr<TGraphAsymmErrors> hist(ver);

        for(const auto& trg : GetListData<string>("trglst")){
            
            TH1D* passPt;
            TH1D* passEta;
            TH1D* totalPt;
            TH1D* totalEta;
            
            dir->GetObject( ("pass_pt_" + trg)  .c_str(), passPt  );
            dir->GetObject( ("pass_eta_" + trg) .c_str(), passEta );
            dir->GetObject( ("total_pt_" + trg) .c_str(), totalPt );
            dir->GetObject( ("total_eta_" + trg).c_str(), totalEta);

            TGraphAsymmErrors* peff = new TGraphAsymmErrors( passPt , totalPt , "b" ); 
            TGraphAsymmErrors* eeff = new TGraphAsymmErrors( passEta, totalEta, "b" );

            peff->SetName( ("pteff_"  + trg ).c_str() );
            eeff->SetName( ("etaeff_" + trg ).c_str() );

            hist.AddObj(peff);
            hist.AddObj(eeff);
        }
        _histmgr.emplace_back(hist);
        file->Close();
    }

}

vector<TGraphAsymmErrors*> 
TrgPlotMgr::GetHist(const string& name){
    
    vector<TGraphAsymmErrors*> histlst;
    for(auto& h : _histmgr){
        histlst.push_back( h.GetObj( name ) ); 
    }

    return histlst;
}
