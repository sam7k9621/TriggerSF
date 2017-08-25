#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/FWLite/interface/Handle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"

#include <string>
#include <vector>

class LepEfficiency {

    public:
        LepEfficiency() {}
        virtual ~LepEfficiency() noexcept( false ) {}

        virtual void AddHist( const std::string& title, const std::vector<double>& lst ) {
            TH1D* h = fs->make<TH1D>( title.c_str(), title.c_str(), lst.size()-1, &( lst[0] ) );
            _histmgr.AddObj( h );
        }

        virtual TH1* Hist( const std::string& name ) {
            return _histmgr.GetObj( name );
        }
    private:

        edm::Service<TFileService> fs;
        HistMgr _histmgr;

};

class MuEfficiency : public edm::one::EDAnalyzer<edm::one::SharedResources>, public LepEfficiency  {
    public:
        explicit MuEfficiency( const edm::ParameterSet& );
        ~MuEfficiency();

        static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );



    private:
        virtual void beginJob() override;
        virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
        virtual void endJob() override;

        // ----------member data ---------------------------
        std::vector<edm::ParameterSet> _tagtri;
        std::vector<edm::ParameterSet> _protri;
        const edm::EDGetToken _pro;
        const edm::EDGetToken _tag;
};


class ElEfficiency : public edm::one::EDAnalyzer<edm::one::SharedResources>, public LepEfficiency  {
    public:
        explicit ElEfficiency( const edm::ParameterSet& );
        ~ElEfficiency();

        static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );


    private:
        virtual void beginJob() override;
        virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
        virtual void endJob() override;

        std::vector<edm::ParameterSet> _tagtri;
        std::vector<edm::ParameterSet> _protri;
        const edm::EDGetToken _pro;
        const edm::EDGetToken _tag;
};
