#include "TriggerEfficiency/DataFltr/interface/DataFltr.h"
#include <iostream>
using namespace std;

DataFltr::DataFltr( const edm::ParameterSet& iConfig ):
    _musrc ( consumes<vector<pat::Muon> >( iConfig.getParameter<edm::InputTag>( "musrc" ) ) ),
    _elsrc ( consumes<vector<pat::Electron> >( iConfig.getParameter<edm::InputTag>( "elsrc" ) ) ),
    _vtxsrc  ( consumes<vector<reco::Vertex> > ( iConfig.getParameter<edm::InputTag >( "vtxsrc" ) ) ),
    _hltInputTag ( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "HLTInputTag" ) ) ),
    _hltObjectsInputTag ( consumes<vector<pat::TriggerObjectStandAlone> >( iConfig.getParameter<edm::InputTag>( "HLTObjectsInputTag" ) ) ),
    _zmin ( iConfig.getParameter   <double>( "Zmassmin" ) ),
    _zmax ( iConfig.getParameter   <double>( "Zmassmax" ) ),
    _useMC ( iConfig.getParameter<bool>( "useMC" ) ),
    _lepton ( iConfig.getParameter<string>( "lepton_type" ) ),
    _tagid ( iConfig.getParameter<string>( "TagPassID" ) ),
    _proid ( iConfig.getParameter<string>( "ProbePassID" ) ),
    _tEtaMax ( iConfig.getParameter<double>( "tagEtaMax" ) ),
    _pEtaMax ( iConfig.getParameter<double>( "probeEtaMax" ) ),
    _tPtMin ( iConfig.getParameter <double>( "tagPtMin" ) ),
    _pPtMin ( iConfig.getParameter <double>( "probePtMin" ) ),
    _trigger ( iConfig.getParameter<vector<edm::ParameterSet>>( "triggerCache" ) ) {
}



// ------------ method called on each new Event  ------------
bool
DataFltr::filter( edm::Event& iEvent, const edm::EventSetup& iSetup ) {
    return true;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
DataFltr::beginStream( edm::StreamID ) {
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
DataFltr::endStream() {
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DataFltr::fillDescriptions( edm::ConfigurationDescriptions& descriptions ) {
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}
DEFINE_FWK_MODULE( DataFltr );
