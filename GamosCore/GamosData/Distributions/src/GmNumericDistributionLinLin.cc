#include "GmNumericDistributionLinLin.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmNumericDistributionLinLin::GmNumericDistributionLinLin(G4String name)
  : GmVNumericDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericDistributionLinLin::InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp )
{
  G4double value = valDown + (indexVal-indexDown)/(indexUp-indexDown)*(valUp-valDown);

#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericDistributionLinLin::InterpolateValue " << value 
				   << " valDown= " << valDown << " valUp= " << valUp 
				   << " indexVal= " << indexVal 
				   << " indexDown= " << indexDown << " indexUp= " << indexUp << G4endl;
#endif


  return value;

}

