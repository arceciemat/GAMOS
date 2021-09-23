#include "GmSDSimpleSmearDOI.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Box.hh"

#include "CLHEP/Random/RandGauss.h"

//--------------------------------------------------------------------
GmSDSimpleSmearDOI::GmSDSimpleSmearDOI(const char* sdtype): GmVSD( sdtype )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimpleSmearDOI: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

  theDOIResol = GmParameterMgr::GetInstance()->GetNumericValue("SD:GmSDSimpleSmearDOI:DOIResol:"+theSDType,0.); 
}

GmSDSimpleSmearDOI::GmSDSimpleSmearDOI(G4String sdtype): GmVSD( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimpleSmearDOI: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

  theDOIResol = GmParameterMgr::GetInstance()->GetNumericValue("SD:GmSDSimpleSmearDOI:DOIResol:"+theSDType,0.); 

}

//--------------------------------------------------------------------
GmSDSimpleSmearDOI::~GmSDSimpleSmearDOI(){}

//--------------------------------------------------------------------
unsigned long long GmSDSimpleSmearDOI::GetDetUnitID( G4Step* aStep )
{
  return theDetUnitIDBuilder->BuildID( aStep );

}

//--------------------------------------------------------------------
void GmSDSimpleSmearDOI::UpdateHit( GmHit* hit, G4Step* aStep  )
{
  G4double energy = GetEnergyInSD( aStep );

  //--- Set position as barycenter
  G4ThreeVector posold;
  const G4VTouchable*  touch= aStep->GetPreStepPoint()->GetTouchable();
  G4AffineTransform transInv = touch->GetHistory()->GetTopTransform().Inverse();
  G4AffineTransform trans = touch->GetHistory()->GetTopTransform();

  G4ThreeVector globalPosOld = hit->GetPosition();
  G4ThreeVector localPosOld = trans.TransformPoint( globalPosOld );
  //  G4cout << " GmSDSimpleSmearDOI::UpdateHit globalPosOld=" << globalPosOld << " localPosOld=" << localPosOld << G4endl; //GDEB
  // Undo smearing of hit, because smearing is done after barycentre is calculated
  localPosOld -= G4ThreeVector(0.,0.,theLastSmearing[hit]);
  //  G4cout << " GmSDSimpleSmearDOI::UpdateHit undoSmearing localPosOld=" << localPosOld << G4endl; //GDEB

  G4ThreeVector globalPosNew = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector localPosNew = trans.TransformPoint( globalPosNew );
  //  G4cout << " GmSDSimpleSmearDOI::UpdateHit globalPosNew=" << globalPosNew << " localPosNew=" << localPosNew << G4endl; //GDEB

  G4double enerold = hit->GetEnergy();
  G4double enernew = energy;
  G4ThreeVector localPosTemp = (localPosOld*enerold+localPosNew*enernew)/(enerold+enernew);
  //  G4cout << " GmSDSimpleSmearDOI::UpdateHit new localPos(Barycentre)= " << localPosTemp << G4endl; //GDEB
  G4VSolid* solid = touch->GetSolid();
  G4Box* solidBox = dynamic_cast<G4Box*>(solid);
  if( solidBox == 0 ) {
    G4Exception("GmSDSimpleSmearDOI::UpdateHit",
		"",
		FatalException,
		("Detector solid type must be G4Box, while it is "+solid->GetEntityType()).c_str());
  }
  int ii = 0;
  G4ThreeVector localPos;
  G4double ran;
  for( ; ii < 10000; ii++ ) {
    ran = CLHEP::RandGauss::shoot(0., theDOIResol );
    localPos = G4ThreeVector(0.,0.,localPosTemp.z()+ran);
  //--- check localPos is inside detector Z
    if( localPos.z() < solidBox->GetZHalfLength() && localPos.z() > -solidBox->GetZHalfLength() ) {
      break;
    }
  }
  if( ii > 10000 ) {
    G4Exception("GmSDSimpleSmearDOI::UpdateHit ",
		"",
		JustWarning,
		("More than 10000 iterations and resulting Zposition is always outside SD: "
		 +GmGenUtils::ftoa(localPosTemp.z())).c_str());
    localPos = localPosTemp;
  }

  //  G4cout << " GmSDSimpleSmearDOI::UpdateHit localPos(Barycentre)_smeared=" << localPos << G4endl; //GDEB
  G4ThreeVector globalPos = transInv.TransformPoint( localPos );
  
  hit->SetPosition( globalPos );
  theLastSmearing[hit] = ran;

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmSimpleSmearDOI::UpdateHit " << hit->GetDetUnitID() << " position old " << globalPosOld << " new " << globalPosNew << " FINAL " << globalPos << " enerold " << enerold << " new " << enernew << G4endl;
#endif
  hit->UpdateMe( aStep, energy );
}

//-------------------------------------------------------------------
void GmSDSimpleSmearDOI::CalculateAndSetPosition( GmHit* hit, G4Step* aStep)
{
  const G4VTouchable*  touch= aStep->GetPreStepPoint()->GetTouchable();
  G4AffineTransform transInv = touch->GetHistory()->GetTopTransform().Inverse();
  G4AffineTransform trans = touch->GetHistory()->GetTopTransform();

  G4ThreeVector globalPos = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector localPosTemp = trans.TransformPoint( globalPos );
  //  G4cout << " GmSDSimpleSmearDOI::CalculateAndSetPosition globalPos=" << globalPos << " localPos=" << localPos << G4endl; //GDEB
  //--- check localPos is inside detector Z
  G4VSolid* solid = touch->GetSolid();
  G4Box* solidBox = dynamic_cast<G4Box*>(solid);
  if( solidBox == 0 ) {
    G4Exception("GmSDSimpleSmearDOI::CalculateAndSetPosition",
		"",
		FatalException,
		("Detector solid type must be G4Box, while it is "+solid->GetEntityType()).c_str());
  }
  int ii = 0;
  G4ThreeVector localPos;
  G4double ran;
  for( ; ii < 10000; ii++ ) {
    ran = CLHEP::RandGauss::shoot(0., theDOIResol );
    localPos = G4ThreeVector(0.,0.,localPosTemp.z()+ran);
  //--- check localPos is inside detector Z
    if( localPos.z() < solidBox->GetZHalfLength() && localPos.z() > -solidBox->GetZHalfLength() ) {
      break;
    }
  }
  if( ii > 10000 ) {
    G4Exception("GmSDSimpleSmearDOI::UpdateHit ",
		"",
		JustWarning,
		("More than 10000 iterations and resulting Zposition is always outside SD: "
		 +GmGenUtils::ftoa(localPosTemp.z())).c_str());
    localPos = localPosTemp;
  }

  //  G4cout << " GmSDSimpleSmearDOI::CalculateAndSetPosition localPos_Smeared=" << localPos << " ran= " << ran << " theDOIResol=" << theDOIResol << G4endl; //GDEB
  globalPos = transInv.TransformPoint( localPos );
  //  G4cout << " GmSDSimpleSmearDOI::CalculateAndSetPosition globalPos_FINAL=" << globalPos << G4endl; //GDEB

  hit->SetPosition( globalPos );
  theLastSmearing[hit] = ran;
  //  G4cout << " GmSDSimpleSmearDOI::CalculateAndSetPosition pos " <<
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmSimpleSmearDOI::CalculateAndSetPositon " << hit->GetDetUnitID() << " position old " << aStep->GetPostStepPoint()->GetPosition() << " FINAL " << globalPos << G4endl;
#endif
} 


//-------------------------------------------------------------------
void GmSDSimpleSmearDOI::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSimpleSmearDOI::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
