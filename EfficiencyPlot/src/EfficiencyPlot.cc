#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"


using namespace std;
/*******************************************************************************
*   Global function
*******************************************************************************/
extern TrgPlotMgr&
PlotMgr( const string& subdir )
{
    static TrgPlotMgr mgr( subdir );
    return mgr;
}

/*******************************************************************************
*   PlotEfficiency
*******************************************************************************/
extern void
PlotEff()
{
    MergeData();
    MergeMC();
    InitSetting( "data", (PlotMgr().DatasDir() / "data_temp.root").c_str() );
    InitSetting( "mc",   (PlotMgr().DatasDir() / "mc_temp.root"  ).c_str() );
    PlotSF();
    DrawEff();
    CleanSample();
}

extern void 
MergeData()
{
    string cmd = "hadd -f ";
    vector<string> datalst = PlotMgr().GetListData<string>("datalst");

    string output = PlotMgr().DatasDir() / "data_temp.root ";
    string input = "";
    for(const auto& s : datalst){
        input += PlotMgr().GetSingleData<string>(s) + " ";
    }

    system( ( cmd + output + input ).c_str() ) ;
}

extern void
MergeMC()
{
    string cmd = "hadd -f ";
    string output = PlotMgr().DatasDir() / "mc_temp.root ";
    string input  = PlotMgr().GetSingleData<string>("MC");

    system( ( cmd + output + input ).c_str() ) ;
}

extern void
CleanSample()
{
    string cmd  = "rm ";
    string file = PlotMgr().DatasDir() / "*temp.root ";
    system( ( cmd + file ).c_str() );
}
    
extern void
InitSetting(const string& type, const string& filename)
{
    //For each trg hist manager has 2*ver hists (peff and eeff)*ver
    TFile* file = TFile::Open( filename.c_str() );

    for( const auto& trg : PlotMgr().GetListData<string>( "trglst" ) ){

        string tag = type + trg;
        /***********************************************************************/
        TH1D* passZ = (TH1D*)file->Get( ( "demo/pass_zmass_" + trg ).c_str() );
        TH1D* failZ = (TH1D*)file->Get( ( "demo/fail_zmass_" + trg ).c_str() );

        passZ->SetDirectory( 0 );
        failZ->SetDirectory( 0 );

        passZ->SetName( "pass_zmass" );
        failZ->SetName( "fail_zmass" );

        PlotMgr().AddTH1( tag, passZ );
        PlotMgr().AddTH1( tag, failZ );

        /***********************************************************************/
        TEfficiency* teff = (TEfficiency*)file->Get( ("demo/eff_pt_eta_" + trg ).c_str() );
        TEfficiency* peff = (TEfficiency*)file->Get( ("demo/eff_pt_"  + trg ).c_str() );
        TEfficiency* eeff = (TEfficiency*)file->Get( ("demo/eff_eta_" + trg ).c_str() );

        teff->SetDirectory( 0 );
        peff->SetDirectory( 0 );
        eeff->SetDirectory( 0 );
        
        teff->SetName( "eff_pt_eta" ) ;
        peff->SetName( "eff_pt"  ) ;
        eeff->SetName( "eff_eta" ) ;

        PlotMgr().AddTEff( tag, teff);
        PlotMgr().AddTEff( tag, peff);
        PlotMgr().AddTEff( tag, eeff);
    }

    file->Close();
}


extern TH1*
HistTH1( const string& tag, const string& name )
{
    return PlotMgr().GetTH1( tag, name );
}

extern TH2*
HistTH2( const string& tag, const string& name )
{
    return PlotMgr().GetTH2( tag, name );
}

extern TGraphAsymmErrors*
HistTGraph( const string& tag, const string& name )
{
    return PlotMgr().GetTGraph( tag, name );
}

extern TEfficiency*
HistTEff( const string& tag, const string& name )
{
    return PlotMgr().GetTEff( tag, name );
}

