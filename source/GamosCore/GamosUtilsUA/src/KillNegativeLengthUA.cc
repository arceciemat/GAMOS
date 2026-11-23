#include "KillNegativeLengthUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "G4Event.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
KillNegativeLengthUA::KillNegativeLengthUA()
{
}


//----------------------------------------------------------------
void KillNegativeLengthUA::UserSteppingAction(const G4Step* aStep )
{
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) G4cout << " KillNegativeLengthUA::UserSteppingAction " << G4endl;
#endif
  G4double len = aStep->GetStepLength();
  if ( len == 0.001001 ) {
    G4cerr << "  KillNegativeLengthUA KILL TRACK AND ALL SECONDARIES " << G4endl;
    G4Track* aTrackNC = const_cast<G4Track*>(aStep->GetTrack());
    aTrackNC->SetTrackStatus( fStopAndKill );
    G4TrackVector fSec = GmG4Utils::GetSecondariesOfCurrentStep();
    G4TrackVector::iterator ite;
    for( ite = fSec.begin(); ite != fSec.end(); ite++ ) {
      G4Track* secoTrk = *ite;
      secoTrk->SetKineticEnergy(0.);
      secoTrk->SetTrackStatus( fStopAndKill );
    }
    G4RunManager::GetRunManager()->AbortEvent();
  }
}
