#ifndef ELFTR_H
#define ELFTR_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include <memory>

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <string>
#include <vector>

class ElFltr : public edm::stream::EDFilter<> {
    public:

        explicit ElFltr( const edm::ParameterSet& );
        ~ElFltr();

        static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

    private:

        virtual void beginStream( edm::StreamID ) override;
        virtual bool filter( edm::Event&, const edm::EventSetup& ) override;
        virtual void endStream() override;

        /*------electron function------*/
        double delR( const pat::Electron&, const edm::Event&, const std::string& );
        void   passTrigger( pat::Electron&, const edm::Event& );

        bool zParent( pat::ElectronCollection& ) const;
        bool passKin( const pat::Electron&, const bool& ) const;
        bool passId( const edm::Ptr<pat::Electron>&, const std::string& );
        bool passImpact( const pat::Electron& ) const;

        /*------common memeber------*/
        const edm::EDGetTokenT<std::vector<pat::Muon> > _musrc;
        const edm::EDGetTokenT<std::vector<pat::Electron> > _elsrc;
        const edm::EDGetTokenT<std::vector<reco::Vertex> > _vtxsrc;
        edm::Handle<std::vector<pat::Muon> > _muhandle;
        edm::Handle<std::vector<pat::Electron> > _elhandle;
        edm::Handle<reco::VertexCollection> _vtxhandle;

        const edm::EDGetTokenT<edm::TriggerResults> _hltInputTag;
        const edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> _hltObjectsInputTag;
        edm::Handle<edm::TriggerResults> _triggerResults;
        edm::Handle<pat::TriggerObjectStandAloneCollection> _triggerObjects;

        const double _zmin;
        const double _zmax;
        const bool _useMC;

        const std::string _tagid;
        const std::string _proid;
        const double _tEtaMax;
        const double _pEtaMax;
        const double _tPtMin;
        const double _pPtMin;
        const std::vector<edm::ParameterSet> _trigger;

        /*-------electron member------*/
        const bool _pImpact;
        const bool _tImpact;
        
        const std::string _electronID_vetomap;
        const std::string _electronID_loosemap;
        const std::string _electronID_mediummap;
        const std::string _electronID_tightmap;
        const std::string _electronID_HEEPmap;

        /*------common tool------*/
        reco::VertexCollection _vtx;
        const reco::Candidate* GetDirectMother( const reco::Candidate*, int );
};


#endif
