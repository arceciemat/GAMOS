#!/bin/bash
# Shows the results of the QA tests run with runAllQA (run1qa.sh) in all tutorials:
# prints the failed tests of each exercise and opens its PDF reports with the histogram plots.
# Usage (from the tutorials directory):
#   bash showQA.sh            print the failed tests and open the reports
#   bash showQA.sh NOVIEW     only print the failed tests and the names of the reports
# The PDF viewer is evince, unless another one is set with the environment variable QA_PDF_VIEWER
# (e.g. export QA_PDF_VIEWER=okular). The reports of each exercise are opened together,
# and the next exercise is shown when the viewer is closed.

VIEWER=""
if [ X$1 != "XNOVIEW" ]; then
    VIEWER=${QA_PDF_VIEWER:-evince}
    if ! command -v ${VIEWER%% *} > /dev/null; then
	echo "!!! PDF VIEWER $VIEWER NOT FOUND, SET IT WITH THE ENVIRONMENT VARIABLE QA_PDF_VIEWER. ONLY THE FAILED TESTS ARE PRINTED"
	VIEWER=""
    fi
fi

ls | grep Tutorial | while read tuto; do
    [ -e $tuto/runAllQA ] || continue
    grep "run1qa.sh" $tuto/runAllQA | grep -v "^ *#" | while read line; do
	# line: bash run1qa.sh TUTORIAL EXERCISE_DIR EXERCISE [NOTEXT]
	set -- ${line#*run1qa.sh}
	dir=$1
	subdir=$2
	exer=$3
	echo "@@@@@@@@@@@@ $dir/$subdir/$exer"
	for summ in $dir/$subdir/qatest.summary.$exer $dir/$subdir/qatest.summary.*.$exer.csv; do
	    [ -e $summ ] || continue
	    echo "=== $summ : `grep -c ': FAILED' $summ` FAILED"
	    grep ": FAILED" $summ
	done
	reports=""
	for report in $dir/$subdir/qatest.report.*.$exer.pdf; do
	    [ -e $report ] || continue
	    echo "=== REPORT: $report"
	    reports="$reports $report"
	done
	if [ -n "$VIEWER" ] && [ -n "$reports" ]; then
	    $VIEWER $reports < /dev/null > /dev/null 2>&1
	fi
    done
done
