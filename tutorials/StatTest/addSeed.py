#!/bin/env python
#Main program

import sys
def usage():
    print 'Usage: addSeed.py [-h] [-V] [--list] [--colors] basefile seed '
    print ' Add random seed to a GAMOS input file '
    print '    -h : this help'
    print '    -v : verbose'
    print '    -V : very verbose'
    print '    --list : list all available tests'

def printList():
    from Tests import _testsMap
    print 'Known Tests:'
    for k,v in _testsMap.iteritems():
        print "Test: %s"%k
        print v.__doc__

from Utils import Error,logger,NotYet,WrongDataType
from Interface import Configuration

##################################################################
def addSeed( inputFile, seed ) :
    _logger.info("Preparing to run tests for run")

    fin = open(inputFile)
    outputFile = inputFile + "." + str(seed)
    fout = open(outputFile,"w")

    lines = fin.readlines()
    bSeed = False
    for line in lines :
        if line.find("setSeeds") != -1 :
            bSeed = True
            line = "/gamos/random/setSeeds " + seed + " " + seed + "\n"
        if line.find("restoreSeeds") != -1 :
            bSeed = True
            line = "/gamos/random/restoreSeeds " + seed + " " + seed + "\n"
        if line.find("/run/beamOn") != -1 :
            if not bSeed :
                lineRandom = "/gamos/random/restoreSeeds " + seed + " " + seed + "\n"
                fout.write(lineRandom)
                
        fout.write(line)

    fout.close()
        
#################################################
def run( argv = sys.argv ):
    from Interface import runROOT
    if len(argv)>=1:
        import getopt
        try:
            opts,args = getopt.getopt( argv ,"hvVg:",["list","colors"])
        except getopt.GetoptError,err:
            sys.stderr.write( str(err) )
            usage()
            sys.exit(2)
            
        input = "TEXT"
        for op,ar in opts:
            if op == '-h':
                usage()
                sys.exit(0)
            elif op == '-v':
                from Interface import _logger
                _logger.setLevel('INFO')
            elif op == '-V':
                from Interface import _logger
                _logger.setLevel('DEBUG')
            elif op == '--list':
                printList()
                sys.exit(0)
            elif op == "--colors":
                colors = True
            elif op == '-t':
                input = "TEXT"
            elif op == '-h':
                input = "HISTOS"
        from Interface import _logger
        inWords = []
        if len(args)==2:
            try:
                errcode = addSeed( args[0], args[1])
            except Exception,e:
                sys.stderr.write("Error: %s\n"%e)
                sys.exit(1)
        else:
            usage()
            sys.exit(0)
            
    else:
        usage()
        sys.exit(0)
    if errcode > 0:
        sys.stderr.write( "Number of FAILED tests: %d\n"%errcode)
        sys.exit(1)
    sys.exit( 0 )

from Utils import Error,logger
import math
if __name__ == "__main__":
    _logger=logger().getLogger('Interface')
    run( sys.argv[1:])



