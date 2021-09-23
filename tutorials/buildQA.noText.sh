EXER=$1
##### BUILD TEMPLATES

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
cp out.$EXER std.out.$EXER
echo ../../StatTest/prepareQA.py $histoFiles  $EXER.qa$2 
python ../../StatTest/prepareQA.py $histoFiles $EXER.qa$2 
pwd
