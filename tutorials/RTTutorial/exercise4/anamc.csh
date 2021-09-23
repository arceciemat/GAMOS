analysePS -f test$1.IAEAheader -fOut ps.$1 |& tee zzp.$1
analyseSqdose -f sqdose.$1.out -fOut dose.$1 |& tee zzd.$1
