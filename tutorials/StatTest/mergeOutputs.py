#!/bin/env python
#Main program

import sys
import math
import Interface

def usage():
    print 'Usage: mergeOutputs.py [-h] [-V] conffile inputfilelist'
    print ' Merges files in inputfilelist using lines defined in conffile'
    print '    conffile : configuration file'
    print '    inputfilelist : list of input files containing distributions'
    print '    outputfile : output file, containing merged results'
    print '    -h : this help'
    print '    -v : verbose'
    print '    -V : very verbose'
    print ''
    print ' Example: runtests.py testconf.qa gamos.log'
    print '          See content of example directory for an example of a configuration file'

def printList():
    from Tests import _testsMap
    print 'Known Tests:'
    for k,v in _testsMap.iteritems():
        print "Test: %s"%k
        print v.__doc__
        
def run( argv = sys.argv ):
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
        if len(args)>=3:
            try:
                if len(args)==3:
                    confs = []
                    finlis = open(args[1])
                    fout = open(args[2],"w")
                    llines = finlis.readlines()
### FIND Number of events processed
                    fname1 = llines[0].rstrip()
                    fin1 = open(fname1)
                    lines1 = fin1.readlines()
                    for line1 in lines1:
                        if line1.find("Number of events processed") != -1 : 
                            fout.write("%s\n" % line1)
                            break

                    for lline in llines:
                        Interface._logger.info("LINE : %s"% lline)
                        lline = lline.rstrip()
#                        Interface.runMerge(args[0],args[1])
#                        Interface.runMerge(args[0],lline)
#                        fin = open(lline)
                        conf = Interface.Configuration(args[0])
                        conf.parseAndCheckText()
                        conf.fillRefData(lline)
                        print "READING FILE %s " % lline
                        sys.stdout.flush()
                        conf.readTextFile(lline)
                        confs.append(conf)
# files read, process them
                    Interface._logger.info("N CONFS: %s"% str(len(confs)))
                    clines1st = confs[0].text['Lines']
# loop to lines
                    for cline1st in clines1st:
                        cwords1st = cline1st['Words'] 
                        line2print = ""
                        for cword in cwords1st:
                            line2print = line2print + cline1st['Words'][int(cword)] + " "
                        Interface._logger.info("LINE2PRINT: %s" % line2print)
#                        print "LINE2PRINT: %s" % line2print
                        linestr1st = ""
                        for cword1st in cwords1st:
                            linestr1st = linestr1st + str(cword1st) +  cline1st['Words'][int(cword1st)] 
                        value = 0.
                        value2 = 0.
# get line in each of the conf (= each of the files)
                        icont = 0
                        for conf in confs: 
                            icont += 1
                            Interface._logger.info("LOOPING CONF")
                            clines = conf.text['Lines'] 
                            for cline in clines:
                                cwords = cline['Words']
                                bEqual = 0
#                                for (cword1st, cword) in (cwords1st, cwords):
#                                cwordite = cwords.__iter__()
                                if( len(cwords1st) == len(cwords) ) : 
                                    linestr = ""
                                    for cword in cwords:
                                        linestr = linestr + str(cword) + cline['Words'][int(cword)] 
#                                        print "CHECK: %s " % cline['Words'][int(cword)] 
#                                        print "CHECK: %s " % cline1st['Words'][int(cword1st)] 
#                                    if cline1st['Words'][int(cword1st)] != cline['Words'][int(cword)] :
                                    Interface._logger.info("CHECK %s %s " % (linestr1st,linestr))
                                    if linestr1st == linestr :
                                        bEqual = 1
                                        break
                                   #                                print "WORD: %s" % dline['Words'][dword]
                                    #                            print "VALUE: %s"% dline['Value']
                            if bEqual == 1:
                                if cline.has_key('Value') :
                                    value = value + float(cline['Value'])
                                    value2 = value2 + float(cline['Value'])*float(cline['Value'])
                                    Interface._logger.info("VALUEEQUAL= %f " % value)
                                else :
                                    Interface._logger.info("NO VALUE FOUND %s, %f " % (linestr1st,icont))
#  print "VALUEEQUAL= %f " % value
                        nval = len(confs)
                        rms = math.sqrt(nval*value2-value*value)/(nval-1)
                        prtbuf = line2print + " " + str(value/len(confs)) + " +- " + str(rms)
#                        print "FINAL %s " % prtbuf
                        print "MERGED: %s " % prtbuf
                        fout.write("%s\n" % prtbuf)

            except Exception,e:
                sys.stderr.write("Error: %s\n"%e)
                sys.exit(1)
         
        else:
            usage()
            sys.exit(0)
            
    else:
        usage()
        sys.exit(0)
#    if errcode > 0:
#        sys.stderr.write( "Number of FAILED tests: %d\n"%errcode)
#        sys.exit(1)
    sys.exit( 0 )

if __name__ == "__main__":
    run( sys.argv[1:])
