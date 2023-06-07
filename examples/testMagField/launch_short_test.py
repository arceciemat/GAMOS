#!/usr/bin/python
import os
import sys

G4application=sys.argv[1]


print "Rapid test of the  MagShield code"
print "---------------------------------"
print "Start the  computation of the  grid of the main toroid"
res=os.system(G4application+' MainToroidSmallGridComputation.g4mac >MainToroidSmallGridComputation.log')
print "Computation Finished!\n"
print "Start the computation of the  grid of the upper toroid"
res=os.system(G4application+' UpperToroidSmallGridComputation.g4mac >UpperToroidSmallGridComputation.log')
print "Computation Finished!\n"
print "Start the total field test"
res=os.system(G4application+' SmallGridTest.g4mac > SmallGridTest.log')
print "Total Field Test Finished!\n"

print "Start the current density test "
res=os.system(G4application+' CoilJTest.g4mac > CoilJTest.log')
print "Current Density Test Finished!\n"

print "Test terminated"





