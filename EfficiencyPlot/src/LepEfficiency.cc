#include "TriggerEfficiency/EfficiencyPlot/interface/LepEfficiency.h"

using namespace std;

/*******************************************************************************/
void
LepEfficiency::AddHist(
        const string& title,
        const int& bin_size,
        const double& x_lower,
        const double& x_upper
        )
{
    TH1D* h = fs->make<TH1D>( 
            title.c_str(),
            title.c_str(),
            bin_size,
            x_lower,
            x_upper
            );

    _histmgr.AddObj( h );
}

void
LepEfficiency::AddHist( const string& title, const vector<double>& lst )
{
    TH1D* h = fs->make<TH1D>( 
            title.c_str(),
            title.c_str(),
            lst.size() - 1,
            &( lst[ 0 ] )
            );

    _histmgr.AddObj( h );
}

TH1*
LepEfficiency::Hist( const string& name )
{
    return _histmgr.GetObj( name );
}
/*******************************************************************************/
void
LepEfficiency::Add2DHist(
        const string& title,
        const vector<double>& xlst,
        const vector<double>& ylst
        )
{
    TH2D* h = fs->make<TH2D>( 
            title.c_str(), 
            title.c_str(), 
            xlst.size()-1, 
            &(xlst[0]), 
            ylst.size()-1,
            &(ylst[0]) 
            );

    _hist2Dmgr.AddObj(h);
}
TH2* 
LepEfficiency::Hist2D( const string& name) 
{
    return _hist2Dmgr.GetObj(name);
}

/*******************************************************************************/
void
LepEfficiency::Add2DTEff(
        const string& title, 
        const vector<double>& xlst,
        const vector<double>& ylst
        )
{
    TEfficiency* h = fs->make<TEfficiency>( 
            title.c_str(), 
            title.c_str(), 
            xlst.size()-1, 
            &(xlst[0]), 
            ylst.size()-1, 
            &(ylst[0]) 
            );
    
    _teffmgr.AddObj(h);
}

void
LepEfficiency::AddTEff(
        const string& title, 
        const vector<double>& xlst
        )
{
    TEfficiency* h = fs->make<TEfficiency>( 
            title.c_str(), 
            title.c_str(), 
            xlst.size()-1, 
            &(xlst[0]) 
            );
    
    _teffmgr.AddObj(h);
}

TEfficiency* 
LepEfficiency::HistTEff( const string& name )
{
    return _teffmgr.GetObj(name);
}

/*******************************************************************************/
vector<double>
LepEfficiency::ReadWeight(const string& filename)
{
    vector<double> w;
    ifstream fin;
    fin.open(filename,ifstream::in);
    string line;
    while(getline(fin,line)){
        w.push_back(stod(line));
    }
    return w;
}

