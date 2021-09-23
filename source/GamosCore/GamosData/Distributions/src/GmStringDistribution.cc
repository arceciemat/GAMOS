#include "GmStringDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmStringDistribution::GmStringDistribution(G4String name)
  : GmVStringDistribution(name)
{
  BuildData();
}

//---------------------------------------------------------------------
G4double GmStringDistribution::GetValueFromStep(const G4Step* aStep)
{

  if( !theData ) { 
    G4Exception("GmVStringDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 

  G4String indexVal = theData->GetStringValueFromStep( aStep );

  return GetStringValueFromIndex( indexVal );
}

