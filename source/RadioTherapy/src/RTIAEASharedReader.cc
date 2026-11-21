#include "RTIAEASharedReader.hh"
#include "RTVerbosity.hh"
#include "iaea_record.hh"
#include "iaea_utilities.hh"
#include "G4AutoLock.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

namespace {
  G4Mutex theIAEAMutex = G4MUTEX_INITIALIZER;
  FILE* theSharedFile = nullptr;
  G4int theSkipRunID = -1;
  G4int theSharedNRecycled = 0;
  G4bool bSharedNRecycledInit = false;
  G4bool bExhausted = false;
}

//------------------------------------------------------------------------
RTIAEASharedReader::ReadResult RTIAEASharedReader::ReadParticle( iaea_record_type* record, FILE* ownFile, const G4String& fileName, G4int nSkip, G4int maxNRecycle, G4int& nRecycled, const G4String& caller )
{
  G4AutoLock l(&theIAEAMutex);
  if( !theSharedFile ) theSharedFile = ownFile;
  //--- start counting recycles from the initial value used by the generator (as in sequential)
  if( !bSharedNRecycledInit ) {
    theSharedNRecycled = nRecycled;
    bSharedNRecycledInit = true;
  }
  if( bExhausted ) return RTIAEA_EXHAUSTED;
  record->p_file = theSharedFile;

  //--- skip first particles at the start of each run
  const G4Run* run = G4RunManager::GetRunManager()->GetCurrentRun();
  G4int runID = run ? run->GetRunID() : 0;
  if( runID != theSkipRunID ) {
    theSkipRunID = runID;
    for( G4int ii = 0; ii < nSkip; ii++ ){
      record->read_particle();
    }
  }

  ReadResult result = RTIAEA_OK;
  int fileEnd = record->read_particle();
  if( fileEnd == FAIL) {
    if( theSharedNRecycled < maxNRecycle ) {
      fclose(theSharedFile);
      theSharedFile = open_file(const_cast<char*>(fileName.c_str()),(char *)(".IAEAphsp"),(char *)("rb"));
      record->p_file = theSharedFile;
#ifndef GAMOS_NO_VERBOSE
      if( RTVerb(infoVerb) ) G4cout << caller << "::GeneratePrimaries  RECYCLING phase space file " << theSharedNRecycled << G4endl;
#endif
      theSharedNRecycled++;
      //skip first particles
      for( G4int ii = 0; ii < nSkip; ii++ ){
	record->read_particle(); 
      }
      result = RTIAEA_RECYCLED;
    } else {
      fclose(theSharedFile);
      theSharedFile = nullptr;
      bExhausted = true;
      result = RTIAEA_EXHAUSTED;
    }
  }
  nRecycled = theSharedNRecycled;
  return result;
}
