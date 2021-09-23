// Place a point-like Am-Be source of 1 mCi inside a sphere of lead of inner radius 5 cm and outer radius 6 cm

:ROTM RM0 0. 0. 0.

:VOLU world BOX 100 100 100 G4_AIR
// we define a filled sphere and then substract the inner part: in this way it is easier to check if the track exits the outer or the inner sphere surface
:VOLU AmBeShield SPHERE 5.*cm 6.*cm 0. 360.*deg 0. 180.*deg G4_Pb
:PLACE AmBeShield 1 world RM0 0. 0. 0.




