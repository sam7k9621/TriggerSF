#ifndef ELEFFICIENCY_H
#define ELEFFICIENCY_H

#include "TriggerEfficiency/EfficiencyPlot/interface/LepEfficiency.h"

class ElEfficiency : public edm::one::EDAnalyzer<edm::one::SharedResources>,
                     public LepEfficiency  {
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
        const edm::EDGetToken _pusrc;

        bool _useMC;
        std::vector<double> _puweight;

};
#endif
