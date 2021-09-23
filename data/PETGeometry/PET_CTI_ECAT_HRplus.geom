:P NBLOCKS 288
:P NRINGS 32
:P NCRYSTALS_transaxial 8
:P NCRYSTALS_axial 8
:P CRYS_transaxial 0.439*cm
:P CRYS_axial 0.405*cm
:P CRYS_radial 3.*cm
:P DIAMETER 83.9*cm
:P WORLD_Z 48*cm
:PS CRYS_MATE BGO   // crystal material
 
//--- Build PET geometry
#include PETGeometry.geom
 
//--- Build source
#include sphere_source.geom
