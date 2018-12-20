#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"

using namespace std;

extern void
DrawUncEta()
{
    TCanvas* c = mgr::NewCanvas();
    
    TPad* top = mgr::NewTopPad();
    top->Draw();
    top->cd();
    
    TH1F* h1 = gPad->DrawFrame( -3, 0.85, 3, 1 );
    SetHistTitle( h1, "", "Scale Factor" );
    mgr::SetTopPlotAxis( h1 );

    TLegend* leg = mgr::NewLegend( 0.4, 0.18, 0.5, 0.35 );
    leg->SetLineColor( kWhite );
    
    TGraphAsymmErrors* base = HistTGraph( "base_eta", "sf_eta" );
    TGraphAsymmErrors* comp = HistTGraph( "compare_eta", "sf_eta" );
    base->Draw("EP");
    comp->Draw("EP");
    
    SetHist(base, kGray+1,  33);
    SetHist(comp, kAzure-3, 8);
    
    string era = PlotMgr().GetOption<string>( "era" );
    leg->AddEntry( base, era.c_str(), "lp" );
    leg->AddEntry( comp, (era+"*").c_str(), "lp" );
    leg->Draw();
    
    TPaveText* pt = mgr::NewTextBox( .45, .76, .93, .84 );
    pt->AddText( PlotMgr().GetSingleData<string>( "desc" ).c_str() );
    pt->Draw();
    
    c->cd();
    /**************************************************************************/
    TPad* bot = mgr::NewBottomPad();
    bot->Draw();
    bot->cd();

    TH1F* h2 = gPad->DrawFrame( -3, -0.052, 3, 0.052 );
    SetHistTitle( h2, "SuperCluster #eta", "Systematic Unc" );
    mgr::SetBottomPlotAxis( h2 );
    h2->GetYaxis()->SetLabelSize(14);
    
    TGraphAsymmErrors* scale = DividedUncGraph( comp, base);
    scale->Draw("EP");
    SetBottomHist(scale);

    TLine* line = new TLine( -3, 0, 3, 0 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();

    c->cd();
    mgr::DrawCMSLabelOuter( PRELIMINARY );
    mgr::DrawLuminosity( PlotMgr().GetOption<double>( "lumi" ) );
    mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", "SF_eta" ) );

    delete c;
    delete line;
    delete leg;
    delete pt;
}

extern void
DrawUncPt()
{
    TCanvas* c = mgr::NewCanvas();
    
    TPad* top = mgr::NewTopPad();
    top->Draw();
    top->cd();
    
    TH1F* h1 = gPad->DrawFrame( 0, 0.7, 500, 1.5 );
    SetHistTitle( h1, "", "Scale Factor" );
    mgr::SetTopPlotAxis( h1 );

    TLegend* leg = mgr::NewLegend( 0.60, 0.45, 0.80, 0.65 );
    leg->SetLineColor( kWhite );
    
    TGraphAsymmErrors* base = HistTGraph( "base_pt", "sf_pt" );
    TGraphAsymmErrors* comp = HistTGraph( "compare_pt", "sf_pt" );
    base->Draw("EP");
    comp->Draw("EP");
    
    SetHist(base, kGray+1,  33);
    SetHist(comp, kAzure-3, 8);
    
    string era = PlotMgr().GetOption<string>( "era" );
    leg->AddEntry( base, era.c_str(), "lp" );
    leg->AddEntry( comp, (era+"*").c_str(), "lp" );
    leg->Draw();
    
    TPaveText* pt = mgr::NewTextBox( .45, .76, .93, .84 );
    pt->AddText( PlotMgr().GetSingleData<string>( "desc" ).c_str() );
    pt->Draw();
    
    c->cd();
    /**************************************************************************/
    TPad* bot = mgr::NewBottomPad();
    bot->Draw();
    bot->cd();

    TH1F* h2 = gPad->DrawFrame( 0, -0.052, 500, 0.052 );
    SetHistTitle( h2, "P_{T}", "Systematic Unc" );
    mgr::SetBottomPlotAxis( h2 );
    h2->GetYaxis()->SetLabelSize(14);
    
    TGraphAsymmErrors* scale = DividedUncGraph( comp, base);
    scale->Draw("EP");
    SetBottomHist(scale);

    TLine* line = new TLine( 0, 0, 500, 0 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();

    c->cd();
    mgr::DrawCMSLabelOuter( PRELIMINARY );
    mgr::DrawLuminosity( PlotMgr().GetOption<double>( "lumi" ) );
    mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", "SF_pt" ) );

    delete c;
    delete line;
    delete leg;
    delete pt;
}

extern TGraphAsymmErrors*
DividedUncGraph(
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
      
      ans->SetPoint( i, numx, (numy-deny) / deny );
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
