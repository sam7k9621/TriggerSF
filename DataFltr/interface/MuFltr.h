#ifndef MUFLTR_H
#define MUFLTR_H

#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <string>
#include <vector>

class MuFltr : public edm::stream::EDFilter<>{
    public:
        explicit MuFltr(const edm::ParameterSet&);
        ~MuFltr();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
    
    private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
       
      /*------muon function------*/
        double delR( const pat::Muon&, const edm::Event&, const std::string& );
        void passTrigger( pat::Muon&, const edm::Event& );

        bool zParent( const pat::MuonCollection& )      const;
        bool passId ( const pat::Muon&, const std::string& ) const;
        bool passKin( const pat::Muon&, const bool& )   const;
        bool passTKIso( const pat::Muon&, const double&) const;
        bool passPFIso( const pat::Muon&, const double&) const;
        bool muPreCut(pat::Muon mu){
            return !(mu.pt()>10 && fabs(mu.eta()) < 2.5);
        }
      /*------common memeber------*/
        const edm::EDGetTokenT<std::vector<pat::Muon>> _musrc;
        const edm::EDGetTokenT<std::vector<pat::Electron>> _elsrc;
        const edm::EDGetTokenT<std::vector<reco::Vertex> > _vtxsrc;
        edm::Handle<std::vector<pat::Muon> > _muhandle;
        edm::Handle<std::vector<pat::Electron> > _elhandle;
        edm::Handle<reco::VertexCollection>  _vtxhandle;

        const edm::EDGetTokenT<edm::TriggerResults> _hltInputTag;
        const edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> _hltObjectsInputTag;
        edm::Handle< edm::TriggerResults > _triggerResults;
        edm::Handle<pat::TriggerObjectStandAloneCollection> _triggerObjects;
        const double _zmin;
        const double _zmax;
        const bool   _useMC;

        const std::string _tagid;
        const std::string _proid;
        const double _tEtaMax;
        const double _pEtaMax;
        const double _tPtMin;
        const double _pPtMin;
        const std::vector<edm::ParameterSet> _trigger;

        /*-------muon member------*/  
        const double _tPFIso;
        const double _pPFIso;
        const double _tTKIso;
        const double _pTKIso;

        /*------common tool------*/
        reco::VertexCollection  _vtx;
        pat::MuonCollection     _muons;
        pat::ElectronCollection _electrons;
};

#endif
