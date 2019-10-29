import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts
import importlib
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
    "el",
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    'Which lepton')

options.register('lumimask',
    "",
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    )

options.register('year',
    "",
    opts.VarParsing.multiplicity.singleton,
    opts.VarParsing.varType.string,
    )

options.register('Debug',
     0,
     opts.VarParsing.multiplicity.singleton,
     opts.VarParsing.varType.int,
     'Debugging output level' )

options.setDefault('maxEvents', -1 )
options.parseArguments()

#-------------------------------------------------------------------------------
#   Process Setup
#-------------------------------------------------------------------------------
process = cms.Process("makeEff")
process.load("FWCore.MessageService.MessageLogger_cfi")
mysetting = importlib.import_module('TriggerSF.SFAnlzr.HLT_{}_eff_cfi'.format( options.year ) )
print ">> Running with [ MC sample:{0} | {1}  | HLT_{2}_eff_cfi ]".format( options.useMC, options.lepton, options.year )
print ">> Dataset : {0}".format(options.inputFiles)

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

if not options.useMC :
    import FWCore.PythonUtilities.LumiList as LumiList
    process.source.lumisToProcess = LumiList.LumiList(filenmae = options.lumimask ).getVLuminosityBlockRange()
    print ">> Finished applying lumi mask {}".format( options.lumimask )

#-------------------------------------------------------------------------------
#   Settings for Analyzer
#-------------------------------------------------------------------------------

if options.lepton == "el":
    process.makeEff = cms.EDAnalyzer(
            "MakeElEff",
            mysetting.eltool,
            useMC = cms.bool(options.useMC),
            pusrc = cms.InputTag("slimmedAddPileupInfo"),
            genevtsrc = cms.InputTag('generator'),
            filename = cms.FileInPath("CPVAnalysis/BaseLineSelector/data/pileupweights_{}_69200.csv".format( options.year ) )
            )
elif options.lepton == "mu":
    process.makeEff = cms.EDAnalyzer(
            "MakeMuEff",
            mysetting.mutool,
            useMC = cms.bool(options.useMC),
            pusrc = cms.InputTag("slimmedAddPileupInfo"),
            genevtsrc = cms.InputTag('generator'),
            filename = cms.FileInPath("CPVAnalysis/BaseLineSelector/data/pileupweights_{}_69200.csv".format( options.year ) )
            )

process.TFileService = cms.Service("TFileService",
        fileName=cms.string(options.output),
        closeFileFast = cms.untracked.bool(True)
)


process.p = cms.Path(process.makeEff)
