#!/usr/bin/env python3
"""
prepareQA.py : writes the configuration file EXERCISE.qaref of an exercise, used by runN.sh and run1qa.sh
(Python 3, standard library only).

Usage:
  prepareQA.py [-T OUTPUT_FILE] [-H HISTO_FILE.csv] [-H HISTO_FILE.csv] ... QAREF_FILE [TEMPLATE_FILE_1 TEMPLATE_FILE_2 ...]

  -T OUTPUT_FILE   : GAMOS output file of one job of the exercise; the lines of the template files
                     (written by prepareQATemplate.py) that are found in it are written in the 'Text' block
  -H HISTO_FILE    : CSV histogram file written by the job; all its 1D histograms (and profiles 1D)
                     are written in the 'Binned' block (can be repeated)
  QAREF_FILE       : configuration file to write

Example:
  prepareQATemplate.py out.exercise2b PART_COUNT.qa PART_COUNT
  prepareQATemplate.py out.exercise2b PROC_COUNT.qa PROC_COUNT
  prepareQA.py -T out.exercise2b -H hits.exercise2b.csv exercise2b.qaref PART_COUNT.qa PROC_COUNT.qa
"""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from statTest import read_configuration, get_text_block, read_text_values, read_csv_histos, StatTestError  # noqa: E402
from prepareQATemplate import text_line_entry  # noqa: E402


class OutText:
    def __init__(self):
        self.chunks = []

    def write(self, text):
        self.chunks.append(text)


def prepare_qa(qaref_file, text_file, histo_files, template_files):
    nlines = 0
    nhistos = 0
    fout = OutText()
    if text_file and template_files:
        fout.write("Text = {\n")
        fout.write("    'DefaultTestName' : 'NormalCDF',\n")
        fout.write("    'DefaultThresholds' : [ 0.05, 0.3 ],\n")
        fout.write("\n")
        fout.write("    'Lines' : [\n")
        for template in template_files:
            conf = read_configuration(template)
            try:
                text, lines = get_text_block(conf, template)
            except StatTestError:
                print("!!! NO LINES IN TEMPLATE FILE %s" % template)
                continue
            values = read_text_values(text_file, lines, False)[0]
            for dline, val in zip(lines, values):
                if val is None:
                    continue
                words = [dline['Words'][k] for k in sorted(dline['Words'])]
                fout.write(text_line_entry(words, dline['ValueCol'], dline.get('Thresholds')))
                fout.write("\n")
                nlines += 1
        fout.write("        ] #End of Lines definition\n")
        fout.write("    } #End of configuration\n")
    if histo_files:
        fout.write("Binned = {\n")
        fout.write("    'DefaultTestName' : 'BinnedKolmogorovSmirnovTest',\n")
        fout.write("    'DefaultThresholds' : [ 0.05, 0.3 ],\n")
        fout.write("\n")
        fout.write("    'Histos' : [\n")
        for histo_file in histo_files:
            for name, his in read_csv_histos(histo_file).items():
                if his.is2D():
                    continue
                fout.write("        {\n")
                fout.write("            'Name' : %r\n" % name)
                fout.write("        },\n")
                nhistos += 1
        fout.write("        ] #End of Histos definition\n")
        fout.write("    } #End of configuration\n")
    # the file is written only if there was no error
    with open(qaref_file, "w") as fqa:
        fqa.write("".join(fout.chunks))
    print("CONFIGURATION FILE %s WRITTEN WITH %d TEXT LINES AND %d HISTOGRAMS" % (qaref_file, nlines, nhistos))


def main(argv):
    text_file = None
    histo_files = []
    args = []
    ii = 0
    while ii < len(argv):
        if argv[ii] in ("-h", "--help"):
            print(__doc__)
            return 0
        if argv[ii] in ("-T", "-H"):
            if ii + 1 >= len(argv):
                print("Option %s needs a file name" % argv[ii])
                return 1
            if argv[ii] == "-T":
                text_file = argv[ii + 1]
            else:
                histo_files.append(argv[ii + 1])
            ii += 2
            continue
        args.append(argv[ii])
        ii += 1
    if len(args) < 1 or (text_file is None and not histo_files):
        print(__doc__)
        return 1
    if text_file and len(args) < 2:
        print("!!! -T IS GIVEN BUT NO TEMPLATE FILE: NO TEXT LINES WILL BE TESTED")
    try:
        prepare_qa(args[0], text_file, histo_files, args[1:])
    except StatTestError as exc:
        sys.stderr.write("prepareQA.py ERROR: %s\n" % exc)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
