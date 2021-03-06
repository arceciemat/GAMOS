//=====================================================================
//
//  (Description)
//    GmScoringRun Class is for accumulating scored quantities which is 
//  scored using G4MutiFunctionalDetector and G4VPrimitiveScorer.
//  Accumulation is done using G4THitsMap object.
//
//    The constructor GmScoringRun(const std::vector<G4String> mfdName)
//  needs a vector filled with MultiFunctionalDetector names which
//  was assigned at instantiation of MultiFunctionalDetector(MFD).
//  Then GmScoringRun constructor automatically scans primitive scorers
//  in the MFD, and obtains collectionIDs of all collections associated
//  to those primitive scorers. Futhermore, the G4THitsMap objects 
//  for accumulating during a RUN are automatically created too.
//  (*) Collection Name is same as primitive scorer name.
// 
//    The resultant information is kept inside GmScoringRun objects as
//  data members.
//  std::vector<G4String> theCollName;            // Collection Name,
//  std::vector<G4int> theCollID;                 // Collection ID,
//  std::vector<G4THitsMap<G4double>*> theRunMap; // HitsMap for RUN.
//
//  The resualtant HitsMap objects are obtain using access method,
//  GetHitsMap(..).
//
//=====================================================================

#include "GmScoringRun.hh"
#include "GmScoringMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4SDManager.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

//
//   (The vector of MultiFunctionalDetector name has to given.)
GmScoringRun::GmScoringRun(): G4Run()
{
  std::map<G4String,G4MultiFunctionalDetector*> MFDs = GmScoringMgr::GetInstance()->GetMFDs();
  std::map<G4String,G4MultiFunctionalDetector*>::const_iterator ite;

  //  G4cout <<  "GmScoringRun::GmScoringRun " <<  mfdName << G4endl;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  //=================================================
  //  Initalize RunMaps for accumulation.
  //  Get CollectionIDs for HitCollections.
  //=================================================
  //  G4int Nmfd = mfdName.size();
  //  for ( G4int idet = 0; idet < Nmfd ; idet++){  // Loop for all MFD.
  //    G4String detName = mfdName[idet];
  for( ite = MFDs.begin(); ite != MFDs.end(); ite++ ) {
    //    G4String detName = mfdName;
    G4String detName = ((*ite).second)->GetName();
    //    G4cout <<  "GmScoringRun::GmScoringRun " << detName << G4endl;
    //--- Seek and Obtain MFD objects from SDmanager.
    G4MultiFunctionalDetector* mfd =
      (G4MultiFunctionalDetector*)(SDman->FindSensitiveDetector(detName));
    //  G4cout  <<  "GmScoringRun::GmScoringRun mfd " << mfd << G4endl;
    //
    if ( mfd ){
      //  G4cout  <<  "GmScoringRun::GmScoringRun mfd primi " <<  mfd->GetNumberOfPrimitives() << G4endl;
	//--- Loop over the registered primitive scorers.
	for (G4int icol = 0; icol < mfd->GetNumberOfPrimitives(); icol++){
	    // Get Primitive Scorer object.
	    G4VPrimitiveScorer* scorer=mfd->GetPrimitive(icol);
	    // collection name and collectionID for HitsCollection,
            // where type of HitsCollection is G4THitsMap in case of primitive scorer.
            // The collection name is given by <MFD name>/<Primitive Scorer name>.
	    G4String collectionName = scorer->GetName();
	    G4String fullCollectionName = detName+"/"+collectionName;
	    G4int    collectionID = SDman->GetCollectionID(fullCollectionName);
	    //    G4cout  <<  "GmScoringRun::GmScoringRun collectionID " << collectionID << G4endl;

	    //
	    if ( collectionID >= 0 ){
	      //	      G4cout << "GmScoringRun++ "<<fullCollectionName<< " id " << collectionID << G4endl;
		// Store obtained HitsCollection information into data members.
		// And, creates new G4THitsMap for accumulating quantities during RUN.
		theCollName.push_back(fullCollectionName);
		theCollID.push_back(collectionID);
		theRunMap.push_back(new G4THitsMap<G4double>(detName,collectionName));
	    }else{
		G4cout << "** collection " << fullCollectionName << " not found. "<<G4endl;
	    }
	}
    }
  }
}

//
// Destructor
//    clear all data members.
GmScoringRun::~GmScoringRun()
{
  //--- Clear HitsMap for RUN
  G4int Nmap = theRunMap.size();
  for ( G4int i = 0; i < Nmap; i++){
    if(theRunMap[i] ) theRunMap[i]->clear();
  }
  theCollName.clear();
  theCollID.clear();
  theRunMap.clear();
}

//
//  RecordEvent is called at end of event.
//  For scoring purpose, the resultant quantity in a event,
//  is accumulated during a Run.
void GmScoringRun::RecordEvent(const G4Event* aEvent)
{
  //  G4cout << this << "GmScoringRun::RecordEvent map size " << theRunMap.size() << G4endl;
  numberOfEvent++;  // This is an original line.

  //=============================
  // HitsCollection of This Event
  //============================
  G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
  if (!HCE) return;

  //=======================================================
  // Sum up HitsMap of this Event  into HitsMap of this RUN
  //=======================================================
  G4int Ncol = theCollID.size();
  for ( G4int i = 0; i < Ncol ; i++ ){  // Loop over HitsCollection
    G4THitsMap<G4double>* EvtMap=0;
    if ( theCollID[i] >= 0 ){           // Collection is attached to HCE
      EvtMap = (G4THitsMap<G4double>*)(HCE->GetHC(theCollID[i]));
    }else{
      G4cout <<" Error EvtMap Not Found "<< i << G4endl;
    }
    // G4cout << " EVTMAP " << i << " = " << EvtMap->entries() << G4endl;
    if ( EvtMap )  {
      //=== Sum up HitsMap of this event to HitsMap of RUN.===
      *theRunMap[i] += *EvtMap;
      //======================================================
    }
  }
}

//=================================================================
//  Access method for HitsMap of the RUN
//
//-----
// Access HitsMap.
//  By  MultiFunctionalDetector name and Collection Name.
G4THitsMap<G4double>* GmScoringRun::GetHitsMap(const G4String& detName,
					 const G4String& colName){
    G4String fullName = detName+"/"+colName;
    return GetHitsMap(fullName);
}

//-----
// Access HitsMap.
//  By full description of collection name, that is
//    <MultiFunctional Detector Name>/<Primitive Scorer Name>
G4THitsMap<G4double>* GmScoringRun::GetHitsMap(const G4String& fullName){
    G4int Ncol = theCollName.size();
    for ( G4int i = 0; i < Ncol; i++){
	if ( theCollName[i] == fullName ){
	    return theRunMap[i];
	}
    }
    return NULL;
}

//-----
// - Dump All HitsMap of this RUN. (for debuging and monitoring of quantity).
//   This method calls G4THisMap::PrintAll() for individual HitsMap.
void GmScoringRun::DumpAllScorers()
{
  //  PrintAllScorers();

  GmScoringMgr* scoreMgr = GmScoringMgr::GetInstance();
  std::map<G4String,GmVPrimitiveScorer*> scorers = scoreMgr->GetScorers();

  // - Number of HitsMap in this RUN.
  G4int n = GetNumberOfHitsMap();
  //  G4cout << " GmScoringRun::DumpAllScorer() " << n << " " << theRunMap.size() << G4endl;
  // - GetHitsMap and dump values.
  for ( G4int i = 0; i < n ; i++ ){
    G4THitsMap<G4double>* RunMap =GetHitsMap(i);
    //    G4cout << " GmScoringRun::DumpAllScorer RunMap " << RunMap << G4endl;
    if ( RunMap ) {
      //      G4cout << " PrimitiveScorer RUN: SD = " 
      //	     << RunMap->GetSDname() <<" SCORER = "<< RunMap->GetName() << G4endl;
      scorers[RunMap->GetName()]->CalculateErrors(RunMap);

      scorers[RunMap->GetName()]->DumpAll(RunMap);
      /*      G4cout << " Number of entries " << RunMap->entries() << G4endl;
      std::map<G4int,G4double*>::iterator itr = RunMap->GetMap()->begin();
      for(; itr != RunMap->GetMap()->end(); itr++) {
	G4cout << "  copy no.: " << itr->first
	       << "  Run Value : " << *(itr->second) 
	       << G4endl;
	       } */
    }
  }
  
}
