EXER=exercise4c
##### BUILD TEMPLATES

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
cp out.$EXER std.out.$EXER
echo ../../StatTest/prepareQA.py $histoFiles  $EXER.qa$1 
python ../../StatTest/prepareQA.py $histoFiles $EXER.qa$1 
