:MIXT LSO 7.4 3
   G4_Lu 0.764016
   G4_Si 0.0613283
   G4_O 0.174656

:ELEM Carbon C 6 12.01 
:ELEM Oxygen O 8 15.998
:MIXT_BY_NATOMS CO2 1.8E-3*g/cm3 2
   Carbon 1
   Oxygen 2

:MIXT CO2_Ar 1.7E-3 2
   G4_Ar 0.5
   CO2   0.5

:P H_MASS 1.0078
:P O_MASS 15.999
:P WATER_MASS 2*$H_MASS+$O_MASS
:ROTM R90Y 0. 90.*deg 0.
:ROTM R0 0. 0. 0.

:VOLU world BOX 1.*meter 1.*m 1.*m G4_AIR

:VOLU CrystalBlock BOX 20. 20. 25. LSO
:PLACE CrystalBlock 1 world R90Y 50. -50. 0.
:PLACE CrystalBlock 2 world R90Y 50.   0. 0.
:PLACE CrystalBlock 3 world R90Y 50.  50. 0.

:VOLU Crystal BOX 5. 5. 25*mm LSO
:PLACE_PARAM Crystal 1 CrystalBlock SQUARE_XY R0 4 4 1*cm 1*cm -1.5*cm -1.5*cm
 
:VOLU Tracker BOX 200. 100. 100. CO2
:PLACE Tracker  1 world R0 500. 0. 0.


