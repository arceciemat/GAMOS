//Geometry: a plate of concrete (G4_CONCRETE) of surface 1 m X 1 m

:ROTM RM0 0. 0. 0.

:VOLU world BOX 1000. 1000. 1000. G4_AIR
:P WIDTH 36.
:VOLU shield BOX 1000./2. 1000./2 $WIDTH/2. G4_Pb
:PLACE shield 1 world RM0 0. 0. $WIDTH/2.

