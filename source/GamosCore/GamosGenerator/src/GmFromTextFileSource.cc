#include "GmFromTextFileSource.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmGenerDistEnergyConstant.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "GmGenerDistTimeConstant.hh"
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerDistPositionPoint.hh" 
#include "GmGenerDistEnergyConstant.hh"
#include "GmGeneratorMgr.hh"

#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Element.hh"
#include "G4RunManager.hh"

#include "CLHEP/Random/RandFlat.h"

//-----------------------------------------------------------------------
GmFromTextFileSource::GmFromTextFileSource( const G4String& name, const G4String& fileName ): GmParticleSource( name )
{
  //---- Open file with list of particles
  theFileName = fileName;
  theFileIn = GmFileIn::GetInstance(theFileName);

  theFileIn.SetSeparator(',');

  Initialize();
  theMaxNVertices = GmParameterMgr::GetInstance()->GetNumericValue("GmFromTextFileSource:MaxNVertices",100);
}

//-----------------------------------------------------------------------
GmFromTextFileSource::GmFromTextFileSource( const G4String& name ): GmParticleSource( name )
{
  Initialize();
  theMaxNVertices = GmParameterMgr::GetInstance()->GetNumericValue("GmFromTextFileSource:MaxNVertices",100);
}

//-----------------------------------------------------------------------
void  GmFromTextFileSource::Initialize()
{
  theType = "ReadFromTextFileSource";

  theTimeDistribution = 0;

  theDirectionDistribution = 0;

  thePositionDistribution = 0; 

  theEnergyDistribution = 0;

  theNLinesRead = 0;
  theNEventsRead = 0; // two contiguous lines with same EventID are only one event
  theLastEventTime = 0.;
  thePrevData.eventID = -1;
  bRecycleFile = false;

  theNEventsToSkip = GmParameterMgr::GetInstance()->GetNumericValue(theName+":NEventsToSkip",0);

  bRandomNEventsToSkip = false;
  bInitialised = false;

}

//-----------------------------------------------------------------------
void GmFromTextFileSource::SetNEventsInFile( G4int nEvts )
{
  bRandomNEventsToSkip = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmFromTextFileSource::SkipRandomNEvents",0));  
  theNEventsInFile = nEvts;
}
    
//-----------------------------------------------------------------------
GmFromTextFileSource::~GmFromTextFileSource()
{
  theFileIn.Close();
}

//-----------------------------------------------------------------------
std::vector<G4PrimaryVertex*> GmFromTextFileSource::GenerateVertices( G4double time )
{
  if( !bInitialised ) {
    if( bRandomNEventsToSkip ) {
      theNEventsToSkip = CLHEP::RandFlat::shoot()*theNEventsInFile;
      if( GenerVerb(warningVerb) ) G4cout << theName << " GmFromTextFileSource::GenerateVertices SKIP N EVENTS " << theNEventsToSkip << " out of " << theNEventsInFile << G4endl;
    }
    bInitialised = true;
  }

  std::vector<G4PrimaryVertex*> vtxs;
  if( thePrevData.eventID == -1 ) { // skip first events
    G4int nVertRead = 0;
    for( ;; ) {
      if( nVertRead > theMaxNVertices ) {
	G4Exception("GmFromTextFileSource::GenerateVertices",
		    "",
		    JustWarning,
		    (theName+" Stop reading lines to build vertices, after "+GmGenUtils::itoa(theMaxNVertices)+
		     " (it maybe that file has only one line). \n You may change /gamos/setParam GmFromTextFileSource::MaxNVertices ").c_str());
	break;
      }
       if( ReadOneParticle() ) {
	 if( thePrevData.eventID != theCurrentData.eventID ) {
	   theNEventsRead += 1;
	   nVertRead = 0;
	 } 
	 nVertRead ++;
	 if( GenerVerb(testVerb) ) G4cout << " GmFromTextFileSource::GenerateVertices SKIP EVENTS? " << theCurrentData.eventID << "  theNEventsRead= " << theNEventsRead << " >= theNEventsToSkip= " << theNEventsToSkip << G4endl;
	 thePrevData = theCurrentData;
	 if( theNEventsRead >= theNEventsToSkip ) { // END SKIPPING EVENTS
	   break;
	 } 
       } else {
	G4Exception("GmFromTextFileSource::GenerateVertices",
		  "",
		    FatalException,
		    (theName+" Trying to skip a number of events too big : "+GmGenUtils::itoa(theNEventsToSkip)+" N events read="+GmGenUtils::itoa(theNEventsRead)).c_str());
      }
    }
  }
  vtxs.push_back(GenerateVertex( time )); // add event data read previously, or first event after skipEvents 

  //--- Loop until a different event is found
  for( G4int ii = 0;; ii++ ) {
    if( ii > theMaxNVertices ) {
      G4Exception("GmFromTextFileSource::GenerateVertices",
		  "",
		  JustWarning,
		  (theName+" Stop reading lines to build vertices, after "+GmGenUtils::itoa(theMaxNVertices)+
		   " (it maybe that file has only one line). \n You may change /gamos/setParam GmFromTextFileSource::MaxNVertices ").c_str());
      break;
    }
    if( !ReadOneParticle() ) {
      return vtxs;
    }
    if( GenerVerb(testVerb) ) G4cout << " GmFromTextFileSource::GenerateVertices thePrevData.eventID " << thePrevData.eventID << " =? " << theCurrentData.eventID << G4endl; 
    // After the first line read: check skip events. Also if line is the same event as previous line
    if( thePrevData.eventID == theCurrentData.eventID ) {
      vtxs.push_back(GenerateVertex( time )); // add event data read previously 
      if( GenerVerb(testVerb) ) G4cout << " GmFromTextFileSource::GenerateVertices ADD LINE TO EVENT " << theCurrentData.eventID << G4endl; 
    } else {
      thePrevData = theCurrentData;
      break;
    }
    thePrevData = theCurrentData;
    if( thePrevData.eventID != theCurrentData.eventID ) {
      theNEventsRead += 1;
    }
  }

  theLastEventTime = theCurrentData.time;

  return vtxs;
}

//-----------------------------------------------------------------------
G4PrimaryVertex* GmFromTextFileSource::GenerateVertex( G4double time )
{

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " G4PrimaryVertex* GmFromTextFileSource::GenerateVertex( G4double time ) " << G4endl;
#endif

  thePosition = G4ThreeVector(theCurrentData.posx,theCurrentData.posy,theCurrentData.posz);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << "GmFromTextFileSource::GenerateVertex pos " << thePosition << G4endl;
#endif
  
  G4PrimaryVertex* vtx = new G4PrimaryVertex( thePosition, time );
 
  G4ThreeVector mom = G4ThreeVector(theCurrentData.momx,theCurrentData.momy,theCurrentData.momz);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmFromTextFileSource::GenerateVertex  mom " << mom << G4endl;
#endif
 
  G4ParticleDefinition* partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( theCurrentData.partName, false );
  
  G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, mom.x(), mom.y(), mom.z() );

  particle->SetWeight( theCurrentData.weight );

  vtx->SetPrimary(particle);

  return vtx;

}

//-----------------------------------------------------------------------
G4double GmFromTextFileSource::GenerateTime()
{

  if( theLastDecayTimeAnySource == 0 ) {
  } else {
    theTime -= theLastDecayTimeAnySource;
  }
  
  return theTime;
}

//-----------------------------------------------------------------------
G4bool GmFromTextFileSource::ReadOneParticle()
{
  G4bool bFileEnded = false;
  std::vector<G4String> wl;
  if( !theFileIn.GetWordsInLine(wl) ) {
    bFileEnded = true;
  } 
  /*  if( theFileIn.eof() ) {
    bFileEnded = true;
  }
  if( wl.size() == 0 ) {
    bFileEnded = true;
    }*/
  if( bFileEnded ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmFromTextFileSource::ReadOneParticle() FILE ENDED, RECYCLE? " << bRecycleFile << G4endl;
#endif
    if( !bRecycleFile ) {
      if( GenerVerb(warningVerb) ) G4cout << "GmFromTextFileSource::ReadOneParticle() " << theName << " 1NO RECYCLE, ABORT RUN " << G4endl;
      G4RunManager::GetRunManager()->AbortRun();  //  abortrun
      return false;
    } else {
      theFileIn.Rewind();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(-debugVerb) ) G4cout << "GmFromTextFileSource::ReadOneParticle() FILE Rewind " << theName << G4endl;
#endif
      if( !theFileIn.GetWordsInLine(wl) ) {
	if( GenerVerb(warningVerb) ) G4cout << "GmFromTextFileSource::ReadOneParticle() " << theName << " 2NO RECYCLE, ABORT RUN " << G4endl;
	G4RunManager::GetRunManager()->AbortRun();  //  abortrun
      }
    }
  }
  
  theNLinesRead ++;
  if( wl.size() != 10 ) {
    for( size_t ii = 0; ii < wl.size(); ii++ ) {
      G4cerr << wl[ii] << ",";
    }
    G4cerr << G4endl;
    G4Exception("GmFromTextFileSource::GeneratePrimaries",
		G4String("FILE "+theName+"Wrong number of words in line "+GmGenUtils::itoa(theNLinesRead)).c_str(),
		FatalErrorInArgument,
		(G4String("Number of words is ") + GmGenUtils::itoa(wl.size())).c_str());
  }
  
  theCurrentData.eventID = GmGenUtils::GetInteger(wl[0]);
  theCurrentData.partName = wl[1];
  theCurrentData.posx = GmGenUtils::GetValue(wl[2]);
  theCurrentData.posy = GmGenUtils::GetValue(wl[3]);
  theCurrentData.posz = GmGenUtils::GetValue(wl[4]);
  theCurrentData.momx = GmGenUtils::GetValue(wl[5]);
  theCurrentData.momy = GmGenUtils::GetValue(wl[6]);
  theCurrentData.momz = GmGenUtils::GetValue(wl[7]); 
  theCurrentData.time = GmGenUtils::GetValue(wl[8]);
  theCurrentData.weight = GmGenUtils::GetValue(wl[9]);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << theCurrentData.eventID 
				    << " " << theCurrentData.partName 
				    << " " << theCurrentData.posx 
				    << " " << theCurrentData.posy 
				    << " " << theCurrentData.posz 
				    << " " << theCurrentData.momx 
				    << " " << theCurrentData.momy 
				    << " " << theCurrentData.momz 
				    << " " << theCurrentData.time 
				    << " " << theCurrentData.weight 
				    << G4endl;
#endif
  theTime = theCurrentData.time-theLastEventTime;
  if( GenerVerb(testVerb) ) G4cout << theName << " GmFromTextFileSource::ReadOneParticle(  TIME = " << theTime << " : " << theCurrentData.time << " theLastEventTime " << theLastEventTime << " POSX " << theCurrentData.posx << G4endl;

  return true;
}

//-----------------------------------------------------------------------
void GmFromTextFileSource::SetNLinesToSkip( G4int nLinesToSkip )
{
  G4cout << theName << " GmFromTextFileSource::SetNLinesToSkip( " << nLinesToSkip << G4endl; //GDEB
  for( G4int ii = 0; ii < nLinesToSkip; ii++ ) {
    //    G4cout << theName << " GmFromTextFileSource::SetNLinesToSkip(ReadOneParticle " << ii << G4endl; //GDEB
    if( !ReadOneParticle() ) {
      G4Exception("GmFromTextFileSource::SetNLinesToSkip",
		  "",
		  FatalException,
		  ("File "+theFileName+" has "+GmGenUtils::itoa(ii)+" lines, less than NLinesToSkip= "+GmGenUtils::itoa(nLinesToSkip)).c_str());
    }
  }
}
