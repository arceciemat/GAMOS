:ROTM RM0 0. 0. 0.

:P POSZ 5
:VOLU world BOX 100. 100. 100. G4_AIR
:VOLU "my tube" TUBS 0. 10. 20. 0. 360. G4_AIR
:PLACE "my tube" 1 world RM0 0. 0. $POSZ
:VOLU sphere ORB 5. G4_Pb
:PLACE sphere 1 "my tube" RM0 0. 1. $POSZ
