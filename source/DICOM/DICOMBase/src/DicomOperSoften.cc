#include "DicomOperSoften.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperSoften::DicomOperSoften( G4int iOrder, G4String name)
  : DicomVOperator( iOrder, name )
{  
  bAutomaticOperate = true;
}

//-----------------------------------------------------------------------------
void DicomOperSoften::Operate( DicomVImage* image )
{ 
  Operate(image,2.,1,DBL_MAX,-DBL_MAX,DBL_MAX);
}

//-----------------------------------------------------------------------------
void DicomOperSoften::Operate( DicomVImage* image, G4double factor, G4int nAround, G4double maxValue, G4double minSpareValue, G4double maxSpareValue )
{
  theFactor = factor; // data <= dataAround *factor
  theNAround = nAround; // n voxels around
  theMaxValue = maxValue; // maxValue (if bigger -> maxValue)
  theMinSpareValue = minSpareValue; 
  theMaxSpareValue = maxSpareValue; // if minSpareValue < value < maxSpareValue do not check it
  if( DicomVerb(infoVerb) ) G4cout << "DicomOperSoften::Operate " << theFactor << " " << theNAround << " " << image->GetName() << G4endl; 
 
  //----- Extract the old image data
  G4int imNoVoxX = image->GetNoVoxelsX();
  G4int imNoVoxY = image->GetNoVoxelsY();
  G4int imNoVoxZ = image->GetNoVoxelsZ();
  G4int imNoVoxXY = imNoVoxX*imNoVoxY;

  std::vector<G4double>* oldData = image->GetData();
  std::vector<G4double>* newData = new std::vector<G4double>(imNoVoxX*imNoVoxY*imNoVoxZ);

  G4double maxBig = -DBL_MAX;
  //----- Loop to voxels of image
  for( G4int iz = 0; iz < imNoVoxZ; iz++ ) {
    G4double iminZn = std::max(0,iz-theNAround);
    G4double imaxZn = std::min(imNoVoxZ-1,iz+theNAround);
    for( G4int iy = 0; iy < imNoVoxY; iy++ ) {
      G4double iminYn = std::max(0,iy-theNAround);
      G4double imaxYn = std::min(imNoVoxY-1,iy+theNAround);
      for( G4int ix = 0; ix < imNoVoxX; ix++ ) {
	G4double iminXn = std::max(0,ix-theNAround);
	G4double imaxXn = std::min(imNoVoxX-1,ix+theNAround);
	size_t copyNo = ix + imNoVoxX*iy + imNoVoxXY*iz;
	G4double data = oldData->at(copyNo);
	if( data > theMinSpareValue && data < theMaxSpareValue ) {
	  newData->at(copyNo) = data;
	  continue;
	}
	G4double dataAround = 0.;
	for( G4int ixn = iminXn; ixn < imaxXn; ixn++ ) {
	  for( G4int iyn = iminYn; iyn < imaxYn; iyn++ ) {
	    for( G4int izn = iminZn; izn < imaxZn; izn++ ) {
	      size_t copyNoNew = ixn + imNoVoxX*iyn + imNoVoxXY*izn;
	      dataAround += oldData->at(copyNoNew);
	      //	      if( ix == 207 && iy == 29 && iz == 49 ) G4cout << "DicomOperSoften " << ix<<":"<<iy<<":"<<iz << " DATA " << data << " cp " << copyNoNew << " " <<dataAround << G4endl; //GDEB
	    }
	  }
	}
	dataAround /= ((imaxXn-iminXn)*(imaxYn-iminYn)*(imaxZn-iminZn));
	if( DicomVerb(testVerb) ) G4cout << copyNo << " DicomOperSoften " << ix<<":"<<iy<<":"<<iz << " DATA " << data << " >? " << dataAround << "*" << theFactor << G4endl;
	if( data > dataAround*theFactor ) {
	  maxBig = std::max(maxBig,data);
	  if( DicomVerb(infoVerb) ) G4cout << copyNo << "DicomOperSoften " << ix<<":"<<iy<<":"<<iz << " DATA BIG " << data << " >? " << dataAround << "*" << theFactor << G4endl;
	  newData->at(copyNo) = dataAround;
	} else {
	  newData->at(copyNo) = data;
	}
	if( newData->at(copyNo) > theMaxValue ) {
	  if( DicomVerb(infoVerb) ) G4cout << copyNo << "DicomOperSoften " << ix<<":"<<iy<<":"<<iz << " DATA BIGGER MAX " <<  newData->at(copyNo) << " >? " << theMaxValue << G4endl;
	  newData->at(copyNo) = theMaxValue;
	} 
	if( DicomVerb(testVerb) ) G4cout << "DicomOperSoften " << ix<<":"<<iy<<":"<<iz << " DATA NOW " <<  newData->at(copyNo) << G4endl;

      }
    }
  }
  if( DicomVerb(infoVerb) ) G4cout << "DicomOperSoften DATA MAX BIG " << maxBig << G4endl;

  //----- Change image
  image->SetData(newData);

  G4cout << " DOSEIMAGE IN SOFTENING " << image->GetData(1476) << G4endl; //GDEB

  //  G4cout << "DicomOperSoften new image " << *image << G4endl; //GDEB
}
