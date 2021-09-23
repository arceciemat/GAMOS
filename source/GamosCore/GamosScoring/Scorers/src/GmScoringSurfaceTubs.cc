#include "GmScoringSurfaceTubs.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmScoringSurfaceTubs::GmScoringSurfaceTubs(G4VSolid* tubs )
  : GmVScoringSurfaceSolid()
{

  theTubs = (G4Tubs*)(tubs);

  theNSurfaceMax = 4;
  if( std::fabs(theTubs->GetDeltaPhiAngle() - 360.*CLHEP::deg) < theAngularTolerance ){
    theNSurfaceMax++;
  }

}

 
//-----------------------------------------------------------------------
void GmScoringSurfaceTubs::SetSurfaces( std::vector<G4String> surfaces ) 
{

  for( unsigned int ii = 0; ii < surfaces.size(); ii++ ){
    G4bool bFound = false;
    if( surfaces[ii] == "UP"
	|| surfaces[ii] == "DOWN" 
	|| surfaces[ii] == "INNER"
	|| surfaces[ii] == "OUTER" 
	|| surfaces[ii] == "PHI" ) {
      bFound = true;
      theSurfaces.insert( surfaces[ii] );
    }

    if( !bFound ) {
      G4Exception("GmScoringSurfaceTubs::SetSurfaces",
		  "None of the surfaces is valid",
		  FatalErrorInArgument,
		  G4String("Available types are: UP DOWN INNER OUTER PHI, you are using "+surfaces[ii]).c_str());
    }

  }


}

//-----------------------------------------------------------------------
void GmScoringSurfaceTubs::ComputeArea()
{
  theArea = 0.;
  if( theSurfaces.find( "INNER" ) != theSurfaces.end() ) {
    theArea += 2*theTubs->GetZHalfLength() * theTubs->GetInnerRadius() * theTubs->GetDeltaPhiAngle();
    //    G4cout << " AREA INNER " <<  theArea << " += " << 2*theTubs->GetZHalfLength() << " * " << theTubs->GetInnerRadius() << " * " << theTubs->GetDeltaPhiAngle() << G4endl; //GDEB
  }
  
  if( theSurfaces.find( "OUTER" ) != theSurfaces.end() ) {
    theArea += 2*theTubs->GetZHalfLength() * theTubs->GetOuterRadius() * theTubs->GetDeltaPhiAngle();
  }

  if( theSurfaces.find( "UP" ) != theSurfaces.end() ) {
    theArea +=  theTubs->GetDeltaPhiAngle()*0.5*(pow(theTubs->GetOuterRadius(),2) - pow(theTubs->GetInnerRadius(),2));
  }

  if( theSurfaces.find( "DOWN" ) != theSurfaces.end() ) {
    theArea +=  theTubs->GetDeltaPhiAngle()*0.5*(pow(theTubs->GetOuterRadius(),2) - pow(theTubs->GetInnerRadius(),2));
    //    G4cout << " AREA DOWN " <<  theArea << " += " << pow(theTubs->GetOuterRadius(),2) << " - " << pow(theTubs->GetInnerRadius(),2) << " * " << theTubs->GetDeltaPhiAngle() << G4endl; //GDEB
  }

  if( theSurfaces.find( "PHI" ) != theSurfaces.end() ) {
    if( std::fabs(theTubs->GetDeltaPhiAngle() - 360.*CLHEP::deg) < G4GeometryTolerance::GetInstance()->GetAngularTolerance() ) {
      theArea += (theTubs->GetOuterRadius() - theTubs->GetInnerRadius()) * theTubs->GetZHalfLength()*2*2;
    }
  }

}


//-----------------------------------------------------------------------
G4bool GmScoringSurfaceTubs::IsSelectedSurface(G4StepPoint* stepPoint, G4Step* aStep )
{
  if (stepPoint->GetStepStatus() == fGeomBoundary ){
    if( theSurfaces.find( "INNER" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurfaceRadius( GetLocalPoint(stepPoint->GetPosition(), aStep ), theTubs->GetInnerRadius() );
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::IsSelectedSurface INNER TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(0.,0.,0.);
	return true;
      }
    }

    if( theSurfaces.find( "OUTER" ) != theSurfaces.end() ) {
      G4bool bInSurface = CheckSurfaceRadius( GetLocalPoint(stepPoint->GetPosition(), aStep), theTubs->GetOuterRadius() );
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::IsSelectedSurface OUTER TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(0.,0.,0.);
	return true;
      }
    }

    if( theSurfaces.find( "UP" ) != theSurfaces.end() ) {
      G4bool bInSurface;
      if( theDirection == fFlux_In || theDirection == fFlux_InOut ) {
	bInSurface = CheckSurfaceCap( GetLocalPoint(aStep->GetPreStepPoint()->GetPosition(), aStep).z(), theTubs->GetZHalfLength() );
      }
      if( !bInSurface && ( theDirection == fFlux_Out || theDirection == fFlux_InOut ) ) {
	bInSurface = CheckSurfaceCap( GetLocalPoint(stepPoint->GetPosition(), aStep).z(), theTubs->GetZHalfLength() );
      }
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::IsSelectedSurface UP TRUE " << G4endl;
#endif
	theNormal = G4ThreeVector(0.,0.,1.);
	return true;
      }
    }

    if( theSurfaces.find( "DOWN" ) != theSurfaces.end() ) {
      G4bool bInSurface;
      if( theDirection == fFlux_In || theDirection == fFlux_InOut ) {
	bInSurface = CheckSurfaceCap( GetLocalPoint(aStep->GetPreStepPoint()->GetPosition(),aStep).z(), -theTubs->GetZHalfLength() );
      }
      if( !bInSurface && ( theDirection == fFlux_Out || theDirection == fFlux_InOut ) ) {
	bInSurface = CheckSurfaceCap( GetLocalPoint(stepPoint->GetPosition(),aStep).z(), -theTubs->GetZHalfLength() );
      }
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::IsSelectedSurface DOWN TRUE " << G4endl;
#endif    
	theNormal = G4ThreeVector(0.,0.,-1.);
	return true;
      }
    }
    
    if( theSurfaces.find( "PHI" ) != theSurfaces.end() ) {
      G4ThreeVector localPos = GetLocalPoint(stepPoint->GetPosition(),aStep);
      G4double phi = localPos.phi();
      G4bool bInSurface = CheckSurfacePhi( phi, theTubs->GetStartPhiAngle() ) 
	|| CheckSurfacePhi( localPos.phi(), theTubs->GetStartPhiAngle() + theTubs->GetDeltaPhiAngle() );
      if( bInSurface ) {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::IsSelectedSurface PHI TRUE " << G4endl;
#endif    
	//	theArea = (theTubs->GetOuterRadius() - theTubs->GetInnerRadius()) * theTubs->GetDeltaPhiAngle();
	if( std::fabs(theTubs->GetStartPhiAngle() - phi ) < theAngularTolerance ) {
	  theNormal = G4ThreeVector(sin(phi),-cos(phi),0.);
	} else {
	  theNormal = G4ThreeVector(-sin(phi),cos(phi),0.);
	}
	return true;
      }
    }

    if( G4int(theSurfaces.size()) == theNSurfaceMax ) {
      G4Exception("GmScoringSurfaceTubs::IsSelectedSurface",
		  "Area (and therefore flux) will be wrongly calculated! Step is in one surface not identified",
		  JustWarning,
		  "It is probably due a volume placed inside the scoring volume");
    }
  }

  return false;
}


//-----------------------------------------------------------------------
G4bool GmScoringSurfaceTubs::CheckSurfaceRadius(G4ThreeVector localPos, G4double radius )
{ 
  
  if(std::fabs( localPos.perp() - radius ) < theSurfaceTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::CheckSurfaceRadius TRUE  localPos= " << localPos.perp() << " <> radius= " << radius << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::CheckSurfaceRadius FALSE  localPos= " << localPos.perp() << " <> radius= " << radius << G4endl;
#endif    
    return false;
  }
}

//-----------------------------------------------------------------------
G4bool GmScoringSurfaceTubs::CheckSurfaceCap(G4double localPos, G4double dim )
{ 
  
  if(std::fabs( localPos - dim ) < theSurfaceTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::CheckSurfaceCap TRUE localPos= " << localPos << " <> dim= " << dim << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::CheckSurfaceCap FALSE  localPos= " << localPos << " <> dim= " << dim << G4endl;
#endif    
    return false;
  }
}

//-----------------------------------------------------------------------
G4bool GmScoringSurfaceTubs::CheckSurfacePhi(G4double localPhi, G4double phi )
{ 
  
  if(std::fabs( localPhi - phi ) < theAngularTolerance ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::CheckSurfacePhi TRUE localPhi= " << localPhi << " <> phi= " << phi << G4endl;
#endif    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::CheckSurfacePhi FALSE  localPhi= " << localPhi << " <> phi= " << phi << G4endl;
#endif    
    return false;
  }
}


//-----------------------------------------------------------------------
G4double GmScoringSurfaceTubs::GetAngleFactor( G4StepPoint* stepPoint, G4StepPoint* volumeStepPoint )
{
  G4TouchableHandle touchable = volumeStepPoint->GetTouchableHandle();
  G4ThreeVector dir = stepPoint->GetMomentumDirection();
  G4ThreeVector localDir = touchable->GetHistory()->GetTopTransform().TransformAxis(dir).unit();
  //    G4cout << " localDir " <<localDir << " dir " << dir << G4endl; //GDEB 
  if( theNormal.mag() == 0 ) {
  // build normal for inner and outer surfaces
    G4ThreeVector pos= stepPoint->GetPosition();
    G4ThreeVector localPos = touchable->GetHistory()->GetTopTransform().TransformPoint(pos);
    if( std::fabs(localPos.perp() - theTubs->GetOuterRadius() ) < theSurfaceTolerance ){
      theNormal = localPos.unit();
    } else {
      theNormal = -localPos.unit();
    }
    theNormal.setZ(0.);
  } 
  G4double angleFactor = localDir*theNormal;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmScoringSurfaceTubs::GetAngleFactor " << angleFactor << " localDir " << localDir << " normal " << theNormal << " Dir " << dir << G4endl;
#endif
  return angleFactor;
}


