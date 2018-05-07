#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"

using namespace std;

extern void
FitsCheck()
{
    PlotMgr().SetColor();
    DrawZmass("") ;   
}

extern void
DrawZmass(const string& sample)
{
    for( const auto& trg : PlotMgr().GetListData<string>( "trglst" ) ){
       
        TCanvas* c = mgr::NewCanvas();
        mgr::SetSinglePad( c );
        TLegend* leg = mgr::NewLegend( 0.7, 0.5, 0.9, 0.7 );
        leg->SetLineColor( kWhite );
        leg->SetNColumns(2);
    
        TH1* hist = HistTH1( trg, sample );

        double ymax = mgr::GetYmax( hist );
        TH1F* h     = gPad->DrawFrame( 50, 0, 130, ymax * 1.6 );
        h->SetTitle( "" );
        h->GetXaxis()->SetTitle( "GeV" );
        h->GetYaxis()->SetTitle( "Event" );
        mgr::SetAxis( h );
    
        leg->AddEntry( hist, hist->GetTitle(), "l" );
        leg->Draw();
        mgr::DrawCMSLabel( PRELIMINARY );
    
        TPaveText* pt = mgr::NewTextBox( .55, .81, .93, .87 );
        pt->AddText( ( trg ).c_str() );
        pt->Draw();
    
        mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", sample + trg ) );
    
        delete c;
        delete pt;
        delete leg;
    }
}
