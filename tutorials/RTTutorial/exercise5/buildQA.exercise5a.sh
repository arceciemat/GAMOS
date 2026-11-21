EXER=$1
##### BUILD TEMPLATES

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "RTDose.qa" > qafiles.$EXER.lis
echo "RTDoseError.qa" >> qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
python3 ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
