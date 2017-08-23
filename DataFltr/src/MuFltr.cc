#include "TriggerEfficiency/DataFltr/interface/MuFltr.h"
#include "TLorentzVector.h"
#include <math.h>
using namespace std;

double
MuFltr::delR( const pat::Muon& mu, const edm::Event& iEvent , const string& label ) {
    vector<double> dR;

    for ( pat::TriggerObjectStandAlone obj : *_triggerObjects ) {
        obj.unpackNamesAndLabels( iEvent, *_triggerResults );

        if( obj.hasFilterLabel( label ) ) {
            dR.push_back( deltaR( mu.eta(), mu.phi(), obj.eta(), obj.phi() ) );
        }
    }

    if( dR.empty() ) {
        return 999;
    }

    else {
        std::sort( dR.begin(), dR.end() );
        return dR.front();
    }
}

void
MuFltr::passTrigger( pat::Muon& mu, const edm::Event& iEvent ) {
    for( int i = 0; i < ( int )( _trigger.size() ); i++ ) {
        string name = _trigger[i].getParameter<string>( "HLTName" );
        string label = _trigger[i].getParameter<string>( "FilterName" );

        if ( delR( mu, iEvent, label ) < 0.1 ) {
            mu.addUserInt( name, 1 );
        }
    }
}

bool
MuFltr::zParent( const pat::MuonCollection& muons ) const {
    TLorentzVector lep1( muons[0].px(), muons[0].py(), muons[0].pz(), muons[0].energy() );
    TLorentzVector lep2( muons[1].px(), muons[1].py(), muons[1].pz(), muons[1].energy() );
    double mass = ( lep1 + lep2 ).M();
    return ( mass > _zmin && mass < _zmax );
}

bool
MuFltr::passId( const pat::Muon& mu, const string& level ) const {
    if( level == "loose" ) {
        return mu.isLooseMuon();
    }

    if( level == "tight" ) {
        return mu.isTightMuon( _vtx.front() );
    }

    if( level == "HighPT" ) {
        return mu.isHighPtMuon( _vtx.front() );
    }

    return false;
}

bool
MuFltr::passKin( const pat::Muon& mu, const bool& isTag ) const {
    double eta = mu.eta();
    double pt = mu.pt();
    bool MuKin( false );

    if( isTag ) {
        MuKin = ( ( fabs( eta ) <= _tEtaMax ) && pt > _tPtMin );
    }

    else {
        MuKin = ( ( fabs( eta ) <= _pEtaMax ) && pt > _pPtMin );
    }

    return MuKin;
}

bool
MuFltr::passTKIso( const pat::Muon& mu, const double& cut ) const {
    if( mu.isolationR03().sumPt / mu.pt() < cut ) {
        return true;
    }

    else {
        return false;
    }
}

bool
MuFltr::passPFIso( const pat::Muon& mu, const double& cut ) const {
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

