#!/bin/bash
# Builds the reference files of an exercise (to be run with each GAMOS version; run1qa.sh compares them)
# running gamos 10 times with different seeds and merging the outputs with StatTest/statTest.py
#
# Usage (from the tutorial directory, e.g. PETTutorial):
#   bash ../runN.sh EXERCISE_DIR EXERCISE [NOGAMOS]
#   e.g. bash ../runN.sh exercise2 exercise2b
# If NOGAMOS is 1, gamos is not run: only the existing Nout.EXERCISE.SEED and NEXERCISE.SEED.*.csv files are merged
# It needs EXERCISE_DIR/EXERCISE.in and EXERCISE_DIR/EXERCISE.qaref (the same configuration file as for run1qa.sh)
# Output, in EXERCISE_DIR:
#   ref.out.EXERCISE        mean and spread of the selected text lines
#   ref.HISTO_FILE.csv      sum of the CSV histogram files of the 10 runs (ref.EXERCISE.HISTO_FILE.csv if HISTO_FILE does not contain EXERCISE)

if [ $# -lt 2 ]; then
    echo "Usage: bash runN.sh EXERCISE_DIR EXERCISE"
    exit 1
fi

if [ $# -eq 3 ]; then
    NOGAMOS=$3
else
    NOGAMOS=0
fi
TUTODIR=$(cd "$(dirname "$0")" && pwd)
STATTEST="${PYTHON:-python3} $TUTODIR/StatTest/statTest.py"
if ! command -v gamos > /dev/null; then
    source $TUTODIR/../config/confgamos.sh
fi

cd $1 || exit 1
echo @%@%@%@%@%@ $1/$2.in

### RUN N JOBS (the seeds are also needed to merge the outputs when gamos is not run)
seed1st=1111
seeds=`seq $seed1st 10 1210`
echo NOGAMOS $NOGAMOS
if [ $NOGAMOS == "0" ]; then
    # The jobs run in parallel, each one in its own directory ../run.EXERCISE_DIR.EXERCISE.SEED (at the same level as
    # EXERCISE_DIR, so that relative paths '../' in the .in file still work), so that they do not overwrite each other files.
    # The small files of EXERCISE_DIR are copied there and the big ones (> 10 MB, they are inputs) are linked
    EXDIR=$(pwd)
    run_job() {
	local seed=$1 rdir=$EXDIR/../run.$(basename $EXDIR).$2.$1
	rm -rf $rdir; mkdir $rdir || return 1
	for f in * ; do
	    case "$f" in Nout.*|N$2.*|ref.*|qatest.*|*.inn|gamos.log|gamos_error.log) continue;; esac
	    if [ -d "$f" ] || [ $(stat -c%s "$f") -gt 10000000 ]; then
		ln -s "$EXDIR/$f" $rdir/
	    else
		cp -p "$f" $rdir/
	    fi
	done
	cd $rdir
	grep -v beamOn $EXDIR/$2.in > $2.inn
	echo "/gamos/random/restoreSeeds " $seed $seed >> $2.inn
	grep beamOn $EXDIR/$2.in >> $2.inn
	touch .start_job
	gamos $2.inn < /dev/null > $EXDIR/Nout.$2.$seed 2>&1
	grep "saving histograms in file" $EXDIR/Nout.$2.$seed | while read line; do
	    file=`echo $line | awk '{ print $7}' `
	    mv $file $EXDIR/N$2.$seed.$file
	done
	echo "JOB $seed FINISHED: $(tail -1 $EXDIR/Nout.$2.$seed)"
    }
    for seed in $seeds ; do
	run_job $seed $2 &
    done
    echo "WAITING FOR THE `echo $seeds | wc -w` JOBS ..."
    wait
    # as when the jobs were run one after the other, leave in EXERCISE_DIR the output files of the last job
    lastdir=$EXDIR/../run.$(basename $EXDIR).$2.$seed
    find $lastdir -maxdepth 1 -type f -newer $lastdir/.start_job ! -name "*.inn" -exec mv {} $EXDIR/ \;
    for seed in $seeds ; do
	rm -rf $EXDIR/../run.$(basename $EXDIR).$2.$seed
    done
fi

### MERGE OUTPUT FILES (only if the configuration file has a 'Text' block)
if grep -q "^[[:space:]]*Text[[:space:]]*=" $2.qaref; then
    rm -f file.lis
    for seed in $seeds ; do
	echo Nout.$2.$seed >> file.lis
    done
    $STATTEST mergetext $2.qaref file.lis ref.out.$2
    rm file.lis
else
    echo "No 'Text' block in $2.qaref: ref.out.$2 not produced"
fi

### MERGE HISTO FILES
grep "saving histograms in file" Nout.$2.$seed1st | while read line; do
    file=`echo $line | awk '{ print $7}' `
    if [ "${file%.csv}" = "$file" ]; then
	echo "!!! HISTOGRAM FILE $file IS NOT A CSV FILE, IT CANNOT BE MERGED WITHOUT ROOT"
	continue
    fi
    # ref.HISTO_FILE if HISTO_FILE contains the exercise name, else ref.EXERCISE.HISTO_FILE (as in run1qa.sh),
    # because several exercises may write the same HISTO_FILE (e.g. exercise4b.Dose and exercise4b.DoseEWBS)
    case "$file" in *."$2".*) rfile=$file;; *) rfile=$2.$file;; esac
    $STATTEST mergehistos ref.$rfile N$2.[0-9]*.$file
done

cd - > /dev/null
