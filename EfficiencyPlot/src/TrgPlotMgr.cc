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

    _histmgr.CleanAll();
}

void TrgPlotMgr::initPlot(){

    TFile* file = TFile::Open( GetSingleData<string>("inputfile").c_str() );
    TDirectory* dir = file->GetDirectory("demo");

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

        peff->SetName( ("pteff_"+trg  ).c_str() );
        eeff->SetName( ("etaeff_"+trg ).c_str() );

        _histmgr.AddObj(peff);
        _histmgr.AddObj(eeff);
    }

    file->Close();
}

TGraphAsymmErrors* TrgPlotMgr::GetHist(const string& name){
    return _histmgr.GetObj( name);
}
