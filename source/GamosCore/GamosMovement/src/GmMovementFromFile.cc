#include "GmMovementFromFile.hh"
#include "GmMovementMgr.hh"
#include "GmMovementVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4VVisManager.hh"

//------------------------------------------------------------------------
GmMovementFromFile::GmMovementFromFile(std::vector<G4String> wl )
{
  theVolumes = GmGeometryUtils::GetInstance()->GetPhysicalVolumes( wl[0], true );
  if( wl[1] == "Time" ) {
    theType = AfterTime;
  } else if(  wl[1] == "NEvents" ){
    theType = AfterNEvents;
  } else {
    G4Exception("GmMovementFromFile::GmMovementFromFile",
		"Wrong movement type",
		FatalErrorInArgument,
		G4String("Type can only be: Time or NEvents, while it is " + wl[1]).c_str());
  }

  theInterval = GmGenUtils::GetValue(wl[2]);
  if( wl.size() > 3 ) {
    theOffset = GmGenUtils::GetValue(wl[3]);
  } else { 
    theOffset = 0.;
  }
  if( wl.size() > 4 ) {
    theNIntervals = GmGenUtils::GetInteger(wl[4]);
  } else {
    theNIntervals = INT_MAX;
  }

  InitialiseLastValue();

  GmMovementMgr::GetInstance()->AddMovement(this);
}

//------------------------------------------------------------------------
void GmMovementFromFile::AddMovement(std::vector<G4String> wl )
{
#ifndef GAMOS_NO_VERBOSE
  if( MoveVerb(debugVerb) ) G4cout << " GmMovementFromFile::AddMovement " << G4endl;
#endif
  MovementFF mff;
  mff.DispValue =  theValue = GmGenUtils::GetValue(wl[0]);
  mff.DispAxis = G4ThreeVector(GmGenUtils::GetValue(wl[1]),GmGenUtils::GetValue(wl[2]),GmGenUtils::GetValue(wl[3]));
  mff.RotValue =  theValue = GmGenUtils::GetValue(wl[4]);
  mff.RotAxis = G4ThreeVector(GmGenUtils::GetValue(wl[5]),GmGenUtils::GetValue(wl[6]),GmGenUtils::GetValue(wl[7]));

#ifndef GAMOS_NO_VERBOSE
  if( MoveVerb(debugVerb) ) G4cout << " GmMovementFromFile::AddMovement " << mff.DispValue << " " << mff.DispAxis << " " << mff.RotValue << " " << mff.RotAxis << G4endl;
#endif

  theMovements.push_back(mff);
} 

//------------------------------------------------------------------------
G4bool GmMovementFromFile::Move()
{
  //  G4RunManager::GetRunManager()->AbortRun();
  G4GeometryManager* geomMgr = G4GeometryManager::GetInstance();

  std::vector<G4VPhysicalVolume*>::iterator itev;
  for(itev = theVolumes.begin(); itev != theVolumes.end(); itev++ ){
    geomMgr->OpenGeometry(*itev);

    std::vector<MovementFF>::const_iterator ite;
    for(ite = theMovements.begin(); ite != theMovements.end(); ite++ ){
      MovementFF mff = *ite;
      if( mff.DispValue != 0. ) {
	G4ThreeVector trans = (*itev)->GetTranslation();
	trans += mff.DispValue * mff.DispAxis;
	(*itev)->SetTranslation(trans);
      } 

      if( mff.RotValue != 0. ) { 
	G4RotationMatrix* rm = (*itev)->GetRotation();
	rm->rotate(mff.RotValue,mff.RotAxis);
	(*itev)->SetRotation(rm);
      }
    }

    geomMgr->CloseGeometry(true,false,*itev);
  }
  
  // Notify the VisManager as well
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) pVVisManager->GeometryHasChanged();

  return true;
}

//------------------------------------------------------------------------
GmMovementFromFile::~GmMovementFromFile() 
{
}
