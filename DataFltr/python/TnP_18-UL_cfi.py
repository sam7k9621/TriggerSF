import FWCore.ParameterSet.Config as cms

DataGlobalTag = '106X_dataRun2_v32'
MCGlobalTag   = '106X_upgrade2018_realistic_v15_L1v1'
ElectronEra   = '2018-UL'

commontool = cms.PSet(
        #source
        musrc = cms.InputTag( "slimmedMuons" ),
        elsrc = cms.InputTag( "slimmedElectrons"),

        #trigger object
        vtxsrc = cms.InputTag("offlineSlimmedPrimaryVertices"),
        HLTInputTag = cms.InputTag('TriggerResults::HLT'),
        HLTObjectsInputTag = cms.InputTag('slimmedPatTrigger'),

        #customized cut
        Zmassmin = cms.double(60),
        Zmassmax = cms.double(120),
        )

electrontool = cms.PSet(
        #trigger we used
        triggerCache = cms.VPSet(
            cms.PSet(
                HLTName = cms.string("HLT_Ele32_WPTight_Gsf"),
                FilterName = cms.string("hltEle32WPTightGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Photon200"),
                FilterName = cms.string("hltEG200HEFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele115_CaloIdVT_GsfTrkIdT"),
                FilterName = cms.string("hltEle115CaloIdVTGsfTrkIdTGsfDphiFilter")
            )
        ),

        #tag criteria
        TagPassID = cms.string("tight"),   #Require tag electron to pass a ID ( input will be "loose"/"tight"/"medium"/"heep" )
        tagImpact = cms.bool(True),

        #probe criteria
        ProbePassID = cms.string("tight"),  #Require probe electron to pass a ID ( input will be "loose"/"tight"/"medium"/"heep" )
        probeImpact = cms.bool(True),

        eleVetoIdMap           = cms.string('cutBasedElectronID-Fall17-94X-V2-veto'),
        eleLooseIdMap          = cms.string('cutBasedElectronID-Fall17-94X-V2-loose'),
        eleMediumIdMap         = cms.string('cutBasedElectronID-Fall17-94X-V2-medium'),
        eleTightIdMap          = cms.string('cutBasedElectronID-Fall17-94X-V2-tight'),
        eleHEEPIdMap           = cms.string('heepElectronID-HEEPV70'),

        #customized cut
        tagPtMin = cms.double(34),
        tagEtaMax = cms.double(2.5),
        probePtMin = cms.double(8),
        probeEtaMax = cms.double(2.5)
       )

muontool = cms.PSet(
        #trigger we used
        triggerCache = cms.VPSet(
            cms.PSet(
                HLTName = cms.string("HLT_Mu50_v*"),
                FilterName = cms.string("hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q"),
                TKIso = cms.bool(False),
                PFIso = cms.bool(False)
            )
        ),

        #https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2#Tight_Muon
        #tag criteria
        TagPassID  = cms.string("loose"),       #Require tag muon to pass a ID ( input will be "loose"/"tight"/"HighPT" )
        TagPassPFIso = cms.double(0.15),        #Require tag muon to pass tight particle flow isolation cut
        TagPassTKIso = cms.double(0.05),        #Require tag muon to pass tight tracking isolation cut

        #probe criteria
        ProbePassID  = cms.string("loose"),     #Require probe muon to pass a ID ( input will be "loose"/"tight"/"HighPT" )
        ProbePassPFIso = cms.double(0.15),      #Require probe muon to pass tight particle flow isolation cut
        ProbePassTKIso = cms.double(0.05),      #Require probe muon to pass tight tracking isolation cut

        #customized cut
        tagPtMin = cms.double(30),
        tagEtaMax = cms.double(2.4),
        probePtMin = cms.double(8),
        probeEtaMax = cms.double(2.4)
        )

