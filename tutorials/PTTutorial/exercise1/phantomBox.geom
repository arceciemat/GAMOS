
// ROTATION MATRIX
:ROTM RM0 0. 0. 0.

// WORLD
:VOLU world BOX 2.*m 2.*m 2.*m G4_AIR
:VIS world OFF

// PHANTOM
:VOLU phantomBox BOX 200./2 200./2 200./2 G4_WATER
:PLACE phantomBox 1 world RM0 0. 0. 0.
:COLOR phantomBox 0.7 0.7 0.7

:CHECK_OVERLAPS * ON
