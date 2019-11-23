
#!/usr/bin/env sh
#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d /wk_cms/sam7k9621/qsub/dMESSAGE
#PBS -o /wk_cms/sam7k9621/qsub/oMESSAGE
#PBS -e /wk_cms/sam7k9621/qsub/eMESSAGE
cd /wk_cms2/sam7k9621/CMSSW_10_2_17/src && eval `scramv1 runtime -sh`
cmsRun /wk_cms2/sam7k9621/CMSSW_10_2_17/src/TriggerSF/SFAnlzr/production/MakeEff_cfg.py inputFiles_load=/wk_cms2/sam7k9621/CMSSW_10_2_17/src/TriggerSF/SFAnlzr/data/TnP16_SingleElectron_Run2016F_v1_1.txt output=/wk_cms2/sam7k9621/CMSSW_10_2_17/src/TriggerSF/SFAnlzr/results/TnP16_SingleElectron_Run2016F_v1_1.root useMC=False year=16 lumimask=/wk_cms2/sam7k9621/CMSSW_10_2_17/src/CPVAnalysis/BaseLineSelector/data/lumimask_16.txt
