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

#include "RTGeneratorPSPenelope.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "RTVerbosity.hh"
#include "RTPhaseSpaceHistos.hh"

//------------------------------------------------------------------------
RTGeneratorPSPenelope::RTGeneratorPSPenelope( G4String )
{
}

//------------------------------------------------------------------------
RTGeneratorPSPenelope::RTGeneratorPSPenelope()
{
  Init();
}

//------------------------------------------------------------------------
void RTGeneratorPSPenelope::Init()
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 

  theFileName = parmgr->GetStringValue("RTGeneratorPSPenelope:FileName", "psf-impdet.dat");

  theMaxNReuse = G4int(parmgr->GetNumericValue("RTGeneratorPSPenelope:MaxNReuse", -1));
  theNReused = 0;
  theMaxNRecycle = G4int(parmgr->GetNumericValue("RTGeneratorPSPenelope:MaxNRecycle", 1));
  theNRecycled = 1;
  if( theMaxNReuse == 0 ){
    G4Exception("RTGeneratorPSPenelope::RTGeneratorPSPenelope",
		"MaxNReuse has wrong value",
		FatalErrorInArgument,
		"MaxNReuse is 0, the minimum value is 1");
  }

  if( theMaxNReuse == -1 ){ //set it automatically for each run
    bMaxNReuseAutomatic = TRUE;
  } else {
    bMaxNReuseAutomatic = FALSE;
  }

  G4String strMirrorWhenReuse = parmgr->GetStringValue("RTGeneratorPSPenelope:MirrorWhenReuse", "");
  if( strMirrorWhenReuse == "" ) {
    MirrorWhenReuse = NOMir;
  } else if( strMirrorWhenReuse == "X" ){
    MirrorWhenReuse = MirX;
  } else if( strMirrorWhenReuse == "Y" ){
    MirrorWhenReuse = MirY;
  } else if( strMirrorWhenReuse == "XY" ){
    MirrorWhenReuse = MirXY;
  } else {
    G4Exception("RTGeneratorPSPenelope::RTGeneratorPSPenelope()",
		"Wrong value of MirrorWhenReuse",
		FatalErrorInArgument,
		G4String("It can be X, Y, XY, it is = " + strMirrorWhenReuse).c_str());
  }

  std::vector<G4double> inidisp = parmgr->GetVNumericValue("RTGeneratorPSPenelope:InitialDisplacement", std::vector<G4double>());
  if( inidisp.size() == 0 ){
    theInitialDisp = G4ThreeVector(0.,0.,0.);
  } else if( inidisp.size() == 3 ){
    theInitialDisp = G4ThreeVector(inidisp[0],inidisp[1],inidisp[2]);
  } else {
    G4Exception("RTGeneratorPSPenelope",
		"Error in /gamos/setParam RTGeneratorPSPenelope:InitialDisplacement",
		FatalErrorInArgument,
		G4String("it must have 3 arguments: POS_X POS_Y POS_Z, and it has "+GmGenUtils::itoa(inidisp.size())).c_str());
  }

  std::vector<G4double> inirot = parmgr->GetVNumericValue("RTGeneratorPSPenelope:InitialRotAngles", std::vector<G4double>());
  if( inirot.size() == 0 ){
    theInitialRotAngleX = 0.;
    theInitialRotAngleY = 0.;
    theInitialRotAngleZ = 0.;
  } else if( inirot.size() == 3 ){
    theInitialRotAngleX = inirot[0];
    theInitialRotAngleY = inirot[1];
    theInitialRotAngleZ = inirot[2];
  } else {
    G4Exception("RTGeneratorPSPenelope",
		"Error in /gamos/setParam RTGeneratorPSPenelope:InitialRotAngles",
		FatalErrorInArgument,
		G4String("It must have 3 arguments: ANG_X ANG_Y ANG_Z, and it has "+GmGenUtils::itoa(inirot.size())).c_str());
  }

  std::vector<G4String> strs;
  strs = parmgr->GetVStringValue("RTGeneratorPSPenelope:Transformations",strs);
  if( strs.size()%4 != 0 ) {
    G4Exception("RTGeneratorPSPenelope",
		"Error in number of arguments of parameter RTGeneratorPSPenelope:Transformation",
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
      G4Exception("RTGeneratorPSPenelope",
		  "Error in transformation type of parameter RTGeneratorPSPenelope:Transformation",
		  FatalException,
		  G4String("it is must be D, RXYZ or RTPS on the argument number "+GmGenUtils::itoa(ii+1)+" it is now "+strs[ii]).c_str());
    }
    current_T.opx=GmGenUtils::GetValue(strs[ii+1]);
    current_T.opy=GmGenUtils::GetValue(strs[ii+2]);
    current_T.opz=GmGenUtils::GetValue(strs[ii+3]);
    theTransformations.push_back(current_T);
  }

  GmNumberOfEvent::SetNumberOfEvent( 0 );
  theNEventsInFile = 0;
  theNOrigEvents = 0;      
  theNNewEvents = 0;

  bHistos = G4bool(parmgr->GetNumericValue("RTGeneratorPSPenelope:Histos", 0));

  //t  if( bHistos ) theHistos = new RTPhaseSpaceHistos(p_iaea_header->record_constant[2]*CLHEP::cm ); 

  //  GmUserActionMgr::GetInstance()->RegisterTrackingAction(this);

  theNNewEvents = 0;
  theNParticlesRead = 0.;

  theFileIn2 = new std::ifstream(theFileName);
  // skip header
  if( !theFileIn2->is_open() ) {
	G4Exception("RTGeneratorPSPenelope::RTGeneratorPSPenelope",
		    "",
		    JustWarning,
		    G4String("FAILED opening file " + theFileName).c_str() );
  }
  for( G4int ii = 0; ii < 4; ii++ ) {
    //    *theFileIn2 >> theLineRead;
    theFileIn2->getline(theLineRead,200);
  }
  //skip first particles
  for( G4int ii = 0; ii < theNEventsSkip; ii++ ){
    theFileIn2->getline(theLineRead,200);
    theNEventsInFile++;      
    theNOrigEvents++; 
    theNNewEvents++;
  }
}


//------------------------------------------------------------------------
RTGeneratorPSPenelope::~RTGeneratorPSPenelope()
{
  theFileIn2->close();
}

//------------------------------------------------------------------------
void RTGeneratorPSPenelope::GeneratePrimaries(G4Event* evt)
{
  //---- Reuse the same particle theMaxNReuse times  !! do not reuse charged particles
  //---- If already reused theMaxNReuse times, read next line
  //  G4cout << " READ? " << theNReused << " " << theParticleDef << G4endl; //GDEB
  if( theNReused == 0 || !theParticleDef || theParticleDef->GetPDGCharge() != 0 ) { // first time theNReused = 0, after theMaxNReuse times
    G4ThreeVector pos;

    theFileIn2->getline(theLineRead,200);
    theNEventsInFile++;      
    theNOrigEvents++;
    theNNewEvents++;
    //    std::cout << " RTGeneratorPSPenelope::GeneratePrimaries getline " << theLineRead << " NEVENTS " << theNEventsInFile << std::endl; //GDEB
    theNParticlesRead ++;
    //      if( theBase.size() != 0 ) bFilter = theBase[0]->AcceptStep((G4Step*)0);
    if ( theFileIn2->eof() ) {
      theFileIn2->close();
      if( theNRecycled < theMaxNRecycle ) {
	theFileIn2 = new std::ifstream(theFileName);
#ifndef GAMOS_NO_VERBOSE
	if( RTVerb(infoVerb) ) G4cout << "RTGeneratorPSPenelope::GeneratePrimaries  RECYCLING phase space file " << theNRecycled << G4endl;
#endif
	theNRecycled++; 
	//skip first particles
	for( G4int ii = 0; ii < theNEventsSkip; ii++ ){
	  theFileIn2->getline(theLineRead,200);
	  theNEventsInFile++;      
	  theNOrigEvents++; 
	  theNNewEvents++;
	}
      } else{
	G4RunManager::GetRunManager()->AbortRun();
	G4Exception("RTGeneratorPSPenelope::GeneratePrimaries",
		    "Too many events demanded",
		    JustWarning,
		    G4String("FAILED reading event  " + GmGenUtils::itoa(evt->GetEventID())).c_str() );
	return;     
      }
    }
    //    }while( !bFilter );
    
  }

  //t  if( bHistos ) theHistos->FillHistos( p_iaea_read, p_iaea_read->posz*CLHEP::cm, (G4Step*)0 );
  
  //---- Store number of event
  std::map<G4String,GmVPrimitiveScorer*>::const_iterator ite;
  std::map<G4String,GmVPrimitiveScorer*> scorers = GmScoringMgr::GetInstance()->GetScorers();
  G4bool bNewEvent = (theNReused == 0); // first time theNReused = 0, after theMaxNReuse times 
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPSPenelope::GeneratePrimaries bNewEvent " << bNewEvent << " NReused " << theNReused << G4endl;
#endif
  for( ite = scorers.begin(); ite != scorers.end(); ite++ ){
    (*ite).second->SetNewEvent( bNewEvent );
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << bNewEvent << " RTGeneratorPSPenelope::GeneratePrimaries StoreNewEvent " << (*ite).second->GetNEvents(0) << G4endl;
#endif
  }
  if( bNewEvent ) {
    if( theNNewEvents != 0 ) {
      //-- Rescale by  number of original events that correspond to them when phase space file was created (NOrigRead) / number of events read (theNNewEvents)
      //-- But we only now total number of original events (NOrigTotal). Then assume the ratio NOrigRead/NOrigTotal = number of particles read / total number of particles in file
      //      GmNumberOfEvent::SetNumberOfEventFactor( ( theNOrigEvents * (theNParticlesRead-1) / theNEventsInFile ) / theNNewEvents );
      GmNumberOfEvent::SetNumberOfEventFactor( 1. );

    }

    theNNewEvents++;
    GmNumberOfEvent::AddEvent();
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTGeneratorPSPenelope new event " << GmNumberOfEvent::GetNumberOfEvent() << G4endl;
#endif
  }
  

  std::istringstream istr_line(theLineRead);

  int idPart;
  float energy;
  float posx,posy,posz;
  float momu,momv,momw;
  float weight;
  istr_line >> idPart >> energy >> posx >> posy >> posz >> momu >> momv >> momw >> weight;
  energy *= CLHEP::eV;

  //  std::cout << " RTGeneratorPSPenelope::GeneratePrimaries getline " << theLineRead << std::endl; //GDEB
  //  std::cout << " RTGeneratorPSPenelope::GeneratePrimaries idPart " << idPart << " E= " << energy << " pos " << posx << " " << posy << " " << posz << std::endl; //GDEB
  switch( idPart ){
  case 2:
    theParticleDef = G4Gamma::Gamma();
    break;
  case 1:
    theParticleDef = G4Electron::Electron();
    break;
  case 3:
    theParticleDef = G4Positron::Positron();
    break;
  default:
    G4Exception("RTGeneratorPSPenelope::GeneratePrimaries",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Particle code not supported " + GmGenUtils::itoa( idPart )).c_str());
  }

  G4PrimaryVertex* vtx;
  G4PrimaryParticle* particle;
  G4double momdirz = momw;

  G4double mom = sqrt( energy*energy + 2.*energy*theParticleDef->GetPDGMass() );
  //    G4cout << " MOM " << mom << " " <<  energy*energy + 2.*energy*theParticleDef->GetPDGMass() << " " << energy << " + 2.* " << theParticleDef->GetPDGMass() << G4endl;//GDEB

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
    pos = G4ThreeVector(posx,posy,posz);
    momV = G4ThreeVector(momu*mom, momv*mom, momdirz*mom);
    break;
  case 1: // mirror X
    pos = G4ThreeVector(-posx,posy,posz);
    momV = G4ThreeVector(-momu*mom, momv*mom, momdirz*mom);
    break;
  case 2: // mirror Y
    pos = G4ThreeVector(posx,-posy,posz);
    momV = G4ThreeVector(momu*mom, -momv*mom, momdirz*mom);
    break;
  case 3: // mirror X & Y
    pos = G4ThreeVector(-posx,-posy,posz);
    momV = G4ThreeVector(-momu*mom, -momv*mom, momdirz*mom);
    break;
  }
  pos *= CLHEP::cm;

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
  particle->SetWeight( weight / theMaxNReuse );  

  //  if( theParticleDef->GetPDGCharge() == 0 ){
  theNReused = (theNReused+1)%theMaxNReuse;
  //  }

  vtx = new G4PrimaryVertex( pos, 0. );
  vtx->SetPrimary(particle);

  evt->AddPrimaryVertex( vtx );
 
}
