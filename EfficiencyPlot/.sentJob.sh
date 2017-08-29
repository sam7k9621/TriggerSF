
#!/usr/bin/env sh
#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d /wk_cms/sam7k9621/qsub/dMESSAGE
#PBS -o /wk_cms/sam7k9621/qsub/oMESSAGE
#PBS -e /wk_cms/sam7k9621/qsub/eMESSAGE
cd /wk_cms2/sam7k9621/CMSSW_9_2_8/src && eval `scramv1 runtime -sh`
cmsRun EffPlot_cfg.py sample=file:/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/DataFltr/production/Tnp_2017_data_crab/crab_TnP_SingleElectron_Run2017C_PromptReco_v2_MINIAOD/result/*.root output=SingleElectron_Run2017C_v2.root
