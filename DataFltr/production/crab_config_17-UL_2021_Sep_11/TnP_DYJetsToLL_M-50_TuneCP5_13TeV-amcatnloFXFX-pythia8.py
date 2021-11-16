
from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'TnP_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8'
config.General.workArea = './crab_config_17-UL_2021_Sep_11/'
config.General.instance = 'prod'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'DataFltr_cfg.py'
## Input parameters
config.JobType.pyCfgParams = [
   'useMC=True',
   'lepton=el',
   'Debug=1',
   'year=17-UL'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17MiniAOD-Pilot_106X_mc2017_realistic_v6-v2/MINIAODSIM'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'FileBased'
config.Data.unitsPerJob   = 10
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2021_Sep_11_0505'
config.Site.storageSite = 'T2_TW_NCHC'
