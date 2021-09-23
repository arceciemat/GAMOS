#!/bin/bash
export PYTHONPATH=$ROOTSYS/lib
cd $1 
echo @%@%@%@%@%@ $1/$2.in

### RUN N JOBS
seed1st=1111
for seed in `seq $seed1st 10 1210` ; do
    rm $2.inn
    cat $2.in | grep -v beamOn > $2.inn
    echo "/gamos/random/restoreSeeds " $seed $seed >> $2.inn
    grep beamOn $2.in >> $2.inn
    gamos $2.inn 2>&1 | tee Nout.$2.$seed
    grep "saving histograms in file" Nout.$2.$seed | while read line; do
	file=`echo $line | awk '{ print $7}' `
	mv -v $file N$2.$seed.$file
    done
done

### MERGE OUTPUT FILES
ls Nout.$2.* > file.lis
python $GAMOSINSTALL/StatTest/mergeOutputs.py $2.qastd file.lis ref.out.$2
rm file.lis

### MERGE HISTO FILES
grep "saving histograms in file" Nout.$2.$seed1st | while read line; do
    file=`echo $line | awk '{ print $7}' `
    hadd -f ref.$file N$2.*.$file
done

cd -
