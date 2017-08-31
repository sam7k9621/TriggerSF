#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TH1.h"
#include "TriggerEfficiency/EfficiencyPlot/interface/FitsCheck.h"
#include <iostream>
using namespace std;

TrgPlotMgr<TH1>&
FitsMgr( const string& subdir )
{
    static TrgPlotMgr<TH1> mgr( subdir );
    return mgr;
}

void
PlotFits()
{

    InitSetting_fit();
    DrawFits();
}

void
InitSetting_fit()
{

    for( const auto& ver : FitsMgr().GetListData<string>( "verlst" ) ){
        TFile* file = TFile::Open( FitsMgr().GetSingleData<string>( ver ).c_str() );

        for( const auto& trg : FitsMgr().GetListData<string>( "trglst" ) ){

            TH1D* passZ = (TH1D*)file->Get( ( "demo/pass_zmass" + trg ).c_str() );
            TH1D* failZ = (TH1D*)file->Get( ( "demo/fail_zmass" + trg ).c_str() );

            passZ->SetDirectory( 0 );
            failZ->SetDirectory( 0 );
            /* dir->GetObject( ("pass_zmass" + trg).c_str(), passZ);*/
            // dir->GetObject( ("fail_zmass" + trg).c_str(), failZ);

            passZ->SetName( ( "pass_zmass_" + trg ).c_str() );
            failZ->SetName( ( "fail_zmass_" + trg ).c_str() );

            FitsMgr().AddPlot( ver, passZ );
            FitsMgr().AddPlot( ver, failZ );
        }

        file->Close();
    }

    FitsMgr().SetColor();
}

void
DrawFits()
{
    for( const auto& trg : FitsMgr().GetListData<string>( "trglst" ) ){
        DrawFitsPass( trg );
        DrawFitsFail( trg );
    }
}

void
DrawFitsFail( const string& trg )
{

    string trgname = dra::GetSingle<string>( "trgname", FitsMgr().GetSubTree( trg ) );

    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad( c );

    TLegend* leg = mgr::NewLegend( 0.65, 0.27, 0.75, 0.4 );
    leg->SetLineColor( kWhite );

    double ymax = mgr::GetYmax( HIST( "fail_zmass_" + trg ) );
    TH1F* h     = gPad->DrawFrame( 50, 0, 130, ymax * 1.6 );
    h->SetTitle( "" );
    h->GetXaxis()->SetTitle( "GeV" );
    h->GetYaxis()->SetTitle( "Event" );
    mgr::SetAxis( h );

    for( auto& f : HIST( "fail_zmass_" + trg ) ){
        f->Draw( "same" );
        leg->AddEntry( f, f->GetTitle(), "lp" );
    }

    leg->Draw();
    mgr::DrawCMSLabelOuter( PRELIMINARY );

    TPaveText* pt = mgr::NewTextBox( .55, .81, .93, .87 );
    pt->AddText( ( trgname ).c_str() );
    pt->Draw();

    mgr::SaveToPDF( c, FitsMgr().GetResultsName( "pdf", "zmass_fail_" + trg ) );

    delete c;
    delete pt;
    delete leg;
}

void
DrawFitsPass( const string& trg )
{

    string trgname = dra::GetSingle<string>( "trgname", FitsMgr().GetSubTree( trg ) );

    TCanvas* c = mgr::NewCanvas();
    mgr::SetSinglePad( c );

    TLegend* leg = mgr::NewLegend( 0.65, 0.27, 0.75, 0.4 );
    leg->SetLineColor( kWhite );

    double ymax = mgr::GetYmax( HIST( "pass_zmass_" + trg ) );
    TH1F* h     = gPad->DrawFrame( 50, 0, 130, ymax * 1.6 );
    h->SetTitle( "" );
    h->GetXaxis()->SetTitle( "GeV" );
    h->GetYaxis()->SetTitle( "Event" );
    mgr::SetAxis( h );

    for( auto& f : HIST( "pass_zmass_" + trg ) ){
        f->Draw( "same" );
        leg->AddEntry( f, f->GetTitle(), "lp" );
    }

    leg->Draw();
    mgr::DrawCMSLabelOuter( PRELIMINARY );

    TPaveText* pt = mgr::NewTextBox( .55, .81, .93, .87 );
    pt->AddText( ( trgname ).c_str() );
    pt->Draw();

    mgr::SaveToPDF( c, FitsMgr().GetResultsName( "pdf", "zmass_pass_" + trg ) );

    delete c;
    delete pt;
    delete leg;
}


vector<TH1*>
HIST( const string& name )
{
    return FitsMgr().GetHist( name );
}
