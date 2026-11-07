#!/bin/bash
#SBATCH --time=48:00:00
#SBATCH --ntasks 4
#SBATCH --mem-per-cpu=12288

umask 0027

if [[ ! $# -eq 9 ]]; then
    echo $# 11 $1 22 $2 33 $3 44 $4 55 $5 66 $6 77 $7 88 $8 99 $9 1010 ${10} " WRONG NUMBER OF PARAMETERS TO run-gamos.IDD.bat: $0 <inputFile> <ener> <gener_file> <seed> <noEvents> <ijob1> <ijob2>" $*
    exit 1
fi

uname -a

user=`whoami`
file_base=$1
ENER=$2
ENER_INTERVAL=$3
SITE_R=$4
SEED=$5
NEV=$6
NJOBS=$7
ijob1=${8}
ijob2=${9}

echo ENER $ENER
. ${HOME}/utils/lastDigit.sh $NEV
NEWSEED=$SEED
echo SEED $SEED LD $LASTDIGIT
if [ $LASTDIGIT != 0 ]; then
    NEWSEED=`echo $SEED $NJOBS $LASTDIGIT | awk '{print $1+$2*$3}'`
    echo NEW SEED $NEWSEED
fi

export OS_VERS=Linux-g++
export GAMOS_VER=GAMOS.7.0
export GAMOSDIR=~/gamos
export RELEASEDIR=${GAMOSDIR}/${GAMOS_VER}
export SENDJOBSDIR=${RELEASEDIR}/myExamples/PT_y
export RUNDIR=${TMPDIR}
export OUTDIR=${SENDJOBSDIR}/prod/
export RUNDIR=${SENDJOBSDIR}/prod/temp
source ${RELEASEDIR}/config/confgamos.sh

echo pwd $PWD RELEASEDIR= ${RELEASEDIR} SENDJOBSDIR $SENDJOBSDIR

echo step2 RUNDIR=$RUNDIR OUTDIR=$OUTDIR    PWD=$PWD $ijob1 $ijob26


echo FILE $1 ENER $ENER ENER_INTERVAL $ENER_INTERVAL SITE_R $SITE_R SEED $SEED NEV $NEV NJOBS $NJOBS ijob1 $ijob1 ijob2 $ijob2 

cd ${RUNDIR}
SUFFIX0=$1"_"$2"_"$3"_"$4"_"$5"_"$6
for ii in $(seq $ijob1 $ijob2);do
    SUFFIXii=$1"_"$2"_"$3"_"$4"_"$5"_"$6"_"$ii
    
    new_inputfile=$SUFFIXii
    log_inputfile=zz"_"$new_inputfile
    jobfile=batch"_"$new_inputfile".job"
    
    echo LOOPING $ii RUNDIR $RUNDIR $PWD
    
    echo " The new input file = " $new_inputfile
    
    echo GENER $GENER SEED $NEWSEED"+2*"$ii
    gawk -v FILE_BASE=$file_base -v NEV=$NEV -v ENER=$ENER -v ENER_INTERVAL="${ENER_INTERVAL}" -v SUFFIX="."$SUFFIXii -v NEWSEED=$NEWSEED -v ii=$ii -v SITE_R=$SITE_R '{
       if($2=="GmAnalysisMgr:FileNameSuffix") {printf("%s %s %s \n",$1, $2, "_"SUFFIX) }
      else if ($1=="/gamos/generator/addSingleParticleSource") {printf("%s %s %s %s\n",$1, $2, $3, ENER )}
      else if ($1=="/run/beamOn") {printf("%s %s\n",$1, NEV) }
      else if ($1=="/gamos/random/restoreSeeds") {printf("%s %s %s\n",$1, NEWSEED"+2*"ii, NEWSEED"+2*"ii) }
      else if ($2=="energyF") {printf("%s %s %s %s %s\n",$1, $2, $3, "0.", ENER"-"ENER_INTERVAL) }  
      else if ($2=="GmGeometryFromText:FileName") {printf("%s %s %s \n",$1, $2, FILE_BASE"_"SITE_R".geomm") }  
      else { print $0 }
    }' $SENDJOBSDIR/$file_base".in" > $new_inputfile".inn"

    echo $PWD step3  FILEBASE $SENDJOBSDIR/$file_base".in" 
    grep geom  $new_inputfile".inn"

    gawk -v SITE_R=$SITE_R '{
      if($1==":SITE_RADIUS") {printf("%s %s %s \n",$1, $2, SITE_R"*um") }
      else { print $0 }
    }' $SENDJOBSDIR/$file_base".geom" > $file_base"_"${SITE_R}".geomm"

    ls -l $SENDJOBSDIR/$file_base".in"
    ls -l  $new_inputfile.inn
    head  $new_inputfile".inn"
    
    echo step5 RUNNING IN ${RUNDIR} XX ${JOBID} XX

    cp ${RELEASEDIR}/bin/${OS_VERS}/gamos .
    cp -v ${SENDJOBSDIR}/$GENER_FILE .
    cp -u ${SENDJOBSDIR}/*geom .
    
    echo RUN gamos in background $PWD
    cp -v $new_inputfile.inn ${OUTDIR}
    #    cp -v *inn ${OUTDIR}
    njobsrun=`ps -u $user |grep sh | wc -l`
    echo njobsrun $njobsrun
    if [ $njobsrun -lt 440 ]; then
	#	echo spack load freeglut@3.0.0  %gcc@8.2.0
	#	spack load freeglut@3.0.0  %gcc@8.2.0
#	module load gcc/7.5.0
	echo "gamos $new_inputfile.inn "
 #	(gamos $new_inputfile.inn 2>&1 | tee ${log_inputfile} ; scp ${log_inputfile} arce@tecno31.ciemat.es:gamos/GAMOS.7.0/myExamples/PT_Commissioning/prod ) &
	gamos $new_inputfile.inn 2>&1 >& ${log_inputfile} &
    else
	exit
    fi
    #    ( srun  -N1 -n1 -c1 --exclusive --time=99:00:00 gamos $new_inputfile.inn 2>&1 | tee ${log_inputfile} ; mv -v *${SUFFIXii}* ${OUTDIR} ) &
    #-    gamos $new_inputfile.inn 2>&1 | tee $log_inputfile&
    sleep 1 #PRM (0.3125 = 1/32 * 10)
    
done

wait

#ls -l 
#echo PWD $PWD
##t mv -v *$1_$2_$3*  ${OUTDIR}
#cp *${SUFFIX0}* ${OUTDIR}
#scp *${SUFFIX0}*csv arce@tecno80.ciemat.es:gamos/GAMOS.7.0/myExamples/PT_Commissioning/prod/
#scp *${SUFFIX0}*root arce@tecno80.ciemat.es:gamos/GAMOS.7.0/myExamples/PT_Commissioning/prod/
#cd ${SENDJOBSDIR}/..
##bash  checkAllJobsEnded.sh $1 $2 $3 $4 $5 $6 $7 $8 $9${10} ${11} ${12} 
## ls -lrtAF   
