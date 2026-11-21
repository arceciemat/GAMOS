EXER=$1
##### BUILD TEMPLATES
python3 ../../StatTest/prepareQATemplate.py out.$EXER VL1.qa "FIT chi2=" 2
python3 ../../StatTest/prepareQATemplate.py out.$EXER VL2.qa "FROM FIT=" 3 zzz
python3 ../../StatTest/prepareQATemplate.py out.$EXER VL3.qa "FROM SCORES=" 3 zzz

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "VL1.qa VL2.qa VL3.qa " > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
python3 ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
