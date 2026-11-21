#!/usr/bin/env python3
"""
prepareQATemplate.py : writes a template configuration file with all the lines of a GAMOS output file
that contain a keyword (Python 3, standard library only).

Usage:
  prepareQATemplate.py OUTPUT_FILE TEMPLATE_FILE KEYWORD [VALUE_COL_SHIFT] [WORD_TO_EXCLUDE]

  OUTPUT_FILE     : GAMOS output file (the output of one job)
  TEMPLATE_FILE   : template file to write
  KEYWORD         : lines containing this string are selected
  VALUE_COL_SHIFT : position of the value counting from the end of the line (0 = last word, default 0)
  WORD_TO_EXCLUDE : lines containing this string are not selected

For each selected line, the words before the value are the 'Words' that identify the line, 'ValueCol' is the
position of the value and 'ErrorCol' is ValueCol+2, with 'ErrorType' 'ABS': in the reference file
ref.out.EXERCISE written by runN.sh the spread of the values is written two words after the value.
The template files of an exercise are joined into the EXERCISE.qaref file with prepareQA.py.

Example:
  prepareQATemplate.py out.exercise2b PART_COUNT.qa PART_COUNT
  for the line 'PART_COUNT: e- = 69966' writes
        {
            'Words' : { 1 : 'PART_COUNT:', 2 : 'e-', 3 : '=' },
            'ValueCol' : 4,
            'ErrorCol' : 6,
            'ErrorType' : 'ABS'
        },
"""

import sys


def text_line_entry(words, value_col, thresholds=None):
    """ Configuration entry of a line, in the format of the .qaref files """
    entry = "        {\n"
    if thresholds is not None:
        entry += "            'Thresholds' : [ %s, %s ],\n" % (thresholds[0], thresholds[1])
    entry += "            'Words' : { " + ", ".join("%d : %r" % (ii + 1, word) for ii, word in enumerate(words)) + " },\n"
    entry += "            'ValueCol' : %d,\n" % value_col
    entry += "            'ErrorCol' : %d,\n" % (value_col + 2)
    entry += "            'ErrorType' : 'ABS'\n"
    entry += "        },\n"
    return entry


def prepare_template(output_file, template_file, keyword, value_col_shift=0, word_to_exclude=None):
    selected = []
    with open(output_file, errors='replace') as fin:
        for line in fin:
            if keyword not in line:
                continue
            if word_to_exclude and word_to_exclude in line:
                continue
            words = line.split()
            value_col = len(words) - value_col_shift
            if value_col < 2:
                print("!!! LINE TOO SHORT FOR VALUE_COL_SHIFT %d, NOT USED: %s" % (value_col_shift, line.rstrip()))
                continue
            key = tuple(words[:value_col - 1])
            if key in [sel[0] for sel in selected]:
                continue
            selected.append((key, value_col))
    with open(template_file, "w") as fout:
        fout.write("Text = {\n")
        fout.write("    'DefaultTestName' : 'NormalCDF',\n")
        fout.write("    'DefaultThresholds' : [ 0.05, 0.3 ],\n")
        fout.write("\n")
        fout.write("    'Lines' : [\n")
        for key, value_col in selected:
            fout.write(text_line_entry(key, value_col))
            fout.write("\n")
        fout.write("        ] #End of Lines definition\n")
        fout.write("    } #End of configuration\n")
    print("TEMPLATE %s WRITTEN WITH %d LINES CONTAINING '%s'" % (template_file, len(selected), keyword))


def main(argv):
    if len(argv) < 3 or len(argv) > 5 or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 1
    shift = 0
    if len(argv) >= 4:
        try:
            shift = int(argv[3])
        except ValueError:
            print("VALUE_COL_SHIFT must be an integer: %s" % argv[3])
            return 1
    exclude = argv[4] if len(argv) == 5 else None
    prepare_template(argv[0], argv[1], argv[2], shift, exclude)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
