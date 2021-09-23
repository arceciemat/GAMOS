#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#include <math.h>

#include "RTPhaseSpace_EGSUA.hh"
#include "EGSPhspParticle.hh"
//#include "RTPhaseSpaceHistos.hh"
#include "RTVExtraInfoProviderFloat.hh"
#include "RTExtraInfoMgr.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "RadioTherapy/include/RTVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "G4GeometryTolerance.hh"

//---------------------------------------------------------------------
RTPhaseSpace_EGSUA::RTPhaseSpace_EGSUA()
{

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  
  G4String foutName = paramMgr->GetStringValue("RTPhaseSpace_EGSUA:TextFileName","");
  fileOut = 0;
  if( foutName != "" ) fileOut = new std::ofstream(foutName);

  theFileName = paramMgr->GetStringValue("RTPhaseSpace_EGSUA:FileName","test");
  bZStopInFileName = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:ZStopInFileName", 0 ));

  theZStops = paramMgr->GetVNumericValue("RTPhaseSpace_EGSUA:ZStops", std::vector<G4double>()  );
  std::sort(theZStops.begin(),theZStops.end());
  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) )  G4cout << " ZSTOP : " << theZStops[ii] << G4endl;
#endif
  }

  //order Z stops for faster searching
  
  bKillAfterLastZStop = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:KillAfterLastZStop", 0 ));

  bStoreZ = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:StoreZ", FALSE ));

  /*  bHistos = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:Histos", TRUE ));

  if( bHistos ) {
    theHistos = new RTPhaseSpaceHistos(this); 
    theHistos->SetClassifier( theClassifier );
    }*/

  theNEventsToSave = G4int(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:NEventsToSave", -1 ));

  theMaxNTracksInZ = G4int(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:MaxNTracksInFile", -1 ));

  bNotStoreBackwards = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:NotStoreBackwards", 1 ));
  G4cout << " bNotStoreBackwards " << bNotStoreBackwards << G4endl; //GDEB

  bNotStoreTwice = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:NotStoreTwice", TRUE ));

  bNotStoreDaughtersTwice = G4bool(paramMgr->GetNumericValue("RTPhaseSpace_EGSUA:NotStoreDaughtersTwice", TRUE ));

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << " bStoreZ " << bStoreZ << G4endl;
#endif

}


//---------------------------------------------------------------------
G4bool RTPhaseSpace_EGSUA::CreateHeaders()
{
  G4bool isOK = TRUE;
  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
    //    G4cout << " call CreateHeader " << ii << G4endl;
    G4bool isOKnew = CreateHeader( theZStops[ii] );
    isOK = isOK && isOKnew;
  }
  return isOK;
}

//---------------------------------------------------------------------
G4bool RTPhaseSpace_EGSUA::CreateHeader(G4double zstop)
{

  //  G4cout << G4endl << " RTPhaseSpace_EGSUA::CreateHeader" << zstop << G4endl;
 
  // Creating phsp header and allocating memory for it
  //---- check that z stops are not repeated
  if( thePhspFiles.find( zstop ) != thePhspFiles.end() ) {
    G4Exception("RTPhaseSpace_EGSUA::CreateHeader ",
		"Repeated Zstop ",
		FatalErrorInArgument,
		(" Zstp VALUE = " + GmGenUtils::ftoa(zstop)).c_str());
  }

  char fphsp[81]={'\0'};;
  G4String fname = theFileName;
  if( theZStops.size() != 1 || bZStopInFileName ) fname += "_" + GmGenUtils::ftoa(zstop);
  strcpy(fphsp,fname.c_str());
  FILE* phspFile = fopen(fphsp, "wb");
  thePhspFiles[zstop] = phspFile;

  /* It will be written at the end of run
  float maximumKineticEnergy,minimumKineticEnergy,nIncident;
  int nParticles, nPhotons;

  fwrite(&nParticles,sizeof(int),1,thePhspfile);
  fwrite(&nPhotons,sizeof(int),1,thePhspfile);
  std::cout << " N_PARTICLES " << nParticles
	    << " N_PHOTON S " << nPhotons
	    << std::endl;
  fwrite(&maximumKineticEnergy,sizeof(float),1,thePhspfile);
  fwrite(&minimumKineticEnergy,sizeof(float),1,thePhspfile);
  fwrite(&nIncident,sizeof(float),1,thePhspfile);
 
  */
  
  // advance bytes in output file
  long filePointer_out = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3;
  //  long filePointer_out = 5+2*4+3*4+24;
  //  G4cout << " SIZE " << sizeof(int) << " " << sizeof(float) << G4endl; //GDEB
  fseek(phspFile,filePointer_out,SEEK_SET);
  return(OK);
}


//---------------------------------------------------------------------
void RTPhaseSpace_EGSUA::UserSteppingAction(const G4Step* aStep)
{ 
  preR= aStep->GetPreStepPoint()->GetPosition();
  R= aStep->GetPostStepPoint()->GetPosition();
  //  G4cout << " preR " << preR.z() << " R " << R.z() << G4endl; //GDEB
  //--- Check if it crosses Z plane
#ifndef BEAMZPOS
  if ( bNotStoreBackwards && aStep->GetTrack()->GetMomentumDirection().z() > 0. ) {
    G4cout << "WRONGDIR " << G4endl; //GDEB
    return; }
#else
  if ( bNotStoreBackwards && aStep->GetTrack()->GetMomentumDirection().z() < 0. ) {
    //G4cout << "WRONGDIR " << G4endl; //GDEB
    return;
  }
#endif
  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
    double zstop = theZStops[ii];
    //    G4cout << " zstop " << zstop << " "  << R.z() << " " << preR.z()-zstop << " * " << R.z()-zstop << " " <<  (preR.z()-zstop) * (R.z()-zstop) << " <? 0 " << G4endl; //GDEB
#ifndef BEAMZPOS
    if ( R.z() > zstop && bNotStoreBackwards ) {
      //G4cout << "SMALL RZ " << G4endl; //GDEB
      continue;
    }
#else
    if ( R.z() < zstop && bNotStoreBackwards ) continue;
#endif
    //   G4cout << " zstopISOK  " <<  (preR.z()-zstop) * (R.z()-zstop) << " <? 0 " << G4endl; //GDEB
    if ( (preR.z()-zstop) * (R.z()-zstop) < 0. 
       || fabs(R.z()-zstop) < G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() ){ // track reaches plane 
      
#ifndef BEAMZPOS
      if( bKillAfterLastZStop && ii == 0 ) aStep->GetTrack()->SetTrackStatus(fStopAndKill);
#else
      if( bKillAfterLastZStop && ii == theZStops.size()-1 ) aStep->GetTrack()->SetTrackStatus(fStopAndKill);
#endif
#ifndef GAMOS_NO_VERBOSE
      if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_EGSUA TRACK to PHASE SPACE  preR " << preR.z() << " R " << R.z() << " zstop " << zstop << G4endl;
#endif
      // const G4Track* track = aStep->GetTrack();
      //G4Track* tracknc = const_cast<G4Track*>(track);
      //tracknc->SetTrackStatus(fStopAndKill);
      //      if(  aStep->GetTrack()->GetParentID() == 1 ) {
      StoreTrackRecord( aStep, zstop );
      //      }
    };
  }
}


//---------------------------------------------------------------------
void RTPhaseSpace_EGSUA::StoreTrackRecord( const G4Step* aStep, G4double zstop)
{
  const G4Track* aTrack = aStep->GetTrack();
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpace_EGSUA::StoreTrackRecord " << zstop << " " << aStep->GetPreStepPoint()->GetPosition().z() << " " << aStep->GetPostStepPoint()->GetPosition().z() << G4endl;
#endif
  if( bNotStoreTwice ) {
    G4int trackID = aTrack->GetTrackID();
    if( theTracksStored[zstop].find(trackID) != theTracksStored[zstop].end() ) {
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) )  G4cout << " RTPhaseSpace_EGSUA bNotStoreTwice REJECT " << trackID << G4endl;
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
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpace_EGSUA bNotStoreDaughtersTwice REJECT " << parentID << G4endl; 
#endif
	return;
      }
      std::map<G4int,G4int>::iterator ite = theTrackParents.find(parentID);
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpace_EGSUA bNotStoreDaughtersTwice CHECKED " << parentID << " " << (*ite).second << G4endl; 
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
  G4double          E = aStep->GetPostStepPoint() ->GetKineticEnergy()/CLHEP::MeV;
  G4double       preE = aStep->GetPreStepPoint()  ->GetKineticEnergy()/CLHEP::MeV;

  EGSPhspParticle* egsPart = new EGSPhspParticle();
  int charge;
  float KineticEnergyMeV;
  float posX, posY, posZ;
  float dirU, dirV, dirW;
  float weight = 0.;
  switch( pdgCode ){
  case 22:
    charge = 0;
    KineticEnergyMeV=preE;
    break;
  case 11:
    charge = -1;
    KineticEnergyMeV = -(preE+(E-preE)*(zstop-preR.z())/(R.z()-preR.z())+CLHEP::electron_mass_c2);
    break;
  case -11:
    charge = 1;
    KineticEnergyMeV = preE+(E-preE)*(zstop-preR.z())/(R.z()-preR.z())+CLHEP::electron_mass_c2;
    break;
  default:
    G4Exception("RTPhaseSpace_EGSUA::StoreTrackRecord:",
		" particle code not supported ",
		JustWarning,
		G4String("particle: "+GmGenUtils::itoa(pdgCode)).c_str() );
    return;
  }
  /*  std::string slat;
  if( pdgCode == 11 ) {
    slat = "40000000";
  } else if( pdgCode == -11 ) {
    slat = "20000000";
  } else {
    slat = "00000000";
  }
  unsigned int latch = std::stoul(slat, nullptr, 32);
  egsPart->SetLatch(latch);
  std::cout << " LATCH " << latch << " : " << slat << G4endl; //GDEB
  */
  
  std::stringstream ss;
  if( pdgCode == 11 ) {
    ss << std::hex << "40000000";
  } else if( pdgCode == -11 ) {
  ss << std::hex << "20000000";
  } else {
    ss << std::hex << "00000000";
  }
  unsigned int latch;
  ss >> latch;
  egsPart->SetLatch(latch);
  //  std::cout << " LATCH " << latch << G4endl; //GDEB
  

  egsPart->SetCharge(charge);
  egsPart->SetKineticEnergy(KineticEnergyMeV);
  
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_EGSUA::StoreTrackRecord particle " << pdgCode << " E= " << KineticEnergyMeV << G4endl;
#endif
  
  posX = preR.x()+(R.x()-preR.x())*(zstop-preR.z())/(R.z()-preR.z());
  posY = preR.y()+(R.y()-preR.y())*(zstop-preR.z())/(R.z()-preR.z());
  posZ = zstop/CLHEP::cm;

  egsPart->SetPosition(posX,posY,posZ);
  
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_EGSUA::StoreTrackRecord position " << posX/CLHEP::cm << " " << posY/CLHEP::cm << " " << zstop/CLHEP::cm << G4endl;
#endif
  
  G4ThreeVector momdir = aStep->GetPreStepPoint()->GetMomentumDirection(); 
  dirU = momdir.x(); 
  dirV = momdir.y(); 
  dirW = momdir.z();
  egsPart->SetDirection(dirU,dirV,dirW);
  
#ifndef GAMOS_NO_VERBOSE
   if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_EGSUA::StoreTrackRecord direction " << momdir.x() << " " << momdir.y() << " " << momdir.z() << G4endl;
#endif

  weight = aStep->GetPreStepPoint()->GetWeight();
  egsPart->SetWeight(weight);
  
#ifndef GAMOS_NO_VERBOSE
   if( RTVerb(debugVerb) ) G4cout << "RTPhaseSpace_EGSUA::StoreTrackRecord weight " << weight << G4endl;
#endif
												 
  /*  for( float np = 1; np < 100; np++ ) {
    fwrite(&np,sizeof(float),1,thePhspFiles[zstop]);
    }*/
  
  egsPart->write_EGSPhspParticle(thePhspFiles[zstop]);
  
  //   if( bHistos ) theHistos->FillHistos( p_iaea_record, zstop, aStep );

  theNParticles[zstop]++;
  if( pdgCode == 22 ) theNPhotons[zstop]++;

  //  G4cout << zstop << " theNParticles " << theNParticles[zstop] << G4endl; //GDEB
  //  G4cout << zstop << " theNPhotons " << theNPhotons[zstop] << G4endl; //GDEB
  
  theMaxKinE[zstop] = std::max(float(theMaxKinE[zstop]),float(fabs(KineticEnergyMeV)));
  theMinKinE[zstop] = std::min(float(theMinKinE[zstop]),float(fabs(KineticEnergyMeV)));
}

#include "RTExtraInfoMgr.hh"
#include "RTVExtraInfoProvider.hh"

//---------------------------------------------------------------------
void RTPhaseSpace_EGSUA::BeginOfRunAction(const G4Run*)
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

  CreateHeaders();

}

//---------------------------------------------------------------------
void RTPhaseSpace_EGSUA::BeginOfEventAction(const G4Event*)
{
  bFirstTrackInEvent.clear();
}

//---------------------------------------------------------------------
void RTPhaseSpace_EGSUA::EndOfEventAction(const G4Event* evt)
{
  // ---- loop to stored particles to get rid of particles passing twice by plane ??
  if( theNEventsToSave > 0 && (evt->GetEventID()+1)%G4int(theNEventsToSave) == 0 
      && evt->GetEventID()+1 != G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed() ) {
     CloseFiles();
  } 
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout <<" RTPhaseSpace_EGSUA::EndOfEventAction " << theNEventsToSave << " " << G4int(evt->GetEventID()+1)%G4int(theNEventsToSave) << " " << evt->GetEventID() << "  =? " <<  GmNumberOfEvent::GetNumberOfEvent() <<G4endl; 
#endif

  theTracksStored.clear();

}


//---------------------------------------------------------------------
void  RTPhaseSpace_EGSUA::EndOfRunAction(const G4Run*)
{
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpace_EGSUA::EndOfRunAction " << GmNumberOfEvent::GetNumberOfEvent() <<G4endl; 
#endif

  CloseFiles();

  RTExtraInfoMgr::GetInstance()->DumpProviders();

}

//---------------------------------------------------------------------
int RTPhaseSpace_EGSUA::CloseFiles()
{

  for( unsigned int ii = 0; ii < theZStops.size(); ii++ ){
    G4double zstop = theZStops[ii];
    // Counters for phsp file

    FILE* phspFile = thePhspFiles[zstop];
    rewind(phspFile);
    //long filePointer;     fseek(phspFile,filePointer,SEEK_SET);

    char mode[6] = "MODE0";
    mode[5]='\0';
    fwrite(&mode,5*sizeof(char),1,phspFile);
    
    float maximumKineticEnergy,minimumKineticEnergy,nIncident;
    int nParticles, nPhotons;

    nParticles = theNParticles[zstop];
    nPhotons = theNPhotons[zstop];
    fwrite(&nParticles,sizeof(int),1,phspFile);
    fwrite(&nPhotons,sizeof(int),1,phspFile);
    nIncident = (GmNumberOfEvent::GetNumberOfEvent()); //?? at the EndOfEvent the number of events in run is not updated
    G4cout << " WRITE nParticles " << nParticles << " nPhotons " << nPhotons << " nIncident " << nIncident << G4endl; //GDEB
    maximumKineticEnergy = theMaxKinE[zstop];
    minimumKineticEnergy = theMinKinE[zstop];
    G4cout << " WRITE maximumKineticEnergy " << maximumKineticEnergy << " minimumKineticEnergy " << minimumKineticEnergy << G4endl; //GDEB
    fwrite(&maximumKineticEnergy,sizeof(float),1,phspFile);
    fwrite(&minimumKineticEnergy,sizeof(float),1,phspFile);
    fwrite(&nIncident,sizeof(float),1,phspFile);
    
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout << "RTPhaseSpace_EGSUA::CloseFiles() Number of Events: " << nIncident << G4endl;
#endif

  }
  
   return(OK);
}


RTPhaseSpace_EGSUA::~RTPhaseSpace_EGSUA()
{
}


void RTPhaseSpace_EGSUA::PreUserTrackingAction(const G4Track* aTrack)
{
  if( bNotStoreDaughtersTwice ) {
    theTrackParents[aTrack->GetTrackID()] = aTrack->GetParentID();
  }

}
