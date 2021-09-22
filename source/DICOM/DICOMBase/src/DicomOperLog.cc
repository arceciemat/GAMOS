#include "DicomOperLog.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperLog::DicomOperLog( G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name )    
{
}

//-----------------------------------------------------------------------------
void DicomOperLog::Operate( DicomVImage* image )
{
  size_t nVox = image->GetNoVoxels();
  std::vector<G4double>* data = image->GetData();
  G4double* dataP = &(data->at(0));
  for( size_t ii = 0; ii < nVox; ii++, dataP++ ) {
    if( (*dataP) != 0 ) {
      (*dataP) = log(*dataP);
    } else {
      (*dataP) = -99;
    }
  }
  
}
