#!/usr/bin/env python
#*******************************************************************************
 #
 #  Filename    : submitsample.py
 #  Description : python file for submitting single dataset to crab job
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************
crabcfgformat="""
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()
config.General.requestName = '{0}'
config.General.workArea = '{1}'
config.General.instance = 'prod'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'DataFltr_cfg.py'
## Input parameters
config.JobType.pyCfgParams = [
   'useMC={2}',
   'lepton={3}',
   'Debug=1',
   'year={4}'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '{5}'
config.Data.inputDBS      = 'global'
config.Data.splitting     = '{6}'
config.Data.unitsPerJob   = {7}
config.Data.outLFNDirBase = '{8}'
config.Site.storageSite = '{9}'
"""

import argparse
import subprocess
import os
import re
import sys
import time 
import math
import TriggerSF.DataFltr.MakeName as nametool

def CrabCmd( cmd ):
    s = subprocess.Popen( 'ls', shell=True, stdout=subprocess.PIPE )
    dirlst, err = s.communicate()
    dirlst = filter( lambda x: "crab_config" in x, dirlst.split('\n') )     

    for dir in dirlst:
        s = subprocess.Popen( 'ls {}'.format( dir ), shell=True, stdout=subprocess.PIPE )
        outputlst, err = s.communicate()
        outputlst = filter( lambda x: "crab_TnP" in x, outputlst.split('\n') )
        
        for output in outputlst:
            print "\033[1m\033[31m{}/{}\033[0m".format( dir, output )
            os.system( "{} {}/{}".format( cmd, dir, output ) )

def DivideJob( total ):
    lst = []
    times = int( math.ceil( total / 500. ) )
    for i in range( times ):        
        head = 500*i + 1
        tail = 500* (i+1)
        tail = tail if tail < total else total
        lst.append( (head, tail) )
    return lst

def CrabGetOutput():
    s = subprocess.Popen( 'ls', shell=True, stdout=subprocess.PIPE )
    dirlst, err = s.communicate()
    dirlst = filter( lambda x: "crab_config" in x, dirlst.split('\n') )     

    for dir in dirlst:
        s = subprocess.Popen( 'ls {}'.format( dir ), shell=True, stdout=subprocess.PIPE )
        outputlst, err = s.communicate()
        outputlst = filter( lambda x: "crab_TnP" in x, outputlst.split('\n') )
        
        for output in outputlst:
            print "\033[1m\033[31m{}/{}\033[0m".format( dir, output )
            s = subprocess.Popen( 'crab status -d {}/{} | grep "finished"'.format( dir, output ), shell=True, stdout=subprocess.PIPE )
            joblst, err = s.communicate()
            joblst = map(int, filter( lambda x: x.isdigit(), re.split( '\t|%|\(|\)|\/', joblst ) ) )
            if joblst[0] == joblst[1]:
                for jobid in DivideJob( joblst[0] ):
                    os.system( "crab getoutput -d {}/{} --jobids={}-{}".format(dir, output, jobid[0], jobid[1]) )
            else: 
                print "Not yet {}/{}".format( joblst[0], joblst[1] )

def CrabSubmit( opt ):
    dirname  = './crab_config_{}_{}/'.format( opt.year, time.strftime("%Y_%b_%d") )
    jobname  = nametool.requestName( opt.inputdataset, opt.useMC )
    filename = dirname + jobname + '.py'
    if not os.path.isdir( dirname ):
        os.system('mkdir {}'.format( dirname ) )

    content = crabcfgformat.format(
        jobname,
        dirname, 
        opt.useMC,
        opt.lepton,
        opt.year,
        opt.inputdataset,
        nametool.splitbase(opt.useMC),
        opt.jobnumber,
        opt.directory,
        opt.site
    )

    with open(filename, 'w') as cfgfile:
        cfgfile.write(content)

    if os.path.isdir( dirname + "crab_" + jobname ) and opt.force:
        os.system( "rm -r " + dirname + "crab_" + jobname )

    if opt.submit:
        if opt.dryrun:
            os.system('crab submit -c ' + filename + ' --dryrun')
        else:
            os.system('crab submit -c ' + filename )

def main(argv):

    parser = argparse.ArgumentParser(description='Process to sending crab for TnP')
    parser.add_argument('-i', '--inputdataset', help='which dataset to run', type=str, default=None)
    parser.add_argument('-s', '--site'        , help='which site to store' , type=str, default='T2_TW_NCHC')
    parser.add_argument('-y', '--year'        , help='which year'          , type=str, default=None)
    parser.add_argument('-d', '--directory'   , help='the storage lfn dir' , type=str, default=time.strftime("/store/user/pusheng/HLTSF_%Y_%b_%d_%H%M" ) ) 
    parser.add_argument('-l', '--lepton'      , help='which lepton using'  , type=str, default=None)
    parser.add_argument('-n', '--jobnumber'   , help='unitsPerJob'         , type=str, default='2')
    parser.add_argument('-m', '--useMC'       , action='store_true')
    parser.add_argument('-t', '--dryrun'      , action='store_true')
    parser.add_argument('-f', '--force'       , action='store_true')
    parser.add_argument('-r', '--submit'      , action='store_true')
    parser.add_argument('--resubmit'          , action='store_true')
    parser.add_argument('--status'            , action='store_true')
    parser.add_argument('--getoutput'         , action='store_true')

    try:
        opt = parser.parse_args()
    except:
        print "Error processing arguments!"
        parser.print_help()
        raise

    if opt.resubmit:
        CrabCmd( "crab resubmit -d" )

    elif opt.status:
        CrabCmd( "crab status -d" )

    elif opt.getoutput:
        CrabGetOutput()

    else:
        CrabSubmit( opt )

    

if __name__ == '__main__':
    main( sys.argv[1:] )
