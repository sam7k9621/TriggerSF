#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include "FWCore/Framework/interface/EDConsumerBase.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "RecoEgamma/EgammaTools/interface/EffectiveAreas.h"

#include <string>
#include <tuple>

class elContainer : public edm::EDConsumerBase {

    public:

        elContainer( const edm::ParameterSet& );
        ~elContainer() {}

        elContainer( const elContainer& )            = delete;
        elContainer& operator=( const elContainer& ) = delete;

        void initHandle( const edm::Event& ) ;
        std::tuple<bool, bool> getImpact() const;
        edm::ValueMap<bool> getIDMap( const std::string& );

    private:

        const bool _pImpact;
        const bool _tImpact;

        const edm::EDGetTokenT<edm::ValueMap<bool>> _looseMapToken;
        edm::Handle<edm::ValueMap<bool>> _looseMapHandle;
        const edm::EDGetTokenT<edm::ValueMap<bool>> _mediumMapToken;
        edm::Handle<edm::ValueMap<bool>> _mediumMapHandle;
        const edm::EDGetTokenT<edm::ValueMap<bool>> _tightMapToken;
        edm::Handle<edm::ValueMap<bool>> _tightMapHandle;
        const edm::EDGetTokenT<edm::ValueMap<bool>> _heepMapToken;
        edm::Handle<edm::ValueMap<bool>> _heepMapHandle;

        const edm::EDGetTokenT<edm::ValueMap<bool>> _safeCutToken;
        edm::Handle<edm::ValueMap<bool>> _safecutHandle;
};

class muContainer {

    public:

        muContainer( const edm::ParameterSet& );
        ~muContainer() {}

        muContainer( const muContainer& )            = delete;
        muContainer& operator=( const muContainer& ) = delete;

        bool passTKIso( const pat::Muon& , const double& ) const;
        bool passPFIso( const pat::Muon& , const double& ) const;

    private:

        const double _tPFIso;
        const double _pPFIso;
        const double _tTKIso;
        const double _pTKIso;
};

#endif
