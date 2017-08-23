#include "TriggerEfficiency/DataFltr/interface/ElFltr.h" 

using namespace std;

ElFltr::ElFltr(const edm::ParameterSet& iConfig):
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
    _pImpact( iConfig.getParameter < bool > ( "probeImpact" ) ),
    _tImpact( iConfig.getParameter < bool > ( "tagImpact" ) ),
    _looseMapToken ( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "looseMap" ) ) ),
    _mediumMapToken( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "mediumMap" ) ) ),
    _tightMapToken ( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "tightMap" ) ) ),
    _heepMapToken  ( consumes < edm::ValueMap < bool >> ( iConfig.getParameter < edm::InputTag > ( "heepMap" ) ) )
{
        produces<vector<pat::Electron>>("Tag");
        produces<vector<pat::Electron>>("Probe");
}

ElFltr::~ElFltr(){}

bool
ElFltr::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    iEvent.getByToken(_musrc , _muhandle);
    iEvent.getByToken(_elsrc , _elhandle);
    iEvent.getByToken(_vtxsrc, _vtxhandle);
    iEvent.getByToken(_hltInputTag, _triggerResults);
    iEvent.getByToken(_hltObjectsInputTag, _triggerObjects);
    iEvent.getByToken( _looseMapToken , _looseMapHandle );
    iEvent.getByToken( _mediumMapToken, _mediumMapHandle );
    iEvent.getByToken( _tightMapToken , _tightMapHandle );
    iEvent.getByToken( _heepMapToken  , _heepMapHandle  );
    //initialize the object by handle
    if(_muhandle.isValid())
    {
        _muons = *_muhandle;
    }
    if(_elhandle.isValid())
    {
        _electrons = *_elhandle;
    }
    if(_vtxhandle.isValid() )
    {
        _vtx = *_vtxhandle;
    }

    //no loose muons
    for(const auto& mu : _muons){
        if(mu.pt()>8)
            return false;
    }

    //pre-cut on electron pt
    vector <int> eId;
    pat::ElectronCollection electrons;

    for(int i=0; i< (int)_electrons.size(); i++)
    {
        const edm::Ptr<pat::Electron> elptr(_elhandle,i);
        if(_electrons[i].pt()>8 ){ 
            electrons.push_back(_electrons[i]);
            eId.push_back(i);             // to store the original position in the handle
        }
    }

    if(electrons.size() != 2)
        return false;

    //randomly choose the electrons for tag
    srand( (unsigned) time(NULL) );
    int first  = rand()%2;
    int second = (first+1)%2;

    //build the Ptr of tag&probe
    const edm::Ptr<pat::Electron> tElptr(_elhandle,eId[first]);
    const edm::Ptr<pat::Electron> pElptr(_elhandle,eId[second]);

    //confirm mother in z mass window
    if( !zParent(electrons) )
        return false;

    //tag preselection
    if(!
        (
         //passId( tElptr, _tagid) &&
         passKin(electrons[first], true) 
        )
      )
        return false;

    //probe preselection
    if(!
        (
         //passId( pElptr, _proid) &&
         passKin(electrons[second], false)
        )
      )
        return false;

    if( _tImpact )
        if( !passImpact(electrons[first]) )
            return false;
    if( _pImpact )
        if( !passImpact(electrons[second]) )
            return false;

    if(_useMC)
        cout<<"useMC"<<endl;

    //to mark the electron that pass the criteria
    passTrigger(electrons[first] ,iEvent);
    passTrigger(electrons[second],iEvent);

    unique_ptr< vector< pat::Electron> > tagEleptr ( new vector< pat::Electron> );
    unique_ptr< vector< pat::Electron> > proEleptr ( new vector< pat::Electron> );
    tagEleptr->push_back(electrons[first]);
    proEleptr->push_back(electrons[second]);
    
    //output.root label name
    iEvent.put( move(tagEleptr) , "Tag");
    iEvent.put( move(proEleptr) , "Probe");
    return true;


}


void
ElFltr::beginStream(edm::StreamID)
{
}

void
ElFltr::endStream() {
}
void
ElFltr::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(ElFltr);
