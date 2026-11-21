EXER=$1
##### BUILD TEMPLATES
#  index: 1  = -0.00718896 +-(REL) -0.022421 CLHEP::cm-2 sumV2= 3.11458
python3 ../../StatTest/prepareQATemplate.py out.$EXER SUMALL.qa 'SUM ALL' 3 zxqas

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "SUMALL.qa" > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
python3 ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
