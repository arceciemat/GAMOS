#!/usr/bin/env python3
"""
statTest.py : statistical comparison of GAMOS outputs with reference outputs,
without ROOT and without pyROOT (only the Python 3 standard library).

It is the ROOT-independent version of runtests.py, addSeed.py and mergeOutputs.py.
Histograms are read from the CSV files that GAMOS writes (the default histogram format
when GAMOS is compiled without ROOT), and the plots are written in a PDF file.
The same configuration file (.qaref) is used to merge the outputs of several jobs into a reference file
(mergetext) and to compare the merged files of two versions (text): the values are read in the column ValueCol,
and the errors (spread of the values of the merged jobs) as given by ErrorType and ErrorCol.
The sigma of the difference of two merged files is sqrt(spread_ref^2/Njobs_ref + spread^2/Njobs).
The output of one job can also be compared with a merged file: then sigma is the spread of the reference.

Usage:
  statTest.py addseed INPUT_FILE SEED
      writes INPUT_FILE.SEED with '/gamos/random/restoreSeeds SEED SEED' before /run/beamOn
  statTest.py text [-t TEST_LABEL] [-r REF_LABEL] CONF_FILE OUTPUT_FILE REFERENCE_FILE
      compares the text lines selected in CONF_FILE ('Text' block)
  statTest.py histos [-g REPORT.pdf] [-t TEST_LABEL] [-r REF_LABEL] CONF_FILE CSV_FILE REFERENCE_CSV_FILE
      compares the histograms selected in CONF_FILE ('Binned' block); with -g writes the plots
  statTest.py mergetext CONF_FILE FILE_LIST OUTPUT_FILE
      merges the text lines selected in CONF_FILE of the output files listed in FILE_LIST
  statTest.py mergehistos OUTPUT_CSV CSV_FILE_1 CSV_FILE_2 ...
      sums the histograms of several CSV files (as ROOT hadd does)
  statTest.py list
      lists the available tests

The configuration file has the same format as for runtests.py (see README.qa).
The histograms are compared without normalization (both files should correspond to the same number of events,
as those built by runN.sh): the chi2 tests compare the contents bin by bin, and the Kolmogorov-Smirnov test
(that only compares the shapes) is combined with a test of the integrals (the p-value is the smallest of both).
To compare only the shapes (scaling the reference to the integral of the test histogram, as ROOT does), add
'DefaultNormalize' : True in the 'Binned' block, or 'Normalize' : True for a histogram.
Each test result is printed as
   NAME : FAILED / NOTPASSED / SUCCESS (Test: TEST_NAME) (pval= P_VALUE)
where the result is FAILED if P_VALUE < first threshold, NOTPASSED if P_VALUE < second threshold
and SUCCESS otherwise. The exit code is 1 if any test FAILED.
"""

import sys
import math
import re
import csv

FAILED = "FAILED"
NOTPASSED = "NOTPASSED"
SUCCESS = "SUCCESS"
UNDEFINED = "UNDEFINED"


class StatTestError(Exception):
    pass


#####################################################################
# Configuration file
def read_configuration(conf_file):
    """ The configuration file is a Python file defining the dictionaries 'Text' and/or 'Binned' """
    namespace = {}
    try:
        fin = open(conf_file)
    except IOError as exc:
        raise StatTestError("Cannot open configuration file %s: %s" % (conf_file, exc))
    with fin:
        exec(compile(fin.read(), conf_file, 'exec'), {}, namespace)
    return namespace


def check_thresholds(thr, where):
    if not isinstance(thr, (list, tuple)) or len(thr) != 2:
        raise StatTestError("Thresholds should be a list of size 2 in %s" % where)
    return thr


def result_from_pval(pval, thresholds):
    if pval is None:
        return UNDEFINED
    if pval < thresholds[0]:
        return FAILED
    if pval < thresholds[1]:
        return NOTPASSED
    return SUCCESS


def print_result(name, result, test_name, pval, comment=""):
    msg = " %s : %s (Test: %s) (pval= %s)" % (name.rjust(35), result.ljust(10), test_name, pval)
    if comment:
        msg += " " + comment
    print(msg)


#####################################################################
# Math functions (as in ROOT TMath)
def kolmogorov_prob(z):
    """ TMath::KolmogorovProb """
    fj = (-2., -8., -18., -32.)
    w = 2.50662827
    c1 = -1.2337005501361697
    c2 = -11.103304951225528
    c3 = -30.842513753404244
    u = abs(z)
    if u < 0.2:
        return 1.
    if u < 0.755:
        v = 1. / (u * u)
        return 1. - w * (math.exp(c1 * v) + math.exp(c2 * v) + math.exp(c3 * v)) / u
    if u < 6.8116:
        r = [0., 0., 0., 0.]
        v = u * u
        maxj = max(1, int(round(3. / u)))
        for j in range(maxj):
            r[j] = math.exp(fj[j] * v)
        return 2. * (r[0] - r[1] + r[2] - r[3])
    return 0.


def gamma_q(a, x):
    """ Upper regularized incomplete gamma function Q(a,x) """
    if x <= 0.:
        return 1.
    gln = math.lgamma(a)
    if x < a + 1.:
        # series
        ap = a
        dsum = 1. / a
        delta = dsum
        for _ in range(10000):
            ap += 1.
            delta *= x / ap
            dsum += delta
            if abs(delta) < abs(dsum) * 1.e-15:
                break
        return 1. - dsum * math.exp(-x + a * math.log(x) - gln)
    # continued fraction
    tiny = 1.e-300
    b = x + 1. - a
    c = 1. / tiny
    d = 1. / b
    h = d
    for i in range(1, 10000):
        an = -i * (i - a)
        b += 2.
        d = an * d + b
        if abs(d) < tiny:
            d = tiny
        c = b + an / c
        if abs(c) < tiny:
            c = tiny
        d = 1. / d
        delta = d * c
        h *= delta
        if abs(delta - 1.) < 1.e-15:
            break
    return math.exp(-x + a * math.log(x) - gln) * h


def chi2_prob(chi2, ndf):
    """ TMath::Prob """
    if ndf <= 0:
        return 0.
    if chi2 <= 0.:
        return 1.
    return gamma_q(0.5 * ndf, 0.5 * chi2)


#####################################################################
# Histograms in GAMOS CSV format
class Histo:
    """
    Histogram read from a GAMOS CSV file.
    1D: "1D"/"Prof1D",name,nbins,xmin,xmax, (nbins+2) pairs content,error (first is underflow, last is overflow),
        entries,mean,mean_error,rms,rms_error
    2D: "2D"/"Prof2D",name,nbinsx,xmin,xmax,nbinsy,ymin,ymax, (nbinsx+2)*(nbinsy+2) pairs content,error
        (loop in X outside, in Y inside), entries, mean,mean_error,rms,rms_error for X and for Y
    """
    def __init__(self, kind, name, nx, xmin, xmax, ny=0, ymin=0., ymax=0.):
        self.kind = kind
        self.name = name
        self.nx = nx
        self.xmin = xmin
        self.xmax = xmax
        self.ny = ny
        self.ymin = ymin
        self.ymax = ymax
        ncells = (nx + 2) * (ny + 2) if self.is2D() else nx + 2
        self.cont = [0.] * ncells
        self.err = [0.] * ncells
        self.stats = []

    def is2D(self):
        return self.kind in ("2D", "Prof2D")

    def same_binning(self, other):
        return (self.kind == other.kind and self.nx == other.nx and self.ny == other.ny and
                close(self.xmin, other.xmin) and close(self.xmax, other.xmax) and
                close(self.ymin, other.ymin) and close(self.ymax, other.ymax))

    def index(self, ix, iy):
        return ix * (self.ny + 2) + iy

    def inrange_cells(self, y_outside=False):
        """ indices of the cells inside the axis limits (no underflow/overflow), in the cumulation order """
        if not self.is2D():
            return list(range(1, self.nx + 1))
        if y_outside:
            return [self.index(ix, iy) for iy in range(1, self.ny + 1) for ix in range(1, self.nx + 1)]
        return [self.index(ix, iy) for ix in range(1, self.nx + 1) for iy in range(1, self.ny + 1)]

    def projection_x(self):
        """ contents and errors of the bins 1..nx (summing the Y bins inside the limits for 2D) """
        if not self.is2D():
            return self.cont[1:self.nx + 1], self.err[1:self.nx + 1]
        cont = []
        err = []
        for ix in range(1, self.nx + 1):
            cells = [self.index(ix, iy) for iy in range(1, self.ny + 1)]
            cont.append(sum(self.cont[ic] for ic in cells))
            err.append(math.sqrt(sum(self.err[ic] ** 2 for ic in cells)))
        return cont, err

    def to_csv_row(self):
        row = [self.kind, self.name, self.nx, fmt(self.xmin), fmt(self.xmax)]
        if self.is2D():
            row += [self.ny, fmt(self.ymin), fmt(self.ymax)]
        for ic in range(len(self.cont)):
            row += [fmt(self.cont[ic]), fmt(self.err[ic])]
        row += [fmt(v) for v in self.stats]
        return row


def close(a, b):
    return abs(a - b) <= 1.e-6 * max(1., abs(a), abs(b))


def fmt(val):
    return "%.10g" % val


def read_csv_histos(file_name):
    """ Returns an ordered dictionary name -> Histo """
    histos = {}
    try:
        with open(file_name, 'rb') as fbin:   # check the first bytes in binary: a ROOT file is not valid UTF-8
            head = fbin.read(4)
        fin = open(file_name, newline='', errors='replace')
    except IOError as exc:
        raise StatTestError("Cannot open histogram file %s: %s" % (file_name, exc))
    if head[:1] not in (b'"', b''):
        fin.close()
        what = "a ROOT file" if head == b'root' else "not a GAMOS CSV histogram file"
        raise StatTestError("File %s is %s, it cannot be read as CSV" % (file_name, what))
    with fin:
        for iline, row in enumerate(csv.reader(fin)):
            if len(row) == 0:
                continue
            kind = row[0].strip()
            try:
                if kind in ("1D", "Prof1D"):
                    nx = int(float(row[2]))
                    his = Histo(kind, row[1], nx, float(row[3]), float(row[4]))
                    ifirst = 5
                elif kind in ("2D", "Prof2D"):
                    nx = int(float(row[2]))
                    ny = int(float(row[5]))
                    his = Histo(kind, row[1], nx, float(row[3]), float(row[4]), ny, float(row[6]), float(row[7]))
                    ifirst = 8
                else:
                    continue
                ncells = len(his.cont)
                values = [float(v) for v in row[ifirst:]]
                if len(values) < 2 * ncells:
                    raise StatTestError("Histogram %s in file %s has %d values, it should have at least %d"
                                        % (row[1], file_name, len(values), 2 * ncells))
                his.cont = values[0:2 * ncells:2]
                his.err = values[1:2 * ncells:2]
                his.stats = values[2 * ncells:]
            except (ValueError, IndexError) as exc:
                raise StatTestError("Error reading line %d of file %s: %s" % (iline + 1, file_name, exc))
            histos[his.name] = his
    return histos


def write_csv_histos(file_name, histos):
    with open(file_name, "w") as fout:
        for his in histos.values():
            row = his.to_csv_row()
            fout.write('"%s","%s",' % (row[0], row[1]) + ",".join(str(v) for v in row[2:]) + "\n")


def merge_histos(histos_list):
    """ Sum histograms with the same name (contents are summed, errors are summed in quadrature)
    Histograms that do not have the same binning in all files (e.g. with automatic limits) are not merged """
    # first pass: find the histograms with different binning in some file
    first = {}
    skipped = []
    for histos in histos_list:
        for name, his in histos.items():
            if name not in first:
                first[name] = his
            elif name not in skipped and not first[name].same_binning(his):
                skipped.append(name)
    for name in skipped:
        print("!!! WARNING: histogram %s has different binning in the files to merge, it is not merged" % name)
    merged = {}
    weights = {}
    for histos in histos_list:
        for name, his in histos.items():
            if name in skipped:
                continue
            if name not in merged:
                new = Histo(his.kind, his.name, his.nx, his.xmin, his.xmax, his.ny, his.ymin, his.ymax)
                new.err = [0.] * len(his.err)
                merged[name] = new
                weights[name] = []
            mer = merged[name]
            for ic in range(len(his.cont)):
                mer.cont[ic] += his.cont[ic]
                mer.err[ic] += his.err[ic] ** 2
            weights[name].append((sum(his.cont[ic] for ic in his.inrange_cells()), his.stats))
    for name, mer in merged.items():
        mer.err = [math.sqrt(e2) for e2 in mer.err]
        mer.stats = merge_stats(mer, weights[name])
    return merged


def merge_stats(mer, wstats):
    """ entries are summed, means and RMS are combined weighting with the sum of contents """
    nstat = len(wstats[0][1])
    if any(len(st) != nstat for w, st in wstats) or nstat == 0:
        return wstats[0][1]
    entries = sum(st[0] for w, st in wstats)
    cells = mer.inrange_cells()
    sumw = sum(mer.cont[ic] for ic in cells)
    sumw2 = sum(mer.err[ic] ** 2 for ic in cells)
    neff = sumw * sumw / sumw2 if sumw2 > 0 else entries
    stats = [entries]
    for iaxis in range(nstat // 4):
        imean = 1 + 4 * iaxis
        wtot = sum(w for w, st in wstats)
        if wtot == 0:
            wl = [(st[0], st) for w, st in wstats]
            wtot = sum(w for w, st in wl)
        else:
            wl = wstats
        if wtot == 0:
            mean = rms = 0.
        else:
            mean = sum(w * st[imean] for w, st in wl) / wtot
            mom2 = sum(w * (st[imean + 2] ** 2 + st[imean] ** 2) for w, st in wl) / wtot
            rms = math.sqrt(max(0., mom2 - mean * mean))
        meanerr = rms / math.sqrt(neff) if neff > 0 else 0.
        rmserr = rms / math.sqrt(2. * neff) if neff > 0 else 0.
        stats += [mean, meanerr, rms, rmserr]
    return stats


#####################################################################
# Histogram tests (as ROOT TH1::KolmogorovTest and TH1::Chi2Test)
class HistoTest:
    """ If normalize is True the shapes are compared (the reference is scaled to the integral of the test histogram,
    as ROOT does); if it is False (default) the contents are compared as they are, which is right when both
    histograms correspond to the same number of events """
    def __init__(self, h1, h2, normalize=False):
        self.h1 = h1
        self.h2 = h2
        self.normalize = normalize
        self.comment = ""

    @staticmethod
    def sums(his, cells):
        return sum(his.cont[ic] for ic in cells), sum(his.err[ic] ** 2 for ic in cells)


class BinnedKolmogorovSmirnovTest(HistoTest):
    """ Kolmogorov-Smirnov test of two histograms (as ROOT TH1::KolmogorovTest with no option,
    TH2::KolmogorovTest for 2D histograms); under/overflows are not used """
    def pval(self):
        h1, h2 = self.h1, self.h2
        cells = h1.inrange_cells()
        sum1, w1 = self.sums(h1, cells)
        sum2, w2 = self.sums(h2, cells)
        if sum1 == 0 and sum2 == 0:
            return 1.
        if sum1 == 0 or sum2 == 0:
            self.comment = "(one histogram is empty)"
            return 0.
        esum1 = sum1 * sum1 / w1 if w1 > 0 else 0.
        esum2 = sum2 * sum2 / w2 if w2 > 0 else 0.
        if w1 <= 0 and w2 <= 0:
            self.comment = "(errors are zero for both histograms)"
            return 0.
        if w1 <= 0:
            factnm = math.sqrt(esum2)
        elif w2 <= 0:
            factnm = math.sqrt(esum1)
        else:
            factnm = math.sqrt(esum1 * esum2 / (esum1 + esum2))
        # for 2D histograms the maximum distance is the average of the cumulations along X first and along Y first
        # (ROOT TH2::KolmogorovTest uses sum2 instead of esum2 in factnm, which is wrong for weighted histograms)
        orders = [False, True] if h1.is2D() else [False]
        dfmaxs = []
        for y_outside in orders:
            dfmax = rsum1 = rsum2 = 0.
            for ic in h1.inrange_cells(y_outside):
                rsum1 += h1.cont[ic] / sum1
                rsum2 += h2.cont[ic] / sum2
                dfmax = max(dfmax, abs(rsum1 - rsum2))
            dfmaxs.append(dfmax)
        pks = kolmogorov_prob(sum(dfmaxs) / len(dfmaxs) * factnm)
        if self.normalize:
            return pks
        # the KS test only compares the shapes: without normalization the integrals are also compared
        # (normal test with sigma = sqrt(sum of errors^2 of both)) and the p-value is the smallest of the two
        sig = math.sqrt(w1 + w2)
        nsig = (sum1 - sum2) / sig if sig > 0 else (0. if sum1 == sum2 else float('inf'))
        pint = 0. if math.isinf(nsig) else 1. + math.erf(-abs(nsig) / math.sqrt(2.))
        self.comment = "(KS pval= %.4g, integrals %s= %g %s= %g pval= %.4g)" % (pks, LABELS['test'], sum1, LABELS['ref'], sum2, pint)
        return min(pks, pint)


class Binned1DChi2Test(HistoTest):
    """ Chi2 test of two unweighted histograms (as ROOT TH1::Chi2Test with option UU) """
    def pval(self):
        h1, h2 = self.h1, self.h2
        cells = h1.inrange_cells()
        sum1 = sum(h1.cont[ic] for ic in cells)
        sum2 = sum(h2.cont[ic] for ic in cells)
        if sum1 == 0 or sum2 == 0:
            self.comment = "(one histogram is empty)"
            return 0. if sum1 != sum2 else 1.
        chi2 = 0.
        ndf = len(cells) - (1 if self.normalize else 0)
        for ic in cells:
            cnt1, cnt2 = h1.cont[ic], h2.cont[ic]
            if cnt1 == 0 and cnt2 == 0:
                ndf -= 1
                continue
            if self.normalize:
                chi2 += (sum2 * cnt1 - sum1 * cnt2) ** 2 / (cnt1 + cnt2)
            else:
                chi2 += (cnt1 - cnt2) ** 2 / (cnt1 + cnt2)
        if self.normalize:
            chi2 /= sum1 * sum2
        self.comment = "(chi2/ndf= %g/%d)" % (chi2, ndf)
        return chi2_prob(chi2, ndf)


class BinnedWeighted1DChi2Test(HistoTest):
    """ Chi2 test of two weighted histograms (as ROOT TH1::Chi2Test with option WW) """
    def pval(self):
        h1, h2 = self.h1, self.h2
        cells = h1.inrange_cells()
        sum1 = sum(h1.cont[ic] for ic in cells)
        sum2 = sum(h2.cont[ic] for ic in cells)
        if sum1 == 0 or sum2 == 0:
            self.comment = "(one histogram is empty)"
            return 0. if sum1 != sum2 else 1.
        chi2 = 0.
        ndf = len(cells) - (1 if self.normalize else 0)
        for ic in cells:
            e1sq, e2sq = h1.err[ic] ** 2, h2.err[ic] ** 2
            if e1sq == 0 and e2sq == 0:
                ndf -= 1
                continue
            if self.normalize:
                sigma = sum1 * sum1 * e2sq + sum2 * sum2 * e1sq
                delta = sum2 * h1.cont[ic] - sum1 * h2.cont[ic]
            else:
                sigma = e1sq + e2sq
                delta = h1.cont[ic] - h2.cont[ic]
            chi2 += delta * delta / sigma
        self.comment = "(chi2/ndf= %g/%d)" % (chi2, ndf)
        return chi2_prob(chi2, ndf)


HISTO_TESTS = {
    "BinnedKolmogorovSmirnovTest": BinnedKolmogorovSmirnovTest,
    "KolmogorovSmirnovTest": BinnedKolmogorovSmirnovTest,
    "Binned1DChi2Test": Binned1DChi2Test,
    "BinnedWeighted1DChi2Test": BinnedWeighted1DChi2Test,
}


#####################################################################
# Text tests
def split_line(line):
    return line.split()


def line_name(dline):
    return "".join("%s " % dline['Words'][k] for k in dline['Words'])


def line_matches(dline, words):
    dwords = dline['Words']
    if len(dwords) == 0:
        return False
    for k, word in dwords.items():
        if k < 1 or k > len(words) or words[k - 1] != word:
            return False
    return True


def nev_error(value):
    """ error for ErrorType 'NEV' (as computed for the reference file by runtests.py) """
    value = float(value)
    if value == 0:
        return 0.
    err = math.sqrt(1. / abs(value))
    if err < 1:
        err = err * math.sqrt(abs(1. - err)) * value
    return abs(err)


def read_text_values(file_name, lines, with_errors):
    """ Returns a list with (value, error) for each configured line (None if not found) and the number of events.
    The errors are only read if with_errors is True (for the reference file, whose lines are 'VALUE +- SPREAD');
    otherwise error is None """
    try:
        fin = open(file_name, errors='replace')
    except IOError as exc:
        raise StatTestError("Cannot open file %s: %s" % (file_name, exc))
    values = [None] * len(lines)
    nevents = 0
    with fin:
        for line in fin:
            words = split_line(line)
            if "Number of events processed" in line and len(words) > 5:
                nevents = words[5]
            if "Run Aborted after" in line and len(words) > 3:
                nevents = words[3]
            for idl, dline in enumerate(lines):
                if not line_matches(dline, words):
                    continue
                vcol = dline['ValueCol']
                if vcol > len(words):
                    raise StatTestError("Line too short to get value, %d < %d: %s" % (len(words), vcol, line))
                value = words[vcol - 1]
                if not with_errors:
                    values[idl] = (value, None, words)
                    continue
                etype = dline.get('ErrorType', 'ABS')
                ecol = dline.get('ErrorCol', 0)
                if etype == "ABS":
                    if 0 < ecol <= len(words):
                        error = float(words[ecol - 1])
                    else:
                        raise StatTestError("Line too short to get error, %d < %d: %s" % (len(words), ecol, line))
                elif etype == "REL":
                    if not 0 < ecol <= len(words):
                        raise StatTestError("Line too short to get error, %d < %d: %s" % (len(words), ecol, line))
                    error = float(words[ecol - 1]) * float(value)
                elif etype == "GIVEN":
                    error = float(ecol) * float(value)
                elif etype == "NEV":
                    error = nev_error(value)
                else:
                    raise StatTestError("Error type is not valid: %s it should be 'ABS', 'REL', 'GIVEN' or 'NEV'" % etype)
                values[idl] = (value, error, words)
    return values, nevents


def get_text_block(conf, conf_file):
    if 'Text' not in conf:
        raise StatTestError("No 'Text' block in configuration file %s" % conf_file)
    text = conf['Text']
    lines = text.get('Lines', [])
    if not isinstance(lines, list) or len(lines) == 0:
        raise StatTestError("'Text' block should have a non-empty list 'Lines' in %s" % conf_file)
    for dline in lines:
        if 'Words' not in dline or 'ValueCol' not in dline:
            raise StatTestError("Text element %s should have 'Words' and 'ValueCol' keys" % dline)
    return text, lines


NJOBS_LINE = "QA: number of jobs merged ="


def get_njobs(file_name):
    """ Number of jobs merged in a file written by mergetext (None if it is not a merged file) """
    try:
        with open(file_name, errors='replace') as fin:
            for line in fin:
                if line.startswith(NJOBS_LINE):
                    return int(line.split()[-1])
    except (IOError, ValueError):
        pass
    return None


def run_text(conf_file, file_name, ref_file):
    """ Compares the values of the lines of file_name with those of the reference file ref_file.
    If both are files merged by mergetext (runN.sh), the sigma of the difference is
    sqrt(spread_ref^2/Njobs_ref + spread^2/Njobs); if file_name is the output of one job, sigma is the spread
    of the reference (the expected fluctuation of the value of one job) """
    conf = read_configuration(conf_file)
    text, lines = get_text_block(conf, conf_file)
    njobs = get_njobs(file_name)
    njobs_ref = get_njobs(ref_file)
    if njobs is not None and njobs_ref is None:
        njobs_ref = 10
        print("!!! WARNING: NUMBER OF JOBS MERGED NOT FOUND IN %s, 10 IS ASSUMED" % ref_file)
    ref_values, nev_ref = read_text_values(ref_file, lines, True)
    values, nev = read_text_values(file_name, lines, njobs is not None)
    nfailed = 0
    print("=" * 80)
    for idl, dline in enumerate(lines):
        if ref_values[idl] is None:
            continue
        test_name = dline.get('TestName', text.get('DefaultTestName'))
        if test_name != "NormalCDF":
            raise StatTestError("Test %s is not available for text lines, only NormalCDF" % test_name)
        thresholds = check_thresholds(dline.get('Thresholds', text.get('DefaultThresholds')), line_name(dline))
        if values[idl] is None:
            pval = 0.
            comment = "(line not found in %s)" % file_name
        else:
            val = float(values[idl][0])
            vref, eref = float(ref_values[idl][0]), abs(float(ref_values[idl][1]))
            if njobs is None:
                errtot = eref
            else:
                err = abs(float(values[idl][1]))
                errtot = math.sqrt(eref * eref / njobs_ref + err * err / njobs)
            if errtot != 0:
                nsig = (val - vref) / errtot
            else:
                nsig = 0. if val == vref else float('inf')
            pval = 1. + math.erf(-abs(nsig) / math.sqrt(2.)) if not math.isinf(nsig) else 0.
            comment = "(%s= %s %s= %s sigma= %g)" % (LABELS['test'], values[idl][0], LABELS['ref'], ref_values[idl][0], errtot)
        result = result_from_pval(pval, thresholds)
        if result == FAILED:
            nfailed += 1
        print_result(line_name(dline), result, "NormalCDF", pval, comment)
    print("=" * 80)
    return nfailed


def run_merge_text(conf_file, list_file, out_file):
    """ Merges several output files: for each configured line writes the line of the first file
    with the value replaced by the mean of the values of all files and, in the column ErrorCol, the spread
    sqrt(N*sum(v^2)-sum(v)^2)/(N-1) (for ErrorType 'ABS'; for 'REL' the spread divided by the mean; for 'NEV'
    the error is computed from the value), so that the merged file is read with the same configuration file """
    conf = read_configuration(conf_file)
    text, lines = get_text_block(conf, conf_file)
    with open(list_file) as flis:
        files = [ln.strip() for ln in flis if ln.strip()]
    if len(files) < 2:
        raise StatTestError("At least two files are needed to merge, there are %d in %s" % (len(files), list_file))
    all_values = []
    nev_line = None
    for fname in files:
        if nev_line is None:
            with open(fname, errors='replace') as fin:
                for line in fin:
                    if "Number of events processed" in line:
                        nev_line = line.rstrip("\n")
                        break
        print("READING FILE %s " % fname)
        fvalues = read_text_values(fname, lines, False)[0]
        if all(v is None for v in fvalues):
            print("!!! WARNING: none of the configured lines found in %s (job failed?), it is not merged" % fname)
            continue
        all_values.append(fvalues)
    if len(all_values) < 2:
        raise StatTestError("At least two valid files are needed to merge, there are %d" % len(all_values))
    with open(out_file, "w") as fout:
        if nev_line is not None:
            fout.write("%s\n" % nev_line)
        fout.write("%s %d\n\n" % (NJOBS_LINE, len(all_values)))
        for idl, dline in enumerate(lines):
            found = [v[idl] for v in all_values if v[idl] is not None]
            if len(found) == 0:
                continue
            # a line missing in a job means a value 0 (e.g. PROC_COUNT of a process that did not occur in that job),
            # so the mean and spread are computed over all the jobs, not only those where the line is found
            vals = [float(v[0]) for v in found] + [0.] * (len(all_values) - len(found))
            nval = len(vals)
            vsum = sum(vals)
            vsum2 = sum(v * v for v in vals)
            mean = vsum / nval
            spread = math.sqrt(max(0., nval * vsum2 - vsum * vsum)) / (nval - 1) if nval > 1 else 0.
            words = list(found[0][2])
            vcol = dline['ValueCol']
            words[vcol - 1] = repr(mean)
            etype = dline.get('ErrorType', 'ABS')
            ecol = dline.get('ErrorCol', 0)
            if etype in ("ABS", "REL"):
                if ecol <= vcol:
                    print("!!! WARNING: line %s has ErrorCol (%d) <= ValueCol (%d), the error cannot be written"
                          % (line_name(dline), ecol, vcol))
                else:
                    while len(words) < ecol:
                        words.append("+-")
                    if etype == "ABS":
                        words[ecol - 1] = repr(spread)
                    else:
                        words[ecol - 1] = repr(spread / mean) if mean != 0 else "0."
            prtbuf = " ".join(words)
            print("MERGED: %s " % prtbuf)
            fout.write("%s\n" % prtbuf)
    return 0


#####################################################################
# Histogram comparison
def match_selection(selection, name):
    if selection == name:
        return True
    try:
        return re.match("^%s$" % selection, name) is not None
    except re.error:
        return False


# names of the test and reference files in the outputs (options -t and -r)
LABELS = {'test': 'test', 'ref': 'reference'}


def run_histos(conf_file, file_name, ref_file, report=None):
    conf = read_configuration(conf_file)
    if 'Binned' not in conf:
        raise StatTestError("No 'Binned' block in configuration file %s" % conf_file)
    binned = conf['Binned']
    hconfs = binned.get('Histos', [])
    if not isinstance(hconfs, list) or len(hconfs) == 0:
        raise StatTestError("'Binned' block should have a non-empty list 'Histos' in %s" % conf_file)
    histos = read_csv_histos(file_name)
    refs = {}
    results = []
    nfailed = 0
    print("=" * 80)
    selected = []
    for hconf in hconfs:
        for name in histos:
            if name not in selected and match_selection(hconf['Name'], name):
                selected.append(name)
    for name in selected:
        hconf = [hc for hc in hconfs if match_selection(hc['Name'], name)][0]
        rfile = hconf.get('ReferenceFile', ref_file or binned.get('DefaultReferenceFile'))
        if rfile is None:
            raise StatTestError("No reference file for histogram %s" % name)
        if rfile not in refs:
            refs[rfile] = read_csv_histos(rfile)
        test_name = hconf.get('TestName', binned.get('DefaultTestName'))
        if test_name not in HISTO_TESTS:
            raise StatTestError("Test %s is not available for histograms; available tests: %s"
                                % (test_name, ", ".join(HISTO_TESTS)))
        thresholds = check_thresholds(hconf.get('Thresholds', binned.get('DefaultThresholds')), name)
        his = histos[name]
        href = refs[rfile].get(name)
        if href is None:
            print("HISTOGRAM NOT FOUND %s IN FILE %s " % (name, rfile))
            continue
        normalize = bool(hconf.get('Normalize', binned.get('DefaultNormalize', False)))
        test = HISTO_TESTS[test_name](his, href, normalize)
        if not his.same_binning(href):
            pval = 0.
            test.comment = "(histograms have different binning)"
        else:
            pval = test.pval()
        result = result_from_pval(pval, thresholds)
        if result == FAILED:
            nfailed += 1
        print_result(name, result, test_name, pval, test.comment)
        results.append((his, href, result, test_name, pval, test.comment, normalize))
    # histograms selected in the configuration that are in the reference file but not in the file tested
    rfile = ref_file or binned.get('DefaultReferenceFile')
    if rfile is not None:
        if rfile not in refs:
            refs[rfile] = read_csv_histos(rfile)
        for name in refs[rfile]:
            if name in histos or not any(match_selection(hc['Name'], name) for hc in hconfs):
                continue
            nfailed += 1
            print_result(name, FAILED, "-", 0., "(histogram not found in %s)" % file_name)
    print("=" * 80)
    if report:
        write_report(report, results, file_name, ref_file)
    return nfailed


#####################################################################
# PDF report (written directly, with the standard PDF fonts Helvetica and Helvetica-Bold)
COLORS = {FAILED: (0.78, 0.16, 0.16), NOTPASSED: (0.94, 0.56, 0.), SUCCESS: (0.18, 0.49, 0.2), UNDEFINED: (0.33, 0.33, 0.33)}
COLOR_TEST = (0.12, 0.37, 0.75)
COLOR_REF = (0.82, 0.19, 0.19)
BLACK = (0., 0., 0.)
GREY = (0.48, 0.48, 0.48)
LIGHTGREY = (0.89, 0.89, 0.89)
PAGE_W, PAGE_H = 842., 595.   # A4 landscape, in points
HELVETICA_WIDTHS = dict(zip(
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~",
    [278, 278, 355, 556, 556, 889, 667, 191, 333, 333, 389, 584, 278, 333, 278, 278] + [556] * 10 +
    [278, 278, 584, 584, 584, 556, 1015, 667, 667, 722, 722, 667, 611, 778, 722, 278, 500, 667, 556, 833,
     722, 778, 667, 778, 722, 667, 611, 722, 667, 944, 667, 667, 611, 278, 278, 278, 469, 556, 333, 556,
     556, 500, 556, 556, 278, 556, 556, 222, 222, 500, 222, 833, 556, 556, 556, 556, 333, 500, 278, 556,
     500, 722, 500, 500, 500, 334, 260, 334, 584]))


def text_width(text, size, bold=False):
    width = sum(HELVETICA_WIDTHS.get(ch, 556) for ch in text) * size / 1000.
    return width * 1.06 if bold else width


class PdfPage:
    """ Drawing operations on one page; coordinates in points from the top left corner """
    def __init__(self):
        self.ops = []
        self.links = []   # (x, y, w, h, index of the target page), y of the top of the box from the top of the page

    def _y(self, y):
        return PAGE_H - y

    def stroke_color(self, rgb):
        self.ops.append("%.3f %.3f %.3f RG" % rgb)

    def fill_color(self, rgb):
        self.ops.append("%.3f %.3f %.3f rg" % rgb)

    def line(self, x1, y1, x2, y2, rgb=BLACK, width=0.6):
        self.stroke_color(rgb)
        self.ops.append("%.2f w %.2f %.2f m %.2f %.2f l S" % (width, x1, self._y(y1), x2, self._y(y2)))

    def polyline(self, points, rgb=BLACK, width=1.):
        if not points:
            return
        self.stroke_color(rgb)
        path = ["%.2f w %.2f %.2f m" % (width, points[0][0], self._y(points[0][1]))]
        path += ["%.2f %.2f l" % (x, self._y(y)) for x, y in points[1:]]
        self.ops.append(" ".join(path) + " S")

    def rect(self, x, y, w, h, fill=None, stroke=None, width=0.6):
        if fill:
            self.fill_color(fill)
        if stroke:
            self.stroke_color(stroke)
        op = "B" if fill and stroke else ("f" if fill else "S")
        self.ops.append("%.2f w %.2f %.2f %.2f %.2f re %s" % (width, x, self._y(y + h), w, h, op))

    def text(self, x, y, text, size=10, anchor="start", bold=False, rgb=BLACK, rotate=False):
        width = text_width(text, size, bold)
        shift = {"start": 0., "middle": width / 2., "end": width}[anchor]
        enc = text.encode("latin-1", "replace").decode("latin-1")
        enc = enc.replace("\\", "\\\\").replace("(", "\\(").replace(")", "\\)")
        self.fill_color(rgb)
        if rotate:
            matrix = "0 1 -1 0 %.2f %.2f" % (x, self._y(y) - shift)
        else:
            matrix = "1 0 0 1 %.2f %.2f" % (x - shift, self._y(y))
        self.ops.append("BT /%s %g Tf %s Tm (%s) Tj ET" % ("F2" if bold else "F1", size, matrix, enc))

    def link(self, x, y, w, h, target_page):
        """ clickable box (x, y = top left corner) that goes to the page with index target_page """
        self.links.append((x, y, w, h, target_page))

    def fitted_text(self, x, y, text, size, max_width, bold=False, rgb=BLACK):
        """ text reduced in size (and then truncated) to fit in max_width """
        while size > 6 and text_width(text, size, bold) > max_width:
            size -= 0.5
        while len(text) > 4 and text_width(text, size, bold) > max_width:
            text = text[:-4] + "..."
        self.text(x, y, text, size, bold=bold, rgb=rgb)


def write_pdf(file_name, pages):
    """ Writes the pages (list of PdfPage) in a PDF file """
    objects = ["<< /Type /Catalog /Pages 2 0 R >>", None,
               "<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica /Encoding /WinAnsiEncoding >>",
               "<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica-Bold /Encoding /WinAnsiEncoding >>"]
    kids = []
    # each page has two objects (contents, page): the page object number of page ipage is len(objects) + 2*ipage + 2
    page_obj0 = len(objects) + 2
    for page in pages:
        stream = "\n".join(page.ops).encode("latin-1", "replace")
        objects.append(b"<< /Length %d >>\nstream\n" % len(stream) + stream + b"\nendstream")
        annots = ""
        links = [lk for lk in page.links if 0 <= lk[4] < len(pages)]
        if links:
            annots = " /Annots [%s]" % " ".join(
                "<< /Type /Annot /Subtype /Link /Rect [%.2f %.2f %.2f %.2f] /Border [0 0 0] /Dest [%d 0 R /Fit] >>"
                % (x, PAGE_H - y - h, x + w, PAGE_H - y, page_obj0 + 2 * target) for x, y, w, h, target in links)
        objects.append("<< /Type /Page /Parent 2 0 R /MediaBox [0 0 %g %g] /Resources << /Font << /F1 3 0 R /F2 4 0 R >> >> "
                       "/Contents %d 0 R%s >>" % (PAGE_W, PAGE_H, len(objects), annots))
        kids.append("%d 0 R" % len(objects))
    objects[1] = "<< /Type /Pages /Kids [%s] /Count %d >>" % (" ".join(kids), len(kids))
    out = bytearray(b"%PDF-1.4\n%\xe2\xe3\xcf\xd3\n")
    offsets = []
    for iobj, obj in enumerate(objects):
        offsets.append(len(out))
        body = obj if isinstance(obj, bytes) else obj.encode("latin-1")
        out += b"%d 0 obj\n" % (iobj + 1) + body + b"\nendobj\n"
    xref = len(out)
    out += b"xref\n0 %d\n0000000000 65535 f \n" % (len(objects) + 1)
    for off in offsets:
        out += b"%010d 00000 n \n" % off
    out += b"trailer\n<< /Size %d /Root 1 0 R >>\nstartxref\n%d\n%%%%EOF\n" % (len(objects) + 1, xref)
    with open(file_name, "wb") as fout:
        fout.write(out)


def nice_ticks(vmin, vmax, nmax=6):
    if vmax <= vmin:
        vmax = vmin + 1.
    span = vmax - vmin
    raw = span / nmax
    mag = 10 ** math.floor(math.log10(raw))
    for mult in (1, 2, 2.5, 5, 10):
        step = mult * mag
        if span / step <= nmax:
            break
    first = math.ceil(vmin / step - 1.e-9) * step
    ticks = []
    val = first
    while val <= vmax + 1.e-9 * span:
        ticks.append(0. if abs(val) < 1.e-12 * span else val)
        val += step
    return ticks


def tick_label(val):
    return "%g" % float("%.6g" % val)


def draw_frame(page, x0, y0, w, h, xmin, xmax, ymin, ymax, ylabel, show_xlabels):
    page.rect(x0, y0, w, h, stroke=BLACK)

    def xmap(v):
        return x0 + (v - xmin) / (xmax - xmin) * w

    def ymap(v):
        return y0 + h - (v - ymin) / (ymax - ymin) * h
    for yt in nice_ticks(ymin, ymax, 5):
        yp = ymap(yt)
        page.line(x0 + 5, yp, x0 + w, yp, LIGHTGREY, 0.5)
        page.line(x0, yp, x0 + 5, yp)
        page.text(x0 - 5, yp + 3.5, tick_label(yt), 9, "end")
    for xt in nice_ticks(xmin, xmax, 8):
        xp = xmap(xt)
        page.line(xp, y0 + h, xp, y0 + h - 5)
        if show_xlabels:
            page.text(xp, y0 + h + 12, tick_label(xt), 9, "middle")
    page.text(x0 - 55, y0 + h / 2., ylabel, 9, "middle", rotate=True)
    return xmap, ymap


def histo_page(his, href, result, test_name, pval, comment, normalize=False):
    """ Page with the test histogram (blue) and the reference (red), scaled to the same integral if normalize is True;
    below, the pulls """
    page = PdfPage()
    x0, pw = 95., 700.
    y_top, h_top = 82., 320.
    y_res, h_res = 425., 120.
    cont1, err1 = his.projection_x()
    cont2, err2 = href.projection_x()
    sum1, sum2 = sum(cont1), sum(cont2)
    scale = sum1 / sum2 if sum2 > 0 and normalize else 1.
    cont2s = [c * scale for c in cont2]
    err2s = [e * scale for e in err2]
    edges = [his.xmin + (his.xmax - his.xmin) * ib / his.nx for ib in range(his.nx + 1)]
    ymin = min(0., min(cont1 + cont2s))
    ymax = max(cont1 + cont2s)
    if ymax <= ymin:
        ymax = ymin + 1.
    ymax += 0.08 * (ymax - ymin)
    color = COLORS.get(result, GREY)
    page.rect(0, 0, PAGE_W, 8, fill=color)
    page.fitted_text(x0, 34, his.name, 15, PAGE_W - x0 - 30, bold=True)
    page.text(x0, 56, result, 11, bold=True, rgb=color)
    page.text(x0 + 85, 56, "p-value = %s   (%s) %s" % ("%.4g" % pval if pval is not None else "undef", test_name, comment), 10)
    xmap, ymap = draw_frame(page, x0, y_top, pw, h_top, his.xmin, his.xmax, ymin, ymax,
                            "Projection on X" if his.is2D() else "Contents", False)
    for ib in range(his.nx):
        if err1[ib] > 0:
            xc = xmap(0.5 * (edges[ib] + edges[ib + 1]))
            page.line(xc, ymap(max(ymin, cont1[ib] - err1[ib])), xc, ymap(min(ymax, cont1[ib] + err1[ib])), COLOR_TEST, 0.6)
    for cont, rgb in ((cont2s, COLOR_REF), (cont1, COLOR_TEST)):
        points = []
        for ib, yv in enumerate(cont):
            points += [(xmap(edges[ib]), ymap(yv)), (xmap(edges[ib + 1]), ymap(yv))]
        page.polyline(points, rgb, 1.2)
    lx, ly = x0 + pw - 240, y_top + 18
    page.line(lx, ly - 3, lx + 25, ly - 3, COLOR_TEST, 1.5)
    page.text(lx + 32, ly, "%s (%s entries)" % (LABELS['test'], tick_label(his.stats[0]) if his.stats else "?"), 9)
    page.line(lx, ly + 11, lx + 25, ly + 11, COLOR_REF, 1.5)
    if normalize:
        page.text(lx + 32, ly + 14, "%s x %.3g (same integral)" % (LABELS['ref'], scale), 9)
    else:
        page.text(lx + 32, ly + 14, "%s (not normalized)" % LABELS['ref'], 9)
    pulls = []
    for ib in range(his.nx):
        sig = math.sqrt(err1[ib] ** 2 + err2s[ib] ** 2)
        pulls.append((cont1[ib] - cont2s[ib]) / sig if sig > 0 else 0.)
    pmax = math.ceil(max([3.] + [abs(p) for p in pulls]))
    xmap, ymap = draw_frame(page, x0, y_res, pw, h_res, his.xmin, his.xmax, -pmax, pmax, "(%s - %s)/error" % (LABELS['test'], LABELS['ref']), True)
    yzero = ymap(0.)
    for ib, pull in enumerate(pulls):
        if pull == 0:
            continue
        xa, xb = xmap(edges[ib]), xmap(edges[ib + 1])
        yp = ymap(pull)
        page.rect(xa, min(yp, yzero), max(xb - xa, 0.3), abs(yzero - yp),
                  fill=COLORS[FAILED] if abs(pull) > 3 else GREY)
    page.line(x0, yzero, x0 + pw, yzero)
    return page


def summary_pages(results, file_name, ref_file):
    """ Pages with the list of histograms and their results """
    counts = {}
    for res in results:
        counts[res[2]] = counts.get(res[2], 0) + 1
    rows_per_page = 30
    pages = []
    nsummary = max(1, (len(results) + rows_per_page - 1) // rows_per_page)   # the histogram pages come after these
    for ifirst in range(0, max(len(results), 1), rows_per_page):
        page = PdfPage()
        page.fitted_text(40, 34, "Comparison of %s: %s" % (LABELS['test'], file_name), 14, PAGE_W - 80, bold=True)
        page.fitted_text(40, 52, "with %s: %s" % (LABELS['ref'], ref_file), 11, PAGE_W - 80)
        xc = 40.
        for res in (FAILED, NOTPASSED, SUCCESS, UNDEFINED):
            if res in counts:
                label = "%s: %d" % (res, counts[res])
                page.text(xc, 74, label, 11, bold=True, rgb=COLORS[res])
                xc += text_width(label, 11, True) + 25
        y = 100.
        page.text(40, y, "Histogram", 10, bold=True)
        page.text(560, y, "Result", 10, bold=True)
        page.text(640, y, "p-value", 10, bold=True)
        page.text(710, y, "Test", 10, bold=True)
        page.line(40, y + 4, PAGE_W - 30, y + 4)
        for ires, (his, href, result, test_name, pval, comment, _norm) in enumerate(results[ifirst:ifirst + rows_per_page]):
            y += 15
            page.fitted_text(40, y, his.name, 9.5, 510)
            page.text(560, y, result, 9.5, bold=True, rgb=COLORS[result])
            # clicking on the result goes to the page of the histogram
            page.link(556, y - 11, text_width(result, 9.5, True) + 8, 14, nsummary + ifirst + ires)
            page.text(640, y, "%.4g" % pval if pval is not None else "undef", 9.5)
            page.fitted_text(710, y, test_name.replace("Binned", "").replace("Test", ""), 9.5, PAGE_W - 740)
        pages.append(page)
    return pages


def write_report(report, results, file_name, ref_file):
    pages = summary_pages(results, file_name, ref_file)
    for res in results:
        pages.append(histo_page(*res))
    for ipage, page in enumerate(pages):
        page.text(PAGE_W - 30, PAGE_H - 15, "%d / %d" % (ipage + 1, len(pages)), 8, "end", rgb=GREY)
    write_pdf(report, pages)
    print("REPORT WRITTEN IN %s" % report)


#####################################################################
def add_seed(input_file, seed):
    """ Writes INPUT_FILE.SEED with the random seeds set to SEED """
    output_file = "%s.%s" % (input_file, seed)
    bseed = False
    with open(input_file) as fin, open(output_file, "w") as fout:
        for line in fin:
            if "setSeeds" in line:
                bseed = True
                line = "/gamos/random/setSeeds %s %s\n" % (seed, seed)
            if "restoreSeeds" in line:
                bseed = True
                line = "/gamos/random/restoreSeeds %s %s\n" % (seed, seed)
            if "/run/beamOn" in line and not bseed:
                fout.write("/gamos/random/restoreSeeds %s %s\n" % (seed, seed))
                bseed = True
            fout.write(line)
    return 0


def usage():
    print(__doc__)


def main(argv):
    if len(argv) < 1 or argv[0] in ("-h", "--help", "help"):
        usage()
        return 0
    command = argv[0]
    args = argv[1:]
    report = None
    for opt, key in (("-t", "test"), ("-r", "ref")):   # names of the test and reference files in the outputs
        if opt in args:
            io = args.index(opt)
            if io + 1 >= len(args):
                raise StatTestError("Option %s needs a label" % opt)
            LABELS[key] = args[io + 1]
            args = args[:io] + args[io + 2:]
    if "-g" in args:
        ig = args.index("-g")
        if ig + 1 >= len(args):
            raise StatTestError("Option -g needs a file name")
        report = args[ig + 1]
        args = args[:ig] + args[ig + 2:]
    if command == "addseed" and len(args) == 2:
        return add_seed(args[0], args[1])
    if command == "text" and len(args) == 3:
        return run_text(args[0], args[1], args[2])
    if command == "histos" and len(args) in (2, 3):
        return run_histos(args[0], args[1], args[2] if len(args) == 3 else None, report)
    if command == "mergetext" and len(args) == 3:
        return run_merge_text(args[0], args[1], args[2])
    if command == "mergehistos" and len(args) >= 2:
        write_csv_histos(args[0], merge_histos([read_csv_histos(f) for f in args[1:]]))
        print("MERGED %d FILES IN %s" % (len(args) - 1, args[0]))
        return 0
    if command == "list":
        print("Tests for text lines: NormalCDF")
        print("Tests for histograms: %s" % ", ".join(HISTO_TESTS))
        return 0
    usage()
    sys.exit(2)


if __name__ == "__main__":
    try:
        nfailed = main(sys.argv[1:])
    except StatTestError as exc:
        sys.stderr.write("statTest.py ERROR: %s\n" % exc)
        sys.exit(2)
    if nfailed and nfailed > 0:
        sys.stderr.write("Number of FAILED tests: %d\n" % nfailed)
        sys.exit(1)
    sys.exit(0)
