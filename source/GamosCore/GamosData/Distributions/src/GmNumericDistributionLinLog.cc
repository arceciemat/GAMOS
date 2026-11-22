#include "GmNumericDistributionLinLog.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmNumericDistributionLinLog::GmNumericDistributionLinLog(G4String name)
  : GmVNumericDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericDistributionLinLog::InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp )
{
  G4double value = valDown + exp( (indexVal-indexDown)/(indexUp-indexDown)*(log(valUp)-log(valDown)));
#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericDistributionLinLog::InterpolateValue " << value 
				   << " logValDown= " << log(valDown) << " logValUp= " << log(valUp)
				   << " indexVal= " << indexVal 
				   << " indexDown= " << indexDown << " indexUp= " << indexUp << G4endl;
#endif


  return value;

}

