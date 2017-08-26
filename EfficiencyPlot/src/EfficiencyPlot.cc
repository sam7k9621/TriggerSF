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
}

extern void DrawEff(){
    
    for( const auto& trg : PlotMgr().GetListData<string>("trglst") ) {
        DrawEta(trg);
        DrawPt(trg);
    }
}

extern void DrawEta(const string& trg){

    string triname = dra::GetSingle<string>("triname", PlotMgr().GetSubTree(trg) );
    string cut     = dra::GetSingle<string>("pcut"   , PlotMgr().GetSubTree(trg) );
    
    
    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad(c);

    TH1F*    h = gPad->DrawFrame(-3,0,3,1.2);
    SetHist( h, "#eta", "efficiency");
    mgr::SetAxis( h );
    
    Hist("etaeff"+trg)->Draw("EP");

    mgr::DrawCMSLabelOuter( PRELIMINARY );

    if( PlotMgr().GetOption<string>( "source" ) != "mc" ) {
        mgr::DrawLuminosity( 36811 );
    }

    TPaveText *pt = mgr::NewTextBox( .73, .81, .93, .87 );
    pt->AddText( PlotMgr().GetOption<string>( "lepton" ).c_str() );
    pt->Draw();

    TLine* line = new TLine( -3, 1, 3, 1 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();

    TLegend* leg = mgr::NewLegend( 0.65, 0.4, 0.75, 0.7 );
    leg->SetLineColor( kWhite );
    leg->SetHeader( trg.c_str(), "C");
    leg->AddEntry( Hist("etaeff"+trg), triname.c_str(), "lp");

    mgr::SaveToROOT( c, GetResultsName( "root", "eff"), "Eta_Eff_"+trg );

    delete c;
    delete pt;
    delete line;
    delete leg;
}

extern void DrawPt(const string& trg){

    string triname = dra::GetSingle<string>("triname", PlotMgr().GetSubTree(trg) );
    string cut     = dra::GetSingle<string>("ecut"   , PlotMgr().GetSubTree(trg) );
    
    
    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad(c);

    TH1F*    h = gPad->DrawFrame(0, 0, 200, 1.2);
    SetHist( h, "P_{T}", "efficiency");
    mgr::SetAxis( h );
    
    Hist("pteff"+trg)->Draw("EP");

    mgr::DrawCMSLabelOuter( PRELIMINARY );

    if( PlotMgr().GetOption<string>( "source" ) != "mc" ) {
        mgr::DrawLuminosity( 36811 );
    }

    TPaveText *pt = mgr::NewTextBox( .73, .81, .93, .87 );
    pt->AddText( PlotMgr().GetOption<string>( "lepton" ).c_str() );
    pt->Draw();

    TLine* line = new TLine( 0, 1, 200, 1 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();

    TLegend* leg = mgr::NewLegend( 0.65, 0.4, 0.75, 0.7 );
    leg->SetLineColor( kWhite );
    leg->SetHeader( trg.c_str(), "C");
    leg->AddEntry( Hist("pteff"+trg), triname.c_str(), "lp");

    mgr::SaveToROOT( c, GetResultsName( "root", "eff"), "Pt_Eff_"+trg );
    
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

extern TGraphAsymmErrors* Hist(const string& name){
    return PlotMgr().GetHist(name);
}
