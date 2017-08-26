#include "TriggerEfficiency/EfficiencyPlot/interface/EfficiencyPlot.h"
#include <iostream>

using namespace std;

int main( int argc, char *argv[] ) {
    opt::options_description de( "Command for SelectionCut" );
    de.add_options()
    ( "lepton,l", opt::value<string>()->required(), "which lepton" )
    ( "source,s", opt::value<string>()->required(), "Which era of data or mc" )
    ;
    PlotMgr( "EfficiencyPlot" ).AddOptions( de );
    const int run = PlotMgr().ParseOptions( argc, argv );

    if( run == dra::Parsermgr::HELP_PARSER  ) {
        return 0;
    }

    if( run == dra::Parsermgr::FAIL_PARSER ) {
        return 1;
    }

    PlotMgr().SetFileName( {"source", "lepton"} ) ;
    PlotEff();
}
