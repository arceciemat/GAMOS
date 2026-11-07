for STEP in 1. 0.5 0.1; do
    echo :DATA ENER RBE > LoncolRBE.$STEP.lis
    ls microtrack_*_${STEP}_30._1111_ALL.csv | while read file; do
	ener=`echo $file | awk -F'_' '{print $2}'`
echo @#@# " RBE=python3 calculateRBE_BiolWeight.py $file LoncolFunction.lis "
	RBE=`python3 calculateRBE_BiolWeight.py $file LoncolFunction.lis  | awk -F' ' '{print $3}'`
	echo LoncolBWF $ener $STEP $RBE
	if [ "$(echo "$RBE != 0.0" | bc -l)" -eq 1 ]; then
	    echo $ener $RBE >>  LoncolRBE.$STEP.lis
	fi
    done
    python3 $PYTHONPATH/plotNColumn.py  LoncolRBE.$STEP.lis
    mv plotNColumn.jpg  LoncolRBE.$STEP.jpg
done

