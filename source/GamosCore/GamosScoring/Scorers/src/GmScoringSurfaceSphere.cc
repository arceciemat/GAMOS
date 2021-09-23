#include "GmScoringSurfaceSphere.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmScoringSurfaceSphere::GmScoringSurfaceSphere(G4VSolid* sphere)
  : GmVScoringSurfaceSolid()
{
  theSphere = (G4Sphere*)sphere;

  theNSurfaceMax = 2;
  if( std::fabs(theSphere->GetDeltaThetaAngle() - 180.*CLHEP::deg) < theAngularTolerance ){
    theNSurfaceMax++;
  }
  if( std::fabs(theSphere->GetDeltaPhiAngle() - 360.*CLHEP::deg) < theAngularTolerance ){
    theNSurfaceMax++;
  }
}

 
//-----------------------------------------------------------------------
void GmScoringSurfaceSphere::SetSurfaces( std::vector<G4String> surfaces ) 
{

  G4bool bFound = false;
  for( unsigned int ii = 0; ii < surfaces.size(); ii++ ){
    if( surfaces[ii] == "INNER" 
	|| surfaces[ii] == "OUTER"
	|| surfaces[ii] == "THETA" 
	|| surfaces[ii] == "PHI" ) {
      bFound = true;
      theSurfaces.insert( surfaces[ii] );
    }
  }

  if( !bFound ) {
    G4Exception("GmScoringSurfaceSphere::SetSurfaces",
		"None of the surfaces is valid",
		FatalErrorInArgument,
		"Available types are: INNER OUTER THETA PHI");
  }

}


//-----------------------------------------------------------------------
void GmScoringSurfaceSphere::ComputeArea()
{
  theArea = 0.;
  if( theSurfaces.find( "INNER" ) != theSurfaces.end() ) {
    theArea += theSphere->GetInnerRadius()*theSphere->GetInnerRadius() *
      theSphere->GetDeltaPhiAngle() * 
      ( -std::cos(theSphere->GetStartThetaAngle()+theSphere->GetDeltaThetaAngle()) 
	+ std::cos(theSphere->GetStartThetaAngle()) );
  }

  if( theSurfaces.find( "OUTER" ) != theSurfaces.end() ) {
    theArea += theSphere->GetOuterRadius()*theSphere->GetOuterRadius() *
      theSphere->GetDeltaPhiAngle() * 
      ( -std::cos(theSphere->GetStartThetaAngle()+theSphere->GetDeltaThetaAngle()) 
	+ std::cos(theSphere->GetStartThetaAngle()) );
  }

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::ComputeArea " << theArea << G4endl;
#endif

}


//-----------------------------------------------------------------------
G4bool GmScoringSurfaceSphere::IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep )
{
  if (stepPoint->GetStepStatus() == fGeomBoundary ){
    if( theSurfaces.find( "INNER" ) != theSurfaces.end() ) {
      G4ThreeVector localPos = GetLocalPoint(stepPoint->GetPosition(),aStep);
      G4bool bInSurface = CheckSurface( localPos.mag(), theSphere->GetInnerRadius() );  
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::IsSelectedSurface INNER TRUE " << G4endl;
#endif    
	theNormal = -localPos.unit();
	return true;
      }
    }

    if( theSurfaces.find( "OUTER" ) != theSurfaces.end() ) {
      G4ThreeVector localPos = GetLocalPoint(stepPoint->GetPosition(),aStep);
      G4bool bInSurface = CheckSurface( localPos.mag(), theSphere->GetOuterRadius() );
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::IsSelectedSurface OUTER TRUE " << G4endl;
#endif    
	theNormal = localPos.unit();
	return true;
      }
    }

    if( theSurfaces.find( "THETA" ) != theSurfaces.end() ) {
      G4ThreeVector localPos = GetLocalPoint(stepPoint->GetPosition(),aStep);
      G4double theta = localPos.theta();
      G4bool bInSurface = CheckSurfaceAngle( theta, theSphere->GetStartThetaAngle() ) 
	|| CheckSurfaceAngle( theta, theSphere->GetStartThetaAngle() + theSphere->GetDeltaThetaAngle() );
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::IsSelectedSurface THETA TRUE " << G4endl;
#endif    
	G4double phi = localPos.phi();
	if( std::fabs(theSphere->GetStartThetaAngle() - theta ) < theAngularTolerance ) {
	  theNormal = G4ThreeVector(-cos(theta)*cos(phi),cos(theta)*sin(phi) ,sin(theta));
	} else {
	  theNormal = G4ThreeVector(cos(theta)*cos(phi),-cos(theta)*sin(phi) ,sin(theta));
	}
	return true;
      }
    }
    
    if( theSurfaces.find( "PHI" ) != theSurfaces.end() ) {
      G4ThreeVector localPos = GetLocalPoint(stepPoint->GetPosition(),aStep);
      G4double phi = localPos.phi();
      G4bool bInSurface = CheckSurfaceAngle( phi, theSphere->GetStartPhiAngle() ) 
	|| CheckSurfaceAngle( phi, theSphere->GetStartPhiAngle() + theSphere->GetDeltaPhiAngle() );
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::IsSelectedSurface PHI TRUE " << G4endl;
#endif    
	G4double theta = localPos.theta();
	if( std::fabs(theSphere->GetStartPhiAngle() - phi ) < theAngularTolerance ) {
	  theNormal = G4ThreeVector(sin(theta)*sin(phi),sin(theta)*cos(phi) ,-cos(theta));
	} else {
	  theNormal = G4ThreeVector(-sin(theta)*sin(phi),-sin(theta)*cos(phi) ,cos(theta));
	}
	return true;
      }
    }

    if( G4int(theSurfaces.size()) == theNSurfaceMax ) {
      G4Exception("GmScoringSurfaceSphere::IsSelectedSurface",
		  "Area (and therefore flux) will be wrongly calculated! Step is in one surface not identified",
		  JustWarning,
		  "It is probably due a volume placed inside the scoring volume");
    }
 
  }

  return false;
}


//-----------------------------------------------------------------------
G4bool GmScoringSurfaceSphere::CheckSurface(G4double localPos, G4double radius )
{ 
  
  if(std::fabs( localPos - radius ) < theSurfaceTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::CheckSurface TRUE " << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::CheckSurface FALSE  localPos= " << localPos << " <> radius= " << radius << G4endl;
#endif    
    return false;
  }
}

//-----------------------------------------------------------------------
G4bool GmScoringSurfaceSphere::CheckSurfaceAngle(G4double localAng, G4double angle )
{ 
  
  if(std::fabs( localAng - angle ) < theSurfaceTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::CheckSurface TRUE " << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::CheckSurface FALSE  localAng= " << localAng << " <> angle= " << angle << G4endl;
#endif    
    return false;
  }
}


//-----------------------------------------------------------------------
G4double GmScoringSurfaceSphere::GetAngleFactor( G4StepPoint* stepPoint, G4StepPoint* volumeStepPoint )
{
  G4TouchableHandle touchable = volumeStepPoint->GetTouchableHandle();
  G4ThreeVector dir = stepPoint->GetMomentumDirection();
  G4ThreeVector localDir = touchable->GetHistory()->GetTopTransform().TransformAxis(dir).unit();
  G4ThreeVector pos= stepPoint->GetPosition();
  G4double angleFactor = localDir*theNormal;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceSphere::GetAngleFactor " << angleFactor << " localDir " << localDir << " normal " << theNormal << " Dir " << dir << " Pos " << pos << G4endl;
#endif
  return angleFactor;
}


