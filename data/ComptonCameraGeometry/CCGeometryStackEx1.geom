#include ../../data/NIST_elements.txt
#include ../../data/PET_materials.txt


:P NSCATLAYERS 2  // number of scatterer layers
:P NSCATCRYSTALS_X 12   // number of crystals in each block, X direction:
P NSCATCRYSTALS_Y 12   // number of crystals in each block, Y direction
:P NSCATCRYSTALS_Z 1   // number of crystals in each block, Z direction
:P SCATCRYS_X 0.5*cm   // X crystal dimension
:P SCATCRYS_Y 0.5*cm   // Y crystal dimension
:P SCATCRYS_Z 1.0*cm   // Z crystal dimension
:P SCATDIST 10.5*cm    // First scatterer dist to origin 
:P SCATSEP 2.0*cm      //Separation between scatterer detectors

:PS SCATCRYS_MATE CZT   // crystal material

:P NABSLAYERS 2  // number of absorber layers
:P NABSCRYSTALS_X 12   // number of crystals in each block, X direction
:P NABSCRYSTALS_y 12   // number of crystals in each block, Y direction
:P NABSCRYSTALS_Z 1   // number of crystals in each block, Z direction
:P ABSCRYS_X 0.5*cm   // transaxial crystal dimension
:P ABSCRYS_Y 0.5*cm   // axial crystal dimension
:P ABSCRYS_Z 2.0*cm   // radial crystal dimension
:P ABSDIST 15.5*cm    // First absorber dist to origin 
:P ABSSEP 4.0*cm      //Separation between absorber detectors
:PS ABSCRYS_MATE CZT   // crystal material


#include CCGeometryStack.geom

//--- Water sphere source
//:VOLU water_sphere  ORB 10 G4_WATER
:ROTM RM0 90 0 90 90 0 0
//:PLACE water_sphere 0 world RM0 0 0 0


