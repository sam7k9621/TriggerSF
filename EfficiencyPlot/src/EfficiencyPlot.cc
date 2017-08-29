#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TH1.h"
using namespace std;
/*******************************************************************************
*   Global function
*******************************************************************************/

extern string GetResultsName( const string& type, const string& prefix ){
    string ans = PlotMgr().OptName();

    if( prefix == "" ) {
        ans.erase( ans.begin() );
    }

    return ( PlotMgr().ResultsDir() / ( prefix + ans + "." + type ) );
}

extern TrgPlotMgr& PlotMgr(const string& subdir){
    static TrgPlotMgr mgr( subdir );
    return mgr;
}

/*******************************************************************************
*   PlotEfficiency
*******************************************************************************/
extern void PlotEff(){

    InitSetting();
    DrawEff();

}

extern void InitSetting(){
    PlotMgr().initPlot();
    PlotMgr().SetColor();
}

extern void DrawEff(){
    
    for( const auto& trg : PlotMgr().GetListData<string>("trglst") ) {
        DrawEta(trg);
        DrawPt(trg);
    }
}

extern void DrawEta(const string& trg){

    string trgname = dra::GetSingle<string>("trgname", PlotMgr().GetSubTree(trg) );
    string cut     = dra::GetSingle<string>("pcut"   , PlotMgr().GetSubTree(trg) );
    
    
    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad(c);

    TH1F*    h = gPad->DrawFrame(-3,0,3,1.2);
    SetHist( h, "#eta", "Efficiency");
    mgr::SetAxis( h );
   
    TLegend* leg = mgr::NewLegend( 0.55, 0.2, 0.65, 0.3 );
    leg->SetLineColor( kWhite );
    
    for( const auto& ht : Hist("etaeff_"+trg) ){
        ht->Draw("EP");
        leg->AddEntry(ht, ht->GetTitle(), "lp");
    }
    leg->Draw();
    
    mgr::DrawCMSLabelOuter( PRELIMINARY );

    TPaveText *pt = mgr::NewTextBox( .45, .79, .93, .87 );
    pt->AddText( (trgname+cut).c_str() );
    pt->Draw();

    TLine* line = new TLine( -3, 1, 3, 1 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();

    //mgr::SaveToROOT( c, GetResultsName( "root", "eff"), "Eta_Eff_"+trg );
    mgr::SaveToPDF( c, GetResultsName( "pdf", "Eta_Eff_"+trg ) );
    
    delete c;
    delete pt;
    delete line;
    delete leg;
}

extern void DrawPt(const string& trg){

    string trgname = dra::GetSingle<string>("trgname", PlotMgr().GetSubTree(trg) );
    string cut     = dra::GetSingle<string>("ecut"   , PlotMgr().GetSubTree(trg) );
    
    
    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad(c);

    TH1F*    h = gPad->DrawFrame(0, 0, 200, 1.2);
    SetHist( h, "P_{T}", "Efficiency");
    mgr::SetAxis( h );
    
    TLegend* leg = mgr::NewLegend( 0.65, 0.3, 0.75, 0.4 );
    leg->SetLineColor( kWhite );
   
    for( const auto& ht : Hist("pteff_"+trg) ){
        ht->Draw("EP");
        leg->AddEntry(ht, ht->GetTitle(), "lp");
    }
    leg->Draw();
    
    mgr::DrawCMSLabelOuter( PRELIMINARY );

    TPaveText *pt = mgr::NewTextBox( .55, .81, .93, .87 );
    pt->AddText( (trgname+cut).c_str() );
    pt->Draw();

    TLine* line = new TLine( 0, 1, 200, 1 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();
    
    //mgr::SaveToROOT( c, GetResultsName( "root", "eff"), "Pt_Eff_"+trg );
    mgr::SaveToPDF( c, GetResultsName( "pdf", "Pt_Eff_"+trg ) ); 
    
    delete c;
    delete pt;
    delete line;
    delete leg;
}

extern void SetHist(TH1* h, const string& xaxis, const string& yaxis){
 
    h->GetXaxis()->SetTitle(xaxis.c_str());
    h->GetYaxis()->SetTitle(yaxis.c_str());

    h->SetMarkerStyle(21);
    h->SetMarkerSize(0.6);
    h->SetLineColor(kAzure - 3);
    h->SetMarkerColor(kAzure - 3);
}

extern vector<TGraphAsymmErrors*> Hist(const string& name){
    return PlotMgr().GetHist(name);
}
