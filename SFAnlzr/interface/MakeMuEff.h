#ifndef MAKEMUEFF_H
#define MAKEMUEFF_H

#include "TriggerSF/SFAnlzr/interface/MakeLepEff.h"

class MakeMuEff : public MakeLepEff 
{
    public:

        explicit MakeMuEff( const edm::ParameterSet& );
        ~MakeMuEff();

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
