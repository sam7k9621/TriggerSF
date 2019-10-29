#ifndef MAKEELEFF_H
#define MAKEELEFF_H

#include "TriggerSF/SFAnlzr/interface/MakeLepEff.h"

class MakeElEff : 
    public edm::one::EDAnalyzer<edm::one::SharedResources>,
    public MakeLepEff
{
    public:

        explicit MakeElEff( const edm::ParameterSet& );
        ~MakeElEff();

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
        const edm::EDGetToken _genevtsrc;

        bool _useMC;
        std::vector<double> _puweight;
};

#endif
