#!/bin/bash
export PYTHONPATH=$ROOTSYS/lib
FILE=$1
echo @%@%@%@%@%@ $1.in

### RUN N JOBS
seed1st=1111
NEV=2000000
for seed in `seq $seed1st 10 1510` ; do
    SUFFIX=$FILE.$seed
    new_inputfile=$SUFFIX
    # rm ${FILE}.inn
    #    cat ${FILE}.in | grep -v beamOn > ${FILE}.inn
    gawk -v FILE=$FILE -v NEV=$NEV -v SUFFIX=$SUFFIX -v SEED=$seed '{
       if($2=="GmAnalysisMgr:FileNameSuffix") {printf("%s %s %s \n",$1, $2, "_"SUFFIX) }
      else if ($1=="/run/beamOn") {printf("%s %s\n",$1, NEV) }
      else if ($2=="GmReadPhantomGeometry:FileName") {printf("%s %s %s\n",$1, $2, "world."PATIENT".geomm") }
      else if ($1=="/gamos/random/setSeeds") {printf("%s %s %s\n",$1, SEED, SEED) }
      else if ($1=="/gamos/random/restoreSeeds") {printf("%s %s %s\n",$1, SEED, SEED) }
      else if ($2=="RTPhaseSpaceUA:FileName") {printf("%s %s %s \n",$1, $2, SUFFIX) }  
      else { print $0 }
    }' $FILE".in" > $new_inputfile".inn"
    grep rangeShif $new_inputfile.inn
    gamos ${new_inputfile}.inn 2>&1 | tee Nout.${SUFFIX} &
done

### MERGE HISTO FILES

