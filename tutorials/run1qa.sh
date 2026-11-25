#!/bin/bash
# Compares the reference files of a tutorial exercise built with runN.sh with the new GAMOS version
# with those built with a previous version. It does not run gamos.
# It uses StatTest/statTest.py (Python 3, no ROOT); the histogram plots are written in a PDF file.
#
# Usage (from the tutorials directory):
#   bash run1qa.sh TUTORIAL EXERCISE_DIR EXERCISE [NOTEXT]
#   e.g. bash run1qa.sh PETTutorial exercise2 exercise2b
#
# Files compared (built by runN.sh, 10 jobs merged):
#   NEW: TUTORIAL/EXERCISE_DIR/ref.out.EXERCISE and ref.HISTO_FILE.csv
#   OLD: $GAMOS_RELEASE_OLD/TUTORIAL/EXERCISE_DIR/ref.out.EXERCISE and ref.HISTO_FILE.csv
# The tests are configured in TUTORIAL/EXERCISE_DIR/EXERCISE.qaref
# If the last argument is NOTEXT, the text output is not compared.
# Results, in TUTORIAL/EXERCISE_DIR:
#   qatest.EXERCISE.log            result of each text test
#   qatest.summary.EXERCISE        text lines that failed, with their OLD and NEW lines
#   qatest.HISTO_FILE.csv.log      result of each histogram test
#   qatest.summary.HISTO_FILE.csv  histograms that failed
#   qatest.report.HISTO_FILE.pdf   plots of the histograms

if [ $# -lt 3 ]; then
    echo "Usage: bash run1qa.sh TUTORIAL EXERCISE_DIR EXERCISE [NOTEXT]"
    exit 1
fi

TUTODIR=$(cd "$(dirname "$0")" && pwd)
STATTEST="${PYTHON:-python3} $TUTODIR/StatTest/statTest.py"
GAMOS_RELEASE_OLD=${GAMOS_RELEASE_OLD:-/home/arce/gamos/GAMOS.7.0/tutorials/}
OLDDIR=$GAMOS_RELEASE_OLD/$1/$2
[ -d $OLDDIR ] && OLDDIR=$(cd $OLDDIR && pwd)   # absolute path, without '//'
NEWDIR=$GAMOSINSTALL/tutorials/$1/$2
[ -d $NEWDIR ] && NEWDIR=$(cd $NEWDIR && pwd)
# name of the merged histogram file of EXERCISE (as in runN.sh): ref.HISTO_FILE if HISTO_FILE contains the exercise name,
# else ref.EXERCISE.HISTO_FILE (several exercises may write the same HISTO_FILE, e.g. exercise4b.Dose and exercise4b.DoseEWBS)
refname() { case "$1" in *."$2".*) echo "$1";; *) echo "$2.$1";; esac; }
# names of the two GAMOS versions in the outputs (e.g. GAMOS.7.0.1 and GAMOS.7.0)
NEWNAME=$(basename "$GAMOSINSTALL")
OLDNAME=$(basename "$(dirname "$(cd "$GAMOS_RELEASE_OLD" && pwd)")")
LABELS="-t $NEWNAME -r $OLDNAME"
EXER=$3
echo "@@@@@@@@@@@@ COMPARING $NEWDIR/$EXER WITH $OLDDIR/$EXER"

cd $NEWDIR || exit 1
if [ ! -e $EXER.qaref ]; then
    echo "!!! CONFIGURATION FILE $GAMOSINSTALL/tutorials/$1/$2/$EXER.qaref NOT FOUND"
    exit 1
fi

####################################################
##### TEST text output
if [ X$4 != "XNOTEXT" ] && grep -q "^[[:space:]]*Text[[:space:]]*=" $EXER.qaref; then
    if [ ! -e ref.out.$EXER ]; then
	echo "!!! FILE $GAMOSINSTALL/tutorials/$1/$2/ref.out.$EXER NOT FOUND, RUN runN.sh WITH THE NEW VERSION"
    elif [ ! -e $OLDDIR/ref.out.$EXER ]; then
	echo "!!! FILE $OLDDIR/ref.out.$EXER NOT FOUND, RUN runN.sh WITH THE OLD VERSION"
    else
	$STATTEST text $LABELS $NEWDIR/$EXER.qaref $NEWDIR/ref.out.$EXER $OLDDIR/ref.out.$EXER > qatest.$EXER.log
	cat qatest.$EXER.log
	grep ": FAILED" qatest.$EXER.log > qatest.summary.$EXER
	NFAILED=`wc -l < qatest.summary.$EXER`
	rm -f qatest.summary.tmp
	while IFS='' read -r lineFailed1; do
	    lineFailed=`echo ${lineFailed1%: FAILED*}`
	    echo "%%% LINE FAILED: $lineFailed " >> qatest.summary.tmp
	    echo "@@@@ OLD : " `grep -F -- "$lineFailed" $OLDDIR/ref.out.$EXER` >> qatest.summary.tmp
	    echo "@@@@ NEW : " `grep -F -- "$lineFailed" ref.out.$EXER` >> qatest.summary.tmp
	done < qatest.summary.$EXER
	if [ -e qatest.summary.tmp ]; then
	    cat qatest.summary.tmp >> qatest.summary.$EXER
	    rm qatest.summary.tmp
	fi
	NNOTPASSED=`grep -c ": NOTPASSED" qatest.$EXER.log`
	NALL=`grep -c ": FAILED\|: NOTPASSED\|: SUCCESS\|: UNDEFINED" qatest.$EXER.log`
	echo "@@@@@@@@@@@@@@@@@@@@@@@ SUMMARY TEXT RESULTS $1 $2 $EXER : N_FAILED = $NFAILED N_NOTPASSED = $NNOTPASSED N_ALL = $NALL" | tee -a qatest.summary.$EXER
    fi
fi

####################################################
##### TEST histogram files (CSV)
##### HISTOGRAM FILES WRITTEN BY THE FIRST JOB OF runN.sh (OR BY out.EXERCISE)
outFile=Nout.$EXER.1111
[ -e $outFile ] || outFile=out.$EXER
if [ ! -e $outFile ]; then
    echo "!!! NEITHER Nout.$EXER.1111 NOR out.$EXER FOUND: THE HISTOGRAM FILES ARE NOT KNOWN"
    exit 1
fi
grep "saving histograms in fi" $outFile | tr -d "\r" | tr -d "\r" | awk '{print $7}' | while read hf; do
    if [ "${hf%.csv}" = "$hf" ]; then
	echo "!!! HISTOGRAM FILE $hf IS NOT A CSV FILE, IT CANNOT BE TESTED"
	continue
    fi
    rf=$(refname $hf $EXER)
    if [ ! -e ref.$rf ]; then
	echo "!!! FILE $NEWDIR/ref.$rf NOT FOUND, RUN runN.sh WITH THE NEW VERSION"
	cd ../; bash ../runN.sh $2 $3 1 ; cd -
	continue
    fi
    if [ ! -e $OLDDIR/ref.$rf ]; then
	echo "!!! FILE $OLDDIR/ref.$rf NOT FOUND, RUN runN.sh WITH THE OLD VERSION"
	continue
    fi
    $STATTEST histos $LABELS -g $NEWDIR/qatest.report.${rf%.csv}.pdf $NEWDIR/$EXER.qaref $NEWDIR/ref.$rf $OLDDIR/ref.$rf > qatest.$rf.log
    cat qatest.$rf.log
    grep ": FAILED" qatest.$rf.log > qatest.summary.$rf
    NFAILED=`wc -l < qatest.summary.$rf`
    NNOTPASSED=`grep -c ": NOTPASSED" qatest.$rf.log`
    NALL=`grep -c ": FAILED\|: NOTPASSED\|: SUCCESS\|: UNDEFINED" qatest.$rf.log`
    echo "@@@@@@@@@@@@@@@@@@@@@@@ SUMMARY HISTOGRAM RESULTS $1 $2 $EXER $hf : N_FAILED = $NFAILED N_NOTPASSED = $NNOTPASSED N_ALL = $NALL" | tee -a qatest.summary.$rf
done

cd - > /dev/null
