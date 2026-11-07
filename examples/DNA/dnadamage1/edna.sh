rm  DNADamTimeStepAction.all
ls zz* |while read file; do
    echo FILE $file
    grep DNADamTimeStepAction $file >> DNADamTimeStepAction.all
    wc  DNADamTimeStepAction.all
done
