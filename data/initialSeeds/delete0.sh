ls initialSeeds.1e+11.0* | while read file; do
    filen="initialSeeds.1e+11."`echo $file | cut -d "0" -f2`""`echo $file | cut -d "0" -f3`""`echo $file | cut -d "0" -f4`
    filen="initialSeeds.1e+11."`echo ${file:20:3}`
    echo "mv "$file " " $filen
done
