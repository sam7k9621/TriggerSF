#include "TriggerEfficiency/DataFltr/interface/DataFltr.h"
#include "TLorentzVector.h"
#include <math.h>
using namespace std;

muContainer&
DataFltr::muMgr( edm::ParameterSet* iConfig ) {
    static muContainer mumgr( *iConfig );
    return mumgr;
}

double
DataFltr::delR( const pat::Muon& mu, const edm::Event& iEvent , const string& label ) {
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
DataFltr::passTrigger( pat::Muon& mu, const edm::Event& iEvent ) {
    for( int i = 0; i < ( int )( _trigger.size() ); i++ ) {
        string name = _trigger[i].getParameter<string>( "HLTName" );
        string label = _trigger[i].getParameter<string>( "FilterName" );

        if ( delR( mu, iEvent, label ) < 0.1 ) {
            mu.addUserInt( name, 1 );
        }
    }
}

bool
DataFltr::zParent( const pat::MuonCollection& muons ) const {
    TLorentzVector lep1( muons[0].px(), muons[0].py(), muons[0].pz(), muons[0].energy() );
    TLorentzVector lep2( muons[1].px(), muons[1].py(), muons[1].pz(), muons[1].energy() );
    double mass = ( lep1 + lep2 ).M();
    return ( mass > _zmin && mass < _zmax );
}

bool
DataFltr::passId( const pat::Muon& mu, const string& level ) const {
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
DataFltr::passKin( const pat::Muon& mu, const bool& isTag ) const {
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
