#ifndef MUEFFICIENCY_H
#define MUEFFICIENCY_H

#include "TriggerEfficiency/EfficiencyPlot/interface/LepEfficiency.h"

class MuEfficiency : public edm::one::EDAnalyzer<edm::one::SharedResources>,
                     public LepEfficiency  {
    public:

        MuEfficiency( const edm::ParameterSet& );
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
        const edm::EDGetToken _pusrc;

        bool _useMC;
        std::vector<double> _puweight;
};

#endif
