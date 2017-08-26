#include "TriggerEfficiency/DataFltr/interface/MuFltr.h"

using namespace std;

MuFltr::MuFltr( const edm::ParameterSet& iConfig ):
    _musrc ( consumes<vector<pat::Muon> >( iConfig.getParameter<edm::InputTag>( "musrc" ) ) ),
    _elsrc ( consumes<vector<pat::Electron> >( iConfig.getParameter<edm::InputTag>( "elsrc" ) ) ),
    _vtxsrc  ( consumes<vector<reco::Vertex> > ( iConfig.getParameter<edm::InputTag >( "vtxsrc" ) ) ),
    _hltInputTag ( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "HLTInputTag" ) ) ),
    _hltObjectsInputTag ( consumes<vector<pat::TriggerObjectStandAlone> >( iConfig.getParameter<edm::InputTag>( "HLTObjectsInputTag" ) ) ),
    _zmin ( iConfig.getParameter<double>( "Zmassmin" ) ),
    _zmax ( iConfig.getParameter<double>( "Zmassmax" ) ),
    _useMC ( iConfig.getParameter<bool>( "useMC" ) ),
    _tagid ( iConfig.getParameter<string>( "TagPassID" ) ),
    _proid ( iConfig.getParameter<string>( "ProbePassID" ) ),
    _tEtaMax ( iConfig.getParameter<double>( "tagEtaMax" ) ),
    _pEtaMax ( iConfig.getParameter<double>( "probeEtaMax" ) ),
    _tPtMin ( iConfig.getParameter <double>( "tagPtMin" ) ),
    _pPtMin ( iConfig.getParameter <double>( "probePtMin" ) ),
    _trigger ( iConfig.getParameter<vector<edm::ParameterSet>>( "triggerCache" ) ),
    _tPFIso( iConfig.getParameter<double>( "TagPassPFIso" ) ),
    _pPFIso( iConfig.getParameter<double>( "ProbePassPFIso" ) ),
    _tTKIso( iConfig.getParameter<double>( "TagPassTKIso" ) ),
    _pTKIso( iConfig.getParameter<double>( "ProbePassTKIso" ) ) {
    produces< vector<pat::Muon> >( "Tag" );
    produces< vector<pat::Muon> >( "Probe" );
}

MuFltr::~MuFltr() {}

bool
MuFltr::filter( edm::Event& iEvent, const edm::EventSetup& iSetup ) {
    iEvent.getByToken( _musrc , _muhandle );
    iEvent.getByToken( _elsrc , _elhandle );
    iEvent.getByToken( _vtxsrc, _vtxhandle );
    iEvent.getByToken( _hltInputTag, _triggerResults );
    iEvent.getByToken( _hltObjectsInputTag, _triggerObjects );

    //initialize the object by handle
    if( _muhandle.isValid() ) {
        _muons = *_muhandle;
    }

    if( _elhandle.isValid() ) {
        _electrons = *_elhandle;
    }

    if( _vtxhandle.isValid() ) {
        _vtx = *_vtxhandle;
    }

    //no loose electrons
    for( const auto& el : _electrons ) {
        if( el.pt() > 10 && fabs( el.eta() ) < 2.5 ) {
            return false;
        }
    }

    //pre-cut on muon pt
    //http://en.cppreference.com/w/cpp/algorithm/remove
    //https://stackoverflow.com/questions/4940259/lambdas-require-capturing-this-to-call-static-member-function
    _muons.erase( std::remove_if( _muons.begin(),
                                  _muons.end(),
    [&]( pat::Muon mu ) {
        return muPreCut( mu );
    } ),
    _muons.end() );

    if( _muons.size() != 2 ) {
        return false;
    }

    //randomly choose the muons for tag
    srand( ( unsigned ) time( NULL ) );
    int first  = rand() % 2;
    int second = ( first + 1 ) % 2;

    //confirm mother in z mass window
    if ( !zParent( _muons ) ) {
        return false;
    }

    //tag preselection
    if( ! ( passId ( _muons[first], _tagid ) && passKin( _muons[first], true ) ) ) {
        return false;
    }

    //probe preselection
    if( ! ( passId ( _muons[second], _proid ) && passKin( _muons[second], false ) ) ) {
        return false;
    }

    if( _useMC ) {
        cout << "useMC" << endl;
    }

    //mark the passing particle
    passTrigger( _muons[first] , iEvent, true );
    passTrigger( _muons[second], iEvent, false );
    unique_ptr< vector<pat::Muon> > tagMuonptr ( new vector< pat::Muon> );
    unique_ptr< vector<pat::Muon> > proMuonptr ( new vector< pat::Muon> );
    tagMuonptr ->push_back( _muons[first] );
    proMuonptr ->push_back( _muons[second] );
    iEvent.put( move( tagMuonptr ) , "Tag" );
    iEvent.put( move( proMuonptr ) , "Probe" );
    return true;
}

void
MuFltr::beginStream( edm::StreamID ) {
}

void
MuFltr::endStream() {
}
void
MuFltr::fillDescriptions( edm::ConfigurationDescriptions& descriptions ) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}
//define this as a plug-in
DEFINE_FWK_MODULE( MuFltr );
