EXER=$1
##### BUILD TEMPLATES
python ../../StatTest/prepareQATemplate.py out.$EXER VL1.qa "FIT chi2=" 2 $2
python ../../StatTest/prepareQATemplate.py out.$EXER VL2.qa "FROM FIT=" 3 $2 zzz ABS
python ../../StatTest/prepareQATemplate.py out.$EXER VL3.qa "FROM SCORES=" 3 $2 zzz ABS

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "VL1.qa VL2.qa VL3.qa " > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
cp out.$EXER std.out.$EXER
echo ../../StatTest/prepareQA.py $histoFiles -T $2.out.$EXER $EXER.qa$2 $qaFiles
python ../../StatTest/prepareQA.py $histoFiles -T $2.out.$EXER $EXER.qa$2 $qaFiles
