#include "GmNumericNDimDistributionUpper.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

//---------------------------------------------------------------------
GmNumericNDimDistributionUpper::GmNumericNDimDistributionUpper(G4String name)
  : GmVNumericNDimDistribution(name)
{
}


//---------------------------------------------------------------------
G4double GmNumericNDimDistributionUpper::InterpolateValue( const std::vector<std::vector<G4int> >& indices )
{
  
  std::vector<G4int> indexVal;
  std::vector<G4int> ind1 = indices[indices.size()-1];
  for( G4int ii = 0; ii < theNDim; ii++ ) {
    indexVal.push_back( ind1[ii] );
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb) ) {
      G4cout << "NDim Upper indexVal " << indexVal[ii] << G4endl; //GDEB
    }
#endif
  }
  G4double val = theValuesNDim[indexVal];
  
#ifndef GAMOS_NO_VERB
  if( DistVerb(debugVerb) ) G4cout << " GmNumericNDimDistributionUpper::InterpolateValue " << val << G4endl;
#endif

  //   std::map<std::vector<G4int>,G4double> theValuesNDim;

  return val;

}

