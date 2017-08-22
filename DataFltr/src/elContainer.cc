#include "TriggerEfficiency/DataFltr/interface/DataContainer.h"

using namespace std;

elContainer::elContainer( const edm::ParameterSet & iConfig ):
    _pImpact( iConfig.getParameter < bool > ( "probeImpact" ) ),
    _tImpact( iConfig.getParameter < bool > ( "tagImpact" ) ),
    _looseMapToken ( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "looseMap" ) ) ),
    _mediumMapToken( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "mediumMap" ) ) ),
    _tightMapToken ( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "tightMap" ) ) ),
    _heepMapToken  ( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "heepMap" ) ) ),
    _safeCutToken  ( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "safeCut" ) ) ) {
}

tuple<bool, bool>
elContainer::getImpact() const {
    return make_tuple( _tImpact, _pImpact );
}

void
elContainer::initHandle( const edm::Event& iEvent ) {
    iEvent.getByToken( _safeCutToken  , _safecutHandle  );
    iEvent.getByToken( _looseMapToken , _looseMapHandle );
    iEvent.getByToken( _mediumMapToken, _mediumMapHandle );
    iEvent.getByToken( _tightMapToken , _tightMapHandle );
    iEvent.getByToken( _heepMapToken  , _heepMapHandle  );
}

edm::ValueMap<bool>
elContainer::getIDMap( const string& level ) {
    if( level == "loose" ) {
        return *_looseMapHandle;
    }

    else if( level == "medium" ) {
        return *_mediumMapHandle;
    }

    else if( level == "tight" ) {
        return *_tightMapHandle;
    }

    else {
        return *_safecutHandle;
    }
}
