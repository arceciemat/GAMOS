//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
#include "GmDataInitialUnitID.hh"

#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4TransportationManager.hh"

#include <sstream>

//----------------------------------------------------------------
GmDataInitialUnitID::GmDataInitialUnitID()
{
  bInitial = false;
  theExcludedTypes.clear(); // delete DTRun
}

//----------------------------------------------------------------
GmDataInitialUnitID::~GmDataInitialUnitID()
{
}


//----------------------------------------------------------------
G4String GmDataInitialUnitID::GetStringValueFromStep( const G4Step* aStep)
{

  G4TouchableHistory* touch = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  return GetStringID( touch );
}


//----------------------------------------------------------------
G4String GmDataInitialUnitID::GetStringValueFromTrack( const G4Track* aTrack )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 

  return GetStringID( touch );

}

//----------------------------------------------------------------
G4String GmDataInitialUnitID::GetStringValueFromSecoTrack(const G4Track* aTrack, const G4Track* )
{

  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 

  return GetStringID( touch );
  
}


//----------------------------------------------------------------
G4String GmDataInitialUnitID::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false );

  return GetStringID( touch );
}

//----------------------------------------------------------------
G4String GmDataInitialUnitID::GetStringValueFromStackedTrack( const G4Track* aTrack )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), touch, false ); 

  return GetStringID( touch );
}

//----------------------------------------------------------------
G4String GmDataInitialUnitID::GetStringID( G4TouchableHistory* touch )
{
  std::stringstream convert;
  for( int ii = 0; ii <= touch->GetHistoryDepth(); ii++ ){
    convert <<"_"<<touch->GetReplicaNumber(ii);
  }
  G4String stringID = convert.str();
//  G4cout<<touch->GetHistoryDepth()<<"  "<<stringID<<G4endl;
  return stringID;
}
