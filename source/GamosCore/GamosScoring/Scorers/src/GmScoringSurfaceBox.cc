#include "GmScoringSurfaceBox.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmScoringSurfaceBox::GmScoringSurfaceBox(G4VSolid* box)
  : GmVScoringSurfaceSolid()
{
  theBox = (G4Box*)box;

  theNSurfaceMax = 6;
}

//-----------------------------------------------------------------------
void GmScoringSurfaceBox::SetSurfaces( std::vector<G4String> surfaces ) 
{

  G4bool bFound = false;
  for( unsigned int ii = 0; ii < surfaces.size(); ii++ ){
    if( surfaces[ii] == "X+" 
	|| surfaces[ii] == "X-" 
	|| surfaces[ii] == "Y+" 
	|| surfaces[ii] == "Y-" 
	|| surfaces[ii] == "Z+" 
	|| surfaces[ii] == "Z-" 
	) {
      theSurfaces.insert( surfaces[ii] );
      bFound = true;
    }
  }
  
  if( !bFound ) {
    G4Exception("GmScoringSurfaceBox::SetSurfaces",
		"None of the surfaces is valid",
		FatalErrorInArgument,
		"Available types are: X+ X- Y+ Y- Z+ Z-");
  }

}

//-----------------------------------------------------------------------
void GmScoringSurfaceBox::ComputeArea()
{
  theArea = 0.;
  if( theSurfaces.find( "X+" ) != theSurfaces.end() ) {
    theArea += 4 * theBox->GetYHalfLength() * theBox->GetZHalfLength();
  }
  
  if( theSurfaces.find( "X-" ) != theSurfaces.end() ) {
    theArea += 4 * theBox->GetYHalfLength() * theBox->GetZHalfLength();
  }
  
  if( theSurfaces.find( "Y+" ) != theSurfaces.end() ) {
    theArea += 4 * theBox->GetXHalfLength() * theBox->GetZHalfLength();
  }
  
  if( theSurfaces.find( "Y-" ) != theSurfaces.end() ) {
    theArea += 4 * theBox->GetXHalfLength() * theBox->GetZHalfLength();
  }
  
  if( theSurfaces.find( "Z+" ) != theSurfaces.end() ) {
    theArea += 4 * theBox->GetXHalfLength() * theBox->GetYHalfLength();
  }
  
  if( theSurfaces.find( "Z-" ) != theSurfaces.end() ) {
    theArea += 4 * theBox->GetXHalfLength() * theBox->GetYHalfLength();
  }
  
}

 
//-----------------------------------------------------------------------
G4bool GmScoringSurfaceBox::IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep )
{
  if (stepPoint->GetStepStatus() == fGeomBoundary ){
    if( theSurfaces.find( "X+" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurface( GetLocalPoint(stepPoint->GetPosition(),aStep).x(), theBox->GetXHalfLength() );  
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::IsSelectedSurface X+ TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(1.,0.,0.);
	return true;
      }
    }

    if( theSurfaces.find( "X-" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurface( GetLocalPoint(stepPoint->GetPosition(),aStep).x(), -theBox->GetXHalfLength() );  
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::IsSelectedSurface X- TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(-1.,0.,0.);
	return true;
      }
    }

    if( theSurfaces.find( "Y+" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurface( GetLocalPoint(stepPoint->GetPosition(),aStep).y(), theBox->GetYHalfLength() );  
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::IsSelectedSurface Y+ TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(0.,1.,0.);
	return true;
      }
    }

    if( theSurfaces.find( "Y-" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurface( GetLocalPoint(stepPoint->GetPosition(),aStep).y(), -theBox->GetYHalfLength() );  
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::IsSelectedSurface Y- TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(0.,-1.,0.);
	return true;
      }
    }

    if( theSurfaces.find( "Z+" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurface( GetLocalPoint(stepPoint->GetPosition(),aStep).z(), theBox->GetZHalfLength() );  
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::IsSelectedSurface Z+ TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(0.,0.,1.);
	return true;
      }
    }

    if( theSurfaces.find( "Z-" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurface( GetLocalPoint(stepPoint->GetPosition(),aStep).z(), -theBox->GetZHalfLength() );  
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::IsSelectedSurface Z- TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(0.,0.,-1.);
	return true;
      }
    }

  }

  return false;
}


//-----------------------------------------------------------------------
G4bool GmScoringSurfaceBox::CheckSurface(G4double localPos, G4double dim )
{ 
  
  if(std::fabs( localPos - dim ) < theSurfaceTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::CheckSurface TRUE " << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::CheckSurface FALSE  localPos= " << localPos << " <> dim= " << dim << G4endl;
#endif    
    return false;
  }
}


//-----------------------------------------------------------------------
G4double GmScoringSurfaceBox::GetAngleFactor( G4StepPoint* stepPoint )
{
  G4TouchableHandle touchable = stepPoint->GetTouchableHandle();
  G4ThreeVector dir = stepPoint->GetMomentumDirection();
  G4ThreeVector localDir  = 
    touchable->GetHistory()->GetTopTransform().TransformAxis(dir).unit();

  G4double angleFactor = localDir*theNormal;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceBox::GetAngleFactor " << angleFactor << " localDir " << localDir << " dir " << dir << " normal " << theNormal << G4endl;
#endif
  return angleFactor;
}
