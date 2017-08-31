#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TH1.h"
#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"
using namespace std;
/*******************************************************************************
*   Global function
*******************************************************************************/

TrgPlotMgr<TGraphAsymmErrors>&
PlotMgr( const string& subdir )
{
    static TrgPlotMgr<TGraphAsymmErrors> mgr( subdir );
    return mgr;
}

/*******************************************************************************
*   PlotEfficiency
*******************************************************************************/
void
PlotEff()
{

    InitSetting();
    DrawEff();

}

void
InitSetting()
{

    for( const auto& ver : PlotMgr().GetListData<string>( "verlst" ) ){
        TFile* file = TFile::Open( PlotMgr().GetSingleData<string>( ver ).c_str() );

        for( const auto& trg : PlotMgr().GetListData<string>( "trglst" ) ){

            TH1D* passPt   = (TH1D*)file->Get( ( "pass_pt_" + trg ).c_str() );
            TH1D* passEta  = (TH1D*)file->Get( ( "pass_eta_" + trg ).c_str() );
            TH1D* totalPt  = (TH1D*)file->Get( ( "total_pt_" + trg ).c_str() );
            TH1D* totalEta = (TH1D*)file->Get( ( "total_eta_" + trg ).c_str() );

            TGraphAsymmErrors* peff = new TGraphAsymmErrors( passPt, totalPt, "b" );
            TGraphAsymmErrors* eeff = new TGraphAsymmErrors( passEta, totalEta, "b" );

            peff->SetName( ( "pteff_" + trg ).c_str() );
            eeff->SetName( ( "etaeff_" + trg ).c_str() );

            PlotMgr().AddPlot( ver, peff );
            PlotMgr().AddPlot( ver, eeff );
        }

        file->Close();
    }

    PlotMgr().SetColor();
}

void
DrawEff()
{

    for( const auto& trg : PlotMgr().GetListData<string>( "trglst" ) ){
        DrawEta( trg );
        DrawPt( trg );
    }
}

void
DrawEta( const string& trg )
{

    string trgname = dra::GetSingle<string>( "trgname", PlotMgr().GetSubTree( trg ) );
    string cut     = dra::GetSingle<string>( "pcut", PlotMgr().GetSubTree( trg ) );


    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad( c );

    TH1F* h = gPad->DrawFrame( -3, 0, 3, 1.2 );
    SetHist( h, "#eta", "Efficiency" );
    mgr::SetAxis( h );

    TLegend* leg = mgr::NewLegend( 0.55, 0.2, 0.65, 0.33 );
    leg->SetLineColor( kWhite );

    for( const auto& ht : Hist( "etaeff_" + trg ) ){
        ht->Draw( "EP" );
        leg->AddEntry( ht, ht->GetTitle(), "lp" );
    }

    leg->Draw();

    mgr::DrawCMSLabelOuter( PRELIMINARY );

    TPaveText* pt = mgr::NewTextBox( .45, .79, .93, .87 );
    pt->AddText( ( trgname + cut ).c_str() );
    pt->Draw();

    TLine* line = new TLine( -3, 1, 3, 1 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();

    mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", "Eta_Eff_" + trg ) );

    delete c;
    delete pt;
    delete line;
    delete leg;
}

void
DrawPt( const string& trg )
{

    string trgname = dra::GetSingle<string>( "trgname", PlotMgr().GetSubTree( trg ) );
    string cut     = dra::GetSingle<string>( "ecut", PlotMgr().GetSubTree( trg ) );


    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad( c );

    TH1F* h = gPad->DrawFrame( 0, 0, 200, 1.2 );
    SetHist( h, "P_{T}", "Efficiency" );
    mgr::SetAxis( h );

    TLegend* leg = mgr::NewLegend( 0.65, 0.27, 0.75, 0.4 );
    leg->SetLineColor( kWhite );

    for( const auto& ht : Hist( "pteff_" + trg ) ){
        ht->Draw( "EP" );
        leg->AddEntry( ht, ht->GetTitle(), "lp" );
    }

    leg->Draw();

    mgr::DrawCMSLabelOuter( PRELIMINARY );

    TPaveText* pt = mgr::NewTextBox( .55, .81, .93, .87 );
    pt->AddText( ( trgname + cut ).c_str() );
    pt->Draw();

    TLine* line = new TLine( 0, 1, 200, 1 );
    line->SetLineColor( kRed );
    line->SetLineStyle( 8 );
    line->Draw();

    mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", "Pt_Eff_" + trg ) );

    delete c;
    delete pt;
    delete line;
    delete leg;
}

void
SetHist( TH1* h, const string& xaxis, const string& yaxis )
{

    h->GetXaxis()->SetTitle( xaxis.c_str() );
    h->GetYaxis()->SetTitle( yaxis.c_str() );

}

vector<TGraphAsymmErrors*>
Hist( const string& name )
{
    return PlotMgr().GetHist( name );
}
