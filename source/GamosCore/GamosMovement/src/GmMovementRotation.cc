#include "GmMovementRotation.hh"
#include "GmMovementVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4VVisManager.hh"

//------------------------------------------------------------------------
GmMovementRotation::GmMovementRotation(std::vector<G4String>& words, GmMovementType mt )
  :GmVMovement( words, mt )
{
}

//------------------------------------------------------------------------
G4bool GmMovementRotation::Move()
{
  //  G4RunManager::GetRunManager()->AbortRun();
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();

  std::vector<G4VPhysicalVolume*>::iterator itev;
  for(itev = theVolumes.begin(); itev != theVolumes.end(); itev++ ){
    geomMgr->OpenGeometry(*itev);

    G4RotationMatrix* rm = (*itev)->GetRotation();
    rm->rotate(theValue,theAxis);
    (*itev)->SetRotation(rm);
#ifndef GAMOS_NO_VERBOSE
    if( MoveVerb(debugVerb) ) G4cout << " GmMovementRotation::Move " << (*itev)->GetName() << " " << rm << G4endl;
#endif
    geomMgr->CloseGeometry(true,false,*itev);
  }
  
  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  return true;
}

//------------------------------------------------------------------------
GmMovementRotation::~GmMovementRotation() 
{
}
