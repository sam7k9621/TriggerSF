#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"


using namespace std;

extern void
DrawEff()
{
    for( const auto& trg : PlotMgr().GetListData<string>( "trglst" ) ){
        
        DrawEta( trg );
        DrawPt( trg );
    }
}

extern void
DrawEta(const string& trg)
{
    string trgname = dra::GetSingle<string>( "trgname", PlotMgr().GetSubTree( trg ) );
    string cut     = dra::GetSingle<string>( "pcut", PlotMgr().GetSubTree( trg ) );
    
    TCanvas* c   = mgr::NewCanvas();

    TPad* top = mgr::NewTopPad();
    top->Draw();
    top->cd();
    
    TH1F* h1 = gPad->DrawFrame( -3, 0, 3, 1.2 );
    SetHistTitle( h1, "", "Efficiency" );
    mgr::SetTopPlotAxis( h1 );

    TLegend* leg = mgr::NewLegend( 0.55, 0.18, 0.65, 0.35 );
    leg->SetLineColor( kWhite );

    TGraphAsymmErrors* data = HistTEff( "data" + trg, "eff_eta" )->CreateGraph();
    TGraphAsymmErrors* mc   = HistTEff( "mc"   + trg, "eff_eta" )->CreateGraph();
    data->Draw( "EP" );
    mc->Draw("EP");

    SetHist(data, kGray+1,  33);
    SetHist(mc,   kAzure-3, 8);
    
    leg->AddEntry( data, PlotMgr().GetOption<string>( "source" ).c_str(), "lp" );
    leg->AddEntry( mc,   "MC", "lp" );
    leg->Draw();

    TPaveText* pt = mgr::NewTextBox( .45, .76, .93, .84 );
    pt->AddText( ( trgname + cut ).c_str() );
    pt->Draw();

    TLine* line1 = new TLine( -3, 1, 3, 1 );
    line1->SetLineColor( kRed );
    line1->SetLineStyle( 8 );
    line1->Draw();

    c->cd();
    /**************************************************************************/
    TPad* bot = mgr::NewBottomPad();
    bot->Draw();
    bot->cd();

    TH1F* h2 = gPad->DrawFrame( -3, 0.9, 3, 1.1 );
    SetHistTitle( h2, "SuperCluster #eta", "Data/Mc" );
    mgr::SetBottomPlotAxis( h2 );
    h2->GetYaxis()->SetLabelSize(14);
    
    TGraphAsymmErrors* scale = DividedGraph(data, mc);
    scale->Draw("EP");
    SetBottomHist(scale);

    TLine* line2 = new TLine( -3, 1, 3, 1 );
    line2->SetLineColor( kRed );
    line2->SetLineStyle( 8 );
    line2->Draw();

    c->cd();
    mgr::DrawCMSLabelOuter( PRELIMINARY );
    mgr::DrawLuminosity( PlotMgr().GetOption<double>( "lumi" ) );
    mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", "Eta_Eff_" + trg ) );
    mgr::SaveToROOT( scale, PlotMgr().GetResultsName( "root", "SF_" + trg ), "eta" );

    delete c;
    delete line1;
    delete line2;
    delete leg;
    delete pt;
}

extern void
DrawPt( const string& trg )
{
    string trgname = dra::GetSingle<string>( "trgname", PlotMgr().GetSubTree( trg ) );
    string cut     = dra::GetSingle<string>( "ecut", PlotMgr().GetSubTree( trg ) );

    TCanvas* c = mgr::NewCanvas();

    TPad* top = mgr::NewTopPad();
    top->Draw();
    top->cd();
    
    TH1F* h1 = gPad->DrawFrame( 0, 0, 500, 1.2 );
    SetHistTitle( h1, "", "Efficiency" );
    mgr::SetTopPlotAxis( h1 );

    TLegend* leg = mgr::NewLegend( 0.65, 0.23, 0.75, 0.4 );
    leg->SetLineColor( kWhite );

    TGraphAsymmErrors* data = HistTEff( "data" + trg, "eff_pt" )->CreateGraph();
    TGraphAsymmErrors* mc   = HistTEff( "mc"   + trg, "eff_pt" )->CreateGraph();
    data->Draw( "EP" );
    mc->Draw("EP");

    SetHist(data, kGray+1,  33);
    SetHist(mc,   kAzure-3, 8);

    leg->AddEntry( data, PlotMgr().GetOption<string>( "source" ).c_str(), "lp" );
    leg->AddEntry( mc,   "MC", "lp" );
    leg->Draw();

    TLine* line1 = new TLine( 0, 1, 500, 1 );
    line1->SetLineColor( kRed );
    line1->SetLineStyle( 8 );
    line1->Draw();

    TPaveText* pt = mgr::NewTextBox( .5, .76, .93, .84 );
    pt->AddText( ( trgname + cut ).c_str() );
    pt->Draw();

    c->cd();
    /**************************************************************************/

    TPad* bot = mgr::NewBottomPad();
    bot->Draw();
    bot->cd();
    
    TH1F* h2 = gPad->DrawFrame( 0, 0.9, 500, 1.1 );
    SetHistTitle( h2, "P_{T}", "Data/MC" );
    mgr::SetBottomPlotAxis( h2 );
    h2->GetYaxis()->SetLabelSize(14);
   
    TGraphAsymmErrors* scale = DividedGraph(data, mc);
    scale->Draw("EP");
    SetBottomHist(scale);

    TLine* line2 = new TLine( 0, 1, 500, 1 );
    line2->SetLineColor( kRed );
    line2->SetLineStyle( 8 );
    line2->Draw();
    
    c->cd();

    mgr::DrawCMSLabelOuter( PRELIMINARY );
    mgr::DrawLuminosity( PlotMgr().GetOption<double>( "lumi" ) );
    mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", "Pt_Eff_" + trg ) );
    mgr::SaveToROOT( scale, PlotMgr().GetResultsName( "root", "SF_" + trg ), "pt" );

    delete c;
    delete line1;
    delete line2;
    delete leg;
    delete pt;
}

extern void
SetBottomHist(TGraphAsymmErrors* hist)
{
    hist->SetMarkerSize(1);
    hist->SetMarkerStyle(8);
}

extern void
SetHist(TGraphAsymmErrors* hist, Color_t c, int m)
{
    hist->SetLineWidth(2);
    hist->SetLineColor(c);

    hist->SetMarkerColor(c);
    hist->SetMarkerSize(1);
    hist->SetMarkerStyle(m);
}

extern void
SetHistTitle( TH1* h, const string& xaxis, const string& yaxis )
{
    h->GetXaxis()->SetTitle( xaxis.c_str() );
    h->GetYaxis()->SetTitle( yaxis.c_str() );
}

extern TGraphAsymmErrors*
DividedGraph(
  TGraphAsymmErrors* num,
  TGraphAsymmErrors* den
  )
{
 
    TGraphAsymmErrors* ans = new TGraphAsymmErrors( num->GetN() );

    for( int i = 0; i < num->GetN(); ++i ){
      const double numy      = num->GetY()[i];
      const double numyerrlo = num->GetErrorYlow( i );
      const double numyerrhi = num->GetErrorYhigh( i );
  
      const double deny      = den->GetY()[i];
      const double denyerrlo = den->GetErrorYlow( i );
      const double denyerrhi = den->GetErrorYhigh( i );
  
      const double numx      = num->GetX()[i];
      const double xerrlo = num->GetErrorXlow( i );
      const double xerrhi = num->GetErrorXhigh( i );
      
      ans->SetPoint( i, numx, numy / deny );
      ans->SetPointError(
        i,
        xerrlo,
        xerrhi,
        ErrorProp( numy, numyerrlo, deny, denyerrlo ),
        ErrorProp( numy, numyerrhi, deny, denyerrhi )
        );  
    }
      
    ans->SetTitle( "" );
    return ans;
}
