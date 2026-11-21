EXER=$1
##### BUILD TEMPLATES
python3 ../../StatTest/prepareQATemplate.py out.$EXER YIELD.qa "TOTAL_YIELD=" 0

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "YIELD.qa " > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
python3 ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
