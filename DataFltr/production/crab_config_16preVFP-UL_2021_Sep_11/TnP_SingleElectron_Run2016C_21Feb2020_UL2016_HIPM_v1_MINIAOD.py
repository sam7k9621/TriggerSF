
from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'TnP_SingleElectron_Run2016C_21Feb2020_UL2016_HIPM_v1_MINIAOD'
config.General.workArea = './crab_config_16preVFP-UL_2021_Sep_11/'
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
   'year=16preVFP-UL'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '/SingleElectron/Run2016C-21Feb2020_UL2016_HIPM-v1/MINIAOD'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 50
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2021_Sep_11_0502'
config.Site.storageSite = 'T2_TW_NCHC'
