#ifndef GmProdCutOutsideVoxelFilter_h
#define GmProdCutOutsideVoxelFilter_h 1

#include "globals.hh"
#include "GmVProdCutOutsideVoxelFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles that would be cut by a production cut (and their daughters) and only if they are in the voxel different than the voxel where it would be produced (for daughters it is counted the voxel of the ancestor)
// The cut is only checked for particles created by ionisationor bremsstrahlung processes
// 
///////////////////////////////////////////////////////////////////////////////

#include <set>

class GmProdCutOutsideVoxelFilter : public GmVProdCutOutsideVoxelFilter
{

public: // with description
  GmProdCutOutsideVoxelFilter(G4String name);
  virtual ~GmProdCutOutsideVoxelFilter();

public: // with description
  virtual G4bool CheckProcess(const G4VProcess*);

};

#endif
