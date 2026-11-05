#include "DicomOperReflectY.hh"
#include "DicomVImage.hh"
#include "DicomMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperReflectY::DicomOperReflectY( G4int iOrder, G4String name )
  : DicomVOperator( iOrder, name )    
{
  //  G4cout << " DicomOperReflectY::DicomOperReflectY( " << G4endl; //GDEB
  //  G4cout << " DicomOperReflectY " << DicomMgr::GetInstance()->GetReflectY() <<  G4endl ; //GDEB
  bAutomaticOperate = false;
}

//-----------------------------------------------------------------------------
void DicomOperReflectY::Operate( DicomVImage* image )
{
  if ( ! DicomMgr::GetInstance()->GetReflectY() ) {
    return;
  }
  size_t nVoxX = image->GetNoVoxelsX();  
  size_t nVoxY = image->GetNoVoxelsY();  
  size_t nVoxXY = nVoxX*nVoxY;
  size_t nVoxZ = image->GetNoVoxelsZ();  
  size_t iN = 0;
  std::cout << image->GetName() << " REFLECTING Y " << std::endl; //GDEB
  std::vector<G4double>* data = image->GetData();
  std::vector<G4double>* dataNew = new std::vector<G4double>;
  for( size_t iz = 0; iz < nVoxZ; iz++ ) {
    for( size_t iy = nVoxY-1; int(iy) >= 0; iy-- ) {
      for( size_t ix = 0; ix < nVoxX; ix++ ) {
	iN = ix+iy*nVoxX+iz*nVoxXY;
	//    G4cout << data << "SIZEnew " << ix << " " << iy << " " << iz << " " << iN << " " << data->size() << G4endl; //GDEB
	dataNew->push_back(data->at(iN));
	//    if( iN > 000 && iN < 1010 &&  data->at(iN) != 0) std::cout << iN << " -> " << dataNew->size()-1 << " DATA " << data->at(iN) << " -> " <<  dataNew->at(dataNew->size()-1) << std::endl; //GDEB
      }
    }
  }
  image->SetData(dataNew);
}
