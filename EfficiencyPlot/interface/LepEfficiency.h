#ifndef LEPEFFICIENCY_H
#define LEPEFFICIENCY_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <memory>

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TH1.h"
#include "TH2.h"
#include "TEfficiency.h"
#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class LepEfficiency {

    public:

        LepEfficiency(){};
        virtual ~LepEfficiency() noexcept( false ){}

        // https://kheresy.wordpress.com/2014/10/03/override-and-final-in-cpp-11/
        // only virtual funciton can has final specifier ( just to avoid overriding by child class )
        virtual void AddHist(
            const std::string&, 
            const int &,        
            const double&,     
            const double&     
            ) final ;

        /******************************************************************************************************/
        virtual void AddHist( 
                const std::string&, 
                const std::vector<double>& 
                ) final;

        virtual TH1* Hist( const std::string&) final;
        
        /******************************************************************************************************/
        virtual void Add2DHist( 
                const std::string&, 
                const std::vector<double>&, 
                const std::vector<double>& 
                ) final;

        virtual TH2* Hist2D( const std::string&) final;
        
        /******************************************************************************************************/
        virtual void Add2DTEff( 
                const std::string&, 
                const std::vector<double>&, 
                const std::vector<double>&
                ) final;
        
        virtual void AddTEff( 
                const std::string&, 
                const std::vector<double>&
                ) final;
        
        virtual TEfficiency* HistTEff( const std::string&) final;

        /******************************************************************************************************/
        virtual std::vector<double> ReadWeight(const string& filename) final ;
    
    private:

        edm::Service<TFileService> fs;
        HistMgr<TEfficiency> _teffmgr;
        HistMgr<TH1D> _histmgr;
        HistMgr<TH2D> _hist2Dmgr;
        int* a;

};
#endif
