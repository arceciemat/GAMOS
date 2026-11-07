#!/bin/bash -f
#
if [[ ! $# == 6 ]]; then
    echo "!!! YOU MUST PUT SIX PARAMETERS"
    exit
fi

FILE=$1
ENER=$2
RADIUS=$3
SEED=$4
NEV=$5
NJOBS=$6
if [ $NJOBS -gt 1000 ]; then
    echo "!!! ONLY 1000 JOBS ARE ALLOWED" $NJOBS
    exit
fi
if [ "X"${7} != "X" ]  ; then
    machine1st=${7}
    machine2nd=${7}
else 
    machine1st=cpu36c
    machine2nd=cpu36c
fi
echo MACHINES N $# machine1st $machine1st machine2nd= $machine2nd

NRUNJOB=4

user=`whoami`
echo machine $machine NRUNJOB $NRUNJOB
NRESERVEJOB=1
NJOBSG=`echo $NJOBS $NRUNJOB  |awk '{print int($1/$2+0.999)}'`
HNJOBS=`echo $NJOBS |awk '{print int($1/2.)}'`
echo NJOBSG $NJOBSG NJOBS $NJOBS FROM $NJOBS $NRUNJOB
JOB=$1"_"$2"_"$3"_"$4"_"$5"_"$6

for ijobg in `seq 1 $NJOBSG` ; do 
    ijob1=`echo $ijobg $NRUNJOB | awk '{print $1*$2-$2+1}'`
    ijob2=`echo $ijobg $NRUNJOB | awk '{print $1*$2}'`
    if test $ijob2 -gt $NJOBS 
    then
	ijob2=$NJOBS
    fi
    if [ ! $NJOBSG -eq 1 ]; then
	if test $ijob1 -lt $HNJOBS ;then	
	    machine=$machine1st
	else
	    machine=$machine2nd
	fi
    else 
	rand=$RANDOM
	irand=`echo $rand | awk '{print int($1/16384)}'`
#	echo RANDOM $rand " -> " $irand
	if [ $irand == 0 ]; then
	    machine=$machine1st
	else
	    machine=$machine2nd
	fi
    fi
    echo $ijobg in $NJOBSG  MACHINE $machine $ijob1 "<>" $HNJOBS
    SUFFIX="_"$JOB"_"$ijob1"_"$ijob2
echo    cp -v run-gamos.bat batch$SUFFIX.job
    cp -v run-gamos.bat batch$SUFFIX.job
#c echo    sbatch --partition=$machine --output batch$SUFFIX.job.slurm --ntasks=$NRUNJOB --time=24:20:00 ./batch$SUFFIX.job $1 $2 $3 $4 $5 $6 $ijob1 $ijob2
    #c    sbatch --partition=$machine --output batch$SUFFIX.job.slurm --ntasks=$NRUNJOB --time=24:20:00 ./batch$SUFFIX.job $1 $2 $3 $4 $5 $6 $ijob1 $ijob2
    ./batch$SUFFIX.job $1 $2 $3 $4 $5 $6 $ijob1 $ijob2 | tee zz_$JOB
    sleep 1 #PRM (sleep 0.125 for each job *32 = 4s)

done

(sleep 1000; cemsendjobs.p6 $1 $2 $3 $4 $5 $6) &

