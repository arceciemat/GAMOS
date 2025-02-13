:include ../../../data/NIST_elements.txt
:include ../../../data/PET_materials.txt
:include ../../../data/NIST_materials.txt

:P NABSRINGS 1   //number of rings
:P NABSBLOCKS 1   // number of blocks in each ring
:P NABSCRYSTALS_transaxial 12   // number of crystals in each block, transaxial direction
:P NABSCRYSTALS_axial 12   // number of crystals in each block, axial direction
:P ABSCRYS_transaxial 0.5*cm   // transaxial crystal dimension
:P ABSCRYS_axial 0.5*cm   // axial crystal dimension
:P ABSCRYS_radial 2.0*cm   // radial crystal dimension
:P ABSDIAMETER 18.0*cm    // ring diameter 
:PS ABSCRYS_MATE CZT   // crystal material
:P NSCATRINGS 1   //number of rings
:P NSCATBLOCKS 1   // number of blocks in each ring
:P NSCATCRYSTALS_transaxial 12   // number of crystals in each block, transaxial direction
:P NSCATCRYSTALS_axial 12   // number of crystals in each block, axial direction
:P SCATCRYS_transaxial 0.5*cm   // transaxial crystal dimension
:P SCATCRYS_axial 0.5*cm   // axial crystal dimension
:P SCATCRYS_radial 1.0*cm   // radial crystal dimension
:P SCATDIAMETER 11.0*cm    // ring diameter  
:PS SCATCRYS_MATE G4_Si   // crystal material
:P WORLD_Z 48.0*cm   // world size   


:include ../exercise1/CCGeometryRing.geom

:ROTM RM0 90 0 90 90 0 0
