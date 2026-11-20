#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "globals.hh"
#include "G4AutoLock.hh"

#include "GmGeneratorFromTextFile.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//------------------------------------------------------------------------
GmGeneratorFromTextFile::GmGeneratorFromTextFile()
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 
  G4String fname = parmgr->GetStringValue("GmGeneratorFromTextFile:FileName", "generator.txt");
  theFileIn = GmFileIn::GetInstance(fname,1);
  theFileIn.SetSeparator(',');

  thePrevData.eventID = -1;
  theNLinesRead = 0;

  theNEventsToSkip = GmParameterMgr::GetInstance()->GetNumericValue("GmGeneratorFromTextFile:NEventsToSkip",0);


}


//------------------------------------------------------------------------
GmGeneratorFromTextFile::~GmGeneratorFromTextFile()
{
  theFileIn.Close();
}

//------------------------------------------------------------------------
//--- One file position shared by all threads (multithreading): each event of the file is generated once, as in
//    sequential mode. The lines of an event (same eventID) are read together under the mutex.
namespace {
  G4Mutex theTextGenMutex = G4MUTEX_INITIALIZER;
  GmFileIn* theSharedTextFile = nullptr;
  GenerTextFileData theSharedPrevData;
  G4bool bSharedPrevInit = false;
  G4bool bSharedTextEnd = false;
  G4int theSharedNLinesRead = 0;
}

//------------------------------------------------------------------------
void GmGeneratorFromTextFile::GeneratePrimaries(G4Event* evt)
{
  //P  22 18.7405 -15.5304 -11.4193 0.85396 -0.625037 -0.711131 0  
  std::vector<GenerTextFileData> evtLines;
  G4bool bEnd = false;
  {
    G4AutoLock l(&theTextGenMutex);
    if( !theSharedTextFile ) theSharedTextFile = &theFileIn;
    if( !bSharedPrevInit ) {
      theSharedPrevData.eventID = -1;
      bSharedPrevInit = true;
    }
    if( bSharedTextEnd || theSharedTextFile->eof() ) {
      bSharedTextEnd = true;
      bEnd = true;
    } else {
      if( theSharedPrevData.eventID != -1 ) {
	evtLines.push_back(theSharedPrevData);
      }
      GenerTextFileData evtDat;
      G4bool bRead = false;
      std::vector<G4String> wl;
      for( ;; ){
	if( !theSharedTextFile->GetWordsInLine(wl) || wl.size() == 0 ) {
	  bSharedTextEnd = true; // end of file: this event is the last one
	  break;
	} 
	theSharedNLinesRead ++;
	if( wl.size() != 10 ) {
	  G4Exception("GmGeneratorFromTextFile::GeneratePrimaries",
		      G4String("Wrong number of words in line "+GmGenUtils::itoa(theSharedNLinesRead)).c_str(),
		      FatalErrorInArgument,
		      (G4String("Number of words is ") + GmGenUtils::itoa(wl.size())).c_str());
	}
	bRead = true;
	evtDat.eventID = GmGenUtils::GetInteger(wl[0]);
	evtDat.partName = wl[1];
	evtDat.posx = GmGenUtils::GetValue(wl[2]);
	evtDat.posy = GmGenUtils::GetValue(wl[3]);
	evtDat.posz = GmGenUtils::GetValue(wl[4]);
	evtDat.momx = GmGenUtils::GetValue(wl[5]);
	evtDat.momy = GmGenUtils::GetValue(wl[6]);
	evtDat.momz = GmGenUtils::GetValue(wl[7]); 
	evtDat.time = GmGenUtils::GetValue(wl[8]);
	evtDat.weight = GmGenUtils::GetValue(wl[9]);
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << evtDat.eventID 
					  << " " << evtDat.partName 
					  << " " << evtDat.posx 
					  << " " << evtDat.posy 
					  << " " << evtDat.posz 
					  << " " << evtDat.momx 
					  << " " << evtDat.momy 
					  << " " << evtDat.momz 
					  << " " << evtDat.time 
					  << " " << evtDat.weight 
					  << G4endl;
#endif
	if( theSharedPrevData.eventID == -1 || theSharedPrevData.eventID == evtDat.eventID ) {
	  if( evtDat.eventID >= theNEventsToSkip ) {
	    evtLines.push_back(evtDat);
	    if( theSharedPrevData.eventID == -1 ) {
	      theSharedPrevData = evtDat;
	    }
	  }
	} else {
	  break;  // generate particle in next event
	}
      }
      if( bRead ) theSharedPrevData = evtDat;
      if( bSharedTextEnd ) bEnd = true;
    }
    theNLinesRead = theSharedNLinesRead;
  }

  if( bEnd ) G4RunManager::GetRunManager()->AbortRun();  //  abortrun (after this event, if it has particles)
  for( size_t ii = 0; ii < evtLines.size(); ii++ ) {
    GenerateParticle( evtLines[ii], evt );
  }
}
  
void GmGeneratorFromTextFile::GenerateParticle( const GenerTextFileData& evtDat, G4Event* evt )
{
  G4PrimaryVertex* vtx = new G4PrimaryVertex( G4ThreeVector(evtDat.posx,evtDat.posy,evtDat.posz), evtDat.time );
  evt->AddPrimaryVertex( vtx );
  G4ParticleDefinition* partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( evtDat.partName, false );
  if( !partDef ) {
    GmGetParticleMgr::GetInstance()->CreateIon( evtDat.partName );
    partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( evtDat.partName, true );
  }

#ifndef GAMOS_NO_VERBOSE
  if( !partDef ) if( GenerVerb(debugVerb) ) G4cout << " partDef " << partDef << " partName " << evtDat.partName << G4endl;
#endif
  G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, evtDat.momx, evtDat.momy, evtDat.momz);
  particle->SetWeight( evtDat.weight );  

  vtx->SetPrimary(particle);

}
