#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "globals.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include <vector>

#include "RTGeneratorPhaseSpace_EGS.hh"
#include "RTExtraInfoMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "iaea_header.hh"
#include "iaea_record.hh"
#include "iaea_utilities.hh"
#include "RTVerbosity.hh"
#include "RTPhaseSpaceHistos.hh"
#include "EGSPhspParticle.hh"

//------------------------------------------------------------------------
RTGeneratorPhaseSpace_EGS::RTGeneratorPhaseSpace_EGS() 
  : RTGeneratorPhaseSpace("NO_INIT")
{
  Init();
}

void RTGeneratorPhaseSpace_EGS::Init()
{

  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 

  theFileName = parmgr->GetStringValue("RTGeneratorPhaseSpace_EGS:FileName", "test");

  theMaxNReuse = G4int(parmgr->GetNumericValue("RTGeneratorPhaseSpace_EGS:MaxNReuse", -1));
  theNReused = 0;
  theMaxNRecycle = G4int(parmgr->GetNumericValue("RTGeneratorPhaseSpace_EGS:MaxNRecycle", 1));
  theNRecycled = 1;
  if( theMaxNReuse == 0 ){
    G4Exception("RTGeneratorPhaseSpace_EGS::RTGeneratorPhaseSpace_EGS",
		"MaxNReuse has wrong value",
		FatalErrorInArgument,
		"MaxNReuse is 0, the minimum value is 1");
  }

  if( theMaxNReuse == -1 ){ //set it automatically for each run
    bMaxNReuseAutomatic = TRUE;
  } else {
    bMaxNReuseAutomatic = FALSE;
  }

  G4String strMirrorWhenReuse = parmgr->GetStringValue("RTGeneratorPhaseSpace_EGS:MirrorWhenReuse", "");
  if( strMirrorWhenReuse == "" ) {
    MirrorWhenReuse = NOMir;
  } else if( strMirrorWhenReuse == "X" ){
    MirrorWhenReuse = MirX;
  } else if( strMirrorWhenReuse == "Y" ){
    MirrorWhenReuse = MirY;
  } else if( strMirrorWhenReuse == "XY" ){
    MirrorWhenReuse = MirXY;
  } else {
    G4Exception("RTGeneratorPhaseSpace_EGS::RTGeneratorPhaseSpace_EGS()",
		"Wrong value of MirrorWhenReuse",
		FatalErrorInArgument,
		G4String("It can be X, Y, XY, it is = " + strMirrorWhenReuse).c_str());
  }

  std::vector<G4double> inidisp = parmgr->GetVNumericValue("RTGeneratorPhaseSpace_EGS:InitialDisplacement", std::vector<G4double>());
  if( inidisp.size() == 0 ){
    theInitialDisp = G4ThreeVector(0.,0.,0.);
  } else if( inidisp.size() == 3 ){
    theInitialDisp = G4ThreeVector(inidisp[0],inidisp[1],inidisp[2]);
  } else {
    G4Exception("RTGeneratorPhaseSpace_EGS",
		"Error in /gamos/setParam RTGeneratorPhaseSpace_EGS:InitialDisplacement",
		FatalErrorInArgument,
		G4String("it must have 3 arguments: POS_X POS_Y POS_Z, and it has "+GmGenUtils::itoa(inidisp.size())).c_str());
  }

  std::vector<G4double> inirot = parmgr->GetVNumericValue("RTGeneratorPhaseSpace_EGS:InitialRotAngles", std::vector<G4double>());
  if( inirot.size() == 0 ){
    theInitialRotAngleX = 0.;
    theInitialRotAngleY = 0.;
    theInitialRotAngleZ = 0.;
  } else if( inirot.size() == 3 ){
    theInitialRotAngleX = inirot[0];
    theInitialRotAngleY = inirot[1];
    theInitialRotAngleZ = inirot[2];
  } else {
    G4Exception("RTGeneratorPhaseSpace_EGS",
		"Error in /gamos/setParam RTGeneratorPhaseSpace_EGS:InitialRotAngles",
		FatalErrorInArgument,
		G4String("It must have 3 arguments: ANG_X ANG_Y ANG_Z, and it has "+GmGenUtils::itoa(inirot.size())).c_str());
  }

  std::vector<G4String> strs;
  strs = parmgr->GetVStringValue("RTGeneratorPhaseSpace_EGS:Transformations",strs);
  if( strs.size()%4 != 0 ) {
    G4Exception("RTGeneratorPhaseSpace_EGS",
		"Error in number of arguments of parameter RTGeneratorPhaseSpace_EGS:Transformation",
 		FatalException,
		(G4String("Number of arguments should be 4*n, it is ")+GmGenUtils::itoa(strs.size())).c_str());
  }
  
  transformation_struct current_T;
  
  theTransformations.clear();
  for(unsigned int ii=0; ii< strs.size(); ii+=4){
    if(strs[ii]=="D"){ current_T.operation=TTDisplacement;}
    else if (strs[ii]=="RXYZ"){ current_T.operation=TTRotateXYZ;}
    else if (strs[ii]=="RTPS"){ current_T.operation=TTRotateThetaPhiSelf;}	
    else { 
      G4Exception("RTGeneratorPhaseSpace_EGS",
		  "Error in transformation type of parameter RTGeneratorPhaseSpace_EGS:Transformation",
		  FatalException,
		  G4String("it is must be D, RXYZ or RTPS on the argument number "+GmGenUtils::itoa(ii+1)+" it is now "+strs[ii]).c_str());
    }
    current_T.opx=GmGenUtils::GetValue(strs[ii+1]);
    current_T.opy=GmGenUtils::GetValue(strs[ii+2]);
    current_T.opz=GmGenUtils::GetValue(strs[ii+3]);
    theTransformations.push_back(current_T);
  }

  theNEventsSkip = parmgr->GetNumericValue("RTGeneratorPhaseSpace_EGS:NEventsSkip", 0);

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) )
    G4cout << " READING FILE " << theFileName << G4endl;
#endif
  thePhspfile = fopen(theFileName.c_str(), "rb");	
  if( !thePhspfile ) {
    G4Exception("RTGeneratorPhaseSpace_EGS:",
		"",
		FatalErrorInArgument,
		("FILE NOT FOUND: " + theFileName).c_str());
  }
  char mode[6];
  fread(mode,5*sizeof(char),1,thePhspfile);
  mode[5]='\0';
  
  int mode_n;
  if(mode[4] == '0')
    mode_n = 0;
  else
    mode_n = 2;

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) )
    std::cout << " MODE = " << mode[4] << " => " << mode_n << std::endl;
#endif

  //--- Read header  
  G4int nParticles;
  G4int nPhotons;
  float maximumKineticEnergy, minimumKineticEnergy;
  float nIncident;
  fread(&nParticles,sizeof(int),1,thePhspfile);
  fread(&nPhotons,sizeof(int),1,thePhspfile);
  std::cout << " N_PARTICLES " << nParticles
	    << " N_PHOTON S " << nPhotons
	    << std::endl;
  fread(&maximumKineticEnergy,sizeof(float),1,thePhspfile);
  fread(&minimumKineticEnergy,sizeof(float),1,thePhspfile);
  fread(&nIncident,sizeof(float),1,thePhspfile);
  std::cout << " MAX_KINE " << maximumKineticEnergy 
	    << " MIN_KINE " << minimumKineticEnergy 
	    << " N_INCIDENT " << nIncident 
	    << std::endl;

  //---- Initialize with header info
  theNEventsInFile = nParticles;

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << " NPART " << nParticles << " " << nPhotons << G4endl;
  //a_header->particle_number[0] << " " << p_iaea_header->particle_number[1]  << " " << p_iaea_header->particle_number[2] << " STAT "  << " " << p_iaea_header->minimumX << " " << p_iaea_header->maximumZ  << " " << p_iaea_header->minimumY << " " << p_iaea_header->maximumY << G4endl; 
#endif

  // theNOrigEvents = p_iaea_header->orig_histories;
  theNOrigEvents = nIncident;
  
  GmNumberOfEvent::SetNumberOfEventFactor( theNOrigEvents / theNEventsInFile );  // temporary: it does not take into account the weights of the particles 
  //- GmNumberOfEvent::SetNumberOfEventFactor( 1. );  // temporary: it does not take into account the weights of the particles 
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) )  G4cout << "RTGeneratorPhaseSpace_EGS::GeneratePrimaries   SetNumberOfEventFactor " << theNOrigEvents / theNEventsInFile  << " " << G4double(theNOrigEvents) << " " << theNEventsInFile << G4endl;
#endif
  if( theNEventsInFile <= theNEventsSkip ){
    //    G4Exception("RTGeneratorPhaseSpace_EGS:","Asking to skip more events that the number of events in file ",FatalErrorInArgument,G4String("NEventsInFile= " + GmGenUtils::itoa(theNEventsInFile) + " <= NEventsSkip= " + GmGenUtils::itoa(theNEventsSkip)).c_str());
    G4Exception("RTGeneratorPhaseSpace_EGS:",
		"Asking to skip more events that the number of events in file ",
		JustWarning,
		G4String("NEventsInFile= " + GmGenUtils::itoa(theNEventsInFile) + " <= NEventsSkip= " + GmGenUtils::ftoa(theNEventsSkip)).c_str());
    theNEventsSkip -= int(theNEventsSkip/theNEventsInFile)*theNEventsInFile; 
  }

  theExtraInfoMgr = RTExtraInfoMgr::GetInstance();
  //  theExtraInfoMgr->InitExtraInfo( p_iaea_read );
      
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << "RTGeneratorPhaseSpace_EGS::RTGeneratorPhaseSpace_EGS  n events in file " <<nParticles << G4endl;
#endif

  GmNumberOfEvent::SetNumberOfEvent( 0 );

  bHistos = G4bool(parmgr->GetNumericValue("RTGeneratorPhaseSpace_EGS:Histos", 0));

  if( bHistos ) theHistos = new RTPhaseSpaceHistos(0. ); 

  theFilePosZ = parmgr->GetNumericValue("RTGeneratorPhaseSpace_EGS:FilePosZ", DBL_MAX);
  if( theFilePosZ == DBL_MAX ) {
    G4Exception("RTGeneratorPhaseSpace_EGS::Init",
		"",
		JustWarning,
		"No parameter RTGeneratorPhaseSpace_EGS:FilePosZ defined, it will be set to 0.");
    theFilePosZ = 0.;
  }

  //  GmUserActionMgr::GetInstance()->RegisterTrackingAction(this);

  theNNewEvents = 0;
  theNParticlesRead = 0.;

  theEGSParticle = new EGSPhspParticle(thePhspfile,mode);
  theEGSParticle->SetPosZ(theFilePosZ);


}


//------------------------------------------------------------------------
RTGeneratorPhaseSpace_EGS::~RTGeneratorPhaseSpace_EGS()
{
  //  theFileIn->close();
}

//------------------------------------------------------------------------
void RTGeneratorPhaseSpace_EGS::GeneratePrimaries(G4Event* evt)
{
  if( bMaxNReuseAutomatic && evt->GetEventID() == 0 ) {
    //set MaxNReuse, MaxNRecycle automatically for each run
    theMaxNReuse = G4int( (G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed()-1) / theNEventsInFile ) + 1;
    theMaxNRecycle = 1; 
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout<< " RTGeneratorPhaseSpace_EGS::GeneratePrimaries automatic NReuse= " << theMaxNReuse << G4endl;
#endif
  }
  /*  if( evt->GetEventID() > maxNEvents ) {
    //    G4Exception("RTGeneratorPhaseSpace_EGS::GeneratePrimaries: more events than those on file = " + GmGenUtils::itoa(maxNEvents) );
    }*/


  //skip first particles
  if( evt->GetEventID() == 0 ) {
    for( G4int ii = 0; ii < theNEventsSkip; ii++ ){
      int fileEnd = theEGSParticle->RefreshParticle(thePhspfile,ii+1);
      if(fileEnd){
	G4Exception("RTGeneratorPhaseSpace_EGS::GeneratePrimaries",
		    "",
		    FatalException,
		    ("Skipping more particles thatn those existing in file : " + GmGenUtils::itoa(ii)).c_str());
	return;
      }
    }
  }

  //---- Reuse the same particle theMaxNReuse times  !! do not reuse charged particles
  if( theNReused == 0 || !theParticleDef || theParticleDef->GetPDGCharge() != 0 ) { // first time theNReused = 0, after theMaxNReuse times 
    //    G4bool bFilter = (theBase.size() == 0);
    //    do {
    int fileEnd = theEGSParticle->RefreshParticle(thePhspfile,theNParticlesRead+1);
    //    std::cout << irecord << " PARTICLE " << particle->theLatch 
    //	      << " " << particle->theEnergy << " " << particle->thePosX << " " << particle->thePosY << " " << particle->theDirU << " " << particle->theDirV << " " << particle->theWeight << std::endl;
    if(!fileEnd){
      /*  float xcorr = (particle->thePosX - theXmin)/theDeltaX;
	  float ycorr = (particle->thePosY - theYmin)/theDeltaY;
	  if(particle->theCharge == 0){
	  count_photons++;
	  }*/
      theNParticlesRead ++;
    } else {
    
    //t    theExtraInfoMgr->FillExtraInfo( p_iaea_read );
    //      if( theBase.size() != 0 ) bFilter = theBase[0]->AcceptStep((G4Step*)0);
      if( theNRecycled < theMaxNRecycle ) {
	fclose(thePhspfile);
	thePhspfile = fopen(theFileName.c_str(), "rb");	
#ifndef GAMOS_NO_VERBOSE
	if( RTVerb(infoVerb) ) G4cout << "RTGeneratorPhaseSpace_EGS::GeneratePrimaries  RECYCLING phase space file " << theNRecycled << G4endl;
#endif
	theNRecycled++; 
	  //skip first particles
	for( G4int ii = 0; ii < theNEventsSkip; ii++ ){
	  theEGSParticle->RefreshParticle(thePhspfile,ii+1);
	}
      } else{
	fclose(thePhspfile);
	G4RunManager::GetRunManager()->AbortRun();
	G4Exception("RTGeneratorPhaseSpace_EGS::GeneratePrimaries",
		    "Too many events demanded",
		    JustWarning,
		    G4String("FAILED reading event  " + GmGenUtils::itoa(evt->GetEventID())).c_str() );
	return;
      }
    }
    //    }while( !bFilter );
  }
  
  if( bHistos ) theHistos->FillHistos( theEGSParticle, 0 );

  //---- Store number of event
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite;
  std::map<G4String,GmVPrimitiveScorer*> scorers = GmScoringMgr::GetInstance()->GetScorers();
  //t  G4bool bNewEvent = p_iaea_read->IsNewHistory;
  G4bool bNewEvent = true;
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpace_EGS::GeneratePrimaries bNewEvent " << bNewEvent << " NReused " << theNReused << G4endl;
#endif
  bNewEvent = bNewEvent && (theNReused == 0); // first time theNReused = 0, after theMaxNReuse times 
  for( ite = scorers.begin(); ite != scorers.end(); ite++ ){
    (*ite).second->SetNewEvent( bNewEvent );
  }
  if( bNewEvent ) {
    if( theNNewEvents != 0 ) {
      //-- Rescale by  number of original events that correspond to them when phase space file was created (NOrigRead) / number of events read (theNNewEvents)
      //-- But we only now total number of original events (NOrigTotal). Then assume the ratio NOrigRead/NOrigTotal = number of particles read / total number of particles in file
      GmNumberOfEvent::SetNumberOfEventFactor( ( theNOrigEvents * (theNParticlesRead-1) / theNEventsInFile ) / theNNewEvents );
      //      GmNumberOfEvent::SetNumberOfEventFactor( 1. );
					       
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpace_EGS::GeneratePrimaries SetNumberOfEventFactor "
				   << ( theNOrigEvents * (theNParticlesRead-1) / theNEventsInFile ) / theNNewEvents
				   << " theNNewEvents= "<< theNNewEvents
				   << " theNOrigEvents= " << theNOrigEvents
				   << " theNParticlesRead-1 " << theNParticlesRead-1 
				   << " theNEventsInFile " << theNEventsInFile
				   << G4endl;
#endif
    }
    theNNewEvents++;
    GmNumberOfEvent::AddEvent();
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpace_EGS new event " << GmNumberOfEvent::GetNumberOfEvent() << G4endl;
#endif
  }

  G4int hid = theEGSParticle->theCharge;

  switch( hid ) {
  case 0:
    theParticleDef = G4Gamma::Gamma();
    break;
  case -1:
    theParticleDef = G4Electron::Electron();
    break;
  case +1:
    theParticleDef = G4Positron::Positron();
    break;
  default:
    G4Exception("RTGeneratorPhaseSpace_EGS::GeneratePrimaries",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Particle charge not supported " + GmGenUtils::itoa( hid)).c_str());
  }

  G4PrimaryVertex* vtx;
  G4PrimaryParticle* particle;
  //  G4double momdirz = sqrt(1. - theEGSParticle->u* theEGSParticle->u + theEGSParticle->v* theEGSParticle->v );
  //t  G4double momdirz = theEGSParticle->theDirW;

  G4double mom = sqrt( theEGSParticle->GetKineticEnergy()*theEGSParticle->GetKineticEnergy() + 2.*theEGSParticle->GetKineticEnergy()*theParticleDef->GetPDGMass() );
  
  G4ThreeVector pos;  
  G4int nMirror = 0;
  switch ( MirrorWhenReuse ){
    case NOMir:
      nMirror = 0;
      break;
    case MirX:
      nMirror = theNReused%2;
      break;
    case MirY:
      nMirror = (theNReused%2) * 2;
      break;
    case MirXY:
      nMirror = theNReused%4;
      break;
    }
  if( nMirror < 0 ) nMirror = 0; // when theMaxNReuse = theNReuse + 1
  
  //for better reusing mirror the particle
  
  G4ThreeVector momV;
  switch (nMirror) {
  case 0: // no mirror
    pos = G4ThreeVector(theEGSParticle->thePosX,theEGSParticle->thePosY,theEGSParticle->thePosZ);
    momV = G4ThreeVector(theEGSParticle->theDirU*mom, theEGSParticle->theDirV*mom, theEGSParticle->theDirW*mom);
    //t momV = G4ThreeVector(theEGSParticle->theDirU*mom, theEGSParticle->theDirV*mom, momdirz*mom);
    break;
  case 1: // mirror X
    pos = G4ThreeVector(-theEGSParticle->thePosX,theEGSParticle->thePosY,theEGSParticle->thePosZ);
    momV = G4ThreeVector(-theEGSParticle->theDirU*mom, theEGSParticle->theDirV*mom, theEGSParticle->theDirW*mom);
    break;
    case 2: // mirror Y
      pos = G4ThreeVector(theEGSParticle->thePosX,-theEGSParticle->thePosY,theEGSParticle->thePosZ);
      momV = G4ThreeVector(theEGSParticle->theDirU*mom, -theEGSParticle->theDirV*mom, theEGSParticle->theDirW*mom);
      break;
    case 3: // mirror X & Y
      pos = G4ThreeVector(-theEGSParticle->thePosX,-theEGSParticle->thePosY,theEGSParticle->thePosZ);
      momV = G4ThreeVector(-theEGSParticle->theDirU*mom, -theEGSParticle->theDirV*mom, theEGSParticle->theDirW*mom);
      break;
    }
//  pos *= CLHEP::cm;
  
  momV.rotateX( theInitialRotAngleX);
  momV.rotateY( theInitialRotAngleY);
  momV.rotateZ( theInitialRotAngleZ);
  
  pos.rotateX( theInitialRotAngleX);
  pos.rotateY( theInitialRotAngleY);
  pos.rotateZ( theInitialRotAngleZ);
  
  pos += theInitialDisp;

  
  G4ThreeVector Dpos;
  
  std::vector<transformation_struct>::const_iterator itet;
  for( itet = theTransformations.begin(); itet != theTransformations.end(); itet++) {
      transformation_struct current_t = *itet;
      switch (current_t.operation){
      
    case TTDisplacement:
      Dpos=G4ThreeVector(current_t.opx,current_t.opy,current_t.opz);
      
      pos +=Dpos;
      break;
      
    case TTRotateXYZ:
      momV.rotateX( current_t.opx );
      momV.rotateY( current_t.opy );
      momV.rotateZ( current_t.opz ); 
      pos.rotateX( current_t.opx );
      pos.rotateY( current_t.opy );
      pos.rotateZ( current_t.opz );
      break;
    case TTRotateThetaPhiSelf:
      momV.rotateZ( current_t.opz );
      momV.rotateY( current_t.opx );
      momV.rotateZ( current_t.opy ); 
      
      pos.rotateZ( current_t.opz );
      pos.rotateY( current_t.opx );
      pos.rotateZ( current_t.opy );
      
      break;
      
    } 
    
  }
  
  particle = new G4PrimaryParticle( theParticleDef, momV.x(), momV.y(), momV.z() );
  particle->SetWeight( theEGSParticle->theWeight / theMaxNReuse );  
  
  //  if( theParticleDef->GetPDGCharge() == 0 ){
  theNReused = (theNReused+1)%theMaxNReuse;
  //  }
  
  vtx = new G4PrimaryVertex( pos, 0. );
  vtx->SetPrimary(particle);
  
  evt->AddPrimaryVertex( vtx );
 
    }
