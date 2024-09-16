#include "DicomOperExtendAir.hh"
#include "DicomVImageStr.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperExtendAir::DicomOperExtendAir( G4int iOrder, G4String name)
  : DicomVOperator( iOrder, name )
{  
  bAutomaticOperate = true;
}

//-----------------------------------------------------------------------------
//void DicomOperExtendAir::BuildMaterials( DicomVImage* image )
void DicomOperExtendAir::Operate( DicomVImage* image )
{
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomOperExtendAir::Operate"  << *image << G4endl;
#endif
  //----- Extract the new image limits (if not changed, set it equal to image limits)
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4double newMinX = paramMgr->GetNumericValue("minX",image->GetMinX()); // minimum extension of voxels (position of wall)
  G4double newMaxX = paramMgr->GetNumericValue("maxX",image->GetMaxX()); // maximum extension of voxels (position of wall)
  size_t newNoVoxelsX = paramMgr->GetNumericValue("nVoxX",image->GetNoVoxelsX()); // number of voxels in each dimensions
  G4double newMinY = paramMgr->GetNumericValue("minY",image->GetMinY()); 
  G4double newMaxY = paramMgr->GetNumericValue("maxY",image->GetMaxY()); 
  size_t newNoVoxelsY = paramMgr->GetNumericValue("nVoxY",image->GetNoVoxelsY()); 
  G4double newMinZ = paramMgr->GetNumericValue("minZ",image->GetMinZ());
  G4double newMaxZ = paramMgr->GetNumericValue("maxZ",image->GetMaxZ());
  size_t newNoVoxelsZ = paramMgr->GetNumericValue("nVoxZ",image->GetNoVoxelsZ()); 

  return Operate( image, newNoVoxelsX, newMinX, newMaxX, newNoVoxelsY, newMinY, newMaxY, newNoVoxelsZ, newMinZ, newMaxZ);
  
}


//-----------------------------------------------------------------------------
void DicomOperExtendAir::Operate( DicomVImage* image, G4int newNoVoxelsX, G4double newMinX, G4double newMaxX, G4int newNoVoxelsY, G4double newMinY, G4double newMaxY, G4int newNoVoxelsZ, G4double newMinZ, G4double newMaxZ)
{
  if( image->GetModality() == DIM_G4dcmCT_StructID ) {
    return OperateStr( dynamic_cast<DicomVImageStr*>(image), newNoVoxelsX, newMinX, newMaxX, newNoVoxelsY, newMinY, newMaxY, newNoVoxelsZ, newMinZ, newMaxZ);
  }

  G4double PRECISION = std::min(1e-4,2*image->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ)));
  G4cout << " DicomOperExtendAir::Operate PRECISION " << PRECISION << " " << image->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ << G4endl; //GDEB

  //----- Extract the old image data
  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxZ = image->GetNoVoxelsZ();
  size_t imNVoxXY = imNVoxX*imNVoxY;
  //  size_t imNVoxZ = image->GetNoVoxelsZ();
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMinZ = image->GetMinZ();
  G4double imMaxX = image->GetMaxX();
  G4double imMaxY = image->GetMaxY();
  G4double imMaxZ = image->GetMaxZ();
  G4double imVoxDimX = image->GetVoxelDimX();
  G4double imVoxDimY = image->GetVoxelDimY();
  G4double imVoxDimZ = image->GetVoxelDimZ();

#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-warningVerb) ) {
    G4cout << "IMAGE SIZE " << G4endl
	   << " X: " << imNVoxX << " " << imMinX << " " << imMaxX << G4endl
	   << " Y: " << imNVoxY << " " << imMinY << " " << imMaxY << G4endl
	   << " Z: " << imNVoxZ << " " << imMinZ << " " << imMaxZ << G4endl;
    G4cout << "NEW REQUESTED SIZE " << G4endl
	   << " X: " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl
	   << " Y: " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl
	   << " Z: " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl;
    G4cout << "DIFFERENCE IN LIMITS (must be <= 0) " << G4endl
	   << " X: " << -imMinX + newMinX << " " << imMaxX - newMaxX << G4endl
	   << " Y: " << -imMinY + newMinY << " " << imMaxY - newMaxY << G4endl
	   << " Z: " << -imMinZ + newMinZ << " " << imMaxZ - newMaxZ << G4endl;
    G4cout << "PRECISION " << PRECISION << G4endl;
  }
#endif

  //----- CHECK NEW VOXEL DIMS ARE EQUAL TO OLD ONES
  G4double newVoxDimY = (newMaxY-newMinY)/newNoVoxelsY;
  if( fabs(newVoxDimY - imVoxDimY) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New widthY "+GmGenUtils::ftoa(newVoxDimY)+" is different than old one: "+GmGenUtils::ftoa(imVoxDimY)+" DIFF= "+GmGenUtils::ftoa(fabs( newVoxDimY-imVoxDimY))).c_str());
  }
  G4double newVoxDimZ = (newMaxZ-newMinZ)/newNoVoxelsZ;
  if( fabs(newVoxDimZ - imVoxDimZ) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New widthZ "+GmGenUtils::ftoa(newVoxDimZ)+" is different than old one: "+GmGenUtils::ftoa(imVoxDimZ)+" DIFF= "+GmGenUtils::ftoa(fabs( newVoxDimZ-imVoxDimZ))).c_str());
  }

  //----- CHECK EXTENSION IS BY AN INTEGER NUMBER OF VOXELS AT ANY SIDE, AND THAT IT IS NOT NEGATIVE
  G4double changeMinX = imMinX - newMinX;
  G4double nBinMinX = changeMinX / imVoxDimX + PRECISION*0.2;
  if( fabs(nBinMinX - int(nBinMinX)) > PRECISION ) {
    G4cerr << "DIFF " << fabs(nBinMinX - int(nBinMinX)) << " > " << PRECISION << G4endl;
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at minX is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMinX)).c_str());
  }
  if( nBinMinX < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New minX "+GmGenUtils::ftoa(newMinX)+" is bigger than old one: "+GmGenUtils::ftoa(imMinX)+" DIFF= "+GmGenUtils::ftoa(fabs(newMinX-imMinX))).c_str());
  }
  G4double changeMaxX =  -imMaxX + newMaxX + PRECISION*0.2;
  G4double nBinMaxX = changeMaxX / imVoxDimX;
  if( fabs(nBinMaxX - int(nBinMaxX)) > PRECISION ) {
      G4cerr << "DIFF " << fabs(nBinMaxX - int(nBinMaxX)) << " > " << PRECISION << G4endl;
      G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at maxX is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMaxX)).c_str());
  }
  if( nBinMaxX < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New maxX "+GmGenUtils::ftoa(newMaxX)+" is smaller than old one: "+GmGenUtils::ftoa(imMaxX)+" DIFF= "+GmGenUtils::ftoa(fabs(newMaxX-imMaxX))).c_str());
  }
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate minX " << changeMinX << "=" << imMinX<<" - "<<newMinX <<" ; "<< nBinMinX <<"="<< changeMinX <<" / "<< imVoxDimX << G4endl
				    << " DicomOperExtendAir::Operate maxX " << changeMaxX << "=" << -imMaxX << " + " << newMaxX <<" ; "<< nBinMaxX<<"="<< changeMaxX << " / " << imVoxDimX << G4endl;
#endif

  G4double changeMinY = imMinY - newMinY + PRECISION*0.2;
  G4double nBinMinY = changeMinY / imVoxDimY;
  if( fabs(nBinMinY - int(nBinMinY)) > PRECISION ) {
     G4cerr << "DIFF " << fabs(nBinMinY - int(nBinMinY)) << " > " << PRECISION << G4endl;
     G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at minY is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMinY)).c_str());
  }
  if( nBinMinY < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New minY "+GmGenUtils::ftoa(newMinY)+" is bigger than old one: "+GmGenUtils::ftoa(imMinY)+" DIFF= "+GmGenUtils::ftoa(fabs(newMinY-imMinY))).c_str());
  }
  G4double changeMaxY =  -imMaxY + newMaxY + PRECISION*0.2;
  G4double nBinMaxY = changeMaxY / imVoxDimY;
  if( fabs(nBinMaxY - int(nBinMaxY)) > PRECISION ) {
    G4cerr << "DIFF " << fabs(nBinMaxY - int(nBinMaxY)) << " > " << PRECISION << G4endl;
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at maxY is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMaxY)).c_str());
  }
  if( nBinMaxY < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New maxY "+GmGenUtils::ftoa(newMaxY)+" is smaller than old one: "+GmGenUtils::ftoa(imMaxY)+" DIFF= "+GmGenUtils::ftoa(fabs(newMaxY-imMaxY))).c_str());
  }
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate minY " << changeMinY << "=" << imMinY<<" - "<<newMinY <<" ; "<< nBinMinY <<"="<< changeMinY <<" / "<< imVoxDimY << G4endl
				    << " DicomOperExtendAir::Operate maxY " << changeMaxY << "=" << -imMaxY << " + " << newMaxY <<" ; "<< nBinMaxY<<"="<< changeMaxY << " / " << imVoxDimY << G4endl;
#endif

  G4double changeMinZ = imMinZ - newMinZ + PRECISION*0.2;
  G4double nBinMinZ = changeMinZ / imVoxDimZ;
  if( fabs(nBinMinZ - int(nBinMinZ)) > PRECISION ) {
    G4cerr << "DIFF " << fabs(nBinMinZ - int(nBinMinZ)) << " > " << PRECISION << G4endl;
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at minZ is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMinZ)).c_str());
  }
  if( nBinMinZ < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New minZ "+GmGenUtils::ftoa(newMinZ)+" is bigger than old one: "+GmGenUtils::ftoa(imMinZ)+" DIFF= "+GmGenUtils::ftoa(fabs(newMinZ-imMinZ))).c_str());
  }
  G4double changeMaxZ =  -imMaxZ + newMaxZ + PRECISION*0.2;
  G4double nBinMaxZ = changeMaxZ / imVoxDimZ;
  if( fabs(nBinMaxZ - int(nBinMaxZ)) > PRECISION ) {
    G4cerr << "DIFF " << fabs(nBinMaxZ - int(nBinMaxZ)) << " > " << PRECISION << G4endl;
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at maxZ is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMaxZ)).c_str());
  }
  if( nBinMaxZ < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New maxZ "+GmGenUtils::ftoa(newMaxZ)+" is smaller than old one: "+GmGenUtils::ftoa(imMaxZ)+" DIFF= "+GmGenUtils::ftoa(fabs(newMaxZ-imMaxZ))).c_str());
  }
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate minZ " << changeMinZ << "=" << imMinZ<<" - "<<newMinZ <<" ; "<< nBinMinZ <<"="<< changeMinZ <<" / "<< imVoxDimZ << G4endl
				    << " DicomOperExtendAir::Operate maxZ " << changeMaxZ << "=" << -imMaxZ << " + " << newMaxZ <<" ; "<< nBinMaxZ<<"="<< changeMaxZ << " / " << imVoxDimZ << G4endl;
#endif

  
  //----- GET NEW VOXEL NUMBERS
  G4int iNewMinX = 0 - nBinMinX;
  G4int iNewMaxX = imNVoxX + nBinMaxX;
  G4int iNewMinY = 0 - nBinMinY;
  G4int iNewMaxY = imNVoxY + nBinMaxY;
  G4int iNewMinZ = 0 - nBinMinZ;
  G4int iNewMaxZ = imNVoxZ + nBinMaxZ;
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate iMinMaxX= " << iNewMinX << " " << iNewMinY 
				    << " Y= " << iNewMinY << " " << iNewMaxY
				    << " Z= " << iNewMinZ << " " << iNewMaxZ << G4endl;
#endif

  //----- FILL   
  std::vector<G4double>* newData = new std::vector<G4double>(newNoVoxelsX*newNoVoxelsY*newNoVoxelsZ);
  G4double datNew = 0.;
  if( image->GetModality() == DIM_G4dcmCT_MateDens ) {
    datNew = 0.00120479;
  }
  G4int imNewCopyNo = 0;
  for( G4int iz = iNewMinZ; iz < iNewMaxZ; iz++ ) {
    for( G4int iy = iNewMinY; iy < iNewMaxY; iy++ ) {
      for( G4int ix = iNewMinX; ix < iNewMaxX; ix++ ) {
	if( ix < 0 || iy < 0 || iz < 0 ||
	    ix >= G4int(imNVoxX) || iy >= G4int(imNVoxY) || iz >= G4int(imNVoxZ) ){
	  //	  G4cout << " NEWDATA " << imNewCopyNo << " "<<ix<<":"<<iy<<":"<<iz << " " <<newNoVoxelsX*newNoVoxelsY*newNoVoxelsZ << G4endl; //GDEB
	  newData->at(imNewCopyNo) = datNew;
	} else {
	  G4int imCopyNo = ix + imNVoxX*iy + imNVoxXY*iz;
	  newData->at(imNewCopyNo) = image->GetData(imCopyNo);
	}
	imNewCopyNo ++;
      }
    }
  }

  //----- Change image
  image->SetMinX(newMinX);
  image->SetMaxX(newMaxX);
  image->SetNoVoxelsX(newNoVoxelsX);
  image->SetMinY(newMinY);
  image->SetMaxY(newMaxY);
  image->SetNoVoxelsY(newNoVoxelsY);
  image->SetMinZ(newMinZ);
  image->SetMaxZ(newMaxZ);
  image->SetNoVoxelsZ(newNoVoxelsZ);
  image->SetData(newData);

  //  G4cout << "DicomOperExtendAir new image " << *image << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
void DicomOperExtendAir::OperateStr( DicomVImageStr* imageStr, G4int newNoVoxelsX, G4double newMinX, G4double newMaxX, G4int newNoVoxelsY, G4double newMinY, G4double newMaxY, G4int newNoVoxelsZ, G4double newMinZ, G4double newMaxZ)
{ 
  G4double PRECISION = std::min(1e-4,2*imageStr->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ)));
  G4cout << " DicomOperExtendAir::OperateStr PRECISION " << PRECISION << " " << imageStr->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ << G4endl; //GDEB

  //----- Extract the old image data
  size_t imNVoxX = imageStr->GetNoVoxelsX();
  size_t imNVoxY = imageStr->GetNoVoxelsY();
  size_t imNVoxZ = imageStr->GetNoVoxelsZ();
  size_t imNVoxXY = imNVoxX*imNVoxY;
  //  size_t imNVoxZ = imageStr->GetNoVoxelsZ();
  G4double imMinX = imageStr->GetMinX();
  G4double imMinY = imageStr->GetMinY();
  G4double imMinZ = imageStr->GetMinZ();
  G4double imMaxX = imageStr->GetMaxX();
  G4double imMaxY = imageStr->GetMaxY();
  G4double imMaxZ = imageStr->GetMaxZ();
  G4double imVoxDimX = imageStr->GetVoxelDimX();
  G4double imVoxDimY = imageStr->GetVoxelDimY();
  G4double imVoxDimZ = imageStr->GetVoxelDimZ();

#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-warningVerb) ) {
    G4cout << "IMAGE SIZE " << G4endl
	   << " X: " << imNVoxX << " " << imMinX << " " << imMaxX << G4endl
	   << " Y: " << imNVoxY << " " << imMinY << " " << imMaxY << G4endl
	   << " Z: " << imNVoxZ << " " << imMinZ << " " << imMaxZ << G4endl;
    G4cout << "NEW REQUESTED SIZE " << G4endl
	   << " X: " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl
	   << " Y: " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl
	   << " Z: " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl;
    G4cout << "DIFFERENCE IN LIMITS (must be <= 0) " << G4endl
	   << " X: " << -imMinX + newMinX << " " << imMaxX - newMaxX << G4endl
	   << " Y: " << -imMinY + newMinY << " " << imMaxY - newMaxY << G4endl
	   << " Z: " << -imMinZ + newMinZ << " " << imMaxZ - newMaxZ << G4endl;
    G4cout << "PRECISION " << PRECISION << G4endl;
  }
#endif

  //----- CHECK NEW VOXEL DIMS ARE EQUAL TO OLD ONES
  G4double newVoxDimY = (newMaxY-newMinY)/newNoVoxelsY;
  if( fabs(newVoxDimY - imVoxDimY) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New widthY "+GmGenUtils::ftoa(newVoxDimY)+" is different than old one: "+GmGenUtils::ftoa(imVoxDimY)+" DIFF= "+GmGenUtils::ftoa(fabs( newVoxDimY-imVoxDimY))).c_str());
  }
  G4double newVoxDimZ = (newMaxZ-newMinZ)/newNoVoxelsZ;
  if( fabs(newVoxDimZ - imVoxDimZ) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New widthZ "+GmGenUtils::ftoa(newVoxDimZ)+" is different than old one: "+GmGenUtils::ftoa(imVoxDimZ)+" DIFF= "+GmGenUtils::ftoa(fabs( newVoxDimZ-imVoxDimZ))).c_str());
  }

  //----- CHECK EXTENSION IS BY AN INTEGER NUMBER OF VOXELS AT ANY SIDE, AND THAT IT IS NOT NEGATIVE
  G4double changeMinX = imMinX - newMinX;
  G4double nBinMinX = changeMinX / imVoxDimX;
  if( fabs(nBinMinX - int(nBinMinX)) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at minX is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMinX)).c_str());
  }
  if( nBinMinX < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New minX "+GmGenUtils::ftoa(newMinX)+" is bigger than old one: "+GmGenUtils::ftoa(imMinX)+" DIFF= "+GmGenUtils::ftoa(fabs(newMinX-imMinX))).c_str());
  }
  G4double changeMaxX =  -imMaxX + newMaxX;
  G4double nBinMaxX = changeMaxX / imVoxDimX;
  if( fabs(nBinMaxX - int(nBinMaxX)) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at maxX is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMaxX)).c_str());
  }
  if( nBinMaxX < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New maxX "+GmGenUtils::ftoa(newMaxX)+" is smaller than old one: "+GmGenUtils::ftoa(imMaxX)+" DIFF= "+GmGenUtils::ftoa(fabs(newMaxX-imMaxX))).c_str());
  }
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate minX " << changeMinX << "=" << imMinX<<" - "<<newMinX <<" ; "<< nBinMinX <<"="<< changeMinX <<" / "<< imVoxDimX << G4endl
				    << " DicomOperExtendAir::Operate maxX " << changeMaxX << "=" << -imMaxX << " + " << newMaxX <<" ; "<< nBinMaxX<<"="<< changeMaxX << " / " << imVoxDimX << G4endl;
#endif

  G4double changeMinY = imMinY - newMinY;
  G4double nBinMinY = changeMinY / imVoxDimY;
  if( fabs(nBinMinY - int(nBinMinY)) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at minY is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMinY)).c_str());
  }
  if( nBinMinY < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New minY "+GmGenUtils::ftoa(newMinY)+" is bigger than old one: "+GmGenUtils::ftoa(imMinY)+" DIFF= "+GmGenUtils::ftoa(fabs(newMinY-imMinY))).c_str());
  }
  G4double changeMaxY =  -imMaxY + newMaxY;
  G4double nBinMaxY = changeMaxY / imVoxDimY;
  if( fabs(nBinMaxY - int(nBinMaxY)) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at maxY is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMaxY)).c_str());
  }
  if( nBinMaxY < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New maxY "+GmGenUtils::ftoa(newMaxY)+" is smaller than old one: "+GmGenUtils::ftoa(imMaxY)+" DIFF= "+GmGenUtils::ftoa(fabs(newMaxY-imMaxY))).c_str());
  }
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate minY " << changeMinY << "=" << imMinY<<" - "<<newMinY <<" ; "<< nBinMinY <<"="<< changeMinY <<" / "<< imVoxDimY << G4endl
				    << " DicomOperExtendAir::Operate maxY " << changeMaxY << "=" << -imMaxY << " + " << newMaxY <<" ; "<< nBinMaxY<<"="<< changeMaxY << " / " << imVoxDimY << G4endl;
#endif

  G4double changeMinZ = imMinZ - newMinZ;
  G4double nBinMinZ = changeMinZ / imVoxDimZ;
  if( fabs(nBinMinZ - int(nBinMinZ)) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at minZ is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMinZ)).c_str());
  }
  if( nBinMinZ < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New minZ "+GmGenUtils::ftoa(newMinZ)+" is bigger than old one: "+GmGenUtils::ftoa(imMinZ)+" DIFF= "+GmGenUtils::ftoa(fabs(newMinZ-imMinZ))).c_str());
  }
  G4double changeMaxZ =  -imMaxZ + newMaxZ;
  G4double nBinMaxZ = changeMaxZ / imVoxDimZ;
  if( fabs(nBinMaxZ - int(nBinMaxZ)) > PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" Extension at maxZ is not an integer number of voxels: "+GmGenUtils::ftoa(nBinMaxZ)).c_str());
  }
  if( nBinMaxZ < -PRECISION ) {
    G4Exception("DicomOperExtendAir::Operate",
		"",
		FatalException,
		(" New maxZ "+GmGenUtils::ftoa(newMaxZ)+" is smaller than old one: "+GmGenUtils::ftoa(imMaxZ)+" DIFF= "+GmGenUtils::ftoa(fabs(newMaxZ-imMaxZ))).c_str());
  }
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate minZ " << changeMinZ << "=" << imMinZ<<" - "<<newMinZ <<" ; "<< nBinMinZ <<"="<< changeMinZ <<" / "<< imVoxDimZ << G4endl
				    << " DicomOperExtendAir::Operate maxZ " << changeMaxZ << "=" << -imMaxZ << " + " << newMaxZ <<" ; "<< nBinMaxZ<<"="<< changeMaxZ << " / " << imVoxDimZ << G4endl;
#endif

  
  //----- GET NEW VOXEL NUMBERS
  G4int iNewMinX = 0 - nBinMinX;
  G4int iNewMaxX = imNVoxX + nBinMaxX;
  G4int iNewMinY = 0 - nBinMinY;
  G4int iNewMaxY = imNVoxY + nBinMaxY;
  G4int iNewMinZ = 0 - nBinMinZ;
  G4int iNewMaxZ = imNVoxZ + nBinMaxZ;
  #ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(-infoVerb) ) G4cout << " DicomOperExtendAir::Operate iMinMaxX= " << iNewMinX << " " << iNewMinY 
				    << " Y= " << iNewMinY << " " << iNewMaxY
				    << " Z= " << iNewMinZ << " " << iNewMaxZ << G4endl;
#endif

  //----- FILL   
  std::vector<G4String>* newDataStr = new std::vector<G4String>(newNoVoxelsX*newNoVoxelsY*newNoVoxelsZ);
  G4String datNew = "0";
  G4int imNewCopyNo = 0;
  for( G4int iz = iNewMinZ; iz < iNewMaxZ; iz++ ) {
    for( G4int iy = iNewMinY; iy < iNewMaxY; iy++ ) {
      for( G4int ix = iNewMinX; ix < iNewMaxX; ix++ ) {
	if( ix < 0 || iy < 0 || iz < 0 ||
	    ix >= G4int(imNVoxX) || iy >= G4int(imNVoxY) || iz >= G4int(imNVoxZ) ){
	  //-	  G4cout << " extNEWDATA " << imNewCopyNo << " "<<ix<<":"<<iy<<":"<<iz << " = " <<datNew << G4endl; //GDEB
	  newDataStr->at(imNewCopyNo) = datNew;
	} else {
	  G4int imCopyNo = ix + imNVoxX*iy + imNVoxXY*iz;
	  newDataStr->at(imNewCopyNo) = imageStr->GetDataStr(imCopyNo);
	  //- G4cout << " extOLDDATA " << imNewCopyNo << " " << imCopyNo << " " <<ix<<":"<<iy<<":"<<iz << " = " << imageStr->GetDataStr(imCopyNo) << G4endl; //GDEB
	}
	imNewCopyNo ++;
      }
    }
  }

  //----- Change image
  imageStr->SetMinX(newMinX);
  imageStr->SetMaxX(newMaxX);
  imageStr->SetNoVoxelsX(newNoVoxelsX);
  imageStr->SetMinY(newMinY);
  imageStr->SetMaxY(newMaxY);
  imageStr->SetNoVoxelsY(newNoVoxelsY);
  imageStr->SetMinZ(newMinZ);
  imageStr->SetMaxZ(newMaxZ);
  imageStr->SetNoVoxelsZ(newNoVoxelsZ);
  imageStr->SetDataStr(newDataStr);

  //  G4cout << "DicomOperExtendAir new image " << *image << G4endl; //GDEB
}

