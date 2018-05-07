#include "TLorentzVector.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "TriggerEfficiency/EfficiencyPlot/interface/ElEfficiency.h"

#include <iostream>

using namespace std;


ElEfficiency::ElEfficiency( const edm::ParameterSet& iConfig ) :
    _tagtri( iConfig.getParameter<vector<edm::ParameterSet> >( "tagtrigger" ) ),
    _protri( iConfig.getParameter<vector<edm::ParameterSet> >( "protrigger" ) ),
    _pro( consumes<vector<pat::Electron> >( iConfig.getParameter<edm::InputTag>( "probe" ) ) ),
    _tag( consumes<vector<pat::Electron> >( iConfig.getParameter<edm::InputTag>( "tag" ) ) ),
    _pusrc ( consumes<vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("pusrc") ) ),
    _useMC( iConfig.getParameter<bool>( "useMC" ) ),
    _puweight( ReadWeight(iConfig.getParameter<edm::FileInPath>("filename").fullPath()) )

{
    usesResource( "TFileService" );

    vector<double> pt2Dbin  = {20,40,50,55,60,70,80,100,150,200};
    vector<double> eta2Dbin = {-2.5, -2, -1.566, -1.444, -0.8, 0, 0.8, 1.444, 1.566, 2, 2.5};
    /*****common setting*****/
    for( const auto& tagtri : _tagtri ){
        string triname        = tagtri.getParameter<string>( "name" );
        vector<double> etabin = tagtri.getParameter<vector<double> >( "etabin" );
        vector<double> ptbin  = tagtri.getParameter<vector<double> >( "ptbin" );
        AddHist( "pass_zmass_" + triname,    80, 50, 130 );
        AddHist( "fail_zmass_" + triname,    80, 50, 130 );

        Add2DTEff("eff_pt_eta_" + triname, eta2Dbin, pt2Dbin);
        AddTEff("eff_pt_"  + triname, ptbin);
        AddTEff("eff_eta_" + triname, etabin);
    }
}


ElEfficiency::~ElEfficiency()
{
}


void
ElEfficiency::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
    edm::Handle<vector<pat::Electron> > prohandle;
    edm::Handle<vector<pat::Electron> > taghandle;
    iEvent.getByToken( _pro, prohandle );
    iEvent.getByToken( _tag, taghandle );
    pat::Electron tag = ( *taghandle )[ 0 ];
    pat::Electron pro = ( *prohandle )[ 0 ];

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
        /****common setting****/
        string triname = _tagtri[ i ].getParameter<string>( "name" );
        /****setting for tag****/
        vector<string> hltlist = _tagtri[ i ].getParameter<vector<string> >( "HLT" );
        double ptcut           = _tagtri[ i ].getParameter<double>( "ptcut" );
        double etacut          = _tagtri[ i ].getParameter<double>( "etacut" );
        bool passtag           = false;

        for( const auto& hlt : hltlist ){
            if( tag.hasUserInt( hlt ) && tag.pt() > ptcut && fabs( tag.superCluster()->eta() ) < etacut ){
                passtag = true;
            }
        }

        if( !passtag ){
            continue;
        }

        /****setting for probe****/
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

            if( fabs( pro.superCluster()->eta() ) < etacut ){
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
                    pro.superCluster()->eta() 
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
ElEfficiency::beginJob()
{
}

void
ElEfficiency::endJob()
{
}

void
ElEfficiency::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
    // The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}

// define this as a plug-in
DEFINE_FWK_MODULE( ElEfficiency );
