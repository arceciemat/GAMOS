## The number of nuclei of a given species in the file is not the same that the number of nuclei decaying in one second. Calculate the weight applied to each nucluei : WEIGHT = N_NUCLEI_PRODUCED / N_NUCLEI_DECAYING(=ACTIVITY)
grep SHActivationUA $1 | grep " 900 sec " | grep -v neutron | grep -v TOTAL | awk '{print $6 " " $8 "/" $14}' | tee activity.weights.lis
echo "e- 1." >> activity.weights.lis
echo "e+ 1." >> activity.weights.lis
echo "gamma 1." >> activity.weights.lis
echo "nu_e 1." >> activity.weights.lis
