#include "TriggerEfficiency/DataFltr/interface/DataContainer.h"
#include "math.h"
using namespace std;

muContainer::muContainer( const edm:: ParameterSet& iConfig ):
    _tPFIso( iConfig.getParameter<double>( "TagPassPFIso" ) ),
    _pPFIso( iConfig.getParameter<double>( "ProbePassPFIso" ) ),
    _tTKIso( iConfig.getParameter<double>( "TagPassTKIso" ) ),
    _pTKIso( iConfig.getParameter<double>( "ProbePassTKIso" ) ) {
}

inline bool
muContainer::passTKIso( const pat::Muon& mu, const double& cut ) const {
    if( mu.isolationR03().sumPt / mu.pt() < cut ) {
        return true;
    }

    else {
        return false;
    }
}

inline bool
muContainer::passPFIso( const pat::Muon& mu, const double& cut ) const {
    double  chIso   = mu.pfIsolationR04().sumChargedHadronPt;
    double  nhIso   = mu.pfIsolationR04().sumNeutralHadronEt;
    double  gIso    = mu.pfIsolationR04().sumPhotonEt;
    double  puchIso = mu.pfIsolationR04().sumPUPt;
    double  relIso  = ( chIso + TMath::Max( 0., nhIso + gIso - 0.5 * puchIso ) ) / mu.pt();

    if( relIso < cut ) { //0.15
        return true;
    }

    else {
        return false;
    }
}

