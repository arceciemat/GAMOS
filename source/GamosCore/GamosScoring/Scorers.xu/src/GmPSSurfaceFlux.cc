#include "GmPSSurfaceFlux.hh"
#include "GmScoringSurfaceBox.hh"
#include "GmScoringSurfaceTubs.hh"
#include "GmScoringSurfaceSphere.hh"
#include "GmScoringSurfaceOrb.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmPSSurfaceFlux::GmPSSurfaceFlux(G4String name)
 :GmVPrimitiveScorer(name)
{
  theUnit = 1.e-2;
  theUnitName = G4String("cm-2");
  
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();

  G4String direction = paramMgr->GetStringValue(primitiveName+":Direction","InOut");
  if( direction == "InOut" ) {
    theDirection = fFlux_InOut;
  } else if( direction == "In" ) {
    theDirection = fFlux_In;
  } else if( direction == "Out" ) {
    theDirection = fFlux_Out;
  } else {
    G4Exception("GmPSSurfaceFlux::GmPSSurfaceFlux",
		"Invalid direction supplied",
		FatalErrorInArgument,
		("Valid directions are  In / Out / InOut , you are using "+direction).c_str());
  }
  
  theSurfaces = paramMgr->GetVStringValue(primitiveName+":Surfaces",theSurfaces);
  if( theSurfaces.size() == 0 ) {
    G4Exception("GmPSSurfaceFlux::GmPSSurfaceFlux",
		"No surface type supplied",
		FatalErrorInArgument,
		("Please use /gamos/setParam "+primitiveName + ":Surfaces SURFACE_1 SURFACE_2 ... SURFACE_N").c_str());
    
   }

  bArea = G4bool(paramMgr->GetNumericValue(primitiveName+":DivideByArea",1));
  if( bArea ) {
    theUnit = CLHEP::mm*CLHEP::mm/(CLHEP::cm*CLHEP::cm);
    theUnitName = G4String("cm-2");
  }

  bAngleFactor = G4bool(paramMgr->GetNumericValue(primitiveName+":DivideByAngle",1));
  
}
  

//-----------------------------------------------------------------------
G4bool GmPSSurfaceFlux::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4VSolid* solid = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid();
  std::map<G4VSolid*, GmVScoringSurfaceSolid*>::const_iterator ite = theScoringSolids.find(solid);
  
  if( ite == theScoringSolids.end() ) {
    if( solid->GetEntityType() == "G4Box" ) {
      theScoringSolid = new GmScoringSurfaceBox(solid);
    } else if( solid->GetEntityType() == "G4Tubs" ) {
      theScoringSolid = new GmScoringSurfaceTubs(solid);
    } else if( solid->GetEntityType() == "G4Sphere" ) {
      theScoringSolid = new GmScoringSurfaceSphere(solid);
    } else if( solid->GetEntityType() == "G4Orb" ) {
      theScoringSolid = new GmScoringSurfaceOrb(solid);
    } else {
      G4Exception("GmPSSurfaceFlux::ProcessHits",
		  "Invalid solid type",
		  FatalErrorInArgument,
		  "Solid types can be BOX TUBS SPHERE ORB");
    }   
    theScoringSolids[solid] = theScoringSolid;
    theScoringSolid->SetSurfaces( theSurfaces );
    if( bArea ) theScoringSolid->ComputeArea();
  } else {
    theScoringSolid = (*ite).second;
  }
 

  // one step may enter and exit volume, so check boths
  if( theDirection == fFlux_InOut ) {
    G4bool bpre = ProcessHits( aStep->GetPreStepPoint(), aStep, fFlux_In ) ;
    G4bool bpost = ProcessHits( aStep->GetPostStepPoint(), aStep, fFlux_Out ) ;
    return (bpre || bpost);
      //ProcessHits( aStep->GetPreStepPoint(), aStep, fFlux_In ) 
      //	    || ProcessHits( aStep->GetPostStepPoint(), aStep, fFlux_Out ) );
  } else if( theDirection == fFlux_In ) {
    return ProcessHits( aStep->GetPreStepPoint(), aStep, fFlux_In );
  } else if( theDirection == fFlux_Out ) {
    return ProcessHits( aStep->GetPostStepPoint(), aStep, fFlux_Out );
  }

  return TRUE;

}

//-----------------------------------------------------------------------
G4bool GmPSSurfaceFlux::ProcessHits(G4StepPoint* aStepPoint, G4Step* aStep, G4int aDirFlag )
{
  G4int dirFlag = -1;
  G4bool bFlux = theScoringSolid->IsSelectedSurface( aStepPoint, aStep );
  if( bFlux ) dirFlag = aDirFlag;

  if ( dirFlag > 0 ) {
    if ( theDirection == fFlux_InOut || theDirection == dirFlag ){

      G4StepPoint* stepPoint=0; // step point to calculate angle with normal
      G4StepPoint* volumeStepPoint=0; // step point to know in which volume 
      if ( dirFlag == fFlux_In ){
	stepPoint = aStep->GetPreStepPoint();
	volumeStepPoint = stepPoint;
      }else if ( dirFlag == fFlux_Out ){
	stepPoint = aStep->GetPostStepPoint();
	volumeStepPoint = aStep->GetPreStepPoint();
      }else{
	return FALSE;
      }

      G4double flux  = 1;
      if( bArea ) {     
	G4double area = theScoringSolid->GetArea();
	flux /= area;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmPSSurfaceFlux::ProcessHits flux after area= " << flux << " area " << area << " flux*area " << flux*area << G4endl;
#endif     

      }
      if( bAngleFactor ) {
	G4double angleFactor = theScoringSolid->GetAngleFactor( stepPoint, volumeStepPoint ); // stepPoint for the direction, but aStep->GetPreStepPoint for the touchable
	flux /= angleFactor;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmPSSurfaceFlux::ProcessHits flux after angleFactor= " << flux << " angleFactor " << angleFactor << G4endl;
#endif     
      }
      
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPSSurfaceFlux::ProcessHits flux= " << flux << G4endl;
#endif     
      FillScorer( aStep, flux, stepPoint->GetWeight() );
    }
  }

  return TRUE;
}
