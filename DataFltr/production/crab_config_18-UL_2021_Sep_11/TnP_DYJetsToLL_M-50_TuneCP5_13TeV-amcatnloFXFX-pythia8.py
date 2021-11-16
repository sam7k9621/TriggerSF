
from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'TnP_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8'
config.General.workArea = './crab_config_18-UL_2021_Sep_11/'
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
   'year=18-UL'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL18MiniAOD-Pilot_106X_upgrade2018_realistic_v11_L1v1-v2/MINIAODSIM'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'FileBased'
config.Data.unitsPerJob   = 10
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2021_Sep_11_0507'
config.Site.storageSite = 'T2_TW_NCHC'
