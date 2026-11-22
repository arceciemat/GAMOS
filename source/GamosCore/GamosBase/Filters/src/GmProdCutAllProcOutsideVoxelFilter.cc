#include "GmProdCutAllProcOutsideVoxelFilter.hh"
#include "G4VProcess.hh"

GmProdCutAllProcOutsideVoxelFilter::GmProdCutAllProcOutsideVoxelFilter(G4String name)
  :GmVProdCutOutsideVoxelFilter(name)
{;}

GmProdCutAllProcOutsideVoxelFilter::~GmProdCutAllProcOutsideVoxelFilter()
{;}

G4bool GmProdCutAllProcOutsideVoxelFilter::CheckProcess(const G4VProcess*)
{
  return TRUE; 
}
