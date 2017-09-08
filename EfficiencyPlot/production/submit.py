#!/bin/env python

import os
import sys
import subprocess

datalst = [
        '/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/DataFltr/production/Tnp_2017_data_crab/crab_TnP_SingleElectron_Run2017B_PromptReco_v1_MINIAOD/results/',
        '/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/DataFltr/production/Tnp_2017_data_crab/crab_TnP_SingleElectron_Run2017B_PromptReco_v2_MINIAOD/results/',
        '/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/DataFltr/production/Tnp_2017_data_crab/crab_TnP_SingleElectron_Run2017C_PromptReco_v1_MINIAOD/results/',
        '/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/DataFltr/production/Tnp_2017_data_crab/crab_TnP_SingleElectron_Run2017C_PromptReco_v2_MINIAOD/results/',
        '/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/DataFltr/production/Tnp_2017_data_crab/crab_TnP_SingleElectron_Run2017C_PromptReco_v3_MINIAOD/results/'
        ]

qsub ="""
#!/usr/bin/env sh
#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d /wk_cms/sam7k9621/qsub/dMESSAGE
#PBS -o /wk_cms/sam7k9621/qsub/oMESSAGE
#PBS -e /wk_cms/sam7k9621/qsub/eMESSAGE
cd /wk_cms2/sam7k9621/CMSSW_9_2_8/src && eval `scramv1 runtime -sh`
cmsRun /wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/EfficiencyPlot/production/EffPlot_cfg.py inputFiles_load={0} output={1}
"""

def main(args):

    pos = "/wk_cms2/sam7k9621/CMSSW_9_2_8/src/TriggerEfficiency/EfficiencyPlot/data/"

    for data in datalst :

        outfile = data.split("/")[9].split("_")
        outfile = outfile[2] + "_" + outfile[3] + "_"  + outfile[5]

        datanum = len( [ name for name in os.listdir(data) if ".root" in name ] )

        samplelst = []
        for i in range(1, datanum+1) :
            sample = data + 'TnP_test_electron_{}.root'.format(i)
            samplelst.append(sample)

        inputfilelst = open( pos + outfile + ".txt" ,'w')
        for s in samplelst :
            inputfilelst.write( "file:" + s +"\n" )
        inputfilelst.close()

        output = open( ".sentJob.sh", 'w' )
        output.write( qsub.format( pos + outfile+ ".txt", pos + outfile+".root" ) )
        output.close()


        cmd = "qsub .sentJob.sh -N " + outfile
        print ">>Processing ", cmd
        os.system(cmd)
        os.system("rm .sentJob.sh")

if __name__ == '__main__':
    main(sys.argv)
