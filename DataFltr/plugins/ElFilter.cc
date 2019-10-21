#include "TriggerSF/DataFltr/interface/ElFltr.h"

#include <iostream>

using namespace std;

ElFltr::ElFltr( const edm::ParameterSet& iConfig ) :
    _musrc( consumes<vector<pat::Muon> >( iConfig.getParameter<edm::InputTag>( "musrc" ) ) ),
    _elsrc( consumes<vector<pat::Electron> >( iConfig.getParameter<edm::InputTag>( "elsrc" ) ) ),
    _vtxsrc( consumes<vector<reco::Vertex> >( iConfig.getParameter<edm::InputTag>( "vtxsrc" ) ) ),
    _hltInputTag( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "HLTInputTag" ) ) ),
    _hltObjectsInputTag( consumes<vector<pat::TriggerObjectStandAlone> >( iConfig.getParameter<edm::InputTag>( "HLTObjectsInputTag" ) ) ),
    _zmin( iConfig.getParameter<double>( "Zmassmin" ) ),
    _zmax( iConfig.getParameter<double>( "Zmassmax" ) ),
    _useMC( iConfig.getParameter<bool>( "useMC" ) ),
    _tagid( iConfig.getParameter<string>( "TagPassID" ) ),
    _proid( iConfig.getParameter<string>( "ProbePassID" ) ),
    _tEtaMax( iConfig.getParameter<double>( "tagEtaMax" ) ),
    _pEtaMax( iConfig.getParameter<double>( "probeEtaMax" ) ),
    _tPtMin( iConfig.getParameter <double>( "tagPtMin" ) ),
    _pPtMin( iConfig.getParameter <double>( "probePtMin" ) ),
    _trigger( iConfig.getParameter<vector<edm::ParameterSet> >( "triggerCache" ) ),
    _pImpact( iConfig.getParameter <bool>( "probeImpact" ) ),
    _tImpact( iConfig.getParameter <bool>( "tagImpact" ) ),
    _electronID_vetomap( iConfig.getParameter<string>( "eleVetoIdMap"    ) ),
    _electronID_loosemap( iConfig.getParameter<string>( "eleLooseIdMap"   ) ),
    _electronID_mediummap( iConfig.getParameter<string>( "eleMediumIdMap"  ) ),
    _electronID_tightmap( iConfig.getParameter<string>( "eleTightIdMap"   ) ),
    _electronID_HEEPmap( iConfig.getParameter<string>( "eleHEEPIdMap"    ) )
{
    produces<vector<pat::Electron> >( "Tag" );
    produces<vector<pat::Electron> >( "Probe" );
}

ElFltr::~ElFltr(){}

bool
ElFltr::filter( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
    iEvent.getByToken( _musrc,              _muhandle );
    iEvent.getByToken( _elsrc,              _elhandle );
    iEvent.getByToken( _vtxsrc,             _vtxhandle );
    iEvent.getByToken( _hltInputTag,        _triggerResults );
    iEvent.getByToken( _hltObjectsInputTag, _triggerObjects );

    if( _vtxhandle.isValid() ){
        _vtx = *_vtxhandle;
    }

    // no loose muons
    for( const auto& mu : *_muhandle ){
        if( mu.pt() > 8 ){
            return false;
        }
    }

    // pre-cut on electron pt
    pat::ElectronCollection electrons;
    vector<edm::Ptr<pat::Electron> > elptr;

    for( auto it_el = _elhandle->begin(); it_el != _elhandle->end(); ++it_el ){
        if( it_el->pt() > 8 ){

            electrons.push_back( *it_el );
            elptr.push_back( edm::Ptr<pat::Electron>( _elhandle, it_el - _elhandle->begin() ) );
        }
    }

    if( electrons.size() != 2 ){
        return false;
    }

    // randomly choose the electrons for tag
    srand( (unsigned)time( NULL ) );
    int first  = rand() % 2;
    int second = ( first + 1 ) % 2;

    // confirm mother in z mass window
    if( !zParent( electrons ) ){
        return false;
    }

    // tag preselection
    if( !
        (
            passId( elptr[first], _tagid) &&
            passKin( electrons[ first ], true )
        )
        )
    {
        return false;
    }

    // probe preselection
    if( !
        (
            passId( elptr[second], _proid) &&
            passKin( electrons[ second ], false )
        )
        )
    {
        return false;
    }

    if( _tImpact ){
        if( !passImpact( electrons[ first ] ) ){
            return false;
        }
    }

    if( _pImpact ){
        if( !passImpact( electrons[ second ] ) ){
            return false;
        }
    }

    if( _useMC ){
        if( electrons[ first ].genLepton()== NULL || electrons[ second ].genLepton()== NULL)
            return false;

        if( !( GetDirectMother( electrons[ first ].genLepton(), 23 ) == GetDirectMother( electrons[ second ].genLepton(), 23 ) ) )
            return false;
    }

    // to mark the electron that pass the criteria
    passTrigger( electrons[ first ],  iEvent );
    passTrigger( electrons[ second ], iEvent );
    unique_ptr<vector<pat::Electron> > tagEleptr( new vector<pat::Electron> );
    unique_ptr<vector<pat::Electron> > proEleptr( new vector<pat::Electron> );
    tagEleptr->push_back( electrons[ first ] );
    proEleptr->push_back( electrons[ second ] );
    // output.root label name
    iEvent.put( move( tagEleptr ), "Tag" );
    iEvent.put( move( proEleptr ), "Probe" );
    

    return true;
}


void
ElFltr::beginStream( edm::StreamID )
{
}

void
ElFltr::endStream()
{
}
void
ElFltr::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
    // The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}
// define this as a plug-in
DEFINE_FWK_MODULE( ElFltr );
