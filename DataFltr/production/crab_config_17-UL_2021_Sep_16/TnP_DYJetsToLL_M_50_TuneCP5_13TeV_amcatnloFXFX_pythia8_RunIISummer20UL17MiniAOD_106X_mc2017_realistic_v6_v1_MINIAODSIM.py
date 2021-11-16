
from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'TnP_DYJetsToLL_M_50_TuneCP5_13TeV_amcatnloFXFX_pythia8_RunIISummer20UL17MiniAOD_106X_mc2017_realistic_v6_v1_MINIAODSIM'
config.General.workArea = './crab_config_17-UL_2021_Sep_16/'
config.General.instance = 'prod'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'DataFltr_cfg.py'
## Input parameters
config.JobType.pyCfgParams = [
   'useMC=False',
   'lepton=el',
   'Debug=1',
   'year=17-UL'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-106X_mc2017_realistic_v6-v1/MINIAODSIM'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 50
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2021_Sep_16_1056'
config.Site.storageSite = 'T2_TW_NCHC'
