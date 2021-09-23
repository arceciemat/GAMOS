
:MIXT LSO 7.4 3
   G4_Lu 0.764016
   G4_Si 0.0613283
   G4_O 0.174656

:ELEM Hydrogen H 1 1. 
:ELEM Oxygen O 8 15.998
:MIXT_BY_NATOMS H2O 1.*g/cm3 2
   Hydrogen 2 
   Oxygen 1
:ROTM RM0 0. 0. 0. 

:VOLU world BOX 1.*meter 1.*m 1.*m G4_AIR
:VIS world OFF

:P crysXY 1.*cm
:P crysZ 5.*cm
:VOLU CrystalRing TUBE 50.*cm 70.*cm 8.*cm G4_AIR
:PLACE CrystalRing 1 world RM0 0. 0. -330.
:PLACE CrystalRing 2 world RM0 0. 0. -165.
:PLACE CrystalRing 3 world RM0 0. 0. 0.
:PLACE CrystalRing 4 world RM0 0. 0. 165.
:PLACE CrystalRing 5 world RM0 0. 0. 330.
:VIS CrystalRing OFF

:P Block_Wall 0.5
:VOLU CrystalBlock BOX  $crysZ+$Block_Wall/2 8*$crysXY+$Block_Wall/2 8*$crysXY+$Block_Wall/2  G4_Al
:PLACE_PARAM CrystalBlock 1 CrystalRing CIRCLE_XY RM0 20 18.*deg 0. 60.*cm
:COLOUR CrystalBlock 1. 0. 0.

:VOLU Crystal BOX  $crysZ $crysXY $crysXY LSO
:PLACE_PARAM Crystal 1 CrystalBlock SQUARE_YZ RM0 8 8 2*cm 2*cm -7*cm -7*cm
:COLOUR Crystal 0. 0. 1.

:VOLU "water sphere" ORB 20.*cm G4_WATER
:PLACE "water sphere" 1 world RM0 0. 0. 0. 



