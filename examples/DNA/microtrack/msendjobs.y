#!/bin/bash -f
#
if [[ ! $# == 7 ]] ; then
    echo "!!! YOU MUST PUT 7 PARAMETERS" $*
    exit
fi

FILE=$1
ENER=$2
ENER_INTERVAL=$3
SITE_R=$4
SEED=$5
NEV=$6
NJOBS=$7
if [ $NJOBS -gt 1000 ]; then
    echo "!!! ONLY 1000 JOBS ARE ALLOWED"
    exit
fi
if [ "X"${8} != "X" ]  ; then
    machine1st=${8}
    machine2nd=${8}
    machine3rd=${8}
else 
    machine1st=xula2
    machine2nd=xula3
    machine3rd=xula1
fi
echo MACHINES N $# machine1st $machine1st machine2nd= $machine2nd

NRUNJOB=4

user=`whoami`
NRESERVEJOB=1
NJOBSG=`echo $NJOBS $NRUNJOB  |awk '{print int($1/$2+0.999)}'`
HNJOBS=`echo $NJOBS |awk '{print int($1/2.)}'`
echo NJOBSG $NJOBSG NJOBS $NJOBS FROM $NJOBS $NRUNJOB
JOB="_"$1"_"$2"_"$3"_"$4"_"$5"_"$6"_"$7

for ijobg in `seq 1 $NJOBSG` ; do 
    ijob1=`echo $ijobg $NRUNJOB | awk '{print $1*$2-$2+1}'`
    ijob2=`echo $ijobg $NRUNJOB | awk '{print $1*$2}'`
    if test $ijob2 -gt $NJOBS 
    then
	ijob2=$NJOBS
    fi

    if [ ! $NJOBSG -eq 1 ]; then
	# machine=
	if [ "X"${8} != "X" ]  ; then
	    machine=${8}
	else
	    jj=". ../getBatchMachine.sh $ijobg"
	fi
    else 
	if [ "X"${8} == "X" ]  ; then
	    rand=$RANDOM
	    irand=`echo $rand | awk '{print int($1/16384)}'`
	    #	echo RANDOM $rand " -> " $irand
	    if [ $irand == 0 ]; then
		machine=$machine1st
	    else
		machine=$machine2nd
	    fi
	else
	    machine=$machine1st
	fi
    fi
    echo $ijobg in $NJOBSG  MACHINE $machine $ijob1 "<>" $HNJOBS
    SUFFIX=$JOB"_"$ijob1"_"$ijob2
echo    cp -v run-gamos.y.bat batch$SUFFIX.job
    cp -v run-gamos.y.bat batch$SUFFIX.job
    echo  bash batch$SUFFIX.job $1 $2 $3 $4 $5 $6 $7 $ijob1 $ijob2 &
    bash batch$SUFFIX.job $1 $2 $3 $4 $5 $6 $7 $ijob1 $ijob2&
    sleep 0.2 #PRM (sleep 0.125 for each job *32 = 4s)

done

wait
#iDone=0
#while
# test $iDone -eq 0
#do
#  njobsRun=`squeue -u ${user} | grep -v JOBID | wc -l`
#  echo NJOBS_RUNNING $njobsRun
#  if test $njobsRun -gt 0
#    then
##    echo "TOO MANY JOBS: " $njobsRun " WAITING..."
#    sleep 1000
#  else  
#    iDone=1
#  fi
#done
#pmsendjobs.p5 $1 $2 $3 $4 $5 $6 $7 $8 
#scp $3 arce@tecno31.ciemat.es:gamos/GAMOS.7.0/myExamples/PT_Commissioning/

#(sleep 1000; cemsendjobs.any y $1 $2 $3 $4 $5 $6 $7 $8 $9 > zzcem.$JOB) &

#eqjm.sh
