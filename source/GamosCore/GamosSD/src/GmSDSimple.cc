#include "GmSDSimple.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Para.hh"

//-------------------------------------------------------------------
GmSDSimple::GmSDSimple(G4String sdtype): GmVSD( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSimple: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

  G4String posType = GmParameterMgr::GetInstance()->GetStringValue("SD:PosType:"+theSDType,"centre");
  if( posType == "centre" ) {
    theSDPosType = SDPOS_Centre;
  } else if( posType == "upper" ) {
    theSDPosType = SDPOS_Upper;
  } else if( posType == "lower" ) {
    theSDPosType = SDPOS_Lower;
  } else {
    G4Exception("GmSDSimple::GmSDSimple",
		"",
		FatalErrorInArgument,
		(G4String("Error in parameter SD:SimplePosType") + theSDType +  " it must be 'centre', 'upper' or 'lower', while it is " + posType).c_str());
  }

}

//-------------------------------------------------------------------
GmSDSimple::~GmSDSimple()
{
}


//-------------------------------------------------------------------
unsigned long long GmSDSimple::GetDetUnitID( G4Step* aStep )
{
  return theDetUnitIDBuilder->BuildID( aStep );

}

//-------------------------------------------------------------------
void GmSDSimple::CalculateAndSetPosition( GmHit* hit, G4Step* aStep )
{
  //-  return;

  const G4VTouchable*  touch= aStep->GetPreStepPoint()->GetTouchable();
  G4AffineTransform trans = touch->GetHistory()->GetTopTransform().Inverse();

  G4ThreeVector localPos;
  if( theSDPosType == SDPOS_Centre ) {
    localPos = G4ThreeVector(0.,0.,0.);
  } else if( theSDPosType == SDPOS_Upper ) {
    G4VSolid* solid = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid();
    G4String solidType = solid->GetEntityType();
    if( solidType == "G4Box" ) {
      G4Box* box = static_cast<G4Box*>(solid);
      localPos = G4ThreeVector(0.,0.,box->GetZHalfLength());
    } else if( solidType == "G4Tubs" ) {
      G4Tubs* tubs = static_cast<G4Tubs*>(solid);
      localPos = G4ThreeVector(0.,0.,tubs->GetZHalfLength());
    } else if( solidType == "G4Cons" ) {
      G4Cons* cons = static_cast<G4Cons*>(solid);
      localPos = G4ThreeVector(0.,0.,cons->GetZHalfLength());
    } else if( solidType == "G4Trd" ) {
      G4Trd* trd = static_cast<G4Trd*>(solid);
      localPos = G4ThreeVector(0.,0.,trd->GetZHalfLength());
    } else if( solidType == "G4Trap" ) {
      G4Trap* trap = static_cast<G4Trap*>(solid);
      localPos = G4ThreeVector(0.,0.,trap->GetZHalfLength());
    } else if( solidType == "G4Para" ) {
      G4Para* para = static_cast<G4Para*>(solid);
      localPos = G4ThreeVector(0.,0.,para->GetZHalfLength());  
    } else {
      G4Exception("GmSDSimple::CalculateAndSetPosition",
		  "",
		  FatalException,
		  (G4String("Type of solid: ")+solid->GetName() + " is not G4Box, G4Tubs, G4Cons, G4Trd, G4Trap, G4Para, but: " + solid->GetEntityType()).c_str());
    }
  } else if( theSDPosType == SDPOS_Lower ) {
    G4VSolid* solid = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid();
    G4String solidType = solid->GetEntityType();
    if( solidType == "G4Box" ) {
      G4Box* box = static_cast<G4Box*>(solid);
      localPos = G4ThreeVector(0.,0.,-box->GetZHalfLength());
    } else if( solidType == "G4Tubs" ) {
      G4Tubs* tubs = static_cast<G4Tubs*>(solid);
      localPos = G4ThreeVector(0.,0.,-tubs->GetZHalfLength());
    } else if( solidType == "G4Cons" ) {
      G4Cons* cons = static_cast<G4Cons*>(solid);
      localPos = G4ThreeVector(0.,0.,-cons->GetZHalfLength());
    } else if( solidType == "G4Trd" ) {
      G4Trd* trd = static_cast<G4Trd*>(solid);
      localPos = G4ThreeVector(0.,0.,-trd->GetZHalfLength());
    } else if( solidType == "G4Trap" ) {
      G4Trap* trap = static_cast<G4Trap*>(solid);
      localPos = G4ThreeVector(0.,0.,-trap->GetZHalfLength());
    } else if( solidType == "G4Para" ) {
      G4Para* para = static_cast<G4Para*>(solid);
      localPos = G4ThreeVector(0.,0.,-para->GetZHalfLength());  
    } else {
      G4Exception("GmSDSimple::CalculateAndSetPosition",
		  "",
		  FatalException,
		  (G4String("Type of solid: ")+solid->GetName() + " is not G4Box, G4Tubs, G4Cons, G4Trd, G4Trap, G4Para, but: " + solid->GetEntityType()).c_str());
    }
  }

  G4ThreeVector centreGlobal = trans.TransformPoint( localPos );

  hit->SetPosition( centreGlobal );
  //  G4cout << " GmSDSimple::CalculateAndSetPosition pos " << 
} 


//-------------------------------------------------------------------
void GmSDSimple::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSimple::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
