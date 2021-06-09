#include "GmProdCutOutsideVoxelFilter.hh"
#include "G4VProcess.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

GmProdCutOutsideVoxelFilter::GmProdCutOutsideVoxelFilter(G4String name)
  :GmVProdCutOutsideVoxelFilter(name)
{;}

GmProdCutOutsideVoxelFilter::~GmProdCutOutsideVoxelFilter()
{;}

G4bool GmProdCutOutsideVoxelFilter::CheckProcess(const G4VProcess* proc)
{
  G4String procName = proc->GetProcessName();
  //  G4cout << " GmProdCutOutsideVoxelFilter::CheckProcess " << procName << G4endl;
  //  if( procName.find("Ioni") != std::string::npos || procName.find("Brem") != std::string::npos ) {
  if( procName.find("Ioni") != std::string::npos ) {
    return TRUE;
  }else if( procName.find("Brem") != std::string::npos ) {
    //    G4cout << " GmProdCutOutsideVoxelFilter BREMS" << G4endl;
    return TRUE;
  } else {
    return FALSE;
  }
 
}
