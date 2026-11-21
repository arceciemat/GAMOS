EXER=$1
##### BUILD TEMPLATES
python3 ../../StatTest/prepareQATemplate.py out.$EXER RTDose.qa "IMUM DOSE" 1
python3 ../../StatTest/prepareQATemplate.py out.$EXER RTDoseError.qa "RTPSPDoseHistos AVERAGE ERROR" 0

##### BUILD qa FILE MERGING TEMPLATES + OUTPUT
### BUILD LIST OF qa FILES 
echo "RTDose.qa RTDoseError.qa " > qafiles.$EXER.lis
### GET HISTO FILES TO TEST                                                                             
histoFiles=`sh ../../getHistoFiles.sh out.$EXER`
qaFiles=`cat qafiles.$EXER.lis`
echo ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
python3 ../../StatTest/prepareQA.py $histoFiles -T out.$EXER $EXER.qaref $qaFiles
