EXER=$1
##### BUILD TEMPLATES
#  index: 1  = -0.00718896 +-(REL) -0.022421 CLHEP::cm-2 sumV2= 3.11458
python ../../StatTest/prepareQATemplate.py out.$EXER SUMALL.qa 'SUM ALL' 5 $2 zxqas REL

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "SUMALL.qa" > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
cp out.$EXER std.out.$EXER
echo ../../StatTest/prepareQA.py $histoFiles -T $2.out.$EXER $EXER.qa$2 $qaFiles
python ../../StatTest/prepareQA.py $histoFiles -T $2.out.$EXER $EXER.qa$2 $qaFiles
