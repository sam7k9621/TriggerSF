#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"
#include "TEfficiency.h"

#include <iostream>

using namespace std;

extern double 
ErrorProp(const double& x1, const double& m1, const double& x2,const double& m2)
{
    double result = sqrt( (m1*m1)/(x2*x2) + (m2*m2)*(x1*x1)/(x2*x2*x2*x2));
    return result;
}

extern TH1*
MakeScale(TEfficiency* data, TEfficiency* mc)
{
    TH1* scale =data->GetCopyTotalHisto(); 
   
    //data->SetStatisticOption(TEfficiency::EStatOption::kBBayesian);

    int xbins = scale->GetXaxis()->GetNbins();
    int ybins = scale->GetYaxis()->GetNbins();
    int databin, mcbin;
    double deff, derr, meff, merr, seff, serr;

    for(int i=0; i < xbins+2; ++i){
        for(int j=0; j < ybins+2; ++j){
            
            databin = data->GetGlobalBin(i, j);
            deff    = data->GetEfficiency( databin );
            derr    = data->GetEfficiencyErrorLow( databin );
            
            mcbin   = mc->GetGlobalBin( i,j ) ;
            meff    = mc->GetEfficiency( mcbin );
            merr    = mc->GetEfficiencyErrorLow( mcbin );

            if( meff!=0 ){
                seff = deff/meff;
                serr = ErrorProp(deff, derr, meff, merr);
            }
            else{
                seff = 0;
                serr = 0;
            }

            scale->SetBinContent(i, j, seff);
            scale->SetBinError(i, j, serr);
        }
    }

    return scale;
}

extern void
PlotSF()
{
    for( const auto& trg : PlotMgr().GetListData<string>( "trglst" ) ){
        
        TH1* scale = MakeScale( HistTEff( "data" + trg, "eff_pt_eta"), HistTEff( "mc" + trg, "eff_pt_eta") ); 
        mgr::SaveToROOT( scale, PlotMgr().GetResultsName( "root", "SF_" + trg ), "SF_pt_eta" );
       
        TCanvas* c = mgr::NewCanvas();
       
        scale->Draw("COLZ text");
        scale->SetStats( false );
        scale->SetTitle("");
        scale->GetYaxis()->SetTitle( "P_{T} [GeV]" );
        scale->GetXaxis()->SetTitle( "SuperCluster #eta" );
        mgr::SetAxis( scale );
        mgr::SetSinglePad(c);
        c->SetRightMargin( 1 );
        mgr::DrawCMSLabelOuter( PRELIMINARY );
        mgr::DrawLuminosity(41290);
        mgr::SaveToPDF( c, PlotMgr().GetResultsName( "pdf", "SF_" + trg ) );
    }
}
