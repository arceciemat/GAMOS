#include "GmNumericDistributionLogLog.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmNumericDistributionLogLog::GmNumericDistributionLogLog(G4String name)
  : GmVNumericDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericDistributionLogLog::InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp )
{
  G4double logIndexDown = log(indexDown);
  G4double value = exp( log(valDown) + (log(indexVal)-logIndexDown)/(log(indexUp)-logIndexDown)*(log(valUp)-log(valDown)));
#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericDistributionLogLog::InterpolateValue " << value 
				   << " valDown= " << valDown << " valUp= " << valUp 
				   << " logIndexDown= " << logIndexDown << " logIndexUp= " << log(indexUp) << G4endl;
#endif
  return value;

}

