#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "SHActivationFromFluxUA.hh"
#include "ShieldingVerbosity.hh"
#include "SHMetastableIsotMgr.hh"
#include "SHRadDecayChainSet.hh"
#include "SHRadDecayScorerFromFlux.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//-----------------------------------------------------------------
SHActivationFromFluxUA::SHActivationFromFluxUA()
{
  theName = "SHActivationFromFluxUA";

  theFluxScorer = new SHRadDecayScorerFromFlux();

  theMaximumTime = -1;
  theMinimumTime = DBL_MAX;

}


//-----------------------------------------------------------------
SHActivationFromFluxUA::~SHActivationFromFluxUA() 
{
}

//------------------------------------------------------------------
void SHActivationFromFluxUA::BeginOfRunAction( const G4Run* )
{
  InitialiseRun();

}

//-----------------------------------------------------------------
void SHActivationFromFluxUA::UserSteppingAction(const G4Step* aStep)
{
  theFluxScorer->ProcessHits(aStep);
}

//-----------------------------------------------------------------
void SHActivationFromFluxUA::PreUserTrackingAction(const G4Track*)
{
}

//-----------------------------------------------------------------
void SHActivationFromFluxUA::EndOfEventAction( const G4Event* )
{
  theFluxScorer->SaveEventToRun();
}
 
//-----------------------------------------------------------------
void SHActivationFromFluxUA::EndOfRunAction(const G4Run* )
{
  //--- Multithreading: workers add their scorer values and time span to the master instance, which prints
  if( G4Threading::IsMultithreadedApplication() && !G4Threading::IsMasterThread() ) {
    SHActivationFromFluxUA* master = dynamic_cast<SHActivationFromFluxUA*>(GmUserAction::GetMasterUserAction(GetName()));
    if( master ) {
      G4AutoLock l(&GmUserAction::GetMergeMutex());
      theFluxScorer->AddToMaster( master->theFluxScorer );
      if( theMaximumTime > 0. ) {
	if( master->theMaximumTime < 0. ) master->theMaximumTime = 0.;
	master->theMaximumTime += theMaximumTime;
      }
      theMaximumTime = -1;
    }
    return;
  }
  G4cout << " SHActivationFromFluxUA::EndOfRunAction " << G4endl;
  // set number of ions from flux
  G4THitsMap<G4double>* RunMap = theFluxScorer->GetRunMap();

  G4cout << " SHActivationFromFluxUA::EndOfRunAction " << RunMap->GetMap()->size() << G4endl;

  std::map<G4int,G4double*>::iterator ite;
  G4double nev = GmNumberOfEvent::GetNumberOfEvent();
  //--- errors must be calculated before GetErrorRelative (it was never called: errors printed were always 0)
  theFluxScorer->CalculateErrors( RunMap );

  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){
    G4double sumX = (*(ite->second));

    G4Ions* ion =  theFluxScorer->GetIon(ite->first);
    G4cout << ite->first << "  index: " << ion->GetParticleName() << "  = " << sumX;
    G4double error = theFluxScorer->GetErrorRelative( ite->first, sumX, nev );
    //   if( aveX != 0. ) error /= (aveX/theUnit); // relative error
    G4cout << " +-(REL) " << error << " " << " "; // theUnitName
    /*t    if( bPrintSumV2 ) {
      G4cout << " sumV2= " << theFluxScorer->GetSumV2(ite->first)/(theUnit*theUnit);
      } */
    G4cout << G4endl;

    if( sumX > 0. ) theRadDecayChainSet->StoreIon(ion, sumX );   
//t    G4double error = theFluxScorer->GetErrorRelative( ite->first, sumX, nev );

  }

  PrintActivities();

}


