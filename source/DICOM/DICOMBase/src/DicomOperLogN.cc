#include "DicomOperLogN.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperLogN::DicomOperLogN( G4double logn, G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name )    
{
  theLogNumber = logn;
}

//-----------------------------------------------------------------------------
void DicomOperLogN::Operate( DicomVImage* image )
{
  size_t nVox = image->GetNoVoxels();
  std::vector<G4double>* data = image->GetData();
  G4double* dataP = &(data->at(0));
  G4double logFactor = log(theLogNumber);
  for( size_t ii = 0; ii < nVox; ii++, dataP++ ) {
    if( (*dataP) != 0 ) {
      //      G4cout << ii << "DicomOperLogN " << log(*dataP)/logFactor  << " = log( " << *dataP << " )/ " << logFactor << G4endl; //GDEB 
      (*dataP) = log(*dataP)/logFactor;
    } else {
      (*dataP) = -99;
    }
  }
  
}
