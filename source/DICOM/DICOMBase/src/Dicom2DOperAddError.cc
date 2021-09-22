#include "Dicom2DOperAddError.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandGauss.h"

//-----------------------------------------------------------------------------
Dicom2DOperAddError::Dicom2DOperAddError( G4int iOrder, G4String name, G4double nSig )
  : theName(name), theOrder(iOrder), theNSigmas(nSig)
{

}

//-----------------------------------------------------------------------------
void Dicom2DOperAddError::Operate( DicomVImage* image,  DicomVImage* imageErr )
{
  //  if( DicomVerb(infoVerb) ) G4cout << " Dicom2DOperAddError::Operate " << imageErr->GetData(1000000) << G4endl; //GDEB
  size_t nVox = image->GetNoVoxels();
  std::vector<G4double>* data = image->GetData();
  G4double* dataP = &(data->at(0));
  std::vector<G4double>* dataErr = imageErr->GetData();
  G4double* dataErrP = &(dataErr->at(0));
  G4bool bRandom = true;
  if( theNSigmas < 0 ) bRandom = false;
    
  for( size_t ii = 0; ii < nVox; ii++, dataP++, dataErrP++ ) {
    G4float rnd = 1.;
    if( bRandom ) {   
      rnd = CLHEP::RandGauss::shoot(0.,(*dataErrP))*theNSigmas;
    } else {
      rnd = *dataErrP;
    }
    //    if( ii%100 == 0 ) G4cout << bRandom << " Dicom2DOperAddError " << ii << " = " << (*dataP) << " + " << rnd << " : " << (*dataErrP) << " -> " << (*dataP)+rnd << G4endl; //GDEB
    (*dataP) += rnd;
  }
  
}
