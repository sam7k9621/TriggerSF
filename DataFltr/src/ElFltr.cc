#include "TLorentzVector.h"
#include "TriggerSF/DataFltr/interface/ElFltr.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include <queue>

using namespace std;


double
ElFltr::delR( const pat::Electron& el, const edm::Event& iEvent, const string& label )
{
    vector<double> dR;

    for( pat::TriggerObjectStandAlone obj : *_triggerObjects ){
        obj.unpackNamesAndLabels( iEvent, *_triggerResults );

        if( obj.hasFilterLabel( label ) ){
            dR.push_back( deltaR( el.superCluster()->eta(), el.phi(), obj.eta(), obj.phi() ) );
        }
    }

    if( dR.empty() ){
        return 999;
    }
    else{
        std::sort( dR.begin(), dR.end() );
        return dR.front();
    }
}

void
ElFltr::passTrigger( pat::Electron& el, const edm::Event& iEvent )
{
    for( int i = 0; i < (int)_trigger.size(); i++ ){
        string name  = _trigger[ i ].getParameter<string>( "HLTName" );
        string label = _trigger[ i ].getParameter<string>( "FilterName" );
        
        if( delR( el, iEvent, label ) < 0.1 ){
            el.addUserInt( name, 1 );
        }
    }
}

bool
ElFltr::zParent( pat::ElectronCollection& el ) const
{
    TLorentzVector lep1( el[ 0 ].px(), el[ 0 ].py(), el[ 0 ].pz(), el[ 0 ].energy() );
    TLorentzVector lep2( el[ 1 ].px(), el[ 1 ].py(), el[ 1 ].pz(), el[ 1 ].energy() );
    double mass = ( lep1 + lep2 ).M();
    return mass > _zmin && mass < _zmax;
}

bool
ElFltr::passKin( const pat::Electron& el, const bool& isTag ) const
{
    double eta = el.superCluster()->eta();
    double pt  = el.pt();
    bool ElKin( false );

    if( isTag ){
        ElKin = ( ( fabs( eta ) < _tEtaMax ) && ( fabs( eta ) < 1.4442 || fabs( eta ) > 1.5660 ) && pt > _tPtMin );
    }
    else{
        ElKin = ( ( fabs( eta ) < _tEtaMax ) && pt > _pPtMin );
    }

    return ElKin;
}


bool
ElFltr::passId( const edm::Ptr<pat::Electron>& it_el, const string& level )
{
    
    if( level == "loose" ){
        return it_el->electronID( _electronID_loosemap  );
    }
    else if( level == "medium" ){
        return it_el->electronID( _electronID_mediummap ); 
    }
    else if( level == "tight" ){
        return it_el->electronID( _electronID_tightmap  ); 
    }
    else if( level == "heep" ){
        return it_el->electronID( _electronID_HEEPmap   );
    }
    else{
        return it_el->electronID( _electronID_vetomap   );
    }

}

// https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Recipe80X
bool
ElFltr::passImpact( const pat::Electron& el ) const
{
    if( fabs( el.eta() ) < 1.4442 ){
        if( fabs( el.gsfTrack()->dxy( _vtx.front().position() ) ) < 0.05 ){
            return true;
        }

        if( fabs( el.gsfTrack()->dz( _vtx.front().position() ) ) < 0.10 ){
            return true;
        }
    }
    else{
        if( fabs( el.gsfTrack()->dxy( _vtx.front().position() ) ) < 0.10 ){
            return true;
        }

        if( fabs( el.gsfTrack()->dz( _vtx.front().position() ) ) < 0.20 ){
            return true;
        }
    }

    return false;
}

const reco::Candidate*
ElFltr::GetDirectMother( const reco::Candidate* x, int target_ID )
{
   queue<const reco::Candidate*> bfs_queue;
   bfs_queue.push( x );

   while( !bfs_queue.empty() ){
      const reco::Candidate* temp = bfs_queue.front();
      bfs_queue.pop();
      if( abs( temp->pdgId() ) == abs( target_ID ) ){
         return temp;
      }

      for( unsigned i = 0; i < temp->numberOfMothers(); ++i ){
         if( temp->mother( i )->pdgId() == temp->pdgId() ){
            bfs_queue.push( temp->mother( i ) );
         } else if( abs( temp->mother( i )->pdgId() ) == abs( target_ID ) ){
            return temp->mother( i );
         }
      }
   }
   return NULL;
}
