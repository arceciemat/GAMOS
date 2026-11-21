### PRINTS "-H HISTO_FILE" FOR EACH HISTOGRAM FILE SAVED BY THE GAMOS JOB WHOSE OUTPUT IS $1
grep "saving histograms in fi" $1 | awk '{printf " -H %s", $7}'
echo
