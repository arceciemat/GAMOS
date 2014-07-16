#include "GmMovementDisplacement.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4VVisManager.hh"

//------------------------------------------------------------------------
GmMovementDisplacement::GmMovementDisplacement(std::vector<G4String>& words, GmMovementType mt )
  :GmVMovement( words, mt )
{
}

//------------------------------------------------------------------------
G4bool GmMovementDisplacement::Move()
{
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();

  std::vector<G4VPhysicalVolume*>::iterator itev;
  for(itev = theVolumes.begin(); itev != theVolumes.end(); itev++ ){
    geomMgr->OpenGeometry(*itev);

    G4ThreeVector trans = (*itev)->GetTranslation();
    trans += theValue * theAxis;
    (*itev)->SetTranslation(trans);

    geomMgr->CloseGeometry(true,*itev);
    //   geomMgr->CloseGeometry(true);
  }
  
  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  return true;
}

//------------------------------------------------------------------------
GmMovementDisplacement::~GmMovementDisplacement() 
{
}
