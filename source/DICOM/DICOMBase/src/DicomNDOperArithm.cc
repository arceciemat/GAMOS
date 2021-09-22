#include "DicomNDOperArithm.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandGauss.h"

//-----------------------------------------------------------------------------
DicomNDOperArithm::DicomNDOperArithm( G4int iOrder, G4String name, G4String arithm )
  : theName(name), theOrder(iOrder)    
{
  //@@@@ CHECK ARITHMETIC OPERATION
  theArithmeticOperation = arithm;
  
  theNData = 0;
  size_t ipos = 0;
  while ((ipos = theArithmeticOperation.find("DATA",ipos)) != std::string::npos) {
    ++theNData ;
    ipos += 4;
  }
   
  if( theNData == 0 || theNData >= 10 ) {
    G4Exception("DicomNDOperArithm::DicomNDOperArithm",
		"",
		FatalException,
		("Arithmetic expression must contain between 1 and 9 times the word 'DATA'. It is "+theArithmeticOperation+" n DATA="+GmGenUtils::itoa(theNData)).c_str());
  }

  //----- Check DATA numbers are ordered
  size_t ifdat = theArithmeticOperation.find("DATA");
  size_t ifdatold = 0;
  G4int ndat = 1;
  G4String checkArithm = "";
  G4cout << " DicomNDOperArithm " << theArithmeticOperation << G4endl; //GDEB
  do {
    if( GmGenUtils::GetInt(theArithmeticOperation.substr(ifdat+4,1)) != ndat ) {
      G4Exception("DicomNDOperArithm::DicomNDOperArithm",
		  "",
		  FatalException,
		  ("Arithmetic expression must has contain list of 'DATA' with number orderes 'DATA1', 'DATA2', 'DATA3', .... It is "+theArithmeticOperation).c_str());
    }
    checkArithm += theArithmeticOperation.substr(ifdatold,ifdat-ifdatold)+"1";
    //    G4cout << ndat << " DicomNDOperArithm checkArithm " << checkArithm << " ifdat " << ifdat << " ifdatold " << ifdatold << G4endl; //GDEB
    ifdatold = ifdat+5;
    ifdat = theArithmeticOperation.find("DATA",ifdat+4);
    ndat++;
  } while (ifdat != std::string::npos );

  //  G4cout << " DicomNDOperArithm::DicomNDOperArithm CHECKING ARITHMETIC EXPRESSION WITH '1' " << checkArithm << G4endl; //GDEB
  GmGenUtils::GetValue(checkArithm);
  G4cout << " CHECKED DicomNDOperArithm::DicomNDOperArithm CHECKING ARITHMETIC EXPRESSION WITH '1' " << checkArithm << G4endl; 

  bChecked = false;
}

//-----------------------------------------------------------------------------
void DicomNDOperArithm::Operate( DicomVImage* newImage, std::vector<DicomVImage*> images )
{
  if( !bChecked ) {
    for( size_t ii = 0; ii < images.size(); ii++ ) {
      newImage->CheckImageHeadersAreEqual(images[ii]);
    }
    bChecked = true;
  }
  
  size_t nVox = newImage->GetNoVoxels();
  std::vector<G4double*> theDataP;
  std::vector<G4double>* newData = newImage->GetData();
  //  G4double* newDataP = &(newData->at(0));
  for( size_t ii = 0; ii < images.size(); ii++ ) {
    std::vector<G4double>* data = images[ii]->GetData();
    theDataP.push_back( &(data->at(0)) );
  }
  for( size_t ii = 0; ii < nVox; ii++ ) {
    size_t ifdat = theArithmeticOperation.find("DATA");
    size_t ifdatold = 0;
    G4String checkArithm = "";
    G4int idat = 0;
    do {
      checkArithm += theArithmeticOperation.substr(ifdatold,ifdat-ifdatold)+GmGenUtils::ftoa(*(theDataP[idat]));
      ifdatold = ifdat+5;
      ifdat = theArithmeticOperation.find("DATA",ifdat+4);
      //      if( ii% 100000 == 0 ) G4cout << ii << " DATA IMAGE " << idat << " : " << images[idat]->GetData(ii) << " = " << GmGenUtils::ftoa(*(theDataP[idat])) <<  G4endl; //GDEB
      idat++;
    } while (ifdat != std::string::npos );
    //    if( ii% 100000 == 0 ) G4cout << ii << " DicomNDOperArithm::Operate OLD = " << newData->at(ii) << G4endl; //GDEB
    newData->at(ii) = GmGenUtils::GetValue(checkArithm);
    //    if( ii% 100000 == 0 ) G4cout << ii << " DicomNDOperArithm::Operate " << checkArithm << " = " << newData->at(ii) << G4endl; //GDEB
    for( size_t jj = 0; jj < images.size(); jj++ ) {
      theDataP[jj]++;
    }
  }


}
