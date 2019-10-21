import FWCore.ParameterSet.VarParsing as opts
import FWCore.ParameterSet.Config as cms 
import importlib
import os

#-------------------------------------------------------------------------------
#   Environment settings
#-------------------------------------------------------------------------------
CMSSW_BASE = os.environ['CMSSW_BASE']
dir_path   = "/src/TriggerSF/DataFltr/{}/" 

#-------------------------------------------------------------------------------
#   Options settings + Parsing, see python/optionsInit and python/OptionParser
#-------------------------------------------------------------------------------
options = opts.VarParsing ('analysis')

options.register('useMC',
    False,
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.bool,
    'Sample is mc or data')

options.register('lepton',
    "el",
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Which lepton')

options.register('year',
    "16",
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Which year')

options.register('Debug',
     0,
     opts.VarParsing.multiplicity.singleton,
     opts.VarParsing.varType.int,
     'Debugging output level' )

options.setDefault('maxEvents', -1 )
options.setDefault('inputFiles', "file:" + CMSSW_BASE + dir_path.format( "test" )    + 'test.root' )

options.parseArguments()

mysetting = importlib.import_module('TriggerSF.DataFltr.TnP_{}_cfi'.format( options.year ) )
print ">> Importing module TriggerSF.DataFltr.TnP_{}_cfi".format( options.year )
print ">> Running with [ Use MC: {0} | Lepton: {1} | Year: {2} ]".format(options.useMC, options.lepton, options.year )
print ">> Dataset: {}".format( options.inputFiles )

#-------------------------------------------------------------------------------
#   Process Setup
#-------------------------------------------------------------------------------
process = cms.Process("TagAndProbe")
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
if options.Debug :
    process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

if options.useMC :
    process.GlobalTag.globaltag = mysetting.MCGlobalTag
else:
    process.GlobalTag.globaltag = mysetting.DataGlobalTag

print ">> Loading GlobalTag: {}".format( process.GlobalTag.globaltag )

#-------------------------------------------------------------------------------
#   Settings for Egamma Identification and Energy Correction bug fixing
#-------------------------------------------------------------------------------
# ref : https://twiki.cern.ch/twiki/bin/view/CMS/EgammaMiniAODV2#2017_MiniAOD_V2
from RecoEgamma.EgammaTools.EgammaPostRecoTools import setupEgammaPostRecoSeq
setupEgammaPostRecoSeq( process, runEnergyCorrections = False, era = mysetting.ElectronEra )
print '>>Finished Egamma settings'

#-------------------------------------------------------------------------------
#   Parsing 
#-------------------------------------------------------------------------------
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource", fileNames=cms.untracked.vstring(options.inputFiles))
print '>> Finished basic setups...'

#-------------------------------------------------------------------------------
#   Settings for TnP
#-------------------------------------------------------------------------------
if options.lepton == "el":
    process.datafltr = cms.EDFilter(
            "ElFltr",
            mysetting.commontool,
            mysetting.electrontool,
            useMC = cms.bool(options.useMC) # Check whether lepton has gen level lepton and  whether tag and probe has MC truth Z parent 
            )

elif options.lepton == "mu":
    process.datafltr = cms.EDFilter(
            "MuFltr",
            mysetting.commontool,
            mysetting.muontool,
            useMC = cms.bool(options.useMC)   # Check whether lepton has gen level lepton and  whether tag and probe has MC truth Z parent 
            )

process.filterpath = cms.Path(
        process.egammaPostRecoSeq*
        process.datafltr
        )

process.edmOut = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string( "TnP_{}_{}.root".format( options.lepton, options.year ) ),

        # datatype _ module(process.xxx) _ label(comes from .cc) _ process(cms.Process)
        outputCommands=cms.untracked.vstring(
            "keep *",
            "drop *_slimmedPhotons_*_*",
            "drop *_slimmedJets*_*_*",
            "drop *_slimmedTaus_*_*"
            ),
        SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('filterpath'))
        )

process.endPath = cms.EndPath(
        process.edmOut
        )
