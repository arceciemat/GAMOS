#include "GmCopyWeightToRDSecondaryUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


//----------------------------------------------------------------
GmCopyWeightToRDSecondaryUA::GmCopyWeightToRDSecondaryUA()
{
}


//----------------------------------------------------------------
void GmCopyWeightToRDSecondaryUA::UserSteppingAction(const G4Step* aStep )
{
  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if( proc ) {
    if( ! (proc->GetProcessName() == "RadioactiveDecay") ) return;
  }

  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4Track* aTrack = aStep->GetTrack();
  for( ite = secos.begin(); ite != secos.end(); ite++) {
    (*ite)->SetWeight( (*ite)->GetWeight() * aTrack->GetWeight() );
#ifndef GAMOS_NO_VERBOSE
  if( UtilsUAVerb(debugVerb) ) {
    G4cout << " GmCopyWeightToRDSecondaryUA::PreUserTrackingAction new weight " << (*ite)->GetWeight() 
	   << " old weight " <<  (*ite)->GetWeight()/aTrack->GetWeight() 
	   << " value " << aTrack->GetWeight() << G4endl; 
  }
#endif
  }

}
