#!/usr/bin/env python3
"""Merge the output files written by the worker threads of a multithreaded GAMOS job.

NOTE: this is NOT needed in normal use: GAMOS merges the thread files itself at the end of the job
(GmMTMergeMgr::MergeThreadFiles, called by the gamos main program) and deletes them. This script is only for
thread files kept with /gamos/setParam GmMTMergeMgr:KeepThreadFiles 1, or left by a job that crashed.

In multithreading, user actions that write event by event (GmStepDataTextFileUA, GmTrackDataBinFileUA, ...,
GmHitsWriteUA, PET/SPECT/Compton camera list mode files, GmShowerShapeUA, ...) write one file per worker thread:
NAME.thN.EXT (N = thread number). This script concatenates them, in thread order, into NAME.EXT.

This is valid for files made of independent records (text lines or fixed-size binary records), which is the case
for the default output of these user actions. It is NOT valid for:
  - binary data files written with the header options (*:WriteHeaderData, *:WriteHeaderNEvents, *:WriteHeaderNCalls,
    *:WriteHeaderCheck): each thread file starts with its own header
  - IAEA phase space files: they are merged by RTPhaseSpaceUA at the end of the job (see also mergeIAEAphsp.py)
  - ROOT files (GmDataTTreeUA NAME_tree_INDEX.thN.root): use ROOT 'hadd'
For text files with header lines use --skip-lines N: the first N lines of all files except the first are dropped.

Usage:
  mergeThreadFiles.py [--skip-lines N] [-o OUT] NAME.EXT     -> merges NAME.th*.EXT into NAME.EXT (or OUT)
  mergeThreadFiles.py [--skip-lines N] -o OUT IN1 IN2 ...
"""
import sys, glob, os, re


def thread_files(name):
    base, ext = os.path.splitext(name)
    if os.path.sep in ext:
        base, ext = name, ""
    files = glob.glob(glob.escape(base) + ".th*" + glob.escape(ext))
    pat = re.compile(re.escape(base) + r"\.th(\d+)" + re.escape(ext) + "$")
    files = [f for f in files if pat.match(f)]
    return sorted(files, key=lambda f: int(pat.match(f).group(1)))


def merge(out, inputs, skip_lines):
    if os.path.abspath(out) in [os.path.abspath(i) for i in inputs]:
        sys.exit("ERROR: output file is one of the input files: " + out)
    with open(out, "wb") as fout:
        for ii, fin_name in enumerate(inputs):
            with open(fin_name, "rb") as fin:
                if ii != 0 and skip_lines > 0:
                    for _ in range(skip_lines):
                        fin.readline()
                while True:
                    buf = fin.read(1 << 24)
                    if not buf:
                        break
                    fout.write(buf)
    print("Merged %d files into %s (%d bytes): %s" % (len(inputs), out, os.path.getsize(out), " ".join(inputs)))


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        sys.exit(0)
    skip_lines = 0
    out = None
    rest = []
    ii = 0
    while ii < len(args):
        if args[ii] == "--skip-lines":
            skip_lines = int(args[ii + 1])
            ii += 2
        elif args[ii] == "-o":
            out = args[ii + 1]
            ii += 2
        else:
            rest.append(args[ii])
            ii += 1
    if len(rest) == 1 and (out is None or not os.path.exists(rest[0])):
        name = rest[0]
        inputs = thread_files(name)
        if out is None:
            out = name
    else:
        if out is None:
            sys.exit("ERROR: with several input files the output must be given with -o OUT")
        inputs = rest
    if not inputs:
        sys.exit("ERROR: no input files found")
    merge(out, inputs, skip_lines)


if __name__ == "__main__":
    main()
