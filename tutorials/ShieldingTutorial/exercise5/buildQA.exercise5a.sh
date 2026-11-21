EXER=$1
##### BUILD TEMPLATES
python3 ../../StatTest/prepareQATemplate.py out.$EXER XS.qa "TOTAL_XS" 4   # value = macroscopic XS (the line key must not contain a printed value)

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "XS.qa" > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
python3 ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
