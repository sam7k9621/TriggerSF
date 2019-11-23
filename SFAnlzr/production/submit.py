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
    outputfilelst = []
    for idx, f in enumerate( inputlst ):
        f.close()
        with open( "{}_{}.sh".format( outfile, str(idx)) , "w" ) as outputfile:
            outputfile.write( qsub.format(
                CMSSW_BASE,
                f.name,
                CMSSW_BASE + outputdir + "results/{}_{}.root".format( outfile, idx ),
                useMC,
                year,
                entry
                ) )
        
        print ">> Processing {}_{}".format( outfile, str(idx) )
        outputfilelst.append( outputfile.name )
    return outputfilelst

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
            '-s', '--submit',
            action='store_true'
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
    
    filelst = []
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
                outfile = "TnP" + tag + '_' + outfile[2] + "_" + outfile[5]
                inputlst  = GetInputlst( len(samplelst), per, outfile )
                filelst += SubmitSample( samplelst, inputlst, inputpath, outfile, per, True, tag ) 

            else: #Data
                per = 300
                lumimask = "lumimask={}".format( CMSSW_BASE + datadir + "lumimask_{}.txt".format( tag ) ) 
                
                outfile = output.split("_")
                outfile = "TnP" + tag + "_" + outfile[2] + "_" + outfile[3] + "_" + outfile[5]
                inputlst  = GetInputlst( len(samplelst), per, outfile )
                filelst += SubmitSample( samplelst, inputlst, inputpath, outfile, per, False, tag, lumimask )

    if opt.submit:
        file = " ".join( filelst )
        # cmd     = "nohup {}/src/CPVAnalysis/SentQJob.py -r 10 -q 5 -i {} > cmsRun.out &".format( CMSSW_BASE, file )
        cmd     = "{}/src/CPVAnalysis/SentQJob.py -r 10 -q 5 -i {}".format( CMSSW_BASE, file )
        os.system( cmd )
        # print "DONE"

if __name__ == '__main__':
    main(sys.argv)
