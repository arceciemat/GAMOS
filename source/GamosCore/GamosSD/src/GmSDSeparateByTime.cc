#include "GmSDSeparateByTime.hh"
#include "GmHit.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmSDVerbosity.hh"
#include "GmDetUnitIDBuilderFromAncestors.hh"
#include "GmHitList.hh"
#include "GmHitsEventMgr.hh"
#include "G4RunManager.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistTime.hh"

#include "G4Step.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//-------------------------------------------------------------------
GmSDSeparateByTime::GmSDSeparateByTime(G4String sdtype): GmVSD( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDSeparateByTime: SD type ->" << theSDType << " SD name " << SensitiveDetectorName << G4endl;
#endif

  theDetUnitIDBuilder = new GmDetUnitIDBuilderFromAncestors(theSDType);

  theHitsEventMgr = GmHitsEventMgr::GetInstance();

  GmParameterMgr * paramMgr = GmParameterMgr::GetInstance();
  theMinimalMeasuringTime = paramMgr->GetNumericValue("GmSDSeparateByTime:MinimalMeasuringTime:"+sdtype,100);
 
  bDiscardBigTimes = G4bool(paramMgr->GetNumericValue("GmSDSeparateByTime:DiscardBigTime:"+sdtype,1));
  theBigTimeFactor = paramMgr->GetNumericValue("GmSDSeparateByTime:BigTimeFactor"+sdtype,1000);
  bBigTimeCalculated = false;
  theBiggestHitTime = DBL_MAX;
}

//-------------------------------------------------------------------
GmSDSeparateByTime::~GmSDSeparateByTime()
{
}

//--------------------------------------------------------------------
G4bool GmSDSeparateByTime::ProcessHits(G4Step* aStep,
                             G4TouchableHistory* )
{
  G4double time = aStep->GetPostStepPoint()->GetGlobalTime();

  //--- Get the maximum time allowable
  if( bDiscardBigTimes && !bBigTimeCalculated ) {
    G4double averageTime = 0.;
    GmGenerator* gmGener = (GmGenerator*)(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    std::vector<GmParticleSource*> sources = gmGener->GetSources();
    std::vector<GmParticleSource*>::const_iterator ites;
    for(ites = sources.begin(); ites != sources.end(); ites++ ) {
      GmVGenerDistTime* distTime = (*ites)->GetTimeDistribution();
      G4double averTime = 0.;
      for( size_t ii = 0; ii < 100; ii++ ) {
	averTime += distTime->GenerateTime(*ites);
      }
      if( averTime / 100 > averageTime ) averageTime = averTime;
#ifndef GAMOS_NO_VERBOSE
      if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSeparateByTime::ProcessHits source " << (*ites)->GetName() << " averageTime " << averTime << G4endl;
#endif
    }
    G4int nEvents = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    theBiggestHitTime = averageTime * nEvents * theBigTimeFactor;
#ifndef GAMOS_NO_VERBOSE
    if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSeparateByTime::ProcessHits biggest time " << theBiggestHitTime << " averageTime " << averageTime << " nEvents " << nEvents << " big time factor "<< theBigTimeFactor << G4endl;
#endif

    bBigTimeCalculated  = TRUE;
  }

  if( bDiscardBigTimes && time > theBiggestHitTime ) {
#ifndef GAMOS_NO_VERBOSE
    if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSeparateByTime::ProcessHits reject hit because too big time " << time << " > " << theBiggestHitTime << G4endl;
#endif
    return FALSE;
  }

  if( !IsInterestingStep( aStep ) ) return FALSE;

  unsigned long long detUnitID = GetDetUnitID( aStep );

  //----- Check if hit belongs to current hit
  G4bool hitUpdated = FALSE;

  G4double measuringTime = theHitList->GetMeasuringTime();
  if( measuringTime == 0. ) measuringTime = theMinimalMeasuringTime;
  G4double triggerTime = theHitList->GetTriggerTime( time, detUnitID );
  if( theCurrentHit != 0 ){
    if( theCurrentHit->CheckSameCellAndTime( detUnitID, time, triggerTime, measuringTime ) ) { 
      UpdateHit( theCurrentHit, aStep );
      hitUpdated = TRUE;
    }
  }

  //----- Check if hit belongs to any other hit
  if( !hitUpdated ) {
    G4int isiz = theHitsInEvent.size();
    for( int ii=0; ii<isiz-1; ii++ ){
      if( theHitsInEvent[ii] == theCurrentHit ) continue;
      if( theHitsInEvent[ii]->CheckSameCellAndTime( detUnitID, time, triggerTime, measuringTime ) ) { 
	UpdateHit( theHitsInEvent[ii], aStep );
	hitUpdated = TRUE;
	break;
      }
    }     
  } 
  
  //----- Create hit if is not updating an existing one
  if( !hitUpdated ) CreateHit( aStep, detUnitID );

  return TRUE;
}

//-------------------------------------------------------------------
unsigned long long GmSDSeparateByTime::GetDetUnitID( G4Step* aStep )
{
  return theDetUnitIDBuilder->BuildID( aStep );

}

//-------------------------------------------------------------------
void GmSDSeparateByTime::CalculateAndSetPosition( GmHit* hit, G4Step* aStep )
{
  //-  return;

  const G4VTouchable*  touch= aStep->GetPreStepPoint()->GetTouchable();
  G4AffineTransform trans = touch->GetHistory()->GetTopTransform().Inverse();

  G4ThreeVector centreGlobal = trans.TransformPoint( G4ThreeVector(0.,0.,0.) );

  hit->SetPosition( centreGlobal );
  //  G4cout << " GmSDSeparateByTime::CalculateAndSetPosition pos " << 
} 


//-------------------------------------------------------------------
void GmSDSeparateByTime::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0) if( SDVerb(debugVerb) ) G4cout << "GmSDSeparateByTime::EndOfEvent SD: " << SensitiveDetectorName << " Nhits= " << theHitsInEvent.size() << G4endl;
#endif

  GmVSD::EndOfEvent( HCE );

}

