#include "GmDataAccumulatedDose.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//----------------------------------------------------------------
GmDataAccumulatedDose::GmDataAccumulatedDose()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);

  theGeomUtils= GmGeometryUtils::GetInstance();
}

//----------------------------------------------------------------
GmDataAccumulatedDose::~GmDataAccumulatedDose()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedDose::Accumulate( const G4Step* aStep, G4int index )
{
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();

  theDataAccumulated[index] += aStep->GetTotalEnergyDeposit()/ ( density * theGeomUtils->GetCubicVolume( aStep->GetTrack()->GetVolume() ));
}

//----------------------------------------------------------------
G4double GmDataAccumulatedDose::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();

  return aStep->GetTotalEnergyDeposit()/ ( density * theGeomUtils->GetCubicVolume( aStep->GetTrack()->GetVolume() ));
}
