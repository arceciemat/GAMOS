:P NBLOCKS 280
:P NRINGS 24
:P NCRYSTALS_transaxial 6
:P NCRYSTALS_axial 6
:P CRYS_transaxial 0.62*cm
:P CRYS_axial 0.62*cm
:P CRYS_radial 3.*cm
:P DIAMETER 88.*cm
:P WORLD_Z 48*cm
:PS CRYS_MATE BGO   // crystal material
 
//--- Build PET geometry
#include PETGeometry.geom
 
//--- Build source
#include sphere_source.geom
