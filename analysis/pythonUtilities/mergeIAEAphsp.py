#!/usr/bin/env python3
"""Merge IAEA phase space files written by the worker threads of a multithreaded GAMOS job.

RTPhaseSpaceUA writes, in each worker thread N, the files NAME.thN.IAEAphsp and NAME.thN.IAEAheader,
and merges them automatically at the end of the run into NAME.IAEAphsp/.IAEAheader (the thread files are deleted
unless /gamos/setParam RTPhaseSpaceUA:KeepThreadFiles 1). This script is only needed to merge kept thread files
or the files of several jobs. It concatenates the .IAEAphsp files and builds the merged .IAEAheader:
  - ORIG_HISTORIES, PARTICLES and the number of each particle type are summed
  - CHECKSUM is the size in bytes of the merged .IAEAphsp file
  - STATISTICAL_INFORMATION_PARTICLES: weights summed, minima/maxima combined, <E> weighted by the weights
  - STATISTICAL_INFORMATION_GEOMETRY: minima/maxima combined
All other keys are copied from the first header (all files must have the same record contents).

Usage:
  mergeIAEAphsp.py NAME            -> merges NAME.th*.IAEAphsp into NAME.IAEAphsp / NAME.IAEAheader
  mergeIAEAphsp.py -o OUT IN1 IN2 ... (IN without extension)
"""
import sys, glob, os, re

COUNT_KEYS = ["ORIG_HISTORIES", "PARTICLES", "PHOTONS", "ELECTRONS", "POSITRONS", "NEUTRONS", "PROTONS"]


def read_header(fname):
    """Returns list of (key, [lines]) in file order"""
    blocks = []
    key = None
    lines = []
    with open(fname) as f:
        for line in f:
            line = line.rstrip("\n")
            m = re.match(r"^\$([A-Z_]+):\s*$", line)
            if m:
                if key is not None or lines:
                    blocks.append((key, lines))
                key = m.group(1)
                lines = []
            else:
                lines.append(line)
    blocks.append((key, lines))
    return blocks


def first_number(lines):
    for line in lines:
        w = line.split()
        if w:
            return float(w[0])
    return None


def merge(out, inputs):
    headers = [dict(read_header(i + ".IAEAheader")) for i in inputs]
    blocks0 = read_header(inputs[0] + ".IAEAheader")
    reclen = [first_number(h["RECORD_LENGTH"]) for h in headers]
    if len(set(reclen)) != 1:
        sys.exit("ERROR: different RECORD_LENGTH in input files: " + str(reclen))

    # --- binary files
    with open(out + ".IAEAphsp", "wb") as fout:
        for i in inputs:
            with open(i + ".IAEAphsp", "rb") as fin:
                while True:
                    buf = fin.read(1 << 24)
                    if not buf:
                        break
                    fout.write(buf)
    nbytes = os.path.getsize(out + ".IAEAphsp")

    # --- counts
    sums = {}
    for k in COUNT_KEYS:
        vals = [first_number(h[k]) for h in headers if k in h]
        if vals:
            sums[k] = sum(vals)

    # --- particle statistics:  Weight Wmin Wmax <E> Emin Emax Particle
    stat = {}
    order = []
    for h in headers:
        for line in h.get("STATISTICAL_INFORMATION_PARTICLES", []):
            w = line.split()
            if len(w) < 7 or line.strip().startswith("//"):
                continue
            wei, wmin, wmax, emean, emin, emax = [float(x) for x in w[:6]]
            name = w[6]
            if name not in stat:
                stat[name] = [0., 1.e99, -1.e99, 0., 1.e99, -1.e99]
                order.append(name)
            s = stat[name]
            s[0] += wei
            s[1] = min(s[1], wmin)
            s[2] = max(s[2], wmax)
            s[3] += emean * wei
            s[4] = min(s[4], emin)
            s[5] = max(s[5], emax)

    # --- geometry statistics: one line per stored coordinate, min max
    geom = None
    for h in headers:
        glines = [l.split() for l in h.get("STATISTICAL_INFORMATION_GEOMETRY", []) if len(l.split()) >= 2]
        if geom is None:
            geom = [[float(g[0]), float(g[1])] for g in glines]
        else:
            for ii, g in enumerate(glines):
                if ii < len(geom):
                    geom[ii][0] = min(geom[ii][0], float(g[0]))
                    geom[ii][1] = max(geom[ii][1], float(g[1]))

    # --- write merged header
    with open(out + ".IAEAheader", "w") as fh:
        for key, lines in blocks0:
            if key is None:
                fh.write("\n".join(lines) + ("\n" if lines else ""))
                continue
            fh.write("$" + key + ":\n")
            if key in sums:
                fh.write("%d\n\n" % sums[key])
            elif key == "CHECKSUM":
                fh.write("%d \n\n" % nbytes)
            elif key == "STATISTICAL_INFORMATION_PARTICLES":
                fh.write("//        Weight        Wmin       Wmax       <E>         Emin         Emax    Particle\n")
                for name in order:
                    s = stat[name]
                    emean = s[3] / s[0] if s[0] != 0 else 0.
                    fh.write("%17.6g %10.6g %10.6g %10.6g %13.6g %11.6g   %s\n" % (s[0], s[1], s[2], emean, s[4], s[5], name))
                fh.write("\n")
            elif key == "STATISTICAL_INFORMATION_GEOMETRY" and geom is not None:
                for g in geom:
                    fh.write(" %g  %g\n" % (g[0], g[1]))
                fh.write("\n")
            else:
                fh.write("\n".join(lines) + "\n")
    print("Merged %d files into %s.IAEAphsp/.IAEAheader: %d particles, %d original histories"
          % (len(inputs), out, sums.get("PARTICLES", 0), sums.get("ORIG_HISTORIES", 0)))


def main():
    args = sys.argv[1:]
    if not args or args[0] in ("-h", "--help"):
        print(__doc__)
        sys.exit(0)
    if args[0] == "-o":
        out = args[1]
        inputs = [re.sub(r"\.IAEA(phsp|header)$", "", a) for a in args[2:]]
    else:
        out = args[0]
        inputs = sorted(set(re.sub(r"\.IAEAheader$", "", f) for f in glob.glob(out + ".th*.IAEAheader")),
                        key=lambda s: int(re.search(r"\.th(\d+)$", s).group(1)))
    if not inputs:
        sys.exit("ERROR: no input files found")
    merge(out, inputs)


if __name__ == "__main__":
    main()
