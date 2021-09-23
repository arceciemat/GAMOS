// Place a point-like Cf source of 10 mCi in the centre of a room of  8 X 8 X 8 m, with 20 cm of polyethylene +  50 cm of concrete walls, floor and ceiling

:ROTM RM0 0. 0. 0. 
:VOLU world BOX 10000/2 10000/2 10000/2 G4_AIR

:P ROOM_WIDTH 8.*m
:P POLY_WIDTH 20.*cm
:P CONC_WIDTH 50.*cm

:MIXT CONCRETE 2.289*g/cm3 13
 G4_Mg  0.5670/100.
 G4_Mn  0.02882/100.
 G4_O   52.19/100.
 G4_Al  3.968/100.
 G4_Pb  0.2016/100.
 G4_H   0.52/100.
 G4_Cl  0.3352/100.
 G4_K   1.185/100.
 G4_Si  27.79/100.
 G4_Ca  11.30/100.
 G4_Na  1.057/100.
 G4_C   0.2271/100.
 G4_Fe  0.63/100.

:VOLU wall_concrete  BOX $ROOM_WIDTH/2.+$POLY_WIDTH+$CONC_WIDTH  $ROOM_WIDTH/2.+$POLY_WIDTH+$CONC_WIDTH $ROOM_WIDTH/2.+$POLY_WIDTH+$CONC_WIDTH CONCRETE
:PLACE  wall_concrete 1 world RM0 0. 0. 0. 

:VOLU wall_polyehtylene BOX $ROOM_WIDTH/2.+$POLY_WIDTH $ROOM_WIDTH/2.+$POLY_WIDTH $ROOM_WIDTH/2.+$POLY_WIDTH G4_POLYETHYLENE
:PLACE wall_polyehtylene 1 wall_concrete RM0 0. 0. 0. 

:VOLU room BOX $ROOM_WIDTH/2. $ROOM_WIDTH/2. $ROOM_WIDTH/2. G4_AIR
:PLACE room 1 wall_polyehtylene RM0 0. 0. 0. 

:VOLU PDS_detector  ORB 0.001 G4_AIR 
:PLACE PDS_detector 1 world RM0  $ROOM_WIDTH/2.+$POLY_WIDTH+$CONC_WIDTH+10.*cm 0. 0. 
