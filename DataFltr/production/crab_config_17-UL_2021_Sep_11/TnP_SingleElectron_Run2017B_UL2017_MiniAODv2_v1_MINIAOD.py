
from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'TnP_SingleElectron_Run2017B_UL2017_MiniAODv2_v1_MINIAOD'
config.General.workArea = './crab_config_17-UL_2021_Sep_11/'
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
config.Data.inputDataset  = '/SingleElectron/Run2017B-UL2017_MiniAODv2-v1/MINIAOD'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 50
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2021_Sep_11_0505'
config.Site.storageSite = 'T2_TW_NCHC'
