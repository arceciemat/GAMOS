EXER=exercise4c
##### BUILD TEMPLATES

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py $histoFiles  $EXER.qaref 
python3 ../../StatTest/prepareQA.py $histoFiles $EXER.qaref 
