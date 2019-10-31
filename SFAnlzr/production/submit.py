#!/bin/env python

import os
import sys
import subprocess
import argparse


CMSSW_BASE = os.environ['CMSSW_BASE']
datadir    = "/src/CPVAnalysis/BaseLineSelector/data/"
inputdir   = "/src/TriggerSF/DataFltr/production/"
outputdir  = "/src/TriggerSF/SFAnlzr/"

qsub ="""
#!/usr/bin/env sh
#PBS -V
#PBS -j oe
#PBS -q cms
#PBS -d /wk_cms/sam7k9621/qsub/dMESSAGE
#PBS -o /wk_cms/sam7k9621/qsub/oMESSAGE
#PBS -e /wk_cms/sam7k9621/qsub/eMESSAGE
cd {0}/src && eval `scramv1 runtime -sh`
cmsRun {0}/src/TriggerSF/SFAnlzr/production/MakeEff_cfg.py inputFiles_load={1} output={2} useMC={3} year={4} {5}
"""
def SubmitSample( samplelst, inputlst, inputpath, outfile, per, useMC, year, entry="" ):
    for idx, s in enumerate( samplelst ):
        inputlst[ idx/per ].write( "file:" + inputpath + s + "\n" )

    print "Total files:", len(samplelst) 
    for idx, f in enumerate( inputlst ):
        f.close()
        with open( ".sentJob.sh", "w" ) as outputfile:
            outputfile.write( qsub.format(
                CMSSW_BASE,
                f.name,
                CMSSW_BASE + outputdir + "results/{}_{}.root".format( outfile, idx ),
                useMC,
                year,
                entry
                ) )
        cmd = "qsub .sentJob.sh -N {}_{}".format( outfile, str(idx) )
        print ">> Processing {}_{}".format( outfile, str(idx) )
        os.system(cmd)
        os.system("rm .sentJob.sh")


def GetInputlst(num, per, outfile):
    inputlst = []
    for s in range(num/per + 1):
        inputfile = open( CMSSW_BASE + outputdir + "data/" + outfile + '_' + str(s) +'.txt', 'w')
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

    s = subprocess.Popen( 'ls {}'.format(CMSSW_BASE+inputdir), shell=True, stdout=subprocess.PIPE )
    dirlst, err = s.communicate() 
    dirlst = filter( lambda x: "crab_config" in x, dirlst.split('\n') )
    for dir in dirlst:
        s = subprocess.Popen( 'ls {}'.format(CMSSW_BASE+inputdir+dir), shell=True, stdout=subprocess.PIPE )
        outputlst, err = s.communicate()
        outputlst = filter( lambda x: "crab_TnP" in x, outputlst.split('\n') ) 

        tag = dir.split("_")[2]
        for output in outputlst :
            
            inputpath = "{}{}{}/{}/results/".format(CMSSW_BASE, inputdir, dir, output )
            s = subprocess.Popen( 'ls {}'.format(inputpath), shell=True, stdout=subprocess.PIPE )
            samplelst, err = s.communicate()
            samplelst = filter( lambda x: ".root" in x, samplelst.split('\n') )
            
            if not samplelst:
                continue

            if "Run" not in output: #MC
                per = 30

                outfile = output.split("_")
                outfile = opt.tag + tag + '_' + outfile[2] + "_" + outfile[5]
                inputlst  = GetInputlst( len(samplelst), per, outfile )
                SubmitSample( samplelst, inputlst, inputpath, outfile, per, True, tag ) 

            else: #Data
                per = 300
                lumimask = "lumimask={}".format( CMSSW_BASE + datadir + "lumimask_{}.txt".format( tag ) ) 
                
                outfile = output.split("_")
                outfile = opt.tag + tag + "_" + outfile[2] + "_" + outfile[3] + "_" + outfile[5]
                inputlst  = GetInputlst( len(samplelst), per, outfile )
                SubmitSample( samplelst, inputlst, inputpath, outfile, per, False, tag, lumimask ) 

if __name__ == '__main__':
    main(sys.argv)
