#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#include <math.h>

#include "RTPhaseSpace_extUA.hh"
#include "RTPhaseSpaceHistos.hh"
#include "RTVExtraInfoProviderFloat.hh"
#include "RTExtraInfoMgr.hh"

#define RECORD_TO_PRINT 4
#define MAIN
#define Z_OK 0
//extern "C" int def(FILE *source, FILE *dest, int level);
//extern "C" int inf(FILE *source, FILE *dest);
//extern "C" void zerr(int ret);

#include "iaea_header.hh"
#include "iaea_record.hh"
#include "iaea_utilities.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "RadioTherapy/include/RTVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "G4GeometryTolerance.hh"

//---------------------------------------------------------------------
RTPhaseSpace_extUA::RTPhaseSpace_extUA()
{

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  
  theFileName = paramMgr->GetStringValue("RTPhaseSpace_extUA:FileName","test");

  bStoreZ = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_extUA:StoreZ", FALSE ));

  bHistos = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_extUA:Histos", FALSE ));

  if( bHistos ) {
    theHistos = new RTPhaseSpaceHistos(this); 
    theHistos->SetClassifier( theClassifier );
  }

  theNEventsToSave = G4int(paramMgr->GetNumericValue("RTPhaseSpace_extUA:NEventsToSave", -1 ));

  bNotStoreTwice = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_extUA:NotStoreTwice", TRUE ));

  bNotStoreDaughtersTwice = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_extUA:NotStoreDaughtersTwice", TRUE ));

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << " bStoreZ " << bStoreZ << G4endl;
#endif

  theZStops.push_back(0.);
}

//---------------------------------------------------------------------
G4bool RTPhaseSpace_extUA::CreateIAEAheaders()
{
  G4bool isOK = TRUE;
  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
    //    G4cout << " call CreateIAEAheader " << ii << G4endl;
    G4bool isOKnew = CreateIAEAheader( 0. );
    isOK = isOK && isOKnew;
  }
  return isOK;
}

//---------------------------------------------------------------------
G4bool RTPhaseSpace_extUA::CreateIAEAheader(G4double zstop)
{

  //  G4cout << G4endl << " RTPhaseSpace_extUA::CreateIAEAheader" << zstop << G4endl;
 
  // Creating IAEA phsp header and allocating memory for it
  iaea_header_type * p_iaea_header = (iaea_header_type *) calloc(1, sizeof(iaea_header_type));
  //---- check that z stops are not repeated
  if( p_iaea_headers.find( zstop ) !=  p_iaea_headers.end() ) {
    G4Exception("RTPhaseSpace_extUA::CreateIAEAheader ",
		"Repeated Zstop ",
		FatalErrorInArgument,
		(" Zstp VALUE = " + GmGenUtils::ftoa(zstop)).c_str());
  }
  p_iaea_headers[zstop] = p_iaea_header;

  p_iaea_header->initialize_counters();   
  
  // Creating IAEA record and allocating memory for it
  iaea_record_type * p_iaea_record = (iaea_record_type *) calloc(1, sizeof(iaea_record_type));
  p_iaea_records[zstop] = p_iaea_record;

  // Opening phase space file for output
  char fiaeaphsp[81]={'\0'};;
  
  G4String fname = theFileName;

  strcpy(fiaeaphsp,fname.c_str());
  
  p_iaea_record->p_file = open_file(fiaeaphsp,(char *)(".IAEAphsp"),(char *)("wb"));
  if(p_iaea_record->p_file == NULL) return(FAIL);
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << " Phase Space record file: " << fiaeaphsp << ".IAEAphsp" << G4endl;
#endif
  
  // Setting default i/o flags 
  if(p_iaea_record->initialize() != OK) return(FAIL);
 
  // Changing default i/o flags 
  p_iaea_record->iextralong = theNExtraInfoLong;
  p_iaea_record->iextrafloat = theNExtraInfoFloat;
  p_iaea_record->iz = 1;          // Z is stored  

  // p_iaea_record->z = zstop/CLHEP::cm;        // Z constant 
  
  // Store read/write logical block in the PHSP header
  if( p_iaea_header->set_record_contents(p_iaea_record) == FAIL) return FAIL;
  
  //  printf("\n DEFINED PHASE SPACE RECORD LENGTH = %i\n",p_iaea_header->record_length);
  
  return(OK);
}


//---------------------------------------------------------------------
void RTPhaseSpace_extUA::UserSteppingAction(const G4Step* aStep)
{ 
  StoreTrackRecord( aStep, 0.);
}


//---------------------------------------------------------------------
void RTPhaseSpace_extUA::StoreTrackRecord( const G4Step* aStep, G4double zstop)
{
  const G4Track* aTrack = aStep->GetTrack();
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpace_extUA::StoreTrackRecord " << zstop << " " << aStep->GetPreStepPoint()->GetPosition().z() << " " << aStep->GetPostStepPoint()->GetPosition().z() << G4endl;
#endif
  if( bNotStoreTwice ) {
    G4int trackID = aTrack->GetTrackID();
    if( theTracksStored[zstop].find(trackID) != theTracksStored[zstop].end() ) {
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) )  G4cout << " RTPhaseSpace_extUA bNotStoreTwice REJECT " << trackID << G4endl;
#endif
      return;
    }
  }
  if( bNotStoreDaughtersTwice ) {
    // check if any of the ancestors is already stored
    G4int parentID = aTrack->GetTrackID();
    for(;;) {      
      if( theTracksStored[zstop].find(parentID) != theTracksStored[zstop].end() ) {
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpace_extUA bNotStoreDaughtersTwice REJECT " << parentID << G4endl; 
#endif
	return;
      }
      std::map<G4int,G4int>::iterator ite = theTrackParents.find(parentID);
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpace_extUA bNotStoreDaughtersTwice CHECKED " << parentID << " " << (*ite).second << G4endl; 
#endif
      parentID = (*ite).second;
      if( parentID == 0 ) break;
    }
    theTracksStored[zstop].insert(aTrack->GetTrackID());
  }
  if( bNotStoreTwice ) {
    theTracksStored[zstop].insert(aTrack->GetTrackID());
  }

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " StoreTrackRecord at Z " << zstop << G4endl;
#endif
  if(  theMaxNTracksInZ != -1 ) {
    if( theNEventsStored[zstop] > theMaxNTracksInZ ) return;
    theNEventsStored[zstop]++;
  }

  //int pdgCode = aTrack->GetDefinition()->GetPDGEncoding();

  //  int pdgCode = track->GetDefinition()->GetPDGEncoding();
  
  int pdgCode = aTrack->GetDefinition()->GetPDGEncoding();

  iaea_record_type * p_iaea_record = p_iaea_records[zstop];
  switch( pdgCode ){
  case 22:
    p_iaea_record->particle = 1; // gamma
    break;
  case 11:
    p_iaea_record->particle = 2; // electron
    break;
  case -11:
    p_iaea_record->particle = 3; // positron
    break;
  case 2112:
    p_iaea_record->particle = 4; // neutron
    break;
  case 2212:
    p_iaea_record->particle = 5; // proton
    break;
  default:
    G4Exception("RTPhaseSpace_extUA::StoreTrackRecord:",
		" particle code not supported ",
		JustWarning,
		G4String("particle: "+GmGenUtils::itoa(pdgCode)).c_str() );
    return;
  }

  p_iaea_record->energy = aTrack->GetKineticEnergy()/MeV;

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_extUA::StoreTrackRecord particle " << pdgCode << " E= " << p_iaea_record->energy << G4endl;
#endif
  
  G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
  p_iaea_record->x = pos.x()/CLHEP::cm;
  p_iaea_record->y = pos.y()/CLHEP::cm; 
  p_iaea_record->z = pos.z()/CLHEP::cm; 

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_extUA::StoreTrackRecord position " << pos.x()/CLHEP::cm << " " << pos.y()/CLHEP::cm << " " << pos.z()/CLHEP::cm << G4endl;
#endif
 
  G4ThreeVector momdir = aStep->GetPreStepPoint()->GetMomentumDirection(); 
  //   if(p_iaea_record->iu > 0)
  p_iaea_record->u = momdir.x(); 
  //   if(p_iaea_record->iv > 0)
  p_iaea_record->v = momdir.y(); 
  //   if(p_iaea_record->iw > 0)
  //p_iaea_record->w = fabs(momdir.z())/momdir.z();
  //  G4double charge = aTrack->GetDefinition()->GetPDGCharge();
  p_iaea_record->w = momdir.z();
  //  if( charge != 0 ) p_iaea_record->w *= charge/fabs(charge);
  if( p_iaea_record->w != 0 ) p_iaea_record->iw = G4int(p_iaea_record->w/fabs(p_iaea_record->w));

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_extUA::StoreTrackRecord direction " <<  momdir.x() << " " << momdir.y() << " " << momdir.z() << " iw " <<  p_iaea_record->iw << G4endl;
#endif

  //   if(p_iaea_record->iweight > 0)
  p_iaea_record->weight = aStep->GetPreStepPoint()->GetWeight();

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_extUA::StoreTrackRecord weight " <<   p_iaea_record->weight << G4endl;
#endif

  if( bFirstTrackInEvent.find(zstop) == bFirstTrackInEvent.end() ) {
    p_iaea_record->IsNewHistory = 1;
    bFirstTrackInEvent.insert(zstop);
  } else {
    p_iaea_record->IsNewHistory = 0;
  }

  p_iaea_record->iextralong = theNExtraInfoLong;
  p_iaea_record->iextrafloat = theNExtraInfoFloat;

  for( G4int ii = 0; ii < theNExtraInfoLong; ii++ ){
    p_iaea_record->extralong[ii] = theExtraInfoLongs[ii]->to_ulong();
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_extUA::StoreTrackRecord ExtraInfoLong " << ii << " = " << p_iaea_record->extralong[ii]  << G4endl;
#endif
  } 

  for( G4int ii = 0; ii < theNExtraInfoFloat; ii++ ){
    // recheck ZLast 
    std::vector<RTVExtraInfoProviderFloat*> eiFloat = RTExtraInfoMgr::GetInstance()->GetProvidersFloat();
    std::vector<RTVExtraInfoProviderFloat*>::const_iterator iteei;
    for( iteei = eiFloat.begin(); iteei != eiFloat.end(); iteei++ ){
      (*iteei)->CheckZStop(zstop);
    }
    p_iaea_record->extrafloat[ii] = *(theExtraInfoFloats[ii]);
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_extUA::StoreTrackRecord ExtraInfoFloat " << ii << " = " << p_iaea_record->extrafloat[ii]  << G4endl;
#endif
  } 

  p_iaea_record->write_particle();

  // update counters
  iaea_header_type * p_iaea_header = p_iaea_headers[zstop];
  p_iaea_header->update_counters(p_iaea_record);

  if( bHistos ) theHistos->FillHistos( p_iaea_record, zstop, aStep );
}

#include "RTExtraInfoMgr.hh"
#include "RTVExtraInfoProvider.hh"

//---------------------------------------------------------------------
void RTPhaseSpace_extUA::BeginOfRunAction(const G4Run*)
{
  RTExtraInfoMgr * theExtraInfoMgr = RTExtraInfoMgr::GetInstance();
  std::vector<bs32*> eiwordsL = theExtraInfoMgr->GetWordsLong();
  theNExtraInfoLong = 0;
  for( unsigned int jj = 0; jj < eiwordsL.size();jj++ ){
    theExtraInfoLongs.push_back(eiwordsL[jj]);
    theNExtraInfoLong++;
  }

  std::vector<float*> eiwordsF = theExtraInfoMgr->GetWordsFloat();
  theNExtraInfoFloat = 0;
  for( unsigned int jj = 0; jj < eiwordsF.size();jj++ ){
    theExtraInfoFloats.push_back(eiwordsF[jj]);
    theNExtraInfoFloat++;
  }

  CreateIAEAheaders();

}

//---------------------------------------------------------------------
void RTPhaseSpace_extUA::BeginOfEventAction(const G4Event*)
{
  bFirstTrackInEvent.clear();
}

//---------------------------------------------------------------------
void RTPhaseSpace_extUA::EndOfEventAction(const G4Event* evt)
{
  // ---- loop to stored particles to get rid of particles passing twice by plane ??
  if( theNEventsToSave > 0 && (evt->GetEventID()+1)%G4int(theNEventsToSave) == 0 
      && evt->GetEventID()+1 != G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed() ) {
    G4double nevents = (GmNumberOfEvent::GetNumberOfEvent() + 1); // at the EndOfEvent the number of events in run is not updated
    CloseIAEAHeaderFiles( nevents );
  } 
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout <<" RTPhaseSpace_extUA::EndOfEventAction " << theNEventsToSave << " " << G4int(evt->GetEventID()+1)%G4int(theNEventsToSave) << " " << evt->GetEventID() << "  =? " <<  GmNumberOfEvent::GetNumberOfEvent() <<G4endl; 
#endif

  theTracksStored.clear();

}


//---------------------------------------------------------------------
int RTPhaseSpace_extUA::CloseIAEAFiles()
{
  G4double nevents = GmNumberOfEvent::GetNumberOfEvent();
  CloseIAEAHeaderFiles(nevents);
  CloseIAEARecordFiles();
  return 0;
}


//---------------------------------------------------------------------
int RTPhaseSpace_extUA::CloseIAEAHeaderFiles(G4int nevents)
{

  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
    G4double zstop = theZStops[ii];
    // Counters for phsp file
    
    iaea_header_type * p_iaea_header = p_iaea_headers[zstop];
    
    p_iaea_header->orig_histories = nevents;

#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << "RTPhaseSpace_extUA::CloseIAEAFiles() Number of Events: " << nevents << "  Number of records: " << p_iaea_header->nParticles << G4endl;
#endif

    //    printf(  " Multiple passers discarded %d \n",ndiscarded);
    
    strcpy(p_iaea_header->title,
	   " TEST PHASESPACE for IAEA format (random numbers) ");   
    
    p_iaea_header->iaea_index = 0;    // Assigning IAEA index to the phasespace
    
    // Saving IAEA header file 
    //p_iaea_header->fheader = open_file("test",".IAEAheader","wb");
    
    char L_fname[120];
    G4String fname = theFileName;
    if( theZStops.size() != 1 ) fname += "_" + GmGenUtils::ftoa(zstop);
    strcpy(L_fname,fname.c_str());

    p_iaea_header->fheader = open_file(L_fname,(char *)(".IAEAheader"),(char *)("wb"));
    
    /* Write an IAEA header */
    if( p_iaea_header->write_header() != OK)
      { printf("\n ERROR: writing IAEA header"); return(FAIL); }  
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << " Phase Space header file: " << L_fname << ".IAEAheader" << G4endl;
#endif

    fclose(p_iaea_header->fheader);
  }
  
   return(OK);
}

//---------------------------------------------------------------------
int RTPhaseSpace_extUA::CloseIAEARecordFiles()
{
  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
    G4double zstop = theZStops[ii];
    
    iaea_record_type * p_iaea_record = p_iaea_records[zstop];
    fclose(p_iaea_record->p_file);
  }
  
  return (OK);
}



RTPhaseSpace_extUA::~RTPhaseSpace_extUA()
{
  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
    double zstop = theZStops[ii];
    free (p_iaea_headers[zstop]);

    iaea_record_type * p_iaea_record = p_iaea_records[zstop];
    free(p_iaea_record); 
  }
}

