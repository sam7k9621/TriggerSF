#!/bin/env python

import os
import sys
import subprocess
import argparse

pos      = "/wk_cms2/sam7k9621/CMSSW_9_4_6_patch1/src/TriggerEfficiency/EfficiencyPlot/data/"
crabfile = "/wk_cms2/sam7k9621/CMSSW_9_4_6_patch1/src/TriggerEfficiency/DataFltr/production/Tnp_2017_data_crab/"

datalst = [
        crabfile + 'crab_TnP_SingleElectron_Run2017B_31Mar2018_v1_MINIAOD/results/',
        crabfile + 'crab_TnP_SingleElectron_Run2017C_31Mar2018_v1_MINIAOD/results/',
        crabfile + 'crab_TnP_SingleElectron_Run2017D_31Mar2018_v1_MINIAOD/results/',
        crabfile + 'crab_TnP_SingleElectron_Run2017E_31Mar2018_v1_MINIAOD/results/',
        crabfile + 'crab_TnP_SingleElectron_Run2017F_31Mar2018_v1_MINIAOD/results/'
        ]

mclst = [
        crabfile + 'crab_TnP_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/results/',
        crabfile + 'crab_TnP_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/results/'
        ]

qsub ="""
#!/usr/bin/env sh
#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d /wk_cms/sam7k9621/qsub/dMESSAGE
#PBS -o /wk_cms/sam7k9621/qsub/oMESSAGE
#PBS -e /wk_cms/sam7k9621/qsub/eMESSAGE
cd /wk_cms2/sam7k9621/CMSSW_9_4_6_patch1/src && eval `scramv1 runtime -sh`
cmsRun /wk_cms2/sam7k9621/CMSSW_9_4_6_patch1/src/TriggerEfficiency/EfficiencyPlot/production/EffPlot_cfg.py inputFiles_load={0} output={1} useMC={2}
"""

def GetSamplelst(data):
    datanum = len( [ name for name in os.listdir(data) if ".root" in name ] )
    samplelst = []
    for i in range(1, datanum+1) :
        sample = data + 'TnP_test_electron_{}.root'.format(i)
        samplelst.append(sample)

    return samplelst

def GetInputlst(num, per, outfile):
    inputlst = []
    for s in range(num/per + 1):
        inputfile = open( pos +  outfile + '_' + str(s) +'.txt', 'w')
        inputlst.append(inputfile)

    return inputlst

def main(args):

    parser = argparse.ArgumentParser(
            "Options to decide to use which HLT menu version"
            )

    parser.add_argument(
            '-s', '--tag',
            help='add outputfile tag',
            type=str, default='TnP'
            )
    try:
        opt = parser.parse_args(args[1:])
    except:
        print "Error processing arguments!"
        parser.print_help()
        raise

    for mc in mclst :

        outfile = mc.split("/")[9].split("_")
        outfile = opt.tag + '_' + outfile[2] + "_" + outfile[5]

        samplelst = GetSamplelst(mc)
        inputlst  = GetInputlst( len(samplelst), 30, outfile )

        for idx, s in enumerate(samplelst):
            inputlst[ idx/30 ].write( "file:" + s +"\n" )

        for idx, f in enumerate(inputlst):
            f.close()

            output = open( ".sentJob.sh", 'w' )
            output.write( qsub.format( pos + outfile + '_' + str(idx) + ".txt", pos + outfile + '_' + str(idx) + ".root", "True" ) )
            output.close()

            cmd = "qsub .sentJob.sh -N " + outfile + '_' + str(idx)
            print ">>Processing ", cmd
            os.system(cmd)
            os.system("rm .sentJob.sh")

    # for data in datalst :

        # outfile = data.split("/")[9].split("_")
        # outfile = opt.tag + '_' + outfile[2] + "_" + outfile[3] + "_"  + outfile[5]

        # samplelst = GetSamplelst(data)
        # inputlst  = GetInputlst( len(samplelst), 300, outfile )

        # for idx, s in enumerate(samplelst):
            # inputlst[ idx/300 ].write( "file:" + s +"\n" )

        # for idx, f in enumerate(inputlst):
            # f.close()

            # output = open( ".sentJob.sh", 'w' )
            # output.write( qsub.format( pos + outfile + '_' + str(idx) + ".txt", pos + outfile + '_' + str(idx) + ".root", "False" ) )
            # output.close()

            # cmd = "qsub .sentJob.sh -N " + outfile + '_' + str(idx)
            # print ">>Processing ", cmd
            # os.system(cmd)
            # os.system("rm .sentJob.sh")

if __name__ == '__main__':
    main(sys.argv)
