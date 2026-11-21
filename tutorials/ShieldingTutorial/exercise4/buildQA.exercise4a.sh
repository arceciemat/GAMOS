EXER=$1
##### BUILD TEMPLATES
python3 ../../StatTest/prepareQATemplate.py out.$EXER ACTIVITY.qa "ACTIVITY=" 8
python3 ../../StatTest/prepareQATemplate.py out.$EXER PART_COUNT.qa PART_COUNT 0

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "ACTIVITY.qa PART_COUNT.qa " > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py -T out.$EXER $EXER.qaref $qaFiles
python3 ../../StatTest/prepareQA.py -T out.$EXER $EXER.qaref $qaFiles
