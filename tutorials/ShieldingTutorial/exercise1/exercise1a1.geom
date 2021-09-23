//Geometry: a plate of concrete (G4_CONCRETE) of surface 1 m X 1 m

:ROTM RM0 0. 0. 0.

:P WIDTH 0.1
:VOLU world BOX 1000. 1000. 1000. G4_AIR
:VOLU shield BOX 1000./2. 1000./2 $WIDTH/2. G4_Pb

:PLACE_PARAM shield 1 world LINEAR_Z RM0 1000. 1. $WIDTH/2.

