#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"

#include "GmCopyWeightToSecondaryUA.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"


//----------------------------------------------------------------
GmCopyWeightToSecondaryUA::GmCopyWeightToSecondaryUA()
{
}


//----------------------------------------------------------------
void GmCopyWeightToSecondaryUA::UserSteppingAction(const G4Step* aStep )
{
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  G4Track* aTrack = aStep->GetTrack();
  for( ite = secos.begin(); ite != secos.end(); ite++) {
    (*ite)->SetWeight( (*ite)->GetWeight() * aTrack->GetWeight() );
    //    G4cout << " GmCopyWeightToSecondaryUA " << (*ite)->GetWeight() << " " << aTrack->GetWeight() << G4endl;
  }

}
