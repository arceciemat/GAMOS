
#include "RTCutsStudyFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"

//----------------------------------------------------------------
RTCutsStudyFilter::RTCutsStudyFilter(const G4String& name) : GmVFilter(name)
{ 
  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue("RTCutsStudyFilter:PlaneZ",100.*CLHEP::cm);
  thePlaneXDim = GmParameterMgr::GetInstance()->GetNumericValue("RTCutsStudyFilter:PlaneXDim",100.*CLHEP::cm);
  thePlaneYDim = GmParameterMgr::GetInstance()->GetNumericValue("RTCutsStudyFilter:PlaneYDim",100.*CLHEP::cm);

}


//----------------------------------------------------------------
G4bool RTCutsStudyFilter::AcceptStep(const G4Step* aStep)
{
  G4ThreeVector prePos = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector postPos = aStep->GetPostStepPoint()->GetPosition();
  if( ( (postPos.z() - thePlaneZ) == 0. || 
      (prePos.z() - thePlaneZ) * (postPos.z() - thePlaneZ) < 0. ) && 
      fabs(postPos.x()) < thePlaneXDim && 
      fabs(postPos.y()) < thePlaneYDim ) {
    //  G4cout << " RTCutsStudyFilter: track accepted " << prePos << " " << postPos << G4endl;
    return true; 
  } else {
    //  G4cout << " track not accepted " << prePos << " " << postPos << " thePlaneZ " << thePlaneZ << " thePlaneYDim " << thePlaneYDim << " thePlaneXDim " << thePlaneXDim << G4endl;
    return false;
  }
}
