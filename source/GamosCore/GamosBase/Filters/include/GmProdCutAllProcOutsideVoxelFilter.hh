#ifndef GmProdCutAllProcOutsideVoxelFilter_h
#define GmProdCutAllProcOutsideVoxelFilter_h 1

#include "globals.hh"
#include "GmVProdCutOutsideVoxelFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts aprtricles that would be cut by a production cut (and their daughters) and only if they are in the voxel different than the voxel where it would be produced (for daughters it is counted the voxel of the ancestor)
// The cut is checked for all particles 
// 
///////////////////////////////////////////////////////////////////////////////

#include <set>

class GmProdCutAllProcOutsideVoxelFilter : public GmVProdCutOutsideVoxelFilter
{

public: // with description
  GmProdCutAllProcOutsideVoxelFilter(G4String name);
  virtual ~GmProdCutAllProcOutsideVoxelFilter();

public: // with description
  virtual G4bool CheckProcess(const G4VProcess*);

};

#endif
