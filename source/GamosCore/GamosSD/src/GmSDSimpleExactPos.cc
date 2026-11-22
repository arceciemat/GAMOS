#include "GmSDSimpleExactPos.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "CLHEP/Random/RandGauss.h"

//--------------------------------------------------------------------
GmSDSimpleExactPos::GmSDSimpleExactPos(const char* sdtype): GmVSD( sdtype )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimpleExactPos: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

}

GmSDSimpleExactPos::GmSDSimpleExactPos(G4String sdtype): GmVSD( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimpleExactPos: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

}

//--------------------------------------------------------------------
GmSDSimpleExactPos::~GmSDSimpleExactPos(){}

//--------------------------------------------------------------------
unsigned long long GmSDSimpleExactPos::GetDetUnitID( G4Step* aStep )
{
  return theDetUnitIDBuilder->BuildID( aStep );

}

//--------------------------------------------------------------------
void GmSDSimpleExactPos::UpdateHit( GmHit* hit, G4Step* aStep  )
{
  G4double energy = GetEnergyInSD( aStep );

  //--- Set position as barycenter
  G4ThreeVector posold = hit->GetPosition();
  if( GmParameterMgr::GetInstance()->GetNumericValue("GmHit:LocalHitCoordinates",0) == 1 ) {
    posold = (G4TransportationManager::GetTransportationManager()->
	      GetNavigatorForTracking()->
	      GetLocalToGlobalTransform()).TransformPoint(posold);
  }
  G4ThreeVector posnew = aStep->GetPostStepPoint()->GetPosition();
  G4double enerold = hit->GetEnergy();
  G4double enernew = energy;
  G4ThreeVector pos = (posold*enerold+posnew*enernew)/(enerold+enernew);
  hit->SetPosition( pos );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << hit->GetDetUnitID() << " position old " << posold << " new " << posnew << " FINAL " << pos << " enerold " << enerold << " new " << enernew << G4endl;
#endif
  hit->UpdateMe( aStep, energy );
}

//-------------------------------------------------------------------
void GmSDSimpleExactPos::CalculateAndSetPosition( GmHit* , G4Step* )
{
  return;
  //  G4cout << " GmSDSimpleExactPos::CalculateAndSetPosition pos " << 
} 


//-------------------------------------------------------------------
void GmSDSimpleExactPos::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSimpleExactPos::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
