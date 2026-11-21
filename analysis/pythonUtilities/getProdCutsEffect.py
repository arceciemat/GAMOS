#!/usr/bin/env python3
"""getProdCutsEffect.py: production cuts (or minimum-range user limits) that lose less than a given
percentage of the particles reaching the target.

It reads the histogram file written by GmProdCutsStudyUA (prodcuts.csv / prodcuts.root) or by
GmMinRangeLimitsStudyUA (minrange.csv / minrange.root). For each region and particle it takes the
histogram "<UA>: REGION: PARTICLE: ALL:  log10(Range)", that contains the range of each particle
(and of all its ancestors) that reached the target, and finds the biggest cut such that the number
of entries with range smaller than the cut is not bigger than PERCENTAGE % of the number of tracks
that reached the target.

Usage:
   python3 getProdCutsEffect.py FILE PERCENTAGE

   FILE        prodcuts.csv, prodcuts.root, minrange.csv or minrange.root
   PERCENTAGE  maximum percentage of tracks reaching the target that may be lost (e.g. 1 for 1%)

Output (one line per region and particle, then one line per particle):
   REGION= R PARTICLE= P CUT= C mm LOST= n / N
   PARTICLE= P FINAL= n / N
where N is the number of tracks that reached the target and n the number of track informations
(target tracks or ancestors) that would be lost with that cut. As one target track may be counted
in several regions (itself and its ancestors), the FINAL sum over regions is an upper limit.
"""
import sys
import csv
import math

HISTO_SUFFIX = "log10(Range)"
NTARGET_NAME = "N trackInfos per Track in target"


#----------------------------------------------------------------------
class Histo1:
    """ minimal 1D histogram: name, bin edges, contents (index 0 = underflow, nbins+1 = overflow) """
    def __init__(self, name, nbins, xmin, xmax, contents, nent):
        self.name = name
        self.nbins = nbins
        self.xmin = xmin
        self.xmax = xmax
        self.nent = nent
        # GAMOS normalises the histograms to the number of events by default
        # (parameter <FILE>:NormalizeToNEvents): convert the contents back to number of entries
        total = sum(contents)
        scale = nent/total if total > 0 else 0.
        self.contents = [cc*scale for cc in contents]

    def LowEdge(self, ib):
        """ lower edge of bin ib (1..nbins+1) """
        return self.xmin + (ib-1)*(self.xmax-self.xmin)/self.nbins


#----------------------------------------------------------------------
def ReadCSV(fileName):
    """ read the 1D histograms of a GAMOS .csv histogram file (format of GmHistoWriterCSV) """
    histos = {}
    with open(fileName, 'r') as csvFile:
        for row in csv.reader(csvFile):
            if len(row) == 0 or row[0] != '1D':
                continue
            nbins = int(float(row[2]))
            # after the header come (content,error) for underflow, the nbins bins and overflow, then entries
            contents = [float(row[5+2*ii]) for ii in range(nbins+2)]
            nent = float(row[2*nbins+9])
            histos[row[1]] = Histo1(row[1], nbins, float(row[3]), float(row[4]), contents, nent)
    return histos


#----------------------------------------------------------------------
def ReadROOT(fileName):
    """ read the 1D histograms of a ROOT file, with uproot if available, else with PyROOT """
    histos = {}
    try:
        import uproot
        with uproot.open(fileName) as rfile:
            for key, obj in rfile.items(recursive=True, cycle=False):
                if not obj.classname.startswith("TH1"):
                    continue
                vals = obj.values(flow=True)
                edges = obj.axis().edges()
                name = obj.member("fName")
                histos[name] = Histo1(name, len(edges)-1, edges[0], edges[-1], list(vals), obj.member("fEntries"))
        return histos
    except ImportError:
        pass
    import ROOT
    rfile = ROOT.TFile.Open(fileName)
    if not rfile or rfile.IsZombie():
        sys.exit("getProdCutsEffect.py: cannot open "+fileName)

    def Walk(tdir):
        for key in tdir.GetListOfKeys():
            obj = key.ReadObj()
            if obj.InheritsFrom("TDirectory"):
                Walk(obj)
            elif obj.InheritsFrom("TH1") and obj.GetDimension() == 1:
                nb = obj.GetNbinsX()
                ax = obj.GetXaxis()
                histos[obj.GetName()] = Histo1(obj.GetName(), nb, ax.GetXmin(), ax.GetXmax(),
                                               [obj.GetBinContent(ib) for ib in range(nb+2)], obj.GetEntries())
    Walk(rfile)
    return histos


#----------------------------------------------------------------------
def CutForLoss(his, maxLost):
    """ biggest bin edge such that the entries below it are not more than maxLost.
        Returns (log10(cut), nLost); log10(cut) is None if no entry can be lost (not even the underflow) """
    lost = his.contents[0]  # underflow: range < 10^xmin, lost with any cut
    if lost > maxLost:
        return None, 0.
    for ib in range(1, his.nbins+2):
        if lost + his.contents[ib] > maxLost:
            return his.LowEdge(ib), lost
        lost += his.contents[ib]
    return float("inf"), lost  # all entries can be lost: any cut


#----------------------------------------------------------------------
def main(argv):
    if len(argv) != 3 or argv[1] in ("-h", "-help", "--help"):
        print(__doc__)
        return 1
    fileName = argv[1]
    try:
        percentage = float(argv[2])
    except ValueError:
        sys.exit("getProdCutsEffect.py: PERCENTAGE must be a number, not '"+argv[2]+"'")

    histos = ReadROOT(fileName) if fileName.endswith(".root") else ReadCSV(fileName)

    hisNTarget = None
    for name, his in histos.items():
        if name.endswith(NTARGET_NAME):
            hisNTarget = his
    if hisNTarget is None:
        sys.exit("getProdCutsEffect.py: histogram '"+NTARGET_NAME+"' not found in "+fileName
                 +": is it a file written by GmProdCutsStudyUA or GmMinRangeLimitsStudyUA?")
    nTarget = hisNTarget.nent
    if nTarget == 0:
        sys.exit("getProdCutsEffect.py: no track reached the target in "+fileName)
    maxLost = nTarget*percentage/100.

    # histograms "UA: REGION: PARTICLE: ALL:  log10(Range)"
    results = []
    for name, his in sorted(histos.items()):
        if not name.endswith(HISTO_SUFFIX):
            continue
        words = [ww.strip() for ww in name[:-len(HISTO_SUFFIX)].split(":")]
        if len(words) < 4 or words[3] != "ALL":
            continue
        region, particle = words[1], words[2]
        if region == "ALL":  # sum over regions (GmMinRangeLimitsStudyUA): not a cut per region
            continue
        logCut, nLost = CutForLoss(his, maxLost)
        results.append((region, particle, logCut, nLost, his.nent))

    if len(results) == 0:
        sys.exit("getProdCutsEffect.py: no histogram '...: ALL:  "+HISTO_SUFFIX+"' found in "+fileName)

    print("getProdCutsEffect.py: "+fileName+"  tracks reaching the target= %d  maximum lost= %g %%" % (nTarget, percentage))
    finalLost = {}
    for region, particle, logCut, nLost, nent in results:
        if logCut is None:
            cutStr = "NONE (the underflow, range < %g mm, has already too many entries)" % 10**histos_xmin(histos, region, particle)
        elif math.isinf(logCut):
            cutStr = "ANY (all %d entries can be lost)" % nent
        else:
            cutStr = "%.4g mm" % 10**logCut
        print("REGION= %s PARTICLE= %s CUT= %s LOST= %d / %d" % (region, particle, cutStr, round(nLost), nTarget))
        finalLost[particle] = finalLost.get(particle, 0) + nLost
    for particle in sorted(finalLost):
        print("PARTICLE= %s FINAL= %d / %d" % (particle, round(finalLost[particle]), nTarget))
    return 0


#----------------------------------------------------------------------
def histos_xmin(histos, region, particle):
    for name, his in histos.items():
        if name.endswith(HISTO_SUFFIX) and (": "+region+": "+particle+": ALL:") in name:
            return his.xmin
    return -3.


if __name__ == "__main__":
    sys.exit(main(sys.argv))
