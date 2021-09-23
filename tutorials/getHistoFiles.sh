### GET HISTO FILES CREATED
histoFiles=""
grep "saving histograms in fi" $1 | awk '{print $7}' | while read hf; do
  histoFiles=`echo $histoFiles  -H $hf`
  echo $histoFiles > histofiles.lis
done
histoFiles=`cat histofiles.lis`
echo $histoFiles
rm histofiles.lis
