EXER=exercise3c
##### BUILD TEMPLATES
python ../../StatTest/prepareQATemplate.py out.$EXER PART_COUNT.qa PART_COUNT 0 $1
python ../../StatTest/prepareQATemplate.py out.$EXER PROC_COUNT.qa PROC_COUNT 0 $1
python ../../StatTest/prepareQATemplate.py out.$EXER PROC_CREATOR_COUNT.qa PROC_CREATOR_COUNT 0 $1
python ../../StatTest/prepareQATemplate.py out.$EXER GSD.qa '$$GSD' 3 $1 "n Events" 
python ../../StatTest/prepareQATemplate.py out.$EXER PET.qa '$$PET (' 2 $1

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "PART_COUNT.qa PROC_COUNT.qa PROC_CREATOR_COUNT.qa GSD.qa PET.qa" > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
cp out.$EXER std.out.$EXER
echo ../../StatTest/prepareQA.py $histoFiles -T $1.out.$EXER $EXER.qa$1 $qaFiles
python ../../StatTest/prepareQA.py $histoFiles -T $1.out.$EXER $EXER.qa$1 $qaFiles
