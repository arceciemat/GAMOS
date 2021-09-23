:include ../../../data/NIST_elements.txt
:include ../../../data/PET_materials.txt

:VOLU world BOX 100 100 250 G4_AIR
:P NaI_SIZE 3*2.54*cm
:P DET_SIZE $NaI_SIZE+0.24*cm+2.*cm 
:VOLU detector TUBE 0. $NaI_SIZE/2+0.24*cm $DET_SIZE/2. G4_Al
:ROTM RM0 0 0 0 
:PLACE detector 1 world RM0 0. 0. $DET_SIZE/2. // face starts at z=0.
:VOLU crystal TUBE 0. $NaI_SIZE/2. $NaI_SIZE/2. NaI
:PLACE crystal 1 detector RM0 0. 0. -2*cm+($DET_SIZE-$NaI_SIZE)/2.

