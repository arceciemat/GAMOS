#include "DicomOperMirrorY.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperMirrorY::DicomOperMirrorY( G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name )    
{
  G4cout << " DicomOperMirrorY::DicomOperMirrorY( " << G4endl; //GDEB
  //  DicomParameterMgr* theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  //  theParamMgr->SetRotateXY180( true );
}

//-----------------------------------------------------------------------------
void DicomOperMirrorY::Operate( DicomVImage* image )
{
  size_t nVoxX = image->GetNoVoxelsX();  
  size_t nVoxY = image->GetNoVoxelsY();  
  size_t nVoxXY = nVoxX*nVoxY;
  size_t nVoxZ = image->GetNoVoxelsZ();  
  size_t iN = 0;
  std::cout << image->GetName() << " MIRRORING  Y " << std::endl; //GDEB
  std::vector<G4double>* data = image->GetData();
  std::vector<G4double>* dataNew = new std::vector<G4double>;
  for( size_t iz = 0; iz < nVoxZ; iz++ ) {
    for( size_t iy = nVoxY-1; int(iy) >= 0; iy-- ) {
      for( size_t ix = 0; ix < nVoxX; ix++ ) {
	iN = ix+iy*nVoxX+iz*nVoxXY;
	dataNew->push_back(data->at(iN));
	//	  if( iN > 000 && iN < 1010 &&  data->at(iN) != 0) std::cout << iN << " -> " << dataNew->size()-1 << " DATA " << data->at(iN) << " -> " <<  dataNew->at(dataNew->size()-1) << std::endl; //GDEB
      }
    }
  }
  image->SetData(dataNew);
  
  /*   G4double imMinX = image->GetMinX();
       G4double imMinY = image->GetMinY();
       G4double imMaxX = image->GetMaxX();
       G4double imMaxY = image->GetMaxY();
       image->SetMinX(imMaxX);
       image->SetMaxX(imMinX);
       image->SetMinY(imMaxY);
       image->SetMaxY(imMinY);
  */
  
}
