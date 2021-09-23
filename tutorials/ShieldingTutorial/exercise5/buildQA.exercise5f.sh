EXER=$1
##### BUILD TEMPLATES
python ../../StatTest/prepareQATemplate.py out.$EXER YIELD.qa "TOTAL_YIELD=" 0 $2

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "YIELD.qa " > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
cp out.$EXER std.out.$EXER
echo ../../StatTest/prepareQA.py $histoFiles -T $2.out.$EXER $EXER.qa$2 $qaFiles
python ../../StatTest/prepareQA.py $histoFiles -T $2.out.$EXER $EXER.qa$2 $qaFiles
