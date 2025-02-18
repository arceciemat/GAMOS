//--- World
:P WORLD_X ($ABSDIAMETER+$ABSCRYS_radial*2)*2.2 // slightly bigger than detector
:P WORLD_Y $WORLD_X
:VOLU world BOX $WORLD_X/2. $WORLD_Y/2. $WORLD_Z/2. G4_AIR 



//--- Absorber Detector, built from union of rings
:P ABSRING_INNER_DIAMETER $ABSDIAMETER/2.-$ABSCRYS_radial/2.
:P ABSRING_OUTER_DIAMETER $ABSDIAMETER/2.+$ABSCRYS_radial/2.
:P ABSRING_Z $ABSCRYS_axial*$NABSCRYSTALS_axial
:VOLU absdetector TUBE $ABSRING_INNER_DIAMETER $ABSRING_OUTER_DIAMETER $ABSRING_Z/2.*$NABSRINGS G4_AIR
:PLACE absdetector 1 world RM0 0 0 0 

//--- Absorber Ring of blocks
:VOLU absring TUBE $ABSRING_INNER_DIAMETER $ABSRING_OUTER_DIAMETER $ABSRING_Z/2. G4_AIR
:PLACE_PARAM absring 1 absdetector LINEAR_Z RM0 $NABSRINGS $ABSRING_Z -($NABSRINGS-1)/2.*$ABSRING_Z 
//:PLACE absring 1 absdetector RM0 0 0 0 

//--- Absorber Block of crystals
:P ABSBLOCK_X $ABSCRYS_radial
:P ABSBLOCK_Y $ABSCRYS_transaxial*$NABSCRYSTALS_transaxial
:P ABSBLOCK_Z $ABSCRYS_axial*$NABSCRYSTALS_axial
:VOLU absblock BOX $ABSBLOCK_X/2. $ABSBLOCK_Y/2. $ABSBLOCK_Z/2. G4_AIR
:PLACE_PARAM absblock 1 absring CIRCLE_XY RM0 $NABSBLOCKS 360.*deg/$NABSBLOCKS 0. $ABSDIAMETER/2.

//--- Absorber Crystals 
:VOLU abscrystal BOX $ABSCRYS_radial/2. $ABSCRYS_transaxial/2. $ABSCRYS_axial/2. $ABSCRYS_MATE
:PLACE_PARAM abscrystal 1 absblock SQUARE_YZ RM0 $NABSCRYSTALS_transaxial $NABSCRYSTALS_axial $ABSCRYS_transaxial $ABSCRYS_axial -($NABSCRYSTALS_transaxial-1)/2.*$ABSCRYS_transaxial -($NABSCRYSTALS_axial-1)/2.*$ABSCRYS_axial 



//--- Detector, built from union of rings
:P SCATRING_INNER_DIAMETER $SCATDIAMETER/2.-$SCATCRYS_radial/2.
:P SCATRING_OUTER_DIAMETER $SCATDIAMETER/2.+$SCATCRYS_radial/2.
:P SCATRING_Z $SCATCRYS_axial*$NSCATCRYSTALS_axial
:VOLU scatdetector TUBE $SCATRING_INNER_DIAMETER $SCATRING_OUTER_DIAMETER $SCATRING_Z/2.*$NSCATRINGS G4_AIR
:PLACE scatdetector 1 world RM0 0 0 0 

//--- Ring of blocks
:VOLU scatring TUBE $SCATRING_INNER_DIAMETER $SCATRING_OUTER_DIAMETER $SCATRING_Z/2. G4_AIR
:PLACE_PARAM scatring 1 scatdetector LINEAR_Z RM0 $NSCATRINGS $SCATRING_Z -($NSCATRINGS-1)/2.*$SCATRING_Z 
//:PLACE scatring 1 scatdetector RM0 0 0 0 

//--- Block of crystals
:P SCATBLOCK_X $SCATCRYS_radial
:P SCATBLOCK_Y $SCATCRYS_transaxial*$NSCATCRYSTALS_transaxial
:P SCATBLOCK_Z $SCATCRYS_axial*$NSCATCRYSTALS_axial
:VOLU scatblock BOX $SCATBLOCK_X/2. $SCATBLOCK_Y/2. $SCATBLOCK_Z/2. G4_AIR
:PLACE_PARAM scatblock 1 scatring CIRCLE_XY RM0 $NSCATBLOCKS 360.*deg/$NSCATBLOCKS 0. $SCATDIAMETER/2.

//--- Crystals 
:VOLU scatcrystal BOX $SCATCRYS_radial/2. $SCATCRYS_transaxial/2. $SCATCRYS_axial/2. $SCATCRYS_MATE
:PLACE_PARAM scatcrystal 1 scatblock SQUARE_YZ RM0 $NSCATCRYSTALS_transaxial $NSCATCRYSTALS_axial $SCATCRYS_transaxial $SCATCRYS_axial -($NSCATCRYSTALS_transaxial-1)/2.*$SCATCRYS_transaxial -($NSCATCRYSTALS_axial-1)/2.*$SCATCRYS_axial 




