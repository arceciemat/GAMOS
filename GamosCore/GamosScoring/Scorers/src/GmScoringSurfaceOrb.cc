#define _USE_MATH_DEFINES
#include <cmath>

#include "GmScoringSurfaceOrb.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmScoringSurfaceOrb::GmScoringSurfaceOrb(G4VSolid* orb)
  : GmVScoringSurfaceSolid()
{
  theOrb = (G4Orb*)orb;

  theNSurfaceMax = 1;
}

 
//-----------------------------------------------------------------------
void GmScoringSurfaceOrb::SetSurfaces( std::vector<G4String> surfaces ) 
{

  G4bool bFound = false;
  for( unsigned int ii = 0; ii < surfaces.size(); ii++ ){
    if( surfaces[ii] == "OUTER" ) {
      bFound = true;
      theSurfaces.insert( surfaces[ii] );
    }
  }

  if( !bFound ) {
    G4Exception("GmScoringSurfaceOrb::SetSurfaces",
		"None of the surfaces is valid",
		FatalErrorInArgument,
		"Available types are: OUTER ");
  }

}


//-----------------------------------------------------------------------
void GmScoringSurfaceOrb::ComputeArea()
{
  theArea = theOrb->GetRadius()*theOrb->GetRadius() * 4 * M_PI;

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceOrb::ComputeArea " << theArea << G4endl;
#endif

}


//-----------------------------------------------------------------------
G4bool GmScoringSurfaceOrb::IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep )
{
  if (stepPoint->GetStepStatus() == fGeomBoundary ){
    G4ThreeVector localPos = GetLocalPoint(stepPoint->GetPosition(),aStep);
    G4bool bInSurface = CheckSurface( localPos.mag(), theOrb->GetRadius() );
    if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceOrb::IsSelectedSurface OUTER TRUE " << G4endl;
#endif    
      theNormal = localPos.unit();
      return true;
    }

    if( G4int(theSurfaces.size()) == theNSurfaceMax ) {
      G4Exception("GmScoringSurfaceOrb::IsSelectedSurface",
		  "Area (and therefore flux) will be wrongly calculated! Step is in one surface not identified",
		  JustWarning,
		  "It is probably due a volume placed inside the scoring volume");
    }
    
  }
  
  return false;
}


//-----------------------------------------------------------------------
G4bool GmScoringSurfaceOrb::CheckSurface(G4double localPos, G4double radius )
{ 
  
  if(std::fabs( localPos - radius ) < theSurfaceTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceOrb::CheckSurface TRUE " << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceOrb::CheckSurface FALSE  localPos= " << localPos << " <> radius= " << radius << G4endl;
#endif    
    return false;
  }
}

//-----------------------------------------------------------------------
G4bool GmScoringSurfaceOrb::CheckSurfaceAngle(G4double localAng, G4double angle )
{ 
  
  if(std::fabs( localAng - angle ) < theSurfaceTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceOrb::CheckSurface TRUE " << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceOrb::CheckSurface FALSE  localAng= " << localAng << " <> angle= " << angle << G4endl;
#endif    
    return false;
  }
}


//-----------------------------------------------------------------------
G4double GmScoringSurfaceOrb::GetAngleFactor( G4StepPoint* stepPoint )
{
  G4TouchableHandle touchable = stepPoint->GetTouchableHandle();
  G4ThreeVector dir = stepPoint->GetMomentumDirection();
  G4ThreeVector localDir  = 
    touchable->GetHistory()->GetTopTransform().TransformAxis(dir).unit();
  G4ThreeVector pos= stepPoint->GetPosition();
  G4double angleFactor = localDir*theNormal;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceOrb::GetAngleFactor " << angleFactor << " localDir " << localDir << " normal " << theNormal << " Dir " << dir << " Pos " << pos << G4endl;
#endif
  return angleFactor;
}


