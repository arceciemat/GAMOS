
SUFFIX=$1_$2_$3_$4_$5
echo SUFFIX $SUFFIX
ls prod/*$SUFFIX*csv > file.$SUFFIX.lis
python3 $PYTHONPATH/sumHist.py ${SUFFIX}_ALL.csv file.$SUFFIX.lis

hadd -f ${SUFFIX}_ALL.root prod/*$SUFFIX*root 
 


