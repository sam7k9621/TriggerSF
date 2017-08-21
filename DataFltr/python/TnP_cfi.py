import FWCore.ParameterSet.Config as cms

commontool = cms.PSet(
        #source
        musrc = cms.InputTag( "slimmedMuons" ),
        elsrc = cms.InputTag( "slimmedElectrons"),

        #trigger object
        vtxsrc = cms.InputTag("offlineSlimmedPrimaryVertices"),
        HLTInputTag = cms.InputTag('TriggerResults','','HLT'),
        HLTObjectsInputTag = cms.InputTag('slimmedPatTrigger'),

       #customized cut
       Zmassmin = cms.double(60),
       Zmassmax = cms.double(120),
       tagPtMin = cms.double(30),
       tagEtaMax = cms.double(2.5),
       probePtMin = cms.double(8),
       probeEtaMax = cms.double(2.5),
       useMC = cms.bool(False)   # Check whether lepton has gen level lepton and  whether tag and probe has MC truth Z parent 
        )

electrontool = cms.PSet(
        #trigger we used
        triggerCache = cms.VPSet(
            cms.PSet(
                HLTName = cms.string("HLT_Ele35_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle35noerWPTightGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele38_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle38noerWPTightGsfTrackIsoFilter")
            ),
            cms.PSet(
                HLTName = cms.string("HLT_Ele40_WPTight_Gsf_v*"),
                FilterName = cms.string("hltEle40noerWPTightGsfTrackIsoFilter")
            )
        ),

       #tag criteria
       TagPassID = cms.string("tight"),   #Require tag electron to pass a ID ( input will be "loose"/"tight"/"medium"/"heep" )
       tagImpact = cms.bool(True),

       #probe criteria
       ProbePassID = cms.string("tight"),  #Require probe electron to pass a ID ( input will be "loose"/"tight"/"medium"/"heep" )
       probeImpact = cms.bool(True),

       #safe cut and ID
       #https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Recipe80X
       looseMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose"),
       mediumMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium"),
       tightMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight"),
       heepMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
       safeCut = cms.InputTag("egmGsfElectronIDs:cutBasedElectronHLTPreselection-Summer16-V1"),

       )

muontool = cms.PSet(
        #trigger we used
        triggerCache = cms.VPSet(
            cms.PSet(
                HLTName = cms.string("HLT_Mu50_v*"),
                FilterName = cms.string("hltL3fL1sMu22Or25L1f0L2f10QL3Filtered50Q")
            )
        ),

        #tag criteria
        TagPassID  = cms.string("tight"),       #Require tag muon to pass a ID ( input will be "loose"/"tight"/"HighPT" )
        TagPassPFIso = cms.double(0.12),        #Require tag muon to pass particle flow isolation cut
        TagPassTKIso = cms.double(0.05),        #Require tag muon to pass tracking isolation cut

        #probe criteria
        ProbePassID  = cms.string("tight"),     #Require probe muon to pass a ID ( input will be "loose"/"tight"/"HighPT" )
        ProbePassPFIso = cms.double(0.12),      #Require probe muon to pass particle flow isolation cut
        ProbePassTKIso = cms.double(0.05),      #Require probe muon to pass tracking isolation cut
        )

