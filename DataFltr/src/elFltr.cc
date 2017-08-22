#include "TriggerEfficiency/DataFltr/interface/DataFltr.h"
#include "TLorentzVector.h"
using namespace std;

elContainer&
DataFltr::elMgr( edm::ParameterSet* iConfig ) {
    static elContainer elmgr( *iConfig );
    return elmgr;
}


double
DataFltr::delR( const pat::Electron& el , const edm::Event& iEvent, const string& label ) {
    vector<double> dR;

    for ( pat::TriggerObjectStandAlone obj : *_triggerObjects ) {
        obj.unpackNamesAndLabels( iEvent, *_triggerResults );

        if( obj.hasFilterLabel( label ) ) {
            dR.push_back( deltaR( el.superCluster()->eta(), el.phi(), obj.eta(), obj.phi() ) );
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
DataFltr::passTrigger( pat::Electron& el, const edm::Event& iEvent ) {
    for( int i = 0; i < ( int )_trigger.size(); i++ ) {
        string name  = _trigger[i].getParameter<string>( "HLTName" );
        string label = _trigger[i].getParameter<string>( "FilterName" );

        if( delR( el, iEvent, label ) < 0.1 ) {
            el.addUserInt( name, 1 );
        }
    }
}

bool
DataFltr::zParent( pat::ElectronCollection& ele ) const {
    TLorentzVector lep1( ele[0].px(), ele[0].py(), ele[0].pz(), ele[0].energy() );
    TLorentzVector lep2( ele[1].px(), ele[1].py(), ele[1].pz(), ele[1].energy() );
    double mass = ( lep1 + lep2 ).M();
    return ( mass > _zmin && mass < _zmax );
}

bool
DataFltr::passKin( const pat::Electron& el, const bool& isTag ) const {
    double eta = el.superCluster()->eta();
    double pt = el.pt();
    bool ElKin( false );

    if( isTag ) {
        ElKin = ( ( fabs( eta ) < _tEtaMax )   &&   ( fabs( eta ) <= 1.4442 || fabs( eta ) >= 1.5660 )   &&   pt > _tPtMin );
    }

    else {
        ElKin = ( ( fabs( eta ) < _tEtaMax )   &&   ( fabs( eta ) <= 1.4442 || fabs( eta ) >= 1.5660 )   &&   pt > _pPtMin );
    }

    return ElKin;
}


bool
DataFltr::passId( const edm::Ptr<pat::Electron>& elptr, const string& level ) {
    return ( elMgr().getIDMap( level ) )[elptr];
}

//https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Recipe80X
bool
DataFltr::passImpact( const pat::Electron& el ) const {
    if( fabs( el.eta() ) < 1.4442  ) {
        if( fabs( el.gsfTrack()->dxy( _vtxhandle->front().position() ) ) < 0.05 ) {
            return true;
        }

        if( fabs( el.gsfTrack()->dz( _vtxhandle->front().position() ) ) < 0.10 ) {
            return true;
        }
    }

    else {
        if( fabs( el.gsfTrack()->dxy( _vtxhandle->front().position() ) ) < 0.10 ) {
            return true;
        }

        if( fabs( el.gsfTrack()->dz( _vtxhandle->front().position() ) ) < 0.20 ) {
            return true;
        }
    }

    return false;
}
