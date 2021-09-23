// ROTATION MATRIX
:ROTM RM0 0. 0. 0.

// MATERIALS
:MIXT PMMA 1.19 3
   G4_H 0.080541
   G4_C 0.599846
   G4_O 0.319613

:MIXT BGO 7.13 3
   G4_Bi   0.67097
   G4_Ge   0.17492
   G4_O    0.15411

// WORLD
:VOLU world BOX 1.*m 1.*m 1.*m G4_AIR
:VIS world OFF

// PHANTOM
:VOLU phantom TUBE 0. 200. 400./2. PMMA
:PLACE phantom 1 world RM0 0. 0. 0.
:COLOR phantom 0.7 0.7 0.7
:VIS phantom ON

// DETECTOR
:VOLU detector BOX 2.*cm 2.*cm 2.*cm BGO
:PLACE detector 1 world RM0 0. 40.*cm 74.5*mm
:VIS detector ON

:CHECK_OVERLAPS * ON