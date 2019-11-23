
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
config.General.requestName = 'TnP_SingleElectron_Run2016C_17Jul2018_v1_MINIAOD'
config.General.workArea = './crab_config_16_2019_Nov_22/'
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
   'year=16'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '/SingleElectron/Run2016C-17Jul2018-v1/MINIAOD'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 50
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2019_Nov_22_1842'
config.Site.storageSite = 'T2_TW_NCHC'
