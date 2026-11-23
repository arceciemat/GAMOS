#include "GmSDVirtSegmBox.hh"
#include "GmHit.hh"
#include "GmEDepo.hh"
#include "GmSDVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4GeometryTolerance.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Box.hh"

//-------------------------------------------------------------------
GmSDVirtSegmBox::GmSDVirtSegmBox(G4String name ): GmVSD( name )
{
 
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmSDVirtSegmBox: sdtype ->" << theSDType << " SDName " << SensitiveDetectorName << G4endl;
#endif
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();

  std::vector<G4double> ndivs;
  ndivs = parmgr->GetVNumericValue("SD:VirtSegmBox:NDiv:"+theSDType, ndivs); 
  if( ndivs.size() == 0 ) {
    G4Exception("GmSDVirtSegmBox::GmSDVirtSegmBox",
		"NDiv is not defined",
		FatalErrorInArgument,
		G4String("Use parameter SD:VirtSegmBox:NDiv:"+theSDType).c_str());
  }
  if( ndivs.size() != 3 ) {
    G4Exception("GmSDVirtSegmBox::GmSDVirtSegmBox",
		"NDiv should have three values (X/Y/Z)",
		FatalErrorInArgument,
		G4String("Use parameter SD:VirtSegmBox:NDiv:"+theSDType).c_str());
  }
  theNDivX = G4int(ndivs[0]);
  theNDivY = G4int(ndivs[1]);
  theNDivXY = theNDivX * theNDivY;
  theNDivZ = G4int(ndivs[2]);

  std::vector<G4double> widths;
  widths = parmgr->GetVNumericValue("SD:VirtSegmBox:Width:"+theSDType, widths); 
  if( widths.size() == 0 ) {
    G4Exception("GmSDVirtSegmBox::GmSDVirtSegmBox",
		"Width is not defined",
		FatalErrorInArgument,
		G4String("Use parameter SD:VirtSegmBox:Width:"+theSDType).c_str());
  }
  if( widths.size() != 3 ) {
    G4Exception("GmSDVirtSegmBox::GmSDVirtSegmBox",
		"Width should have three values (X/Y/Z)",
		FatalErrorInArgument,
		G4String("Use parameter SD:VirtSegmBox:Width:"+theSDType).c_str());
  }
  theWidthX = widths[0];
  theWidthY = widths[1];
  theWidthZ = widths[2];
  theG4Box = new G4Box("GmVirtSegmBox",theWidthX/2., theWidthY/2., theWidthZ/2. );

  std::vector<G4double> offsets;
  offsets = parmgr->GetVNumericValue("SD:VirtSegmBox:Offset:"+theSDType, offsets); 
  bOffsetsAreSet = false;
  if( offsets.size() == 0 ) {
  } else if( ndivs.size() != 3 ) {
    G4Exception("GmSDVirtSegmBox::GmSDVirtSegmBox",
		"Offset should have three values (X/Y/Z)",
		FatalErrorInArgument,
		G4String("Use parameter SD:VirtSegmBox:Offset:"+theSDType).c_str());
  } else {
    bOffsetsAreSet = true;
    theOffsetX = offsets[0];
    theOffsetY = offsets[1];
    theOffsetZ = offsets[2];
  }
  bOffsetOnce = G4bool(parmgr->GetNumericValue("SD:VirtSegmBox:OffsetOnce:"+theSDType, false ) ); 
  bOffsetOnceIsDone = false;

  theMaxNVoxels = G4int(parmgr->GetNumericValue("SD:VirtSegmBox:MaxNVoxels:"+theSDType, 100)); 

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmVirtSegmBox: NDiv X/Y/Z " << theNDivX << " " << theNDivY << " " << theNDivZ
	   << " Width X/Y/Z " << theWidthX << " " << theWidthY << " " << theWidthZ
	   << " Offset X/Y/Z " << theOffsetX << " " << theOffsetY << " " << theOffsetZ 
	   << G4endl;
  }
  #endif

  theTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  
  theNAncestors = G4int(parmgr->GetNumericValue("SD:VirtSegmBox:NAncestors:"+theSDType, 0));
  theNShift = G4int(parmgr->GetNumericValue("SD:VirtSegmBox:NShift"+theSDType, 100));
  
}


//--------------------------------------------------------------------
G4bool GmSDVirtSegmBox::ProcessHits(G4Step* aStep,G4TouchableHistory* )
{
  if( !IsInterestingStep( aStep ) ) return FALSE;
  
  G4StepPoint* prePoint = aStep->GetPreStepPoint();
  G4StepPoint* postPoint = aStep->GetPostStepPoint(); 
  G4ThreeVector positionPre = prePoint->GetPosition();
  
  unsigned long long detUnitIDPre;
  G4ThreeVector localPosPre;
  G4VSolid* solid = prePoint->GetPhysicalVolume()->GetLogicalVolume()->GetSolid();
  const G4NavigationHistory* touchable = prePoint->GetTouchable()->GetHistory();
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmVirtSegmBox::ProcessHits: Call GetIDAndLocalPosition Pre " << G4endl;
  }
#endif
  GetIDAndLocalPosition( detUnitIDPre, localPosPre, prePoint->GetPosition(), touchable, solid ); 
  // localPos now contains the position in voxel coordinates
  
  unsigned long long detUnitIDPost;
  G4ThreeVector localPosPost;
  G4Track* aTrack = aStep->GetTrack();
  
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmVirtSegmBox::ProcessHits: Call GetIDAndLocalPosition Post " << G4endl;
  }
#endif

  if(aTrack->GetDefinition()->GetPDGCharge() == 0 ){
    detUnitIDPost = detUnitIDPre;
  } else {
    GetIDAndLocalPosition( detUnitIDPost, localPosPost, postPoint->GetPosition(), touchable, solid );
  // localPos now contains the position in voxel coordinates
  }
  
  G4double totalStepLength = (postPoint->GetPosition() - positionPre ).mag();
  G4double totalEnergy = aStep->GetTotalEnergyDeposit();
  G4double stepLength;
  G4ThreeVector dir = (postPoint->GetPosition() - prePoint->GetPosition()).unit();
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( dir, prePoint->GetTouchable()->GetHistory());
  G4double time = prePoint->GetGlobalTime();
  G4double velocity = prePoint->GetVelocity();

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmVirtSegmBox::ProcessHits: " << G4endl
	   << "   totalStepLength " <<  totalStepLength << G4endl
	   << "   totalEnergy " << totalEnergy << G4endl
	   << "   globalDir " << dir  << G4endl
	   << "   localDir " << localDir << G4endl
	   << "   time " << time << G4endl
	   << "   velocity " << velocity
	   << G4endl; 
  }
#endif

  G4double energy = totalEnergy;
  
  if( detUnitIDPre == detUnitIDPost ) { // first step, after the loop is broken if this condition is not satisfied
    StoreNewHit( energy, time, positionPre, detUnitIDPre, aStep );
  } else {
    
    G4int iv = 0; 
    unsigned long long detUnitIDPre_prev = detUnitIDPre;
    G4bool bLastStep = false;
    G4double sumStepLengths = 0.;
    while( !bLastStep && iv < theMaxNVoxels) {
      
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(debugVerb) ) {
	G4cout << "GmVirtSegmBox::ProcessHits: VOXEL " << iv 
	       << G4endl;   
      }
#endif
      GetNextVoxel( detUnitIDPre, localPosPre, stepLength, localDir );
      sumStepLengths += stepLength;
      if( sumStepLengths+theTolerance >= totalStepLength ) {
	stepLength -= sumStepLengths - totalStepLength; // stepLength was the DistanceToOut
	bLastStep = true;
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) {
	  G4cout << "GmVirtSegmBox::ProcessHits: last step, corrected stepLength = " << stepLength
		 << " sumStepLengths " << sumStepLengths
		 << " totalStepLength " << totalStepLength
		 << " stepLength before correction " << stepLength + sumStepLengths - totalStepLength
		 << G4endl; 
	}
#endif
	/*	if( detUnitIDPre_prev != detUnitIDPost ) { // first step, after the loop is broken if this condition is not satisfied
	  G4cerr << "GmVirtSegmBox::ProcessHits: last step, corrected stepLength = " << stepLength
		 << " sumStepLengths " << sumStepLengths
		 << " totalStepLength " << totalStepLength
		 << " stepLength before correction " << stepLength + sumStepLengths - totalStepLength
		 << G4endl;
	  G4ExceptionSeverity severity = JustWarning;
	  G4Exception("GmSDVirtSegmBox::ProcessHits",
		      "sum of step lengths is bigger than total step and detUnitID is not the same as the one of postStepPoint",
		      severity,
		      "Run with '/gamos/verbosity GmSDVerbosity debug' to understand what is happenning");
	  return false;
	  } */
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(debugVerb) ) {
	  G4cout << "GmVirtSegmBox::ProcessHits: NOT last step, add stepLength = " << stepLength
		 << " sumStepLengths " << sumStepLengths
		 << " totalStepLength " << totalStepLength
		 << " stepLength before correction " << stepLength + sumStepLengths - totalStepLength
		 << G4endl; 
	}
#endif
      }

      //----- Calculate variables at next position
      energy = totalEnergy * stepLength / totalStepLength;
      time += stepLength/velocity;
      positionPre += stepLength*dir; 
      StoreNewHit( energy, time, positionPre, detUnitIDPre_prev, aStep );
      detUnitIDPre_prev = detUnitIDPre;
    }     
    
    if( iv == theMaxNVoxels ) G4Exception("GmVirtSegmBox::ProcessHits",
					  "Too many voxels traversed",
					  FatalException,
					  G4String("Run with debug verbosity or change parameter SD:GmSDVirtSegmBox:MaxNVoxels:"+theSDType).c_str());
  
  }

  return TRUE;
}

void GmSDVirtSegmBox::StoreNewHit( G4double energy, G4double time, G4ThreeVector positionPre, unsigned long long detUnitIDPre, G4Step* aStep )
{

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmSDVirtSegmBox::StoreNewHit: energy " << energy 
	   << " time " << time
	   << " positionPre " << positionPre
	   << " detUnitIDPre " << detUnitIDPre
	   << G4endl;   
  }
#endif
  
  //----- Check det unit ID of PreStepPoint
  //--- Check if hit belongs to current hit
  G4bool hitUpdated = FALSE;
  
  if( theCurrentHit != 0 ){
    if( theCurrentHit->CheckSameCell( detUnitIDPre ) ) { 
      UpdateHitVS( theCurrentHit, energy, positionPre, time, aStep->GetTrack() );
      hitUpdated = TRUE;
    }
  }
  
  //----- Check if hit belongs to any other hit
  if( !hitUpdated ) {
    G4int isiz = theHitsInEvent.size();
    for( int ii=0; ii<isiz-1; ii++ ){
      if( theHitsInEvent[ii] == theCurrentHit ) continue;
      if( theHitsInEvent[ii]->CheckSameCell( detUnitIDPre ) ) {
	UpdateHitVS( theHitsInEvent[ii], energy, positionPre, time, aStep->GetTrack());
	hitUpdated = TRUE;
	break;
      }
    }     
  } 
  
  //----- Create hit if is not updating an existing one
  if( !hitUpdated ) CreateHitVS( aStep, energy, positionPre, time, detUnitIDPre );
  
}    


//-------------------------------------------------------------------
void GmSDVirtSegmBox::GetIDAndLocalPosition( unsigned long long& detUnitID, G4ThreeVector& localPos, G4ThreeVector position, const G4NavigationHistory* touchable, G4VSolid* solid )
{
  localPos = GmG4Utils::GetLocalFromGlobalPos( position, touchable );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmSDVirtSegmBox::GetIDAndLocalPosition localPos " << localPos 
	   << " globalPos " << position
	   << " volume " << touchable->GetVolume(0)->GetName() << G4endl;
  }
#endif

  //--- Get offsets
   if( !bOffsetsAreSet && !bOffsetOnceIsDone ) {
     if( bOffsetOnce ) bOffsetOnceIsDone = true;
    if( solid->GetEntityType() != "G4Box" ) {
      G4Exception("GmSDVirtSegmBox::GetIDAndLocalPosition",
		  "Offsets are not set and SD volume is not a box",
		  FatalException,
		  G4String("Use parameter SD:VirtSegmBox:Offset:"+GetType()).c_str());
    }
    G4Box* box = (G4Box*)(solid);
    theOffsetX = box->GetXHalfLength();
    theOffsetY = box->GetYHalfLength();
    theOffsetZ = box->GetZHalfLength();
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << " Setting offsets "
	   << " offsetX/Y/Z " << theOffsetX << " " << theOffsetY << " " << theOffsetZ 
	   << G4endl;
  }
#endif
  }

   //  G4bool bBadID = false;
  G4int nx = G4int( (localPos.x() + theOffsetX)/theWidthX );
  if( nx == theNDivX ) {
    if( (localPos.x() - theOffsetX) < theTolerance ) {
      nx = theNDivX -1 ;
    } else {
      //      bBadID = true;
    }
  }

  G4int ny = G4int( (localPos.y() + theOffsetY)/theWidthY );
  if( ny == theNDivY ) {
    if( (localPos.y() - theOffsetY) < theTolerance ) {
      ny = theNDivY -1 ;
    } else {
      //      bBadID = true;
    }
  }

  G4int nz = G4int( (localPos.z() + theOffsetZ)/theWidthZ );
  if( nz == theNDivZ ) {
    if( (localPos.z() - theOffsetZ) < theTolerance ) {
      nz = theNDivZ -1 ;
    } else {
      //      bBadID = true;
    }
  }

  //----- Check that it is inside limits
  if( nx < 0 || nx >= theNDivX ||
      ny < 0 || ny >= theNDivY ||
      nz < 0 || nz >= theNDivZ) {
    G4cerr << " nX/Y/Z " << nx << " " << ny << " " << nz 
	   << " NDivX/Y/Z " << theNDivX << " " << theNDivY << " " << theNDivZ 
	   << " offsetX/Y/Z " << theOffsetX << " " << theOffsetY << " " << theOffsetZ 
	   << " localPos " << localPos 
	   << G4endl;
    G4Exception("GmSDVirtSegmBox::GetIDAndLocalPosition",
		"Position is outside limits, check offsets",
		FatalException,
		G4String("Use parameter SD:VirtSegmBox:Offset:"+theSDType).c_str());
  }

  //----- Build detUnitID
  detUnitID = nx + theNDivX * ny + theNDivXY * nz;
  for( int ii = 2; ii < theNAncestors; ii++ ){
    detUnitID += theNDivXY*theNDivZ*pow(double(theNShift),int(ii-2)) * touchable->GetVolume(ii)->GetCopyNo();
#ifndef GAMOS_NO_VERBOSE    
    if( SDVerb(debugVerb) ) {
      G4cout << ii << " Box ID " << detUnitID << " " << touchable->GetVolume(ii)->GetCopyNo() << " " << touchable->GetVolume(ii)->GetName() << G4endl;
    }
#endif 
  }

  //----- Transform local in mother volume to local in voxel volume  
  localPos += G4ThreeVector( theOffsetX - (nx+0.5) * theWidthX, theOffsetY - (ny+0.5) * theWidthY, theOffsetZ - (nz+0.5) * theWidthZ );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmSDVirtSegmBox::GetIDAndLocalPosition nx/y/z " << nx << " " << ny << " " << nz 
	   << " detUnitID " << detUnitID
	   << " new localPos " << localPos 
	   << G4endl;
  }
#endif
}

//-------------------------------------------------------------------
void GmSDVirtSegmBox::GetNextVoxel( unsigned long long& detUnitID, G4ThreeVector& localPos, G4double& stepLength, const G4ThreeVector& localDir )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmSDVirtSegmBox::GetNextVoxel "
	   << " localPos " << localPos 
	   << " localDir " << localDir
	   << G4endl;
  }
#endif

  stepLength = theG4Box->DistanceToOut(localPos, localDir);
  G4int isx, isy, isz;
  localPos += stepLength*localDir;
  if( fabs( fabs(localPos.x()) - theWidthX/2. ) < theTolerance ) {
    isx = GmGenUtils::sign(localPos.x());
    detUnitID += isx * 1;
    localPos.setX( localPos.x() - isx*theWidthX );
  } else if( fabs( fabs(localPos.y()) - theWidthY/2. ) < theTolerance ) {
    isy =  GmGenUtils::sign(localPos.y());
    detUnitID += isy * theNDivX;
    localPos.setY( localPos.y() - isy*theWidthY );
  }else if( fabs( fabs(localPos.z()) - theWidthZ/2. ) < theTolerance ) {
    isz =  GmGenUtils::sign(localPos.z());
    detUnitID += isz * theNDivXY;
    localPos.setZ( localPos.z() - isz*theWidthZ );
  }
   
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmSDVirtSegmBox::GetNextVoxel " << G4endl
	   << "   new detUnitID " << detUnitID << G4endl
	   << "   new localPos " << localPos << G4endl
	   << "   stepLength " << stepLength << G4endl
	   << G4endl;
  }
#endif
}


//--------------------------------------------------------------------
void GmSDVirtSegmBox::CreateHitVS( G4Step* aStep, G4double energy, G4ThreeVector position, G4double time, unsigned long long id )
{
  G4String type = GetPathName();
  theCurrentHit = new GmHit( aStep, energy, id, type, theEventID );
  theCurrentHit->SetTimeMin( time );

  theCurrentHit->SetPosition(position);
  theCurrentHit->GetEDepos()[0]->SetPosition(position);
 
//  CalculateAndSetPosition( theCurrentHit, aStep ); //it is the SD that knows how to calculate the hit position

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) {
    G4cout << "GmSDVirtSegmBox::CreateHitVS " << G4endl
	   << "   detUnitID " << id << G4endl
	   << "   energy " << energy << G4endl
	   << "   pos " << position << G4endl
	   << G4endl;
  }
#endif

  theHitsInEvent.push_back( theCurrentHit );

}

//--------------------------------------------------------------------
void GmSDVirtSegmBox::UpdateHitVS( GmHit* hit, G4double energy, G4ThreeVector& position, G4double& time, G4Track* aTrack )
{
  hit->SetTimeMin( std::min( hit->GetTimeMin(), time ) );
  hit->SetTimeMax( std::max( hit->GetTimeMax(), time ) );

  hit->AddEDepo( new GmEDepo( energy, position, aTrack->GetDefinition(), aTrack->GetGlobalTime() ) );

  hit->SetEnergy( hit->GetEnergy() + energy);

  hit->InsertTrackIDs( aTrack );
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " GmSDVirtSegmBox::UpdateHitVS " << hit->GetDetUnitID() 
				 << " E= " << energy 
				 << " Etot= " << hit->GetEnergy() 
				 << " pos= " << position
				 << " N E depos= " << hit->GetEDepos().size() 
				 << G4endl;
#endif

}


//-------------------------------------------------------------------
void GmSDVirtSegmBox::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0 && SDVerb(infoVerb) ) G4cout << this << "GmSDVirtSegmBox::EndOfEvent nhits " << SensitiveDetectorName << " Nhits " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
