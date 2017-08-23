#include "TriggerEfficiency/DataFltr/interface/DataFltr.h"
#include <algorithm>
#include <iostream>
using namespace std;

DataFltr::DataFltr( const edm::ParameterSet& iConfig ):
    _musrc ( consumes<vector<pat::Muon> >( iConfig.getParameter<edm::InputTag>( "musrc" ) ) ),
    _elsrc ( consumes<vector<pat::Electron> >( iConfig.getParameter<edm::InputTag>( "elsrc" ) ) ),
    _vtxsrc  ( consumes<vector<reco::Vertex> > ( iConfig.getParameter<edm::InputTag >( "vtxsrc" ) ) ),
    _hltInputTag ( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "HLTInputTag" ) ) ),
    _hltObjectsInputTag ( consumes<vector<pat::TriggerObjectStandAlone> >( iConfig.getParameter<edm::InputTag>( "HLTObjectsInputTag" ) ) ),
    _zmin ( iConfig.getParameter<double>( "Zmassmin" ) ),
    _zmax ( iConfig.getParameter<double>( "Zmassmax" ) ),
    _useMC ( iConfig.getParameter<bool>( "useMC" ) ),
    _lepton ( iConfig.getParameter<string>( "lepton_type" ) ),
    _tagid ( iConfig.getParameter<string>( "TagPassID" ) ),
    _proid ( iConfig.getParameter<string>( "ProbePassID" ) ),
    _tEtaMax ( iConfig.getParameter<double>( "tagEtaMax" ) ),
    _pEtaMax ( iConfig.getParameter<double>( "probeEtaMax" ) ),
    _tPtMin ( iConfig.getParameter <double>( "tagPtMin" ) ),
    _pPtMin ( iConfig.getParameter <double>( "probePtMin" ) ),
    _trigger ( iConfig.getParameter<vector<edm::ParameterSet>>( "triggerCache" ) ) 
{
    if(_lepton == "muon"){
        muMgr = new muContainer(iConfig);
        elMgr = 0;
        produces< vector<pat::Muon> >("Tag");
        produces< vector<pat::Muon> >("Probe");
    }
    
    else if(_lepton == "electron"){
        elMgr = new elContainer(iConfig);
        muMgr = 0;
        produces<vector<pat::Electron>>("Tag");
        produces<vector<pat::Electron>>("Probe");
    }
}

DataFltr::~DataFltr(){
    delete muMgr;
    delete elMgr;
}

void
DataFltr::initCommon(edm::Event& iEvent){

    //initialize handles
    iEvent.getByToken(_musrc , _muhandle);
    iEvent.getByToken(_elsrc , _elhandle);
    iEvent.getByToken(_vtxsrc, _vtxhandle);
    iEvent.getByToken(_hltInputTag, _triggerResults);
    iEvent.getByToken(_hltObjectsInputTag, _triggerObjects);

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

}

bool 
DataFltr::muFilter(edm::Event& iEvent){
    
    initCommon(iEvent);

    //no loose electrons
    for(const auto& el : _electrons){
        if(el.pt()>10 && fabs(el.eta()) < 2.5)
            return false;
    }

    //pre-cut on muon pt
    //http://en.cppreference.com/w/cpp/algorithm/remove
    //https://stackoverflow.com/questions/4940259/lambdas-require-capturing-this-to-call-static-member-function
    _muons.erase(std::remove_if(_muons.begin(),
                                _muons.end(),
                                [&](pat::Muon mu){return muPreCut(mu);}),
                 _muons.end());

    if(_muons.size() != 2)
        return false;

    //randomly choose the muons for tag
    srand( (unsigned) time(NULL) );
    int first  = rand()%2;
    int second = (first+1)%2;

    //confirm mother in z mass window
    if (!zParent(_muons))
        return false;

    //tag preselection
    if(!
        (
        passId (_muons[first], _tagid) &&
        passKin(_muons[first], true)   &&
        passPFIso(_muons[first], get<0>(muMgr->getPFIso())) &&
        passTKIso(_muons[first], get<0>(muMgr->getTKIso()))
        )
    )
        return false;

    //probe preselection
    if(!
        (
        passId (_muons[second], _proid) &&
        passKin(_muons[second], false)   &&
        //https://coherence0815.wordpress.com/2015/12/19/tuple-in-c-11/
        passPFIso(_muons[second], get<1>(muMgr->getPFIso())) &&
        passTKIso(_muons[second], get<1>(muMgr->getTKIso()))
        )
    )
        return false;

    if(_useMC)
        cout<<"useMC"<<endl;

    //mark the passing particle
    passTrigger(_muons[first] , iEvent);
    passTrigger(_muons[second], iEvent);

    unique_ptr< vector<pat::Muon> > tagMuonptr ( new vector< pat::Muon> );
    unique_ptr< vector<pat::Muon> > proMuonptr ( new vector< pat::Muon> );
    tagMuonptr ->push_back(_muons[first] );
    proMuonptr ->push_back(_muons[second]);

    iEvent.put( move(tagMuonptr) ,"Tag");
    iEvent.put( move(proMuonptr) ,"Probe");
    return true;

}

bool 
DataFltr::elFilter(edm::Event& iEvent){
   
    initCommon(iEvent);
    elMgr->initHandle(iEvent);
    
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

    if( get<0>( elMgr->getImpact() ) )
        if( !passImpact(electrons[first]) )
            return false;
    if( get<1>( elMgr->getImpact() ) )
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

bool
DataFltr::filter( edm::Event& iEvent, const edm::EventSetup& iSetup ) {
    
    if(_lepton == "muon"){
        return muFilter(iEvent);
    }

    else if(_lepton == "electron"){
        return elFilter(iEvent);
    }

    else
        return false;
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
