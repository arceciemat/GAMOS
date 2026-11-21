:ROTM R00 0. 0. 0.

:VOLU world BOX 50./2 50./2 50./2 G4_AIR

:VOLU mybox BOX 50./2 50./2 2.*cm/2 G4_WATER
:PLACE mybox 1 world R00 0. 0. 0.

:VOLU "my tube" TUBE 0. 20. 10./2 G4_Fe
:PLACE "my tube" 1 mybox R00 0. 5. 0.

:CHECK_OVERLAPS * ON
