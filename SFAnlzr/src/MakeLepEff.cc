#include "TriggerSF/SFAnlzr/interface/MakeLepEff.h"
#include <fstream>

using namespace std;

/*******************************************************************************/
void
MakeLepEff::AddHist(
        const string& title,
        const int& bin_size,
        const double& x_lower,
        const double& x_upper
        )
{
    TH1D* h = _fs->make<TH1D>( 
            title.c_str(),
            title.c_str(),
            bin_size,
            x_lower,
            x_upper
            );

    _histmgr.AddObj( h );
}

void
MakeLepEff::AddHist( const string& title, const vector<double>& lst )
{
    TH1D* h = _fs->make<TH1D>( 
            title.c_str(),
            title.c_str(),
            lst.size() - 1,
            &( lst[ 0 ] )
            );

    _histmgr.AddObj( h );
}

TH1*
MakeLepEff::Hist( const string& name )
{
    return _histmgr.GetObj( name );
}
/*******************************************************************************/
void
MakeLepEff::Add2DHist(
        const string& title,
        const vector<double>& xlst,
        const vector<double>& ylst
        )
{
    TH2D* h = _fs->make<TH2D>( 
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
MakeLepEff::Hist2D( const string& name) 
{
    return _hist2Dmgr.GetObj(name);
}

/*******************************************************************************/
void
MakeLepEff::Add2DTEff(
        const string& title, 
        const vector<double>& xlst,
        const vector<double>& ylst
        )
{
    TEfficiency* h = _fs->make<TEfficiency>( 
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
MakeLepEff::AddTEff(
        const string& title, 
        const vector<double>& xlst
        )
{
    TEfficiency* h = _fs->make<TEfficiency>( 
            title.c_str(), 
            title.c_str(), 
            xlst.size()-1, 
            &(xlst[0]) 
            );
    
    _teffmgr.AddObj(h);
}

TEfficiency* 
MakeLepEff::HistTEff( const string& name )
{
    return _teffmgr.GetObj(name);
}

/*******************************************************************************/
vector<double>
MakeLepEff::ReadWeight(const string& filename)
{
    vector<double> w;
    ifstream fin;
    fin.open(filename,ifstream::in);
    string line;
    while(getline(fin,line)){
        w.push_back(stod(line));
    }
    fin.close();
    return w;
}

