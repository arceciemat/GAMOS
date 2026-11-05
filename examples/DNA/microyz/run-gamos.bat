#!/bin/bash
#SBATCH --time=48:00:00
#SBATCH --ntasks 1
#SBATCH --mem-per-cpu=4096

umask 0027

if [[ ! $# -eq 8 ]]; then
    echo "WRONG NUMBER OF PARAMETERS TO run-gamos.bat: $0 <inputFile> <ener> <gener_file> <seed> <noEvents> <ijob1> <ijob2>"
    exit 1
fi

uname -a

user=`whoami`
file_base=$1
ENER=$2
RADIUS=$3
SEED=$4
NEV=$5
NJOBS=$6
ijob1=${7}
ijob2=${8}

. /home/$user/utils/lastDigit.sh $NEV
NEWSEED=$SEED
echo SEED $SEED LD $LASTDIGIT
if [ $LASTDIGIT != 0 ]; then
    NEWSEED=`echo $SEED $NJOBS $LASTDIGIT | awk '{print $1+$2*$3}'`
    echo NEW SEED $NEWSEED
fi

export OS_VERS=Linux-g++
export GAMOS_VER=GAMOS.6.3.1
export GAMOSDIR=/home/${user}/gamos
export RELEASEDIR=${GAMOSDIR}/${GAMOS_VER}
export SENDJOBSDIR=${RELEASEDIR}/myExamples/dna/microyz
export RUNDIR=${TMPDIR}
export RUNDIR=${SENDJOBSDIR}/prod/temp
export OUTDIR=${SENDJOBSDIR}/prod/
source ${RELEASEDIR}/config/confgamos.sh

echo pwd $PWD RELEASEDIR= ${RELEASEDIR} SENDJOBSDIR $SENDJOBSDIR

echo step2 RUNDIR=$RUNDIR OUTDIR=$OUTDIR    PWD=$PWD $ijob1 $ijob26

echo FILE $1 $SEED NEV $NEV NJOBS $NJOBS ijob12 $ijob1 $ijob2 

cd ${RUNDIR}

for ii in $(seq $ijob1 $ijob2);do
    SUFFIX=$1"_"$2"_"$3"_"$4"_"$5"_"$ii
    
    new_inputfile=$SUFFIX
    log_inputfile=zz"_"$new_inputfile
    jobfile=batch"_"$new_inputfile".job"
    
    echo LOOPING $ii RUNDIR $RUNDIR $PWD
    
    echo " The new input file = " $new_inputfile
    
    awk -v NEV=$NEV -v ENER=$ENER -v RADIUS=$RADIUS -v SUFFIX="."$SUFFIX -v NEWSEED=$NEWSEED -v ii=$ii '{
      if($2=="GmAnalysisMgr:FileNameSuffix") {printf("%s %s %s \n",$1, $2, "_"SUFFIX) }
      else if ($1=="/gamos/generator/addSingleParticleSource") {printf("%s %s %s %s\n",$1, $2, $3, ENER"*keV") }
      else if ($1=="/run/beamOn") {printf("%s %s\n",$1, NEV) }
      else if ($1=="/gamos/random/restoreSeeds") {printf("%s %s %s\n",$1, NEWSEED"+2*"ii, NEWSEED"+2*"ii) }
      else if ($1=="/gamos/random/setSeeds") {printf("%s %s %s\n",$1, NEWSEED"+2*"ii, NEWSEED"+2*"ii) }
      else if ($2=="GmMicroyzSD:SphereRadius") {printf("%s %s %s\n",$1, $2, RADIUS"*nm") }
      else { print $0 }
    }' $SENDJOBSDIR/$file_base".in" > $new_inputfile".inn"

    echo $PWD step4a `ls -l $SENDJOBSDIR/$file_base".in" $new_inputfile".inn"`
    
    cp ${RELEASEDIR}/bin/${OS_VERS}/gamos .
    cp -v ${SENDJOBSDIR}/$GENER_FILE .
    cp -u ${SENDJOBSDIR}/*geom .

    echo RUN gamos in background $PWD
    cp -v $new_inputfile.inn ${OUTDIR}
    #    cp -v *inn ${OUTDIR}
    njobsrun=`ps -u $user |grep sh | wc -l`
    echo njobsrun $njobsrun
    if [ $njobsrun -lt 440 ]; then
#c	module load gcc/7.5.0
	echo "gamos $new_inputfile.inn "
 #	(gamos $new_inputfile.inn 2>&1 | tee ${log_inputfile} ; scp ${log_inputfile} arce@tecno31.ciemat.es:gamos/GAMOS.6.3.0/myExamples/PT_Commissioning/prod ) &
	gamos $new_inputfile.inn 2>&1 | tee ${log_inputfile} &
    else
	exit
    fi
    #    ( srun  -N1 -n1 -c1 --exclusive --time=99:00:00 gamos $new_inputfile.inn 2>&1 | tee ${log_inputfile} ; mv -v *${SUFFIX}* ${OUTDIR} ) &
    #-    gamos $new_inputfile.inn 2>&1 | tee $log_inputfile&
    sleep 1 #PRM (0.3125 = 1/32 * 10)
    
done

#c wait

#ls -l 
echo PWD $PWD
#t mv -v *$1_$2_$3*  ${OUTDIR}
SUFF=${1}"_"${2}"_"${3}"_"
cp *${SUFF}* ${OUTDIR}
cd ${SENDJOBSDIR}/..
#bash  checkAllJobsEnded.sh $1 $2 $3 $4 $5 $6 $7 $8 $9${10} ${11} ${12} 
# ls -lrtAF   
