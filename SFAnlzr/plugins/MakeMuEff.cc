#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "TriggerSF/SFAnlzr/interface/MakeMuEff.h"
#include "TLorentzVector.h"

using namespace std;


MakeMuEff::MakeMuEff( const edm::ParameterSet& iConfig ) :
    _tagtri( iConfig.getParameter<vector<edm::ParameterSet> >( "tagtrigger" ) ),
    _protri( iConfig.getParameter<vector<edm::ParameterSet> >( "protrigger" ) ),
    _pro( consumes<vector<pat::Muon> >( iConfig.getParameter<edm::InputTag>( "probe" ) ) ),
    _tag( consumes<vector<pat::Muon> >( iConfig.getParameter<edm::InputTag>( "tag" ) ) ),
    _pusrc ( consumes<vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("pusrc") ) ),
    _useMC( iConfig.getParameter<bool>( "useMC" ) ),
    _puweight( ReadWeight(iConfig.getParameter<edm::FileInPath>("filename").fullPath()) )
{
    usesResource( "TFileService" );

    /*****common setting*****/
    for( const auto& tagtri : _tagtri ){
        string triname        = tagtri.getParameter<string>( "name" );
        vector<double> etabin = tagtri.getParameter<vector<double> >( "etabin" );
        vector<double> ptbin  = tagtri.getParameter<vector<double> >( "ptbin" );
        AddHist( "pass_zmass_" + triname,    80, 50, 130 );
        AddHist( "fail_zmass_" + triname,    80, 50, 130 );

        Add2DTEff("eff_pt_eta_" + triname, etabin, ptbin);
        AddTEff("eff_pt_"  + triname, ptbin);
        AddTEff("eff_eta_" + triname, etabin);
    }
}


MakeMuEff::~MakeMuEff()
{
}


void
MakeMuEff::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
    edm::Handle<vector<pat::Muon> > prohandle;
    edm::Handle<vector<pat::Muon> > taghandle;
    iEvent.getByToken( _pro, prohandle );
    iEvent.getByToken( _tag, taghandle );
    pat::Muon tag = ( *taghandle )[ 0 ];
    pat::Muon pro = ( *prohandle )[ 0 ];

    edm::Handle<vector<PileupSummaryInfo> > puhandle;
    double weight = 1.0;
    if(_useMC){
        iEvent.getByToken( _pusrc, puhandle );
        const unsigned pu = puhandle->at( 0 ).getPU_NumInteractions();
        if( pu < _puweight.size() ){
            weight = _puweight.at( pu );
        }
    }
    
    TLorentzVector tagLV( tag.px(), tag.py(), tag.pz(), tag.energy() );
    TLorentzVector proLV( pro.px(), pro.py(), pro.pz(), pro.energy() );
    double zmass = ( tagLV + proLV ).M();
    
    for( int i = 0; i < (int)_tagtri.size(); i++ ){
        /*****common setting*****/
        string triname = _tagtri[ i ].getParameter<string>( "name" );
        /*****setting for tag*****/
        vector<string> hltlist = _tagtri[ i ].getParameter<vector<string> >( "HLT" );
        double ptcut           = _tagtri[ i ].getParameter<double>( "ptcut" );
        double etacut          = _tagtri[ i ].getParameter<double>( "etacut" );
        bool passtag           = false;

        for( const auto& hlt : hltlist ){
            if( tag.hasUserInt( hlt ) && tag.pt() > ptcut && fabs( tag.eta() ) < etacut ){
                passtag = true;
            }
        }

        if( !passtag ){
            continue;
        }

        /*****another cut for isolation or tkiso considering HLT*********/
        /*****setting for probe*****/
        hltlist = _protri[ i ].getParameter<vector<string> >( "HLT" );
        ptcut   = _protri[ i ].getParameter<double>( "ptcut" );
        etacut  = _protri[ i ].getParameter<double>( "etacut" );

        for( const auto& hlt : hltlist ){
            
            HistTEff( "eff_pt_eta_" + triname )->FillWeighted( 
                    pro.hasUserInt( hlt ), 
                    weight, 
                    pro.superCluster()->eta(), 
                    pro.pt() 
                    );

            if( fabs( pro.eta() ) < etacut ){
                HistTEff( "eff_pt_" + triname )->FillWeighted( 
                        pro.hasUserInt( hlt ), 
                        weight, 
                        pro.pt() 
                        );
            }

            if( pro.pt() > ptcut ){
            HistTEff( "eff_eta_" + triname )->FillWeighted( 
                    pro.hasUserInt( hlt ), 
                    weight, 
                    pro.eta() 
                    );
            }
        }

        //check Z backgorund
        if( fabs( pro.superCluster()->eta() ) < etacut && pro.pt() > ptcut){

            for( const auto& hlt : hltlist ){
                if( pro.hasUserInt( hlt ) ){
                    Hist( "pass_zmass_" + triname )->Fill( zmass, weight );
                }

                else{
                    Hist( "fail_zmass_" + triname )->Fill( zmass, weight );
                }
            }
        }
    }
}

void
MakeMuEff::beginJob()
{
}

void
MakeMuEff::endJob()
{
}

void
MakeMuEff::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
    // The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}

// define this as a plug-in
DEFINE_FWK_MODULE( MakeMuEff );
