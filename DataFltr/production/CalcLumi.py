#!/bin/env python
import sys
import argparse
import os
import json 
from collections import OrderedDict

def main(args):
    parser = argparse.ArgumentParser(
            ""
            )

    parser.add_argument(
            '-i', '--inputfile',
            type=str
            )
    parser.add_argument(
            '-r', '--range',
            nargs='+',
            type=int 
            )
    try:
        opt = parser.parse_args(args[1:])
    except:
        parser.print_help()
        raise


    if len( opt.range ) % 2 is not 0:
        raise ValueError("Missing run range end for {}".format( opt.range[-1] ) )

    runlst = [ tuple(opt.range[i:i+2]) for i in range(0, len( opt.range ), 2) ]
   
    outputlst = []
    with open( opt.inputfile ) as json_file:
        datadict = json.load(json_file, object_pairs_hook=OrderedDict)
        
        for run in runlst:
            outputlst.append( OrderedDict( (k, v) for k, v in datadict.iteritems() if run[0] <= int(k) <= run[1] ) )

    for output in outputlst:
        with open('temp.txt', 'w') as outfile:
            json.dump(output, outfile)
        
        os.system( "brilcalc lumi -i temp.txt --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_PHYSICS.json -u /fb" )
        os.system( "rm temp.txt" )

if __name__ == '__main__':
    main(sys.argv)
