#include "GmNumericDistributionLower.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmNumericDistributionLower::GmNumericDistributionLower(G4String name)
  : GmVNumericDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericDistributionLower::InterpolateValue(const G4double& , const G4double& , const G4double& , const G4double& valDown, const G4double& )
{
#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericDistributionLower::InterpolateValue " << valDown << G4endl;
#endif

  return valDown;

}

