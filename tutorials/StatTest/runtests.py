#!/bin/env python
#Main program

import sys
def usage():
    print 'Usage: runtests.py [-h] [-V] [--list] [--colors] [-g pdfname] [-H -T] conffile inputfile referencefile'
    print ' Performs statistical tests for distributions contained in inputfile'
    print ' tests are configured in conffile'
    print '    conffile : configuration file'
    print '    inputfile : input file containing distributions'
    print '    referencefile (optional) : reference file name that overwrites the one contained in the conf file'
    print '    -h : this help'
    print '    -v : verbose'
    print '    -V : very verbose'
    print '    -g : produce pdf with plos'
    print '    --list : list all available tests'
    print '    --colors : use ANSI colors for output results table'
    print ''
    print ' Example: runtests.py testconf.qa test.root'
    print '          See content of example directory for an example of a configuration file'

def printList():
    from Tests import _testsMap
    print 'Known Tests:'
    for k,v in _testsMap.iteritems():
        print "Test: %s"%k
        print v.__doc__
        
def run( argv = sys.argv ):
    from Interface import runROOT
    colors = False #By default use ANSI colors for output
    if len(argv)>=1:
        import getopt
        try:
            opts,args = getopt.getopt( argv ,"hvVTg:H",["list","colors"])
        except getopt.GetoptError,err:
            sys.stderr.write( str(err) )
            usage()
            sys.exit(2)
            
        doplots = "report.pdf" 
        input = "TEXT"
        for op,ar in opts:
#            print("OP %s AR %s"%(op,ar)) 
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
            elif op == '-g':
                doplots=ar
            elif op == '-T':
                input = "TEXT"
            elif op == '-H':
                input = "HISTOS"
        if len(args)>=2:
            try:
                if len(args)>=3:
                    if input == "TEXT" :
                        errcode = runROOT( args[0] , args[1] , doPlots = None , doText = True , doHistos = False , doUnBinned = False , defaultReferenceFile = args[2] ) 
                    else :
                        errcode = runROOT( args[0] , args[1] , doPlots = doplots , doText = False, doHistos = True , doUnBinned = False , defaultReferenceFile = args[2] , useColorsForOutput = colors)
                else:
                    if input == "TEXT" :
                        sys.stderr.write("Error: NO REFERENCE FILE %s\n")
                        sys.exit(1)
                    else:
                        errcode = runROOT( args[0] , args[1] , doPlots = doplots , useColorsForOutput = colors)
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

if __name__ == "__main__":
    run( sys.argv[1:])
