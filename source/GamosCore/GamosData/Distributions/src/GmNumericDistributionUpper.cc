#include "GmNumericDistributionUpper.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"

//---------------------------------------------------------------------
GmNumericDistributionUpper::GmNumericDistributionUpper(G4String name)
  : GmVNumericDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericDistributionUpper::InterpolateValue(const G4double& , const G4double& , const G4double& , const G4double& , const G4double& valUp )
{
#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericDistributionUpper::InterpolateValue " << valUp << G4endl;
#endif

  return valUp;

}

//---------------------------------------------------------------------
G4double GmNumericDistributionUpper::GetMinimum(GmHisto1*)
{
  return 0.;
}
