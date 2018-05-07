
#!/usr/bin/env sh
#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d /wk_cms/sam7k9621/qsub/dMESSAGE
#PBS -o /wk_cms/sam7k9621/qsub/oMESSAGE
#PBS -e /wk_cms/sam7k9621/qsub/eMESSAGE
cd /wk_cms2/sam7k9621/CMSSW_9_2_8/src && eval `scramv1 runtime -sh`
cmsRun /wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/EfficiencyPlot/production/EffPlot_cfg.py inputFiles_load=/wk_cms2/sam7k9621/CMSSW_9_4_0_patch1/src/TriggerEfficiency/EfficiencyPlot/data/TnP_SingleElectron_Run2017F_v1_2.txt output=/wk_cms2/sam7k9621/CMSSW_9_4_0_patch1/src/TriggerEfficiency/EfficiencyPlot/data/TnP_SingleElectron_Run2017F_v1_2.root useMC=False