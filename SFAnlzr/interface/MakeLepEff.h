#ifndef MAKELEPEFF_H
#define MAKELEPEFF_H

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

#include "ManagerUtils/HistMgr/interface/RootObjMgr.h"
#include "TH1.h"
#include "TH2.h"
#include "TEfficiency.h"

struct nop
{
    template <typename T>
    void operator() (T const &) const noexcept { }
};

template <typename T>
using nop_unique_ptr = std::unique_ptr<T, nop>;

class MakeLepEff
{
    public:

        MakeLepEff(){};
        virtual ~MakeLepEff() noexcept( false ){}

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
        virtual std::vector<double> ReadWeight(const std::string& filename) final ;
        edm::Service<TFileService> fs;
    
    private:

        mgr::RootObjMgr<TEfficiency, nop_unique_ptr<TEfficiency>> _teffmgr;
        mgr::RootObjMgr<TH1D, nop_unique_ptr<TH1D>>        _histmgr;
        mgr::RootObjMgr<TH2D, nop_unique_ptr<TH2D>>        _hist2Dmgr;
};
#endif
