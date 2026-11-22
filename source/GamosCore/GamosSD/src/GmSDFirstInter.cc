#include "GmSDFirstInter.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "CLHEP/Random/RandGauss.h"

//--------------------------------------------------------------------
GmSDFirstInter::GmSDFirstInter(const char* sdtype): GmVSD( sdtype )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDFirstInter: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

}

GmSDFirstInter::GmSDFirstInter(G4String sdtype): GmVSD( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDFirstInter: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

}

//--------------------------------------------------------------------
GmSDFirstInter::~GmSDFirstInter(){}

//--------------------------------------------------------------------
unsigned long long GmSDFirstInter::GetDetUnitID( G4Step* aStep )
{
  return theDetUnitIDBuilder->BuildID( aStep );

}

//--------------------------------------------------------------------
void GmSDFirstInter::UpdateHit( GmHit* hit, G4Step* aStep  )
{
  G4double energy = GetEnergyInSD( aStep );

  //keep first position
  /*//--- Set position as barycenter
    G4ThreeVector posold;
  if( GmParameterMgr::GetInstance()->GetNumericValue("GmHit:LocalHitCoordinates",0) ) {
    posold = hit->GetPosition();
  } else {
    posold = (G4TransportationManager::GetTransportationManager()->
	      GetNavigatorForTracking()->
	      GetLocalToGlobalTransform()).TransformPoint(posold);
  }
  G4ThreeVector posnew = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector pos = (posold*enerold+posnew*enernew)/(enerold+enernew);
   hit->SetPosition( pos );
  */

#ifndef GAMOS_NO_VERBOSE
  G4double enerold = hit->GetEnergy();
  G4double enernew = energy;
  if( SDVerb(debugVerb) ) G4cout << hit->GetDetUnitID() << " position " << hit->GetPosition() << " enerold " << enerold << " new " << enernew << G4endl;
#endif
  hit->UpdateMe( aStep, energy );
}

//-------------------------------------------------------------------
void GmSDFirstInter::CalculateAndSetPosition( GmHit* , G4Step* )
{
  return;
  //  G4cout << " GmSDFirstInter::CalculateAndSetPosition pos " << 
} 


//-------------------------------------------------------------------
void GmSDFirstInter::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDFirstInter::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
