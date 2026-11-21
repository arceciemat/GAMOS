:VOLU world BOX 200. 200. 300. G4_AIR

:ROTM RM0 0 0 0
:VOLU lead_tube TUBE 0. 15*cm 50*cm/2 G4_Pb
:PLACE lead_tube 1 world RM0 0. 0. 0.
:MIXT Ar1e-8 1.e-8 1 G4_Ar 1.
:VOLU vacuum_tube TUBE 0. 14*cm 49*cm/2 Ar1e-8
:PLACE vacuum_tube 1 lead_tube RM0 0. 0. 0.
:VOLU disk TUBE 0. 100./2 50./2 G4_W
:PLACE disk 1 vacuum_tube RM0 0 0 200

:CHECK_OVERLAPS * ON
