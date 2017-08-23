#include "TriggerEfficiency/DataFltr/interface/DataContainer.h"
#include "math.h"
using namespace std;

muContainer::muContainer( const edm::ParameterSet& iConfig ):
    _tPFIso( iConfig.getParameter<double>( "TagPassPFIso" ) ),
    _pPFIso( iConfig.getParameter<double>( "ProbePassPFIso" ) ),
    _tTKIso( iConfig.getParameter<double>( "TagPassTKIso" ) ),
    _pTKIso( iConfig.getParameter<double>( "ProbePassTKIso" ) ) {
}

tuple<double, double>
muContainer::getPFIso() const{
    return make_tuple( _tPFIso, _pPFIso);
}

tuple<double, double>
muContainer::getTKIso() const{
    return make_tuple( _tTKIso, _pTKIso);
}

