import FWCore.ParameterSet.VarParsing as opts
import FWCore.ParameterSet.Config as cms

#-------------------------------------------------------------------------------
#   Options settings + Parsing, see python/optionsInit and python/OptionParser
#-------------------------------------------------------------------------------

options = opts.VarParsing ('analysis')

options.register('sample',
    'file:/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/DataFltr/test/test.root',
    opts.VarParsing.multiplicity.list,
    opts.VarParsing.varType.string,
    'Sample to analyze')

options.register('output',
    'TnP_test.root',
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

print ">>Running with [ MC sample:{0} | {1} ] \n>>Dataset : {2}".format(options.useMC, options.lepton, options.sample)

#-------------------------------------------------------------------------------
#   Process Setup
#-------------------------------------------------------------------------------

process = cms.Process("TagAndProbe")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 100

if options.Debug :
    process.MessageLogger.cerr.FwkReport.reportEvery = 1

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

#-------------------------------------------------------------------------------
#   Parsing 
#-------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.source = cms.Source("PoolSource",
        fileNames=cms.untracked.vstring(options.sample)
        )

##Egamma
if options.lepton == "electron":
    from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
    dataFormat = DataFormat.MiniAOD
    switchOnVIDElectronIdProducer(process, dataFormat)
    # define which IDs we want to produce
    my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronHLTPreselecition_Summer16_V1_cff',
                     'RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff',
                     'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']
    #add them to the VID producer
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
    print '>>Finished Egamma settings'
##

print '>>Finished basic setups...'

#-------------------------------------------------------------------------------
#   Settings for TnP
#-------------------------------------------------------------------------------

from TriggerEfficiency.DataFltr.TnP_cfi import *
if options.lepton == "electron":
    process.datafltr = cms.EDFilter(
            "DataFltr",
            commontool,
            electrontool,
            lepton_type = cms.string(options.lepton),
            useMC = cms.bool(options.useMC)  # Check whether lepton has gen level lepton and  whether tag and probe has MC truth Z parent 
            )

elif options.lepton == "muon":
    process.datafltr = cms.EDFilter(
            "DataFltr",
            commontool,
            muontool,
            lepton_type = cms.string(options.lepton),
            useMC = cms.bool(options.useMC)   # Check whether lepton has gen level lepton and  whether tag and probe has MC truth Z parent 
            )

process.filterpath = cms.Path(
        process.datafltr
        )

process.edmOut = cms.OutputModule(
        "PoolOutputModule",
        fileName = cms.untracked.string(options.output),

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
