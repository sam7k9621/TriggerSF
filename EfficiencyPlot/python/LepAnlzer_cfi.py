import FWCore.ParameterSet.Config as cms

muontool = cms.PSet(
   tag = cms.InputTag('datafltr','Tag'),
   probe = cms.InputTag('datafltr','Probe'),
   tagtrigger = cms.VPSet(
        cms.PSet(
            ptbin = cms.vdouble(20,25,30,40,45,46,47,48,49,50,51,52,53,54,55,60,120),
            etabin = cms.vdouble(-2.4,-2.1,-1.6,-1.2,-0.9,-0.3,-0.2,0,0.2,0.3,0.9,1.2,1.6,2.1,2.4),
            HLT = cms.vstring("HLT_Mu50_v*","HLT_TkMu50_v*"),
            name = cms.string("total_mu50_tkmu50"),
            ptcut = cms.double(53),
            etacut = cms.double(100)
            )
        ),
   protrigger = cms.VPSet(
        cms.PSet(
            HLT = cms.vstring("HLT_IsoMu27_v*","HLT_IsoTkMu27_v*"),
            etacut = cms.double(100),
            ptcut = cms.double(30)
            )
        )
)

electrontool = cms.PSet(
   tag = cms.InputTag('datafltr','Tag'),
   probe = cms.InputTag('datafltr','Probe'),
   tagtrigger = cms.VPSet(
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,35,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,60,70,80,100,150,200,300,400,500,600),
           etabin = cms.vdouble(-2.5, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.5),
           HLT = cms.vstring("HLT_Ele32_WPTight_Gsf_v*"),
           name = cms.string("ele40_v32"),
           ptcut = cms.double(35),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,35,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,60,70,80,100,150,200,300,400,500,600),
           etabin = cms.vdouble(-2.5, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.5),
           HLT = cms.vstring("HLT_Ele32_WPTight_Gsf_v*"),
           name = cms.string("ele40_v32_pt"),
           ptcut = cms.double(40),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,35,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,60,70,80,100,150,200,300,400,500,600),
           etabin = cms.vdouble(-2.5, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.5),
           HLT = cms.vstring("HLT_Ele35_WPTight_Gsf_v*"),
           name = cms.string("ele40_v35"),
           ptcut = cms.double(38),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,35,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,60,70,80,100,150,200,300,400,500,600),
           etabin = cms.vdouble(-2.5, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.5),
           HLT = cms.vstring("HLT_Ele35_WPTight_Gsf_v*"),
           name = cms.string("ele40_v35_pt"),
           ptcut = cms.double(43),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,35,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,60,70,80,100,150,200,300,400,500,600),
           etabin = cms.vdouble(-2.5, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.5),
           HLT = cms.vstring("HLT_Ele38_WPTight_Gsf_v*"),
           name = cms.string("ele40_v38"),
           ptcut = cms.double(41),
           etacut = cms.double(100)
           ),
       cms.PSet(
           ptbin = cms.vdouble(10,20,25,30,35,36,37,38,39,40,41,42,43,44,45,50,51,52,53,54,55,60,70,80,100,150,200,300,400,500,600),
           etabin = cms.vdouble(-2.5, -2, -1.566, -1.444, -0.8 ,0, 0.8 ,1.444, 1.566, 2, 2.5),
           HLT = cms.vstring("HLT_Ele38_WPTight_Gsf_v*"),
           name = cms.string("ele40_v38_pt"),
           ptcut = cms.double(46),
           etacut = cms.double(100)
           )
       ),
   protrigger = cms.VPSet(
       cms.PSet(
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           ptcut = cms.double(55),
           etacut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           ptcut = cms.double(55),
           etacut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           ptcut = cms.double(55),
           etacut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           ptcut = cms.double(55),
           etacut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           ptcut = cms.double(55),
           etacut = cms.double(100)
           ),
       cms.PSet(
           HLT = cms.vstring("HLT_Ele40_WPTight_Gsf_v*"),
           ptcut = cms.double(55),
           etacut = cms.double(100)
           )
       )
)
