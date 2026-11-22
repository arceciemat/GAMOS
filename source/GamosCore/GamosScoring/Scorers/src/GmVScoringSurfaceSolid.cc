#include "GmVScoringSurfaceSolid.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmVScoringSurfaceSolid::GmVScoringSurfaceSolid()
{
  theSurfaceTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  theAngularTolerance = G4GeometryTolerance::GetInstance()->GetAngularTolerance();
}

//-----------------------------------------------------------------------
G4ThreeVector GmVScoringSurfaceSolid::GetLocalPoint( G4ThreeVector pos, G4Step* aStep ) 
{
  G4TouchableHandle touchable = aStep->GetPreStepPoint()->GetTouchableHandle();
  //  G4cout << " GmVScoringSurfaceSolid::GetLocalPoint global " << pos << " local " << touchable->GetHistory()->GetTopTransform().TransformPoint(pos)
  // << "transform " << touchable->GetHistory()->GetTopTransform().NetRotation() 
  // << "transform " << touchable->GetHistory()->GetTopTransform().NetTranslation() << G4endl;
  return touchable->GetHistory()->GetTopTransform().TransformPoint(pos);
}

