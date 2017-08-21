#ifndef DATACONTAINER_HPP
#define DATACONTAINER_HPP

class elContainer{
    
    public:

        elContainer();
        ~elContainer();

        elContainer(const elContainer&)            = delete;
        elContainer& operator=(const elContainer&) = delete;

    private:

        const bool _pImpact;
        const bool _tImpact;

        const edm::EDGetTokenT<edm::ValueMap<bool>> looseMapToken;
        edm::Handle<edm::ValueMap<bool>> looseMapHandle;
        const edm::EDGetTokenT<edm::ValueMap<bool>> mediumMapToken;
        edm::Handle<edm::ValueMap<bool>> mediumMapHandle;
        const edm::EDGetTokenT<edm::ValueMap<bool>> tightMapToken;
        edm::Handle<edm::ValueMap<bool>> tightMapHandle;
        const edm::EDGetTokenT<edm::ValueMap<bool>> heepMapToken;
        edm::Handle<edm::ValueMap<bool>> heepMapHandle;

        const edm::EDGetTokenT<edm::ValueMap<bool>> safeCutToken;
        edm::Handle<edm::ValueMap<bool>> safecutHandle;
};

class muContainer{

    public:

        muContainer();
        ~muContainer();

        muContainer(const muContainer&)            = delete;
        muContainer& operator=(const muContainer&) = delete;

        bool passTKIso(const pat::Muon& ,const double ) const;
        bool passPFIso(const pat::Muon& ,const double ) const;
    
    private:

        const double _tPFIso;
        const double _pPFIso;
        const double _tTKIso;
        const double _pTKIso;
};
