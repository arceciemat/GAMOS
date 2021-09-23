#!/bin/env python
#Main program

import sys
def usage():
    print 'Usage: prepareQA.py [-h] [-V] [--list] [--colors] [-g pdfname] -O outfile -H histosFile outfile [-T textFile text_qafile_1 text_qafile_2 ...]'
    print ' Produce QA file from reference file, looking for QA lines in qafile_ '
    print '    referencefile : reference file '
    print '    outfile : output file '
    print '    qafile_ : QA files with lines that are used if found in referencefile '
    print '    -h : this help'
    print '    -v : verbose'
    print '    -V : very verbose'
    print '    -g : produce pdf with plots'
    print ''

def printList():
    from Tests import _testsMap
    print 'Known Tests:'
    for k,v in _testsMap.iteritems():
        print "Test: %s"%k
        print v.__doc__

from Utils import Error,logger,NotYet,WrongDataType
from Interface import Configuration

##################################################################
def runQA( textFile, histosFiles, outputFile, qafiles , doText = False , doHistos = True ) :
    _logger.info("Preparing to run tests for run; %s ",doText)

    #We need to reduce verbosity in ROOT...
    #See: http://root.cern.ch/phpBB3/viewtopic.php?t=10902&p=46976
    import ROOT
    ROOT.gErrorIgnoreLevel = 1001

    fout = open(outputFile,"w")

    #Text part...
    if doText:
        _logger.debug("TEXT REFERENCE FILE : %s"%textFile)
        fout.write("Text = {\n")
        fout.write("    'DefaultTestName' : 'NormalCDF',\n")
        fout.write("    'DefaultThresholds' : [ 0.1,0.3],\n")
        fout.write("\n")
        fout.write("    'Lines' : [\n")
        for qafile in qafiles:
            _logger.debug("Configuration is: %s"%qafile)
            conf = Configuration(qafile)
        #Open TEXT file and get its content...
            conf.parseAndCheckText()
            conf.fillRefData(textFile)
            _logger.debug("CHECK LINES")
            for dline in conf.text[Configuration.LINESKEY] :
# write in qa file those lines found in input text reference file
                _logger.debug("CHECK LINE %s"%dline)
                if dline.has_key(Configuration.VALUEREFKEY) :
                    _logger.debug("OK LINE %s"%dline)
                    fout.write("        {\n")
                    if dline.has_key(Configuration.DEFAULTTHRESHOLDSKEY):
                        fout.write("            'Thresholds' : [ " + str(dline[Configuration.THRESHOLDKEY][0]) + " , " + str(dline[Configuration.THRESHOLDKEY][1]) + " ]\n")
                    dwords = dline[Configuration.WORDSKEY]
                    lw = "            'Words' : { "
                    for dwordk in dwords :
                        lw += str(dwordk) + " : '" + dline[Configuration.WORDSKEY][dwordk] + "', "
                    lw += "},\n"
                    fout.write(lw)
#                    fout.write("            'Words' : { 1 : 'PART_COUNT:', 2 : 'gamma' },\n")   
                    fout.write("            'ValueCol' : " + str(dline[Configuration.VALUECOLKEY]) +  ",\n")
                    if dline.has_key(Configuration.ERRORCOLKEY) :
                        fout.write("            'ErrorCol' : " + str(dline[Configuration.ERRORCOLKEY]) +  ",\n")
                    fout.write("            'ErrorType' : '" + str(dline[Configuration.ERRORTYPEKEY]) +  "'\n")

                    fout.write("        },\n")
                    fout.write("\n")
        fout.write("        ] #End of Text definition\n")
        fout.write("    } #End of configuration\n")

    #Histograms part...
    if doHistos:
        fout.write("Binned = {\n")
        fout.write("    'DefaultTestName' : 'BinnedKolmogorovSmirnovTest',\n")
        fout.write("    'DefaultThresholds' : [ 0.1,0.3],\n")
        fout.write("\n")
        fout.write("    'Histos' : [\n")
        for ih in range(0,len(histosFiles)) :
            histosFile = histosFiles[ih]
            tfilecontent = []
            _logger.debug("HISTOS REFERENCE FILE : %s"%histosFile)

        #Open ROOT file and get its content...
            from ROOT import TFile
            _logger.debug("Reading file content...")
            tfile = TFile.Open( histosFile )
            import ROOTIO
            selectedHistos = []
            tfilecontent += ROOTIO.readDirectory( tfile )
            _logger.debug("Input file read. It contains %d objects"%len(tfilecontent))
            for el in tfilecontent:
                _logger.debug('Checking match for: %s '%el)
                selectedHistos.append( el )
                _logger.debug('Checking match for: %s '%tfile)
                histos = ROOTIO.buildHistogramInputs( tfile , selectedHistos )
                _logger.debug('Checking match for: %s '%el)
        #Now loop on histograms, store Algorithms to be executed                

            histos1 = []
            for hin in histos :
                if hin.getObject().GetDimension() == 1 :
                    histos1.append(hin)
            for ii in range(0,len(histos1)):
                hin = histos1[ii]
                _logger.info("Getting configuration for: %s"%hin.name)
                fout.write("        {\n")
                fout.write("            'Name' : '" + hin.name + "'\n")
                if ii == len(histos1)-1 and ih == len(histosFiles) -1 : 
                    fout.write("        }\n")
                else:
                    fout.write("        },\n")
        fout.write("        ] #End of Histos definition\n")
        fout.write("    } #End of configuration\n")

    fout.close()
        
#################################################
def run( argv = sys.argv ):
    from Interface import runROOT
    if len(argv)>=1:
        import getopt
        try:
            opts,args = getopt.getopt( argv ,"hvVT:O:H:g:",["list","colors"])
        except getopt.GetoptError,err:
            sys.stderr.write( str(err) )
            usage()
            sys.exit(2)
            
        inputText = ""
        outputFile = ""
        inputHistos = []
        for op,ar in opts:
#            print("OP AR %s %s"%(op,ar))
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
            elif op == '-g':
                doplots=ar
            elif op == "--colors":
                colors = True
            elif op == '-O':
                outputFile = ar
            elif op == '-T':
                inputText = ar
            elif op == '-H':
                inputHistos.append(ar)
        from Interface import _logger
        qafiles = []

        if inputText == "" and len(inputHistos) == 0 :
            usage()
            sys.exit(0)
        if inputText != "" and len(args) < 1 :  # do not use -T
            print "DO NOT USE -T OPTION"
            usage()
            sys.exit(0)
        if len(args)>=1:
            _logger.info("FILLING qafiles; %ls ",len(args))
            try:
                for ii in range(0,len(args)) :
                    qafiles.append(args[ii])
                    errcode = runQA( inputText, inputHistos, outputFile, qafiles , doText = (inputText!="") , doHistos = (inputHistos!="") ) 
            except Exception,e:
                sys.stderr.write("Error: %s\n"%e)
                sys.exit(1)
        else:
            print "USAGE 5"
            usage()
            sys.exit(0)
            
    else:
        print "USAGE 6"
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
