import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts

#-------------------------------------------------------------------------------
#   Options settings + Parsing, see python/optionsInit and python/OptionParser
#-------------------------------------------------------------------------------

options = opts.VarParsing ('analysis')

options.register('sample',
    'file:/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/EfficiencyPlot/test/TnP_test_electron.root',
    opts.VarParsing.multiplicity.list,
    opts.VarParsing.varType.string,
    'Sample to analyze')

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

options.register('version',
    "",
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Which version HLT menu')

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

process = cms.Process("Demo")
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
    process.source.lumisToProcess = LumiList.LumiList(filename = '/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/EfficiencyPlot/test/'+options.version).getVLuminosityBlockRange()
    print ">>Finished apply lumi mask"

#-------------------------------------------------------------------------------
#   Settings for Analyzer
#-------------------------------------------------------------------------------

from TriggerEfficiency.EfficiencyPlot.LepAnlzer_cfi import *

if options.lepton == "electron":
    process.demo = cms.EDAnalyzer(
            "ElEfficiency",
            electrontool,
            useMC = cms.bool(options.useMC)
            )
elif options.lepton == "muon":
    process.demo = cms.EDAnalyzer(
            "MuEfficiency",
            muontool,
            useMC = cms.bool(options.useMC)
            )

process.TFileService = cms.Service("TFileService",
        fileName=cms.string(options.output),
        closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(process.demo)
