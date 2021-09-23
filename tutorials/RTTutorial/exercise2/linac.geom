:ROTM RM0 0 0 0

:MATE CU521ICRU 29 63.54 8.9333
:MATE W521ICRU 74 183.85 19.3

:VOLU expHall BOX 100*cm 100*cm 200*cm G4_AIR

:VIS expHall OFF

:P TARGET_THICK 2.5

// Target 
:VOLU target BOX 2*cm 2*cm $TARGET_THICK G4_Cu
:COLOR target  1 0 0
:PLACE target  1 expHall RM0 0.0 0.0 -$TARGET_THICK

// Primary collimator
:P RMAX 10*cm
:P PC_ZMIN 1.6*cm
:P PC_ZMAX 7.6*cm

:VOLU "primary collimator_0" TUBE 0 $RMAX ($PC_ZMAX-$PC_ZMIN)/2. G4_W
:COLOR "primary collimator_0"  0 1 0
:VOLU "primary collimator_1" CONE 0 20 0 4 ($PC_ZMAX-$PC_ZMIN)/2. G4_AIR
:COLOR "primary collimator_1"  0.5 0.5 0.5
:PLACE "primary collimator_0"  1 expHall RM0  0. 0. -($PC_ZMIN+$PC_ZMAX)/2.
:PLACE "primary collimator_1"  1 "primary collimator_0"  RM0  0. 0. 0. 

// Flattening filter
:MIXT SiFeMn 8. 3 
       G4_Si 1./3.
       G4_Fe 1./3.
       G4_Mn 1./3.

:VOLU "flattening filter" POLYCONE 0. 360.*deg 5
 0. 0. 1.25
 -10. 0. 9.
 -17. 0. 13.8
 -17. 0. 27.8
 -18. 0. 27.8
SiFeMn
:PLACE "flattening filter" 0 expHall RM0 0. 0. -8.*cm

// Monitor
:VOLU monitor TUBE 0. 100. 15. G4_AIR
:PLACE monitor 1 expHall RM0 0. 0. -150. 

// Jaws
:MODULE JAWS jaws_X X STRAIGHT
10*cm 10*cm 5*cm     //DIMENSIONS
0. 300. -300. -100.*cm       // Z_FOCUS RADIUS Z_CENTRE Z_FIELD
-10*cm 10*cm     // FIELD_X_RIGHT FIELD_X_LEFT  
G4_W  expHall   // MATERIAL 
:MODULE JAWS jaws_Y Y STRAIGHT
10*cm 10*cm 5*cm     //DIMENSIONS
0. 420. -420. -100.*cm     // Z_FOCUS RADIUS Z_CENTRE Z_FIELD
-10*cm 10*cm     // FIELD_Y_RIGHT FIELD_Y_LEFT  
G4_W  expHall   // MATERIAL 

