// Build water as an example (you may use G4_WATER instead)
:P H_MASS 1.0078
:P O_MASS 15.999
:P WATER_MASS 2*$H_MASS+$O_MASS
:ELEM Hydrogen H 1. $H_MASS
:ELEM Oxygen O 8 $O_MASS
:MIXT Water 1.0 2
      Hydrogen   2*$H_MASS/$WATER_MASS
      Oxygen     $O_MASS/$WATER_MASS
:ROTM R00 0. 0. 0.

:VOLU world BOX 300. 300. 300. G4_AIR

:VOLU mybox BOX 300./2. 300./2. 40./2.*cm G4_Al
:PLACE mybox 1 world R00 0. 0. 0.

:VOLU "my tube" TUBE 0. 100. 100. Water
:PLACE "my tube" 1 mybox R00 0. 90.*deg 0.

