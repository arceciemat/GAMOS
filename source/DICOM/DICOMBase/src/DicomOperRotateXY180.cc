#include "DicomOperRotateXY180.hh"
#include "DicomVImage.hh"
#include "DicomMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperRotateXY180::DicomOperRotateXY180( G4double angle, G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name )    
{
  //  G4cout << " DicomOperRotateXY180::DicomOperRotateXY180( " << G4endl; //GDEB
  theAngle = std::fmod(angle,360.);
  if( fabs( theAngle/180 - int(theAngle/180) ) > 1e-6 ) {
    G4Exception("DicomOperRotateXY180::DicomOperRotateXY180",
		"",
		FatalErrorInArgument,
		("Rotation angle must be a multiple of 90 degrees, while it is " + GmGenUtils::ftoa(theAngle)).c_str());
  }
  if( fabs(theAngle-180.) < 1.e-6 ) DicomMgr::GetInstance()->SetRotateXY180( true );
  //  G4cout << " DicomOperRotateXY180 " << fabs(theAngle-180.) << " = " << DicomMgr::GetInstance()->GetRotateXY180() <<  G4endl ; //GDEB
  bAutomaticOperate = false;
}

//-----------------------------------------------------------------------------
void DicomOperRotateXY180::Operate( DicomVImage* image )
{
  size_t nVoxX = image->GetNoVoxelsX();  
  size_t nVoxY = image->GetNoVoxelsY();  
  size_t nVoxXY = nVoxX*nVoxY;
  size_t nVoxZ = image->GetNoVoxelsZ();  
  size_t iN = 0;
  if( fabs(theAngle - 180.) < 1.e-6 ) {
    //    std::cout << image->GetName() << " ROTATING  XY " <<theAngle << std::endl; //GDEB
    std::vector<G4double>* data = image->GetData();
    std::vector<G4double>* dataNew = new std::vector<G4double>;
    for( size_t iz = 0; iz < nVoxZ; iz++ ) {
      for( size_t iy = nVoxY-1; int(iy) >= 0; iy-- ) {
	for( size_t ix = nVoxX-1; int(ix) >= 0; ix-- ) {
	  iN = ix+iy*nVoxX+iz*nVoxXY;
	  dataNew->push_back(data->at(iN));
	  //	  if( iN > 000 && iN < 1010 &&  data->at(iN) != 0) std::cout << iN << " -> " << dataNew->size()-1 << " DATA " << data->at(iN) << " -> " <<  dataNew->at(dataNew->size()-1) << std::endl; //GDEB
	}
      }
    }
    image->SetData(dataNew);

  }
}
