rm file.lis
echo "@#@# ls DamagingChemicalReactions.out*$1* |while read file ; do"
ls DamagingChemicalReactions.out*$1* |while read file ; do
#    DamagingChemicalReactions.outdnadamage1.2222.50.10
#    PhysicalInteractions.outdnadamage1.2222.50.10
    filet=`echo $file |sed s/DamagingChemicalReactions/PhysicalInteractions/g`
    echo FILES $file $filet
    if [ -s $filet ]; then
	echo WRITE $filet $file
	echo $filet $file >> file.lis
    fi
done

python3 sumFiles.py PhysicalInteractions${1}_ALL DamagingChemicalReactions${1}_ALL file.lis
