:P NBLOCKS 144
:P NRINGS 24
:P NCRYSTALS_transaxial 8
:P NCRYSTALS_axial 8
:P CRYS_transaxial 0.32*cm
:P CRYS_axial 0.32*cm
:P CRYS_radial 2.*cm
:P DIAMETER 41.2*cm
:P WORLD_Z 48*cm
:PS CRYS_MATE BGO   // crystal material
 
//--- Build PET geometry
#include PETGeometry.geom
 
//--- Build source
#include sphere_source.geom
