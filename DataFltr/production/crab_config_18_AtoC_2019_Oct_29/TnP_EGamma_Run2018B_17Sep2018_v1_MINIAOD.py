
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
config.General.requestName = 'TnP_EGamma_Run2018B_17Sep2018_v1_MINIAOD'
config.General.workArea = './crab_config_18_AtoC_2019_Oct_29/'
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
   'year=18_AtoC'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '/EGamma/Run2018B-17Sep2018-v1/MINIAOD'
config.Data.inputDBS      = 'global'
config.Data.splitting     = 'LumiBased'
config.Data.unitsPerJob   = 50
config.Data.outLFNDirBase = '/store/user/pusheng/HLTSF_2019_Oct_29_1609'
config.Site.storageSite = 'T2_TW_NCHC'
