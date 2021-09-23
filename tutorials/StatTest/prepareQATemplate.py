#!/bin/env python
#Main program

import sys
def usage():
    print 'Usage: prepareQATemplate.py [-h] [-V] [--list] [--colors] [-g pdfname] referencefile outfile keyword valueColShift ref/std  word_to_exclude'
    print ' Write a template qa file with the words found in referencefile '
    print '    referencefile : reference file'
    print '    outtfile : output qa file'
    print '    keyword : word to search for in reference file'
    print '    valueColShift : column number starting by the end for the value (0 means last)'
    print '    ref/std : ref=write file for reference (ErrorType=ABS, ErrorCol), std=write file for standard (ErrorType=NEV, no ErrorCol)'
    print '    word_to_exclude: word to exclude (optional)'
    print '    -h : this help'
    print '    -v : verbose'
    print '    -V : very verbose'
    print '    -g : produce pdf with plos'
    print '    --list : list all available tests'
    print '    --colors : use ANSI colors for output results table'
    print ''
    print ' Example: prepareQATemplate.py out.exercise2b PROC_COUNT.qa PROC_COUNT ref'

def printList():
    from Tests import _testsMap
    print 'Known Tests:'
    for k,v in _testsMap.iteritems():
        print "Test: %s"%k
        print v.__doc__

from Utils import Error,logger,NotYet,WrongDataType
from Interface import Configuration

##################################################################
from Utils import SplitLine 
def runQA( referenceFile, outputFile, inWords , isRef, wordsExclude, valueColShift, errorType, doText = False , doHistos = True ) :
    _logger.info("Preparing to run tests for run")

    _logger.debug("REF FILE : %s"%referenceFile)
    tfilecontent = []
    #We need to reduce verbosity in ROOT...
    #See: http://root.cern.ch/phpBB3/viewtopic.php?t=10902&p=46976
    import ROOT
    ROOT.gErrorIgnoreLevel = 1001

    fout = open(outputFile,"w")
    fout.write("Text = {\n")
    fout.write("    'DefaultTestName' : 'NormalCDF',\n")
    fout.write("    'DefaultThresholds' : [ 0.01,0.1 ],\n")
    fout.write("\n")
    fout.write("    'Lines' : [\n")

    #Text part...
    if doText:
        for qafile in inWords:
            _logger.debug("Configuration is: %s"%qafile)
            fin = open(referenceFile)
            lines = fin.readlines()
            for line in lines :
                line = line.lstrip()
                for inWord in inWords : # only allowing one word (this is not needed)
#                    print " CHECK %s %s " % (inWord,line)
                    bOK = 0
                    if line.find(inWord) != -1 :
                        bOK = 1
                    if bOK == 1:
                        for wex in wordsExclude : 
                            if line.find(wex) != -1 :
                                bOK = 0
                                break
                    if bOK == 1:
                        _logger.debug("OK LINE %s"%line)
                        fout.write("        {\n")
                        words = SplitLine(line)
                        _logger.debug("OK LINE %i"%len(words))
                        lw = "            'Words' : { "
                        for ii in range(0,len(words)-valueColShift-1):
                            lw += str(ii+1) + " : '" + str(words[ii]) + "'"
                            if ii != len(words)-valueColShift-2 :
                                lw += ", " 
                        lw += "},\n"
                        fout.write(lw)
                        fout.write("            'ValueCol' : " + str(len(words)-valueColShift) + ",\n")
                        if isRef == "ref" :
                            fout.write("            'ErrorType' : 'ABS',\n")
                            fout.write("            'ErrorCol' : " + str(len(words)-valueColShift+2) + "\n")
                        else :
                            fout.write("            'ErrorType' : '"+errorType+"',\n")
                            if errorType == "REL" or errorType == "ABS" :
                                fout.write("            'ErrorCol' : " + str(len(words)-valueColShift+2) + ",\n")
                        fout.write("        },\n")
                        fout.write("\n")
    fout.write("        ] #End of Lines definition\n")
    fout.write("    } #End of configuration\n")
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
        wordsExclude = []
        if len(args)>=5:
            try:
#                for ii in range(2,len(args)) :
#                    inWords.append(args[ii])
                inWords.append(args[2])
                if args[4] == "ref" or args[4] == "std" :
                    isRef = args[4]
                else:
                    usage()
                    sys.exit()                    
#                if len(args) > 5:
#                    for ii in range(5,len(args)) :
#                        wordsExclude.append(args[ii])
                if len(args) > 5:
                    wordsExclude.append(args[5])
                errorType = "NEV"
                if len(args) > 6:
                    errorType = args[6]
                if input == "TEXT" :
                    valueColShift = int(args[3]) 
                    errcode = runQA( args[0], args[1], inWords , isRef, wordsExclude, valueColShift, errorType, doText = True , doHistos = False ) 
                else :
                    valueColShift = 0
                    errcode = runQA( args[0], args[1], inWords , isRef, wordsExclude, valueColShift, errorType, doText = False , doHistos = True )
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



