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
#include "G4EventManager.hh"
#include "G4Run.hh"
#include <vector>

#include "RTGeneratorPhaseSpaceRTPlan.hh"
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
#include "GamosCore/GamosGenerator/include/RTPlanSource.hh"

//------------------------------------------------------------------------
RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan( G4String )
{
}

//------------------------------------------------------------------------
RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan()
{
  Init();
}

//------------------------------------------------------------------------
void RTGeneratorPhaseSpaceRTPlan::Init()
{

  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 

  theFileName = parmgr->GetStringValue("RTGeneratorPhaseSpaceRTPlan:FileName", "test");
  if( theFileName.length() > 11 ) {
    if( theFileName.substr(theFileName.length()-11,11) == ".IAEAheader" ) theFileName = theFileName.substr(0,theFileName.length()-11);
  }

  theMaxNReuse = G4int(parmgr->GetNumericValue("RTGeneratorPhaseSpaceRTPlan:MaxNReuse", -1));
  theNReused = 0;
  theMaxNRecycle = G4int(parmgr->GetNumericValue("RTGeneratorPhaseSpaceRTPlan:MaxNRecycle", 1));
  theNRecycled = 1;
  if( theMaxNReuse == 0 ){
    G4Exception("RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan",
		"MaxNReuse has wrong value",
		FatalErrorInArgument,
		"MaxNReuse is 0, the minimum value is 1");
  }

  if( theMaxNReuse == -1 ){ //set it automatically for each run
    bMaxNReuseAutomatic = TRUE;
  } else {
    bMaxNReuseAutomatic = FALSE;
  }

  G4String strMirrorWhenReuse = parmgr->GetStringValue("RTGeneratorPhaseSpaceRTPlan:MirrorWhenReuse", "");
  if( strMirrorWhenReuse == "" ) {
    MirrorWhenReuse = NOMir;
  } else if( strMirrorWhenReuse == "X" ){
    MirrorWhenReuse = MirX;
  } else if( strMirrorWhenReuse == "Y" ){
    MirrorWhenReuse = MirY;
  } else if( strMirrorWhenReuse == "XY" ){
    MirrorWhenReuse = MirXY;
  } else {
    G4Exception("RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan()",
		"Wrong value of MirrorWhenReuse",
		FatalErrorInArgument,
		G4String("It can be X, Y, XY, it is = " + strMirrorWhenReuse).c_str());
  }

  std::vector<G4double> inidisp = parmgr->GetVNumericValue("RTGeneratorPhaseSpaceRTPlan:InitialDisplacement", std::vector<G4double>());
  if( inidisp.size() == 0 ){
  } else if( inidisp.size() == 3 ){
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"Error in /gamos/setParam RTGeneratorPhaseSpaceRTPlan:InitialDisplacement",
		JustWarning,
		"is not used");
  } else {
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"Error in /gamos/setParam RTGeneratorPhaseSpaceRTPlan:InitialDisplacement",
		FatalErrorInArgument,
		G4String("it must have 3 arguments: POS_X POS_Y POS_Z, and it has "+GmGenUtils::itoa(inidisp.size())).c_str());
  }

  std::vector<G4double> inirot = parmgr->GetVNumericValue("RTGeneratorPhaseSpaceRTPlan:InitialRotAngles", std::vector<G4double>());
  if( inirot.size() == 0 ){
  } else if( inirot.size() == 3 ){
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"Error in /gamos/setParam RTGeneratorPhaseSpaceRTPlan:InitialRotAngles",
		FatalErrorInArgument,
		"Is not used");
  } else {
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"Error in /gamos/setParam RTGeneratorPhaseSpaceRTPlan:InitialRotAngles",
		FatalErrorInArgument,
		G4String("It must have 3 arguments: ANG_X ANG_Y ANG_Z, and it has "+GmGenUtils::itoa(inirot.size())).c_str());
  }

  inidisp = parmgr->GetVNumericValue("RTGeneratorPhaseSpaceRTPlan:OriginalBeamDisplacement", std::vector<G4double>());
  if( inidisp.size() == 0 ) {
    theOriginalBeamDisplacement = G4ThreeVector(0.,0.,0.);
  } else if( inidisp.size() == 3 ){
    theOriginalBeamDisplacement = G4ThreeVector(inidisp[0],inidisp[1],inidisp[2]);
  } else {
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"Error in /gamos/setParam RTGeneratorPhaseSpaceRTPlan:OriginalBeamDisplacement",
		FatalErrorInArgument,
		G4String("it must have 3 arguments: POS_X POS_Y POS_Z, and it has "+GmGenUtils::itoa(inidisp.size())).c_str());
  }

  inirot = parmgr->GetVNumericValue("RTGeneratorPhaseSpaceRTPlan:OriginalBeamRotAngles", std::vector<G4double>());
  if( inirot.size() == 0 ) {
    theOriginalBeamRotAngleX = 0.;
    theOriginalBeamRotAngleY = 0.;
    theOriginalBeamRotAngleZ = 0.;
  } else if( inirot.size() == 3 ){
    theOriginalBeamRotAngleX = inirot[0];
    theOriginalBeamRotAngleY = inirot[1];
    theOriginalBeamRotAngleZ = inirot[2];
  } else {
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"Error in /gamos/setParam RTGeneratorPhaseSpaceRTPlan:OriginalBeamRotAngles",
		FatalErrorInArgument,
		G4String("it must have 3 arguments: POS_X POS_Y POS_Z, and it has "+GmGenUtils::itoa(inirot.size())).c_str());
  }

  bOriginalBeamInvert = G4bool(parmgr->GetNumericValue("RTGeneratorPhaseSpaceRTPlan:OriginalBeamInvert",0));
  if( bOriginalBeamInvert && inirot.size() != 0 ) {
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"",
		FatalErrorInArgument,
		"Error: /gamos/setParam RTGeneratorPhaseSpaceRTPlan:OriginalBeamRotAngles and /gamos/setParam RTGeneratorPhaseSpaceRTPlan:OriginalBeamInvert cannot be used together");
  }
  
  std::vector<G4String> strs;
  strs = parmgr->GetVStringValue("RTGeneratorPhaseSpaceRTPlan:Transformations",strs);
  if( strs.size()%4 != 0 ) {
    G4Exception("RTGeneratorPhaseSpaceRTPlan",
		"Error in number of arguments of parameter RTGeneratorPhaseSpaceRTPlan:Transformation",
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
      G4Exception("RTGeneratorPhaseSpaceRTPlan",
		  "Error in transformation type of parameter RTGeneratorPhaseSpaceRTPlan:Transformation",
		  FatalException,
		  G4String("it is must be D, RXYZ or RTPS on the argument number "+GmGenUtils::itoa(ii+1)+" it is now "+strs[ii]).c_str());
    }
    current_T.opx=GmGenUtils::GetValue(strs[ii+1]);
    current_T.opy=GmGenUtils::GetValue(strs[ii+2]);
    current_T.opz=GmGenUtils::GetValue(strs[ii+3]);
    theTransformations.push_back(current_T);
  }

  theNEventsSkip = parmgr->GetNumericValue("RTGeneratorPhaseSpaceRTPlan:NEventsSkip", 0);

  p_rfile = open_file(const_cast<char*>(theFileName.c_str()),(char *)(".IAEAphsp"),(char *)("rb"));
  if( !p_rfile ) {
    G4Exception("RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Error file not found :  " + theFileName + ".IAEAphsp").c_str());
  }

  iaea_header_type *p_iaea_header;
  p_iaea_header = (iaea_header_type *) calloc(1, sizeof(iaea_header_type));
  
  p_iaea_header->fheader = open_file(const_cast<char*>(theFileName.c_str()),(char *)(".IAEAheader"),(char *)("rb"));
  if( !p_iaea_header->fheader ) {
    G4Exception("RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("File not found :  " + theFileName + ".IAEAheader").c_str());
  }
  if( p_iaea_header->read_header() ) {
    G4Exception("RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Error reading phase space file:  " + theFileName + ".IAEAheader").c_str());
  }

  theNEventsInFile = p_iaea_header->nParticles;

#ifndef GAMOS_NO_VERBOSE
 if( RTVerb(infoVerb) ) G4cout << " NPART " << p_iaea_header->nParticles << " " << p_iaea_header->particle_number[0] << " " << p_iaea_header->particle_number[1]  << " " << p_iaea_header->particle_number[2] << " STAT "  << " " << p_iaea_header->minimumX << " " << p_iaea_header->maximumZ  << " " << p_iaea_header->minimumY << " " << p_iaea_header->maximumY << G4endl; 
#endif

  if( theNEventsInFile <= theNEventsSkip ){
    //    G4Exception("RTGeneratorPhaseSpaceRTPlan:","Asking to skip more events that the number of events in file ",FatalErrorInArgument,G4String("NEventsInFile= " + GmGenUtils::itoa(theNEventsInFile) + " <= NEventsSkip= " + GmGenUtils::itoa(theNEventsSkip)).c_str());
    G4Exception("RTGeneratorPhaseSpaceRTPlan:",
		"Asking to skip more events that the number of events in file ",
		JustWarning,
		G4String("NEventsInFile= " + GmGenUtils::ftoa(theNEventsInFile) + " <= NEventsSkip= " + GmGenUtils::ftoa(theNEventsSkip)).c_str());
    theNEventsSkip -= int(theNEventsSkip/theNEventsInFile)*theNEventsInFile; 
  }

  // Creating IAEA record and allocating memory for it
  p_iaea_read = (iaea_record_type *) calloc(1, sizeof(iaea_record_type));

  //set record data
  p_iaea_header->get_record_contents(p_iaea_read);

  theExtraInfoMgr = RTExtraInfoMgr::GetInstance();
  theExtraInfoMgr->InitExtraInfo( p_iaea_read );
      
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) ) G4cout << "RTGeneratorPhaseSpaceRTPlan::RTGeneratorPhaseSpaceRTPlan  n events in file " << p_iaea_header->nParticles << G4endl;
#endif

  GmNumberOfEvent::SetNumberOfEvent( 0 );

  bHistos = G4bool(parmgr->GetNumericValue("RTGeneratorPhaseSpaceRTPlan:Histos", 0));

  if( bHistos ) theHistos = new RTPhaseSpaceHistos(p_iaea_header->record_constant[2]*CLHEP::cm ); 
  theNOrigEvents = p_iaea_header->orig_histories;

  GmNumberOfEvent::SetNumberOfEventFactor( theNOrigEvents / theNEventsInFile );  // temporary: it does not take into account the weights of the particles 
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(infoVerb) )  G4cout << "RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries   SetNumberOfEventFactor " << theNOrigEvents / theNEventsInFile  << " " << G4double(theNOrigEvents) << " " << theNEventsInFile << G4endl;
#endif
  //  GmUserActionMgr::GetInstance()->RegisterTrackingAction(this);

  theNNewEvents = 0;
  theNParticlesRead = 0.;

  theMinPosX = parmgr->GetNumericValue("RTGeneratorPhaseSpaceRTPlan:MinPosX", 0.);
  theMaxPosX = parmgr->GetNumericValue("RTGeneratorPhaseSpaceRTPlan:MaxPosX", 1000.);

  theRTPlanSource = new RTPlanSource("RTGeneratorPhaseSpaceRTPlanSource","gamma"); //dummy particle

  bEventSkipInitialised = false;
}


//------------------------------------------------------------------------
RTGeneratorPhaseSpaceRTPlan::~RTGeneratorPhaseSpaceRTPlan()
{
  //  theFileIn->close();
}

//------------------------------------------------------------------------
void RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries(G4Event* evt)
{
  if( bMaxNReuseAutomatic && evt->GetEventID() == 0 ) {
    //set MaxNReuse, MaxNRecycle automatically for each run
    theMaxNReuse = G4int( (G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed()-1) / theNEventsInFile ) + 1;
    theMaxNRecycle = 1; 
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(infoVerb) ) G4cout<< " RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries automatic NReuse= " << theMaxNReuse << G4endl;
#endif
  }
  /*  if( evt->GetEventID() > maxNEvents ) {
    //    G4Exception("RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries: more events than those on file = " + GmGenUtils::itoa(maxNEvents) );
    }*/

  p_iaea_read->p_file = p_rfile;
  //skip first particles
  if( evt->GetEventID() == 0 ) {
    for( G4int ii = 0; ii < theNEventsSkip; ii++ ){
      p_iaea_read->read_particle();
    }
  }
  if( !bEventSkipInitialised ) {
    evt->SetEventID( theNEventsSkip* GmNumberOfEvent::GetNumberOfEventFactor() );//for RTPlan
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries SetEventID theNEventsSkip " << evt->GetEventID() << " = " << theNEventsSkip << "*"<< GmNumberOfEvent::GetNumberOfEventFactor() << G4endl;
#endif
    bEventSkipInitialised = true;
  }

  //---- Reuse the same particle theMaxNReuse times  !! do not reuse charged particles
  if( theNReused == 0 || !theParticleDef || theParticleDef->GetPDGCharge() != 0 ) { // first time theNReused = 0, after theMaxNReuse times 
    //    G4bool bFilter = (theBase.size() == 0);
    //    do {
    G4ThreeVector pos;
    do {
      int fileEnd = p_iaea_read->read_particle();
      pos = G4ThreeVector(p_iaea_read->x,p_iaea_read->y,p_iaea_read->z) * CLHEP::cm;
;
//-      G4cout << " CHECK POSX " << theMinPosX << " <? " << fabs(pos.x()) << " <? " << theMaxPosX << G4endl; //GDEB
      theNParticlesRead ++;
      theExtraInfoMgr->FillExtraInfo( p_iaea_read );
      //      if( theBase.size() != 0 ) bFilter = theBase[0]->AcceptStep((G4Step*)0);
      if( fileEnd == FAIL) {
	if( theNRecycled < theMaxNRecycle ) {
	  fclose(p_rfile);
	  p_rfile = open_file(const_cast<char*>(theFileName.c_str()),(char *)(".IAEAphsp"),(char *)("rb"));
#ifndef GAMOS_NO_VERBOSE
	  if( RTVerb(infoVerb) ) G4cout << "RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries  RECYCLING phase space file " << theNRecycled << G4endl;
#endif
	  theNRecycled++; 
	  //skip first particles
	  for( G4int ii = 0; ii < theNEventsSkip; ii++ ){
	    p_iaea_read->read_particle(); // use fseek
	  }
	} else{
	  fclose(p_rfile);
	  G4RunManager::GetRunManager()->AbortRun();
	  G4Exception("RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries",
		      "Too many events demanded",
		      JustWarning,
		      G4String("FAILED reading event  " + GmGenUtils::itoa(evt->GetEventID())).c_str() );
	  return;
	}
      }
      //    }while( !bFilter );
    }while( fabs(pos.x()) < theMinPosX || fabs(pos.x()) > theMaxPosX );

  }
  //-  G4cout << " POSX OK " << G4endl; //GDEB

  if( bHistos ) theHistos->FillHistos( p_iaea_read, p_iaea_read->z*CLHEP::cm, (G4Step*)0 );

  //---- Store number of event
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite;
  std::map<G4String,GmVPrimitiveScorer*> scorers = GmScoringMgr::GetInstance()->GetScorers();
  G4bool bNewEvent = p_iaea_read->IsNewHistory;
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries bNewEvent " << bNewEvent << " NReused " << theNReused << G4endl;
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
					       
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries SetNumberOfEventFactor "
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
    if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpaceRTPlan new event " << GmNumberOfEvent::GetNumberOfEvent() << G4endl;
#endif
  }

  
  switch( p_iaea_read->particle ){
  case 1:
    theParticleDef = G4Gamma::Gamma();
    break;
  case 2:
    theParticleDef = G4Electron::Electron();
    break;
  case 3:
    theParticleDef = G4Positron::Positron();
    break;
  case 4:
    theParticleDef = G4Neutron::Neutron();
    break;
  case 5:
    theParticleDef = G4Proton::Proton();
    break;
  default:
    G4Exception("RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Particle code not supported " + GmGenUtils::itoa( p_iaea_read->particle )).c_str());
  }

  G4PrimaryVertex* vtx;
  G4PrimaryParticle* particle;
  //  G4double momdirz = sqrt(1. - p_iaea_read->u* p_iaea_read->u + p_iaea_read->v* p_iaea_read->v );
  G4double momdirz = p_iaea_read->w;

  G4double mom = sqrt( p_iaea_read->energy*p_iaea_read->energy + 2.*p_iaea_read->energy*theParticleDef->GetPDGMass() );
   //  G4cout << " MOM " << mom << " " <<  p_iaea_read->energy*p_iaea_read->energy + 2.*p_iaea_read->energy*theParticleDef->GetPDGMass() << " " << p_iaea_read->energy << " + 2.* " << theParticleDef->GetPDGMass() << G4endl;//GDEB

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
    pos = G4ThreeVector(p_iaea_read->x,p_iaea_read->y,p_iaea_read->z);
    momV = G4ThreeVector(p_iaea_read->u*mom, p_iaea_read->v*mom, momdirz*mom);
    break;
  case 1: // mirror X
    pos = G4ThreeVector(-p_iaea_read->x,p_iaea_read->y,p_iaea_read->z);
    momV = G4ThreeVector(-p_iaea_read->u*mom, p_iaea_read->v*mom, momdirz*mom);
    break;
  case 2: // mirror Y
    pos = G4ThreeVector(p_iaea_read->x,-p_iaea_read->y,p_iaea_read->z);
    momV = G4ThreeVector(p_iaea_read->u*mom, -p_iaea_read->v*mom, momdirz*mom);
    break;
  case 3: // mirror X & Y
    pos = G4ThreeVector(-p_iaea_read->x,-p_iaea_read->y,p_iaea_read->z);
    momV = G4ThreeVector(-p_iaea_read->u*mom, -p_iaea_read->v*mom, momdirz*mom);
    break;
  }
  pos *= CLHEP::cm;

  //  G4cout << " RTGeneratorPhaseSpaceRTPlan momV0 " << momV << G4endl; //GDEB
  momV.rotateX( theOriginalBeamRotAngleX);
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan momVX " << momV << G4endl; //GDEB
  momV.rotateY( theOriginalBeamRotAngleY);
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan momVY " << momV << G4endl; //GDEB
  momV.rotateZ( theOriginalBeamRotAngleZ);
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan momVZ " << momV << G4endl; //GDEB

  //  G4cout << " RTGeneratorPhaseSpaceRTPlan pos0 " << pos << G4endl; //GDEB
  pos.rotateX( theOriginalBeamRotAngleX);
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan posX " << pos << G4endl; //GDEB
  pos.rotateY( theOriginalBeamRotAngleY);
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan posY " << pos << G4endl; //GDEB
  pos.rotateZ( theOriginalBeamRotAngleZ);
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan posZ " << pos << G4endl; //GDEB

  if( bOriginalBeamInvert ) {
    momV *= -1;
    pos *= -1;
  }
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan posInvert " << pos << G4endl; //GDEB
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan momVInvert " << momV << G4endl; //GDEB
  
  pos += theOriginalBeamDisplacement;
  //  G4cout << " RTGeneratorPhaseSpaceRTPlan posDisp " << pos << G4endl; //GDEB
 
  //--- EVENT ID HAS TO TAKE INTO ACCOUNT THAT EACH PS LINE CORRESPONDS TO GetNumberOfEventFactor BEAM PARTICLES 
  evt->SetEventID( (theNNewEvents-1) * GmNumberOfEvent::GetNumberOfEventFactor() );
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries SetEventID " << evt->GetEventID() << " = " << theNNewEvents << "*"<< GmNumberOfEvent::GetNumberOfEventFactor() << G4endl;
#endif

  G4double time = 0.; 
  G4bool bOK = true;
#ifndef WIN32
  RTBeamStateData rsData = RTPlanMgr::GetInstance()->GetRTBeamStateData(time,evt,bOK); // MoveAccelerator, MovePhantom if new scanspotdata
  //t    theRTPlanSource->GetRTBeamStateData(time,evt,bOK); // MoveAccelerator, MovePhantom if new scanspotdata
#else
	G4Exception("RTGeneratorPhaseSpaceRTPlan::GeneratePrimaries",
		"",
		FatalException,
		"RTGeneratorPhaseSpaceRTPlan IS NOT SUPPORTED FOR WINDOSE. PLEASE CONTACT GAMOS AUTHORS");
#endif
  if( !bOK ) return; // end of Metersets
 
  particle = new G4PrimaryParticle( theParticleDef, momV.x(), momV.y(), momV.z() );
  particle->SetWeight( p_iaea_read->weight / theMaxNReuse );  

  //  if( theParticleDef->GetPDGCharge() == 0 ){
  theNReused = (theNReused+1)%theMaxNReuse;
  //  }

  vtx = new G4PrimaryVertex( pos, 0. );
  vtx->SetPrimary(particle);

  evt->AddPrimaryVertex( vtx );
 
}
