:ROTM RM0 0 0 0 
//--- World
:P WORLD_X ($DIAMETER+$CRYS_radial*2)*1.2 // slightly bigger than detector
:P WORLD_Y $WORLD_X
:VOLU world BOX $WORLD_X/2. $WORLD_Y/2. $WORLD_Z/2. G4_AIR 

//--- Detector, built from union of rings
:P RING_INNER_DIAMETER $DIAMETER/2.-$CRYS_radial*2.
:P RING_OUTER_DIAMETER $WORLD_X/2.
:P RING_Z $CRYS_axial*$NCRYSTALS_axial
:VOLU detector TUBE $RING_INNER_DIAMETER $RING_OUTER_DIAMETER $RING_Z/2.*$NRINGS G4_AIR
:PLACE detector 0 world RM0 0 0 0 

//--- Ring of blocks
:VOLU ring TUBE $RING_INNER_DIAMETER $RING_OUTER_DIAMETER $RING_Z/2. G4_AIR
:PLACE_PARAM ring 1 detector LINEAR_Z RM0 $NRINGS $RING_Z -($NRINGS-1)/2.*$RING_Z 

//--- Block of crystals
:P BLOCK_X $CRYS_radial
:P BLOCK_Y $CRYS_transaxial*$NCRYSTALS_transaxial
:P BLOCK_Z $CRYS_axial*$NCRYSTALS_axial
:VOLU block BOX $BLOCK_X/2. $BLOCK_Y/2. $BLOCK_Z/2. G4_AIR
:PLACE_PARAM block 1 ring CIRCLE_XY RM0 $NBLOCKS 360.*deg/$NBLOCKS 0. $DIAMETER/2.

//--- Crystals 
:VOLU crystal BOX $CRYS_radial/2. $CRYS_transaxial/2. $CRYS_axial/2. $CRYS_MATE
:PLACE_PARAM crystal 1 block SQUARE_YZ RM0 $NCRYSTALS_transaxial $NCRYSTALS_axial $CRYS_transaxial $CRYS_axial -($NCRYSTALS_transaxial-1)/2.*$CRYS_transaxial -($NCRYSTALS_axial-1)/2.*$CRYS_axial 

