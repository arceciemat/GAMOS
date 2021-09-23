:P NBLOCKS 168
:P NRINGS 18
:P NCRYSTALS_transaxial 6
:P NCRYSTALS_axial 6
:P CRYS_transaxial 0.8*cm
:P CRYS_axial 0.8*cm
:P CRYS_radial 3.*cm
:P DIAMETER 92.7*cm
:P WORLD_Z 48*cm
:PS CRYS_MATE BGO   // crystal material
 
//--- Build PET geometry
#include PETGeometry.geom
 
//--- Build source
#include sphere_source.geom
