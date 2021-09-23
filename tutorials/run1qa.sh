#!/bin/bash
# sh run1qa.sh PETTutorial exercise2 exercise2b (NOTEXT)

export PYTHONPATH=$ROOTSYS/lib/
export GAMOS_RELEASE_OLD=/home/arce/gamosOnWeb/GAMOS.6.1.0/tutorials/
cd $1/$2
rm failed.lis 
rm failed.lisn

NOTEXT="FALSE"
if [ X$4 = "XNOTEXT" ]; then
    NOTEXT="TRUE"
fi

echo NOTEXT $NOTEXT

export nn1st=1111

if [ $NOTEXT = "FALSE" ]; then

##### TEST text files 
for nn in `seq $nn1st 1120` ; do 
    python ../../StatTest/addSeed.py $3.in $nn 
#### RUN gamos
    echo " "
    echo "@@@ RUNNING GAMOS  $3.in.$nn  qatest.out.$3.$nn"
#   export PATH=""
#   export LD_LIBRARY_PATH=""
    source ../../../config/confgamos.sh
    echo ROOTSYS $ROOTSYS
    gamos $3.in.$nn >& qatest.out.$3.$nn
#### RENAME HISTOGRAM FILES
    grep "saving histograms in fi" qatest.out.$3.$nn | awk '{print $7}' | while read hf; do
      cp -v $hf qatest.$hf.$nn
    done

#### TEST text output with previous release
    export ROOTSYS=$HOME/gamos/external/root/5.34.36.tmp/root/
    export ROOT_BASE_DIR=$HOME/gamos/external/root/5.34.36.tmp/root/
    export PATH="${ROOT_BASE_DIR}/bin:${PATH}"
    export LD_LIBRARY_PATH="${ROOT_BASE_DIR}/lib:${LD_LIBRARY_PATH}"
    echo     python ../../StatTest/runtests.py -T $3.qaref  qatest.out.$3.$nn $GAMOS_RELEASE_OLD/$1/$2/ref.out.$3 | tee qatest.$3.$nn.log
    python ../../StatTest/runtests.py -T $3.qaref  qatest.out.$3.$nn $GAMOS_RELEASE_OLD/$1/$2/ref.out.$3 | tee qatest.$3.$nn.log
    nFAILED=`grep -c ": FAILED" qatest.$3.$nn.log`
    echo "@@@@@@@@@@@ RUN: $nn FAILED  $nFAILED"

### IF NO FAILED BREAK
    if [ $nFAILED = "0"  ]; then
	rm failed.lis
	break	
    fi

#### BUILD failed.lisn with list of FAILED not failed before
    rm failed.lisn
    echo $ CHECKING FAILED IN  qatest.$3.$nn.log
### IF FIRST JOB USE ALL FAILED
    if [ $nn = $nn1st ]; then
	grep ": FAILED" qatest.$3.$nn1st.log > failed.lis
	cp failed.lis failed.lisn
#echo $ CHECK FAILED  qatest.$3.$nn1st
#ls -l  qatest.$3.$nn1st
#        while IFS=' ' read -r lineFailed1; do
#	  echo "LINEF1:" $lineFailed1
#	done <"failed.lis"
#	IFS=''
#	cat failed.lis | while read -r lineFailed1 ; do 
#	    echo "LINEF:" $lineFailed1
#	done 
#	for lineFailed1 in `cat failed.lis`; do 
#	    echo "LINEFF:" $lineFailed1
#	done 
#	cat failed.lis
    else 
### LOOP TO LIST OF FAILED BEFORE, AND CHECK IF ALSO FAILED NOW
	IFS=''
	cat failed.lis | while read lineFailed1 ; do 
#	lineFailed=`echo $lineFailedt | cut -d "(" -f1`
#-	lineFailed=${lineFailedt##*(}
#	lineFailedC=`echo $lineFailed1 | cut -d "Test:" -f1`
	lineFailedC=${lineFailed1%(*}
#	lineFailedC3=${lineFailedC2%(*}
#	lineFailedC4=${lineFailedC3%(*}
# echo "LINE FAILED:" $lineFailedC "ZZ" $lineFailed1
#echo "LINE FAILED C2:" $lineFailedC2 "C3" $lineFailedC3 "C4" $lineFailedC4
	echo "%%% CHECKING LINE FAILED: $lineFailedC"
	if [ $nn != $nn1st ]; then
	    grep "$lineFailedC" qatest.$3.$nn.log | grep ": FAILED" | while read lineFailed2 ; do 
		echo "%%% LINE FAILED REPEATEDLY: $nn $lineFailed2 "
		echo "$lineFailed1" >> failed.lisn
	    done
	fi
      done
    fi

### IF NO REPEATED FAILED BREAK
    touch failed.lisn # in case failed.lisn does not exits
    nFAILEDREP=99999 
    if [ $nn != $nn1st ]; then
	nFAILEDREP=`grep -c ": FAILED" failed.lisn`
	echo "@@@@@@@@@@@ RUN: $nn FAILED_REPEATED  $nFAILEDREP"
    fi
    mv failed.lisn failed.lis
    if [ $nFAILEDREP = "0"  ]; then
	break
    fi
  
done
   
touch failed.lis # in case it does not exist
mv -v failed.lis qatest.summary.$3

unset IFS
NFAILED=`wc -l qatest.summary.$3 | awk '{print $1}' `
echo "@@@@@@@@@@@@@@@@@@@@@@@ SUMMARY TEXT RESULTS " $1 " " $2 " " $3  : N_FAILED = $NFAILED
cat qatest.summary.$3 | while read lineFailed1 ; do 
    echo "%%% LINE FAILED REPEATEDLY: $lineFailed1 "
    lineFailed=`echo ${lineFailed1%: FAILED*}`
    echo "%%% LINE FAILED REPEATEDLY: $lineFailed "  >> qatest.summary.$3
    echo "@@@@ OLD : " ` grep "$lineFailed" $GAMOS_RELEASE_OLD/$1/$2/ref.out.$3 ` >> qatest.summary.$3
    ls qatest.out.$3.* | while read fileqa ; do 
	echo "@@@@ NEW : $fileqa " ` grep "$lineFailed" $fileqa ` >> qatest.summary.$3
    done
#    echo "@@@@ NEW : " ` grep "$lineFailed" qatest.out.$3.* `
done

echo "@@@@@@@@@@@@@@@@@@@@@@@ SUMMARY TEXT RESULTS " $1 " " $2 " " $3  : N_FAILED = $NFAILED >> qatest.summary.$3

fi

####################################################
##### TEST ROOT files 
##### LOOP TO HISTOGRAM FILES FOUND 
#grep "saving histograms in fi" qatest.out.$3.$nn1st | awk '{print $7}' | while read hf; do
grep "saving histograms in fi" out.$3 | awk '{print $7}' | while read hf; do
    unset IFS
    for nn in `seq $nn1st 1120` ; do 
##### ONLY RUN IF NOT ALREADY RUN FOR TEXT
	if [ -e qatest.out.$3.$nn ]; then
	    echo FILE ALREADY EXISTS:  qatest.out.$3.$nn , DO NOT RUN gamos AGAIN
	else 
	    echo FILE DOES NOT EXIST:  qatest.out.$3.$nn 
	    python ../../StatTest/addSeed.py $3.in $nn 
	    echo "@@@ RUNNING GAMOS  $3.in.$nn"
	    gamos $3.in.$nn >& qatest.out.$3.$nn
#### RENAME HISTOGRAM FILES
	    grep "saving histograms in fi" qatest.out.$3.$nn | awk '{print $7}' | while read hf; do
		cp -v $hf qatest.$hf.$nn
	    done
	fi
    
##### TEST ROOT output with previous release   
       echo  ../../StatTest/runtests.py -H -g qatest.report.$hf.pdf  $3.qaref qatest.$hf.$nn $GAMOS_RELEASE_OLD/$1/$2/ref.$hf | tee qatest.$hf.$nn.log
	python ../../StatTest/runtests.py -H -g qatest.report.$hf.pdf  $3.qaref qatest.$hf.$nn $GAMOS_RELEASE_OLD/$1/$2/ref.$hf | tee qatest.$hf.$nn.log
	nFAILED=`grep -c ": FAILED" qatest.$hf.$nn.log`
	echo "@@@@@@@@ RUN: $hf $nn  FAILED  $nFAILED" 
### IF NO FAILED BREAK  
	if [ $nFAILED = "0"  ]; then
	    touch failed.lis # in case it does not exist
	    mv -v failed.lis qatest.summary.$hf
	    break
	fi	
	
#### BUILD failed.lisn with list of FAILED not failed before   
	rm failed.lisn
        echo %%%%% CHECKING FAILED IN  qatest.$hf.$nn.log
### IF FIRST JOB USE ALL FAILED
	if [ $nn = $nn1st ]; then
	    grep ": FAILED" qatest.$hf.$nn1st.log > failed.lis
	    cp failed.lis failed.lisn
	else
### LOOP TO LIST OF FAILED BEFORE, AND CHECK IF ALSO FAILED NOW 
	    IFS=''
	    cat failed.lis | while read lineFailed1 ; do 
		lineFailedC=${lineFailed1%(*}
		echo "%%% CHECKING LINE FAILED: $lineFailedC "
		if [ $nn != $nn1st ]; then
		    grep "$lineFailedC" qatest.$hf.$nn.log  | grep ": FAILED" | while read lineFailed2 ; do 
			echo "%%% LINE FAILED REPEATEDLY: $nn $lineFailed2 "
			echo "$lineFailed1" >> failed.lisn 
		    done
		fi
	    done
	fi

### IF NO REPEATED FAILED BREAK
	touch failed.lisn # in case failed.lisn does not exits
	nFAILEDREP=99999
	if [ $nn != $nn1st ]; then
	    nFAILEDREP=`grep -c ": FAILED" failed.lisn`
	    echo "@@@@@@@@@@@ RUN: $nn FAILED_REPEATED  $nFAILEDREP"
	fi
#    wc failed.lis*
#    cat failed.lisn
#    wc failed.lis*
	mv failed.lisn failed.lis
	if [ $nFAILEDREP = "0"  ]; then
	    break
	fi
    done
    touch failed.lis # in case it does not exist
    mv -v failed.lis qatest.summary.$hf

    echo "@@@@@@@@@@@@@@@@@@@@@@@ SUMMARY ROOT RESULTS " $1 " " $2 " " $3  : N_FAILED = `wc -l qatest.summary.$hf`
    cat qatest.summary.$hf | while read lineFailed1 ; do
	lineFailed=`echo $lineFailed1 | sed s/": FAILED"//g`
	echo "%%% LINE FAILED REPEATEDLY: $lineFailed "
    done
	
done

cd -
