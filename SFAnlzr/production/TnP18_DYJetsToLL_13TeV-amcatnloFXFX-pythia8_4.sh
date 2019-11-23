
#!/usr/bin/env sh
#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d /wk_cms/sam7k9621/qsub/dMESSAGE
#PBS -o /wk_cms/sam7k9621/qsub/oMESSAGE
#PBS -e /wk_cms/sam7k9621/qsub/eMESSAGE
cd /wk_cms2/sam7k9621/CMSSW_10_2_17/src && eval `scramv1 runtime -sh`
cmsRun /wk_cms2/sam7k9621/CMSSW_10_2_17/src/TriggerSF/SFAnlzr/production/MakeEff_cfg.py inputFiles_load=/wk_cms2/sam7k9621/CMSSW_10_2_17/src/TriggerSF/SFAnlzr/data/TnP18_DYJetsToLL_13TeV-amcatnloFXFX-pythia8_4.txt output=/wk_cms2/sam7k9621/CMSSW_10_2_17/src/TriggerSF/SFAnlzr/results/TnP18_DYJetsToLL_13TeV-amcatnloFXFX-pythia8_4.root useMC=True year=18 
