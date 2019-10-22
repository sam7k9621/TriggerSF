import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

#-------------------------------------------------------------------------------
#   Options settings + Parsing, see python/optionsInit and python/OptionParser
#-------------------------------------------------------------------------------

options = opts.VarParsing ('analysis')

options.register('output',
    'Efficiency_test_electron.root',
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Output label')

options.register('useMC',
    False,
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.bool,
    'Sample is mc or data')

options.register('lepton',
    "electron",
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Which lepton')

options.register('Debug',
     0,
     opts.VarParsing.multiplicity.singleton,
     opts.VarParsing.varType.int,
     'Debugging output level' )

options.setDefault('maxEvents', -1 )

options.parseArguments()

print ">>Running with [ MC sample:{0} | {1} ] \n>>Dataset : {2}".format(options.useMC, options.lepton, options.inputFiles)

#-------------------------------------------------------------------------------
#   Process Setup
#-------------------------------------------------------------------------------

process = cms.Process("makeEff")
process.load("FWCore.MessageService.MessageLogger_cfi")

#-------------------------------------------------------------------------------
#   Parsing 
#-------------------------------------------------------------------------------

process.MessageLogger.cerr.FwkReport.reportEvery = 100
if options.Debug :
    process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.inputFiles)
)
print '>>Finished basic setups...'

if not options.useMC :
    import FWCore.PythonUtilities.LumiList as LumiList
    process.source.lumisToProcess = LumiList.LumiList(filename = '/wk_cms2/sam7k9621/CMSSW_9_4_0_patch1/src/TriggerEfficiency/EfficiencyPlot/test/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt').getVLuminosityBlockRange()
    print ">>Finished apply lumi mask"

#-------------------------------------------------------------------------------
#   Settings for Analyzer
#-------------------------------------------------------------------------------

from TriggerEfficiency.EfficiencyPlot.LepAnlzer_cfi import *

if options.lepton == "el":
    process.makeEff = cms.EDAnalyzer(
            "MakeElEff",
            electrontool,
            useMC = cms.bool(options.useMC),
            pusrc = cms.InputTag("slimmedAddPileupInfo"),
            genevtsrc = cms.InputTag('generator'),
            filename = cms.FileInPath("TriggerEfficiency/EfficiencyPlot/test/pileupweights_69200.csv")
            )
elif options.lepton == "mu":
    process.makeEff = cms.EDAnalyzer(
            "MakeMuEff",
            muontool,
            useMC = cms.bool(options.useMC),
            pusrc = cms.InputTag("slimmedAddPileupInfo"),
            genevtsrc = cms.InputTag('generator'),
            filename = cms.FileInPath("TriggerEfficiency/EfficiencyPlot/test/pileupweights_69200.csv")
            )

process.TFileService = cms.Service("TFileService",
        fileName=cms.string(options.output),
        closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(process.makeEff)
