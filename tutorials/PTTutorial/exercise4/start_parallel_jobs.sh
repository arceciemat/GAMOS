#!/bin/bash

# Exercise 4c. Run Gamos on 8 cores and write log files:
# source sendjobs ENERGY RANDOM_SEED NUMBER_OF_EVENTS NUMBER_OF_JOBS > log_file &

# send 10 jobs (10,000 events each) to the 1st core:
source sendjobs 226.62 1111 10000 10 > log1 &

# send 10 jobs (10,000 events each) to the 2nd core: 
source sendjobs 226.62 2111 10000 10 > log2 &

# send 10 jobs (10,000 events each) to the 3rd core: 
source sendjobs 226.62 3111 10000 10 > log3 &

# send 10 jobs (10,000 events each) to the 4th core: 
source sendjobs 226.62 4111 10000 10 > log4 &

# send 10 jobs (10,000 events each) to the 5th core: 
source sendjobs 226.62 5111 10000 10 > log5 &

# send 10 jobs (10,000 events each) to the 6th core: 
source sendjobs 226.62 6111 10000 10 > log6 &

# send 10 jobs (10,000 events each) to the 7th core: 
source sendjobs 226.62 7111 10000 10 > log7 &

# send 10 jobs (10,000 events each) to the 8th core: 
source sendjobs 226.62 8111 10000 10 > log8 &

# TOTAL EVENTS = 800,000
echo "Done"
