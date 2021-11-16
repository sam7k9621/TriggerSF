
from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'TnP_EGamma_Run2018A_UL2018_MiniAODv2_v1_MINIAOD'
config.General.workArea = './crab_config_18-UL_2021_Sep_11/'
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
   'year=18-UL'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '/EGamma/Run2018A-UL2018_MiniAODv2-v1/MINIAOD'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 50
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2021_Sep_11_0508'
config.Site.storageSite = 'T2_TW_NCHC'
