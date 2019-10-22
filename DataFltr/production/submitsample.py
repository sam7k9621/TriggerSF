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
import TriggerSF.DataFltr.MakeName as nametool
import os
import sys
import time 

def submitsample(argv):

    parser = argparse.ArgumentParser(description='Process to sending crab for TnP')
    parser.add_argument('-i', '--inputdataset', help='which dataset to run', type=str, required=True)
    parser.add_argument('-s', '--site'        , help='which site to store' , type=str, default='T2_TW_NCHC')
    parser.add_argument('-y', '--year'        , help='which year'          , type=str, required=True)
    parser.add_argument('-d', '--directory'   , help='the storage lfn dir' , type=str, default=time.strftime("/store/user/pusheng/HLTSF_%Y_%b_%d_%H%M" ) ) 
    parser.add_argument('-l', '--lepton'      , help='which lepton using'  , type=str, required=True)
    parser.add_argument('-n', '--jobnumber'   , help='unitsPerJob'         , type=str, default='2')
    parser.add_argument('-m', '--useMC'       , action='store_true')
    parser.add_argument('-t', '--dryrun'      , action='store_true')
    parser.add_argument('-f', '--force'       , action='store_true')
    parser.add_argument('-r', '--submit'      , action='store_true')

    try:
        opt = parser.parse_args()
    except:
        print "Error processing arguments!"
        parser.print_help()
        raise

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


if __name__ == '__main__':
    sys.exit(submitsample(sys.argv[1:]))
