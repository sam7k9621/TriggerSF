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
config.General.requestName = 'TnP{0}'
config.General.workArea = 'Tnp_2017_data_crab'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'DataFltr_cfg.py'
## Input parameters
config.JobType.pyCfgParams = [
   'useMC=False',
   'lepton={1}',
   'Debug=1'
   ]
config.JobType.maxMemoryMB      = 2000 # Requesting slightly more memory
config.JobType.maxJobRuntimeMin = 2000 # Requesting slightly more runtime
config.Data.inputDataset  = '{2}'
config.Data.inputDBS      = 'global'
config.Data.splitting     = '{3}'
config.Data.unitsPerJob   = {4}
config.Data.outLFNDirBase = '{5}'
config.Data.outputDatasetTag = '{6}'
config.Site.storageSite = '{7}'
"""

import argparse
import TriggerEfficiency.DataFltr.MakeName as nametool
import os
import sys

def submitsample(argv):

    parser = argparse.ArgumentParser(description='Process to sending crab for TnP')
    parser.add_argument('-i', '--inputdataset', help='which dataset to run', type=str, default=None, required=True)
    parser.add_argument('-s', '--site'        , help='which site to store' , type=str, default='T2_TW_NCHC')
    parser.add_argument('-d', '--directory'   , help='the storage lfn dir' , type=str, default='/store/user/pusheng/20170827B2GTriggerEfficiency')
    parser.add_argument('-l', '--lepton'      , help='which lepton using'  , type=str, default=None, required=True)
    parser.add_argument('-n', '--jobnumber'   , help='unitsPerJob'         , type=str, default='1')
    parser.add_argument('-m', '--useMC'       , action='store_true')

    try:
        opt = parser.parse_args()
    except:
        print "Error processing arguments!"
        parser.print_help()
        raise

    content = crabcfgformat.format(
        nametool.requestName( opt.inputdataset, opt.useMC),
        opt.lepton,
        opt.inputdataset,
        nametool.splitbase(opt.useMC),
        opt.jobnumber,
        opt.directory,
        opt.lepton + nametool.outputname( opt.inputdataset, opt.useMC),
        opt.site
    )

    filename = './crab_config/TnP' + nametool.requestName( opt.inputdataset, opt.useMC) + '.py'
    if not os.path.isdir('./crab_config'):
        os.system('mkdir ./crab_config')

    cfgfile = open(filename, 'w')
    cfgfile.write(content)
    cfgfile.close()

    os.system('source /cvmfs/cms.cern.ch/crab3/crab.sh')
    os.system('crab submit ' +filename)


if __name__ == '__main__':
    sys.exit(submitsample(sys.argv[1:]))
