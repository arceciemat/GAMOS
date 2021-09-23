#include "GmHit.hh"
#include "GmEDepo.hh"

#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"

#include "CLHEP/Random/RandGauss.h"

//----------------------------------------------------------------------
GmHit::GmHit(G4Step* aStep, G4double energy, unsigned long long id, const G4String& sdtyp, G4int evtid )
{
  G4Track* aTrack = aStep->GetTrack();
  theDetUnitID = id;
  theEventID = evtid;
  theSDType = sdtyp;
  theTimeMin = std::min( 1.E99, aTrack->GetGlobalTime() );
  theTimeMax = std::max( 0., aTrack->GetGlobalTime() );
  theEnergy = energy;

  InsertTrackIDs( aTrack );
  theEDepos.push_back( new GmEDepo( energy, aTrack->GetPosition(), aStep->GetTrack()->GetDefinition(), aStep->GetPostStepPoint()->GetGlobalTime() ) );

  // only for checks, it should be set by SD (at the center of face of entrance)
  if( GmParameterMgr::GetInstance()->GetNumericValue("GmHit:LocalHitCoordinates",0) ) {
    thePosition = aStep->GetTrack()->GetPosition();
    thePosition = GmG4Utils::GetLocalFromGlobalPos( thePosition, aStep->GetPostStepPoint()->GetTouchable()->GetHistory());
  } else {
    thePosition = aStep->GetTrack()->GetPosition();
  }

  GmHitsEventMgr* evtmgr = GmHitsEventMgr::GetInstance();
  theHitsTimeType = evtmgr->GetHitsTimeType();

  //  G4cout << " TIMEMIN " << theTimeMin << " MAX " << theTimeMax << " GLOBAL_TIME " << aTrack->GetGlobalTime() << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " new GmHit " << id << " in det type " << sdtyp << " E " << theEnergy << " posXYZ " << aStep->GetTrack()->GetPosition().x() << " " << aStep->GetTrack()->GetPosition().y() << " " << aStep->GetTrack()->GetPosition().z() << " time= " << GetTime() << " " << theTimeMin << " " << theTimeMax << " " <<  aStep->GetTrack()->GetGlobalTime() << G4endl;
#endif
  //<< " posRPHIZ " << aStep->GetTrack()->GetPosition().perp() << " " << aStep->GetTrack()->GetPosition().phi()/deg << " " << aStep->GetTrack()->GetPosition().z() << G4endl;

  evtmgr->AddHit( this, sdtyp );

  bDeadTimeFound = false;

}

//----------------------------------------------------------------------
void GmHit::SetPosition(G4ThreeVector pos ) {

  if( GmParameterMgr::GetInstance()->GetNumericValue("GmHit:LocalHitCoordinates",0) ) {
    thePosition = (G4TransportationManager::GetTransportationManager()->
		   GetNavigatorForTracking()->
		   GetGlobalToLocalTransform()).TransformPoint(pos);
  } else {
    thePosition = pos;
  }
}

//----------------------------------------------------------------------
GmHit::GmHit( std::vector<G4String> wl )
{
  Build(wl);
  bDeadTimeFound = false;
}

//----------------------------------------------------------------------
GmHit::~GmHit()
{ 
  DeleteEDepoList();
}


#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4UImanager.hh"

//----------------------------------------------------------------------
void GmHit::InsertTrackIDs( const G4Track* aTrack )
{
  //  G4int ogNumber = theCheckOriGamma.CheckIfOriginalGamma(aTrack);
  G4int ogNumber = GmCheckOriginalGamma::GetOriginalGamma(aTrack);
  if( ogNumber == 0 ) {
    //    G4cerr << "!!! WARNING GmHit::Update: OriginalGammaNumber = -1. Is GmOriginalGammaUA not used? or a positron has reached a sensitive detector? " << " particle ID " << aTrack->GetDefinition()->GetParticleName() << G4endl;
  } else {
    theOriginalTrackIDs.insert( ogNumber );

    /*-    std::vector<G4int>::iterator ite;
      G4bool bFound = FALSE;
      for( ite = theOriginalTrackIDs.begin(); ite != theOriginalTrackIDs.end(); ite++ ){
      if( ogNumber == *ite ) {
      bFound = TRUE;
      break;
      }
      }
      if( !bFound) theOriginalTrackIDs.push_back( ogNumber );
    */
  }

  //-    return;
  theTrackIDs.insert( aTrack->GetTrackID() );
    
    /*    std::vector<G4int>::iterator ite;
    G4bool bFound = FALSE;
    for( ite = theTrackIDs.begin(); ite != theTrackIDs.end(); ite++ ){
      if( aTrack->GetTrackID() == *ite ) {
	bFound = TRUE;
	break;
      }
    }
    if( !bFound) theTrackIDs.push_back( aTrack->GetTrackID() );
    */

}


//----------------------------------------------------------------------
G4bool GmHit::CheckSameCell( unsigned long long id )
{

  //don't check time for the moment
  if( id == theDetUnitID ) {
    return TRUE;
  }else {
    return FALSE;
  }

}

//----------------------------------------------------------------------
G4bool GmHit::CheckSameCellAndTime( unsigned long long id, G4double time, G4double triggerTime, G4double measuringTime )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHit::CheckSameCellAndTime hit time " << theTimeMin << " time " << time << " trig " << triggerTime << " meas " << measuringTime 
	 << " hitInter " << (long long int)((theTimeMin-triggerTime)/measuringTime) << " ==? " << (long long int)((time-triggerTime)/measuringTime)
	 <<" " << ((time-triggerTime)/measuringTime) << G4endl;
#endif
  //don't check time for the moment
  if( id == theDetUnitID && 
     (long long int)((theTimeMin-triggerTime)/measuringTime) == (long long int)((time-triggerTime)/measuringTime) ) {
    return TRUE;
  }else {
    return FALSE;
  }

}

//----------------------------------------------------------------------
void GmHit::UpdateMe( G4Step* aStep, G4double energy )
{

  G4Track* aTrack = aStep->GetTrack();
  theTimeMin = std::min( theTimeMin, aTrack->GetGlobalTime() );
  theTimeMax = std::max( theTimeMax, aTrack->GetGlobalTime() );

  theEDepos.push_back( new GmEDepo( energy, aTrack->GetPosition(), aTrack->GetDefinition(), aStep->GetPostStepPoint()->GetGlobalTime() ) );
  
  theEnergy += energy;

  InsertTrackIDs( aTrack );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << " Update GmHit " << theDetUnitID << " E= " << energy << " Etot= " << theEnergy<< " N E depos= " << theEDepos.size() << " time=" << GetTime() << G4endl;
#endif

}


//----------------------------------------------------------------------
#include "G4Visible.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

void GmHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) {
    G4Circle circle(thePosition);
    circle.SetScreenSize(0.4);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}


//----------------------------------------------------------------------
G4double GmHit::GetTime() const 
{
  if( theHitsTimeType == 1 ) {
    return GetTimeMin();
  } else if( theHitsTimeType == 2 ) {
    return GetTimeMax();
  } else { 
    G4Exception("GmHit::GetTime",
		"wrong time type",
		FatalErrorInArgument,
		G4String("TimeType must be 1 or 2, while it is " + GmGenUtils::itoa(theHitsTimeType)).c_str());
    return 0.; // to avoid compilation warning
  }

}

//-----------------------------------------------------------------------
void GmHit::Print(std::ostream& os, G4bool bVerbose )
{
  std::set<G4int>::iterator itesi;
  std::set<G4int> htidori = theOriginalTrackIDs;
  //- std::vector<G4int>::iterator itesi;
  //- std::vector<G4int> htidori = theOriginalTrackIDs;
  if( bVerbose ) os << "Hit in Event= ";
  os << theEventID << " ";
  if( bVerbose ) os << G4endl << " SD type= ";
  G4String sdtype = GetSDType();
  sdtype = sdtype.substr(1,sdtype.size()-2);
  os << sdtype << " ";
  if( bVerbose ) os << G4endl << " DetUnit= ";
  os << theDetUnitID << " ";
  if( bVerbose ) os << G4endl << " energy= ";
  os << theEnergy << " ";
  if( bVerbose ) os << G4endl << " time_min= ";
  os << std::setprecision(15) << theTimeMin << " ";
  if( bVerbose ) os << G4endl << " time_max= ";
  os << std::setprecision(15) << theTimeMax << " ";
  //     << " pos= " << thePosition.mag() << " " << thePosition.theta()/deg << " " if( bVerbose ) os<< thePosition.phi()/deg
  if( bVerbose ) os << G4endl << " posXYZ= ";
  os << std::setprecision(8) << thePosition.x() << " " << thePosition.y() << " " << thePosition.z() << " ";
  if( bVerbose ) os << G4endl << " N original tracks= ";
  os << theOriginalTrackIDs.size() << " ";
  htidori = theOriginalTrackIDs;
  for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
    if( bVerbose ) os << " id= ";
    os << *itesi << " ";
  }
  if( bVerbose ) os << G4endl << " N tracks= ";
  os << theTrackIDs.size() << " ";
  htidori = theTrackIDs;
  for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
    if( bVerbose ) os << G4endl << " id= ";
    os << *itesi << " ";
  }
  /*
  if( bVerbose ) os << G4endl << " N E_depos= ";
  os << theEDepos.size() << " ";
  std::vector<GmEDepo*>::const_iterator iteed;
  for( iteed = theEDepos.begin(); iteed != theEDepos.end(); iteed++) {
    (*iteed)->Print( os, bVerbose );    
  }
  */
  os << G4endl;

}


//----------------------------------------------------------------------
void GmHit::Print(FILE* fout, G4bool )
{
  std::set<G4int>::iterator itesi;
  std::set<G4int> htidori = theOriginalTrackIDs;
  fwrite((float *) &theEventID,  sizeof(float), 1, fout);
  G4String sdtype = GetSDType();
  sdtype = sdtype.substr(1,sdtype.size()-2);
  fwrite((char *) &sdtype, sizeof(char*), sdtype.length(), fout);
  fwrite((unsigned long long *) &theDetUnitID,  sizeof(unsigned long long), 1, fout);
  fwrite((float *) &theEnergy,  sizeof(float), 1, fout);
  fwrite((G4double *) &theTimeMin,  sizeof(G4double), 1, fout);
  fwrite((G4double *) &theTimeMax,  sizeof(G4double), 1, fout);

  float pos = thePosition.x();
  fwrite((float *) &pos,  sizeof(float), 1, fout);
  pos = thePosition.y();
  fwrite((float *) &pos,  sizeof(float), 1, fout);
  pos = thePosition.z();
  fwrite((float *) &pos,  sizeof(float), 1, fout);
  G4int ntrko = theOriginalTrackIDs.size();
  fwrite((unsigned int *) &ntrko,  sizeof(unsigned int), 1, fout);
  htidori = theOriginalTrackIDs;
  for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
    G4int id = *itesi;
    fwrite((unsigned int *) &id,  sizeof(unsigned int), 1, fout);
  }
  G4int ntrk = theTrackIDs.size();
  fwrite((unsigned int *) &ntrk,  sizeof(unsigned int), 1, fout);
  htidori = theTrackIDs;
  for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
    G4int id = *itesi;
    fwrite((unsigned int *) &id,  sizeof(unsigned int), 1, fout);
  }
  
}


//----------------------------------------------------------------------
void GmHit::Build(std::vector<G4String>& wl )
{
  if( wl.size() < 10 ) { 
    PrintBadHit( wl );
    return;
  }
  G4int nTrkOrig = GmGenUtils::GetInteger(wl[9]);
  if( G4int(wl.size()) < 10+nTrkOrig ) {
    PrintBadHit( wl );
    return;
  }
  for( G4int ii = 0; ii < nTrkOrig; ii++ ){
    theOriginalTrackIDs.insert(GmGenUtils::GetInteger(wl[10+ii]));
  }
  G4int nTrk = GmGenUtils::GetInteger(wl[10+nTrkOrig]);
  if( G4int(wl.size()) < 11+nTrkOrig+nTrk ) {
    PrintBadHit( wl );
    return;
  }
  for( G4int ii = 0; ii < nTrk; ii++ ){
    theTrackIDs.insert(GmGenUtils::GetInteger(wl[11+nTrkOrig+ii]));
  }

  theEventID = GmGenUtils::GetInteger(wl[0]);
  theSDType = G4String("/"+wl[1]+"/");
  theDetUnitID = GmGenUtils::GetInteger(wl[2]);
  theEnergy = GmGenUtils::GetValue(wl[3]);
  theTimeMin = GmGenUtils::GetValue(wl[4]);
  theTimeMax = GmGenUtils::GetValue(wl[5]);
  G4double posx = GmGenUtils::GetValue(wl[6]);
  G4double posy = GmGenUtils::GetValue(wl[7]);
  G4double posz = GmGenUtils::GetValue(wl[8]);
  thePosition = G4ThreeVector( posx, posy, posz );
  theHitsTimeType = 1;

  if( SDVerb(infoVerb) ) Print(std::cout, 0);

}

//----------------------------------------------------------------------
void GmHit::PrintBadHit(std::vector<G4String>& wl )
{
  theEnergy = 0.;
  std::vector<G4String>::const_iterator ite;
  G4cerr << "GmHit::Build ";
  for( ite = wl.begin(); ite != wl.end(); ite++ ){
    G4cerr << *ite << " ";
  }
  G4cerr << G4endl;
  G4Exception("GmHit::Build","WARNING",JustWarning," problem building hit");

}

//----------------------------------------------------------------------
void GmHit::DeleteEDepoList()
{

  std::vector<GmEDepo*>::iterator ite;
  for( ite = theEDepos.begin(); ite != theEDepos.end(); ite++ ){
    delete *ite;
  }
  theEDepos.clear();

}


//----------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const GmHit& hit)
{
  GmHit hitnc = hit;
  hitnc.Print(os);

  return os;
}

//----------------------------------------------------------------------
GmHit GmHit::operator+=(const GmHit& hit)
{

  SetTimeMin( std::max( hit.GetTimeMin(), GetTimeMin() ) );
  SetTimeMax( std::max( hit.GetTimeMax(), GetTimeMax() ) );
  
  std::vector<GmEDepo*> edepos = hit.GetEDepos();
  for( size_t ii = 0; ii < edepos.size(); ii++ ) {
    GmEDepo* edepo = edepos[ii];
    AddEDepo( new GmEDepo( edepo->GetEnergy(), edepo->GetPosition(), edepo->GetParticle(), edepo->GetTime() ) );
    SetEnergy( GetEnergy() + edepo->GetEnergy() );
  }
  std::set<G4int> origTrks = hit.GetOriginalTrackIDs();
  std::set<G4int>::const_iterator itet;
  for( itet = origTrks.begin(); itet != origTrks.end(); itet++ ) {
    theOriginalTrackIDs.insert( *itet );
  }
  std::set<G4int> trks = hit.GetTrackIDs();
  for( itet = trks.begin(); itet != trks.end(); itet++ ) {
    theTrackIDs.insert( *itet );
  }

  return *this;

}

