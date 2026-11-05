#include "DicomOperResize.hh"
#include "DicomVImage.hh"
#include "DicomVImageStr.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomVReader.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTStruct.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperResize::DicomOperResize( G4int iOrder, G4String name)
  : DicomVOperator( iOrder, name )
{  
  bAutomaticOperate = true;
}

//-----------------------------------------------------------------------------
void DicomOperResize::Operate( DicomVImage* image )
{
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomOperResize::Operate"  << *image << G4endl;
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
  G4bool bInterpolate = G4bool(paramMgr->GetNumericValue("bInterpolate",1));
  
  G4double PRECISION = std::min(2e-5,2*image->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ)));
  //  PRECISION /= 10.;
  //G4cout << " 1PRECISION " << PRECISION << " " << image->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ << G4endl; //GDEB

  G4bool bChangeNX = newNoVoxelsX == image->GetNoVoxelsX();
  G4bool bChangeNY = newNoVoxelsY == image->GetNoVoxelsY();
  G4bool bChangeNZ = newNoVoxelsZ == image->GetNoVoxelsZ();
  //----- DO NOT INTERPOLATE: MOVE MIN/MAX TO VOXEL EDGES
  //  G4cout << " BINTERPOLATE " << bInterpolate << "=" << paramMgr->GetNumericValue("bInterpolate",1) << G4endl; //GDEB
  bInterpolate = true; //GDEB
  G4cout << " BINTERPOLATE " << bInterpolate << G4endl; //GDEB
  if( !bInterpolate ) {
    if( bChangeNX ) {
      G4cout << bChangeNX << " newNoVoxelsX " << newNoVoxelsX << "==" << image->GetNoVoxelsX() << G4endl;
      G4Exception("DicomOperResize::Operate",
		  "",
		  FatalException,
		  "No interpolation: nVoxX will be calculated as (newMaxX - newMinX)/stepX, it cannot be set by the user");
    }
    if( bChangeNY ) {
      G4cout << bChangeNY << " newNoVoxelsY " << newNoVoxelsY << "==" << image->GetNoVoxelsY() << G4endl;
      G4Exception("DicomOperResize::Operate",
		  "",
		  FatalException,
		  "No interpolation: nVoxY will be calculated as (newMaxY - newMinY)/stepY, it cannot be set by the user");
    }
    if( bChangeNZ ) {
      G4cout << bChangeNZ << " newNoVoxelsZ " << newNoVoxelsZ << "==" << image->GetNoVoxelsZ() << G4endl;
      G4Exception("DicomOperResize::Operate",
		  "",
		  FatalException,
		  "No interpolation: nVoxZ will be calculated as (newMaxZ - newMinZ)/stepZ, it cannot be set by the user");
    }
    
    //    if( DicomVerb(warningVerb) ) G4cout << "DicomOperResize Min/MaxX/Z may be changed to include the totality of the voxel that is cut by the new value, unless you change the number of voxels (use -resizeFitOld 0  if you do not want this change) " << G4endl; 
    //-- if nVoxX is not changed, use minX, maxX to cut the image: if it cuts a voxel, include it also
    //  G4cout << " NEWMINX " << newMinX << " != " << image->GetMinX() << " && " << paramMgr->IsParameterInScript("nVoxX") << G4endl; //GDEB
    if( newMinX != image->GetMinX() && bChangeNX ) {
      int ibinX = GmGenUtils::GetBelowInt((newMinX-image->GetMinX())/image->GetVoxelDimX(),PRECISION);
      if( ibinX < 0 ) ibinX = 0;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MINX " << newMinX << " ->IBINX " << ibinX << " = (" << newMinX << " - " << image->GetMinX() << " )/ " << image->GetVoxelDimX() << " " << (newMinX-image->GetMinX())/image->GetVoxelDimX() << " " << GmGenUtils::GetBelowInt((newMinX-image->GetMinX())/image->GetVoxelDimX(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MINX " << image->GetMinX()+ibinX*image->GetVoxelDimX() << G4endl;
      }
#endif
      newMinX = image->GetMinX()+ibinX*image->GetVoxelDimX();
      if( ibinX >= 0 && bChangeNX ) newNoVoxelsX -= ibinX;
    }
    //    G4cout << " XLIM " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl; //GDEB
    
    if( newMaxX != image->GetMaxX() && bChangeNX ) {
      int ibinX = GmGenUtils::GetAboveInt((newMaxX-image->GetMinX())/image->GetVoxelDimX(),PRECISION);
      if( ibinX >= G4int(image->GetNoVoxelsX()) ) ibinX = image->GetNoVoxelsX();
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MAXX " << newMaxX << " ->IBINX " << ibinX << " = (" << newMaxX << " - " << image->GetMaxX() << " )/ " << image->GetVoxelDimX() << " " << (newMaxX-image->GetMaxX())/image->GetVoxelDimX() << " " << GmGenUtils::GetBelowInt((newMaxX-image->GetMaxX())/image->GetVoxelDimX(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MAXX " << image->GetMinX()+ibinX*image->GetVoxelDimX() << G4endl;
      }
#endif
      newMaxX = image->GetMinX()+ibinX*image->GetVoxelDimX();
      if( ibinX < G4int(image->GetNoVoxelsX()) && bChangeNX ) newNoVoxelsX -= image->GetNoVoxelsX() - ibinX;
    }
    //  G4cout << " XLIM " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl; //GDEB
    
    if( newMinY != image->GetMinY() && bChangeNY ) {
      int ibinY = GmGenUtils::GetBelowInt((newMinY-image->GetMinY())/image->GetVoxelDimY(),PRECISION);
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize newMINY " << newMinY << " ->IBINY " << ibinY << " = (" << newMinY << " - " << image->GetMinY() << " )/ " << image->GetVoxelDimY() << " " << (newMinY-image->GetMinY())/image->GetVoxelDimY() << " " << GmGenUtils::GetBelowInt((newMinY-image->GetMinY())/image->GetVoxelDimY(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MINY " << image->GetMinY()+ibinY*image->GetVoxelDimY() << G4endl;
      }
#endif
      newMinY = image->GetMinY()+ibinY*image->GetVoxelDimY();
      if( ibinY >= 0 && bChangeNY ) newNoVoxelsY -= ibinY;
    }
    //    if( DicomVerb(debugVerb) ) G4cout << " YLIM " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl; //GDEB
    
   //    if( newMaxY != image->GetMaxY() && !paramMgr->IsParameterInScript("nVoxY") ) {
    if( newMaxY != image->GetMaxY() && bChangeNY ) {
      int ibinY = GmGenUtils::GetAboveInt((newMaxY-image->GetMinY())/image->GetVoxelDimY(),PRECISION);
      //    G4cout << " IBINY " << ibinY << " " << newMaxY << " - " << image->GetMinY() << " )/ " << image->GetVoxelDimY() << " " << (newMaxY-image->GetMinY())/image->GetVoxelDimY() << " " << GmGenUtils::GetAboveInt((newMaxY-image->GetMinY())/image->GetVoxelDimY(),PRECISION);) << G4endl; //GDEB
      //    G4cout << " NEW MAY Y CHANGED " << newMaxY << " -> " << image->GetMinY()+ibinY*image->GetVoxelDimY()<< G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MAXY " << newMaxY << " ->IBINY " << ibinY << " = (" << newMaxY << " - " << image->GetMaxY() << " )/ " << image->GetVoxelDimY() << " " << (newMaxY-image->GetMaxY())/image->GetVoxelDimY() << " " << GmGenUtils::GetBelowInt((newMaxY-image->GetMaxY())/image->GetVoxelDimY(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MAXY " << image->GetMinY()+ibinY*image->GetVoxelDimY() << G4endl;
      }
#endif
      newMaxY = image->GetMinY()+ibinY*image->GetVoxelDimY();
      if( ibinY < G4int(image->GetNoVoxelsY()) && bChangeNY ) newNoVoxelsY -= image->GetNoVoxelsY() - ibinY;
    }
    //    if( DicomVerb(infoVerb) ) G4cout << " YLIM " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl; //GDEB
        
    if( newMinZ != image->GetMinZ() && bChangeNZ ) {
      int ibinZ = GmGenUtils::GetBelowInt((newMinZ-image->GetMinZ())/image->GetVoxelDimZ(),PRECISION);
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MINZ " << newMinZ << " ->IBINZ " << ibinZ << " = (" << newMinZ << " - " << image->GetMinZ() << " )/ " << image->GetVoxelDimZ() << " " << (newMinZ-image->GetMinZ())/image->GetVoxelDimZ() << " " << GmGenUtils::GetBelowInt((newMinZ-image->GetMinZ())/image->GetVoxelDimZ(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MINZ " << image->GetMinZ()+ibinZ*image->GetVoxelDimZ() << G4endl;
      }
#endif
      newMinZ = image->GetMinZ()+ibinZ*image->GetVoxelDimZ();
      if( ibinZ >= 0 && bChangeNZ ) newNoVoxelsZ -= ibinZ;
    }
    //    G4cout << " ZLIM " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl; //GDEB
    
    if( newMaxZ != image->GetMaxZ() && bChangeNZ ) {
      int ibinZ = GmGenUtils::GetAboveInt((newMaxZ-image->GetMinZ())/image->GetVoxelDimZ(),PRECISION);
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MAXZ " << newMaxZ << " ->IBINZ " << ibinZ << " = (" << newMaxZ << " - " << image->GetMaxZ() << " )/ " << image->GetVoxelDimZ() << " " << (newMaxZ-image->GetMaxZ())/image->GetVoxelDimZ() << " " << GmGenUtils::GetBelowInt((newMaxZ-image->GetMaxZ())/image->GetVoxelDimZ(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MAXZ " << image->GetMinZ()+ibinZ*image->GetVoxelDimZ() << G4endl;
      }
#endif
      newMaxZ = image->GetMinZ()+ibinZ*image->GetVoxelDimZ();
      if( ibinZ < G4int(image->GetNoVoxelsZ()) && bChangeNZ ) newNoVoxelsZ -= image->GetNoVoxelsZ() - ibinZ;
    }
    //    G4cout << " ZLIM " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl; //GDEB
  }
  
  //  G4cout << " NEWMINX " << newMinX << " != " << image->GetMinX() << " && " << paramMgr->IsParameterInScript("nVoxX") << G4endl; //GDEB
  if( newMinX != image->GetMinX() && bChangeNX ) {
    int ibinX = GmGenUtils::GetBelowInt((newMinX-image->GetMinX())/image->GetVoxelDimX(),PRECISION);
    if( ibinX < 0 ) ibinX = 0;
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) ) {
      G4cout << " DicomOperResize MINX " << newMinX << " ->IBINX " << ibinX << " = (" << newMinX << " - " << image->GetMinX() << " )/ " << image->GetVoxelDimX() << " " << (newMinX-image->GetMinX())/image->GetVoxelDimX() << " " << GmGenUtils::GetBelowInt((newMinX-image->GetMinX())/image->GetVoxelDimX(),PRECISION) << G4endl;
      G4cout << " DicomOperResize NEW MINX " << image->GetMinX()+ibinX*image->GetVoxelDimX() << G4endl;
    }
#endif      
    newMinX = image->GetMinX()+ibinX*image->GetVoxelDimX();
    if( ibinX >= 0 && bChangeNX ) newNoVoxelsX -= ibinX;
  }
  //  G4cout << " XLIM " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl; //GDEB
  
  return Operate( image, newNoVoxelsX, newMinX, newMaxX, newNoVoxelsY, newMinY, newMaxY, newNoVoxelsZ, newMinZ, newMaxZ);
  
}

//-----------------------------------------------------------------------------
void DicomOperResize::Operate( DicomVImage* image, G4int newNoVoxelsX, G4double newMinX, G4double newMaxX, G4int newNoVoxelsY, G4double newMinY, G4double newMaxY, G4int newNoVoxelsZ, G4double newMinZ, G4double newMaxZ)
{ 
  //  G4cout << "   DicomOperResize::Operate( " << image->GetModalityStr() << G4endl; //GDEB
  G4double PRECISION = std::min(1e-5,2*image->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ)));
  PRECISION *= 1.2;
  //-G4double PRECISION = 2*image->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ));
  G4cout << " 2PRECISION " << PRECISION << " " << image->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ << G4endl; //GDEB
  G4int newNoVoxelsXY = newNoVoxelsX*newNoVoxelsY;
  //----- Extract the old image data
  size_t oldNVoxX = image->GetNoVoxelsX();
  size_t oldNVoxY = image->GetNoVoxelsY();
  size_t oldNVoxZ = image->GetNoVoxelsZ();
  size_t oldNVoxXY = oldNVoxX*oldNVoxY;
  //  size_t oldNVoxZ = image->GetNoVoxelsZ();
  G4double oldMinX = image->GetMinX();
  G4double oldMinY = image->GetMinY();
  G4double oldMinZ = image->GetMinZ();
  G4double oldMaxX = image->GetMaxX();
  G4double oldMaxY = image->GetMaxY();
  G4double oldMaxZ = image->GetMaxZ();
  G4double oldVoxDimX = image->GetVoxelDimX();
  G4double oldVoxDimY = image->GetVoxelDimY();
  G4double oldVoxDimZ = image->GetVoxelDimZ();

#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(warningVerb) ) {
    G4cout << "IMAGE SIZE " << G4endl
	   << " X: " << oldNVoxX << " " << oldMinX << " " << oldMaxX << G4endl
	   << " Y: " << oldNVoxY << " " << oldMinY << " " << oldMaxY << G4endl
	   << " Z: " << oldNVoxZ << " " << oldMinZ << " " << oldMaxZ << G4endl;
    G4cout << "NEW REQUESTED SIZE " << G4endl
	   << " X: " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl
	   << " Y: " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl
	   << " Z: " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl;
    G4cout << "DIFFERENCE IN LIMITS<?0 " << G4endl
	   << " X: " << -oldMinX + newMinX << " " << oldMaxX - newMaxX << G4endl
	   << " Y: " << -oldMinY + newMinY << " " << oldMaxY - newMaxY << G4endl
	   << " Z: " << -oldMinZ + newMinZ << " " << oldMaxZ - newMaxZ << G4endl;
    G4cout << "PRECISION " << PRECISION << G4endl;
  }
#endif

  //----- Check that new limits are inside old limits
  G4bool bInLimits = true;
  if( oldMinX - newMinX > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINX IMAGE LIMITS " << G4endl
	   << " X: " << oldMinX << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " X: " << newMinX << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " X:>0? " << oldMinX - newMinX << G4endl;
    newMinX = oldMinX;
  }
  if( oldMinY - newMinY > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINY IMAGE LIMITS " << G4endl
	   << " Y: " << oldMinY << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Y: " << newMinY << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " Y:>0? " << oldMinY - newMinY << G4endl;
    newMinY = oldMinY;
  }
  if( oldMinZ - newMinZ > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINZ IMAGE LIMITS " << oldMinZ - newMinZ << G4endl
	   << " Z: " << oldMinZ << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Z: " << newMinZ << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " Z:>0? " << oldMinZ - newMinZ << G4endl;
    newMinZ = oldMinZ;
  }
  if( oldMaxX - newMaxX < -PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXX IMAGE LIMITS " << G4endl
	   << " X: " << oldMaxX << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " X: " << newMaxX << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " X:<?0 " << oldMaxX - newMaxX << G4endl;
    newMaxX = oldMaxX;
  }
  if( oldMaxY - newMaxY < -PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXY IMAGE LIMITS " << G4endl
	   << " Y: " << oldMaxY << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Y: " << newMaxY << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " Y:<?0 " << oldMaxY - newMaxY << G4endl;
    newMaxY = oldMaxY;
  }
  if( oldMaxZ - newMaxZ < -PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXZ IMAGE LIMITS " << G4endl
      //	   << " PREC " << PRECISION //GDEB
	   << " Z: " << oldMaxZ << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Z: " << newMaxZ << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " Z:<?0 " << oldMaxZ - newMaxZ << G4endl;
    newMaxZ = oldMaxZ;
  }
  if( !bInLimits ) {
    G4Exception("DicomOperResize::Operate",
		"",
		FatalException,
		(" New limits to resize are outside image limits for image: " + image->GetName()).c_str());
  }

  //----- Check that new limits Min < Max
  if( newMinX >= newMaxX || 
      newMinY >= newMaxY || 
      newMinZ >= newMaxZ ) {
    G4cerr << "!! WRONG MIN/MAX NEW REQUESTED LIMITS  X: " << newMinX << " " << newMaxX << G4endl
	   << " Y: " << newMinY << " " << newMaxY << G4endl
	   << " Z: " << newMinZ << " " << newMaxZ << G4endl;
    G4Exception("DicomOperResize::Operate",
		"",
		FatalException,
		(" New limits Minimum is greater or equal than Maximum for image "+image->GetName()).c_str());
  }

  std::vector<G4double>* newData = new std::vector<G4double>(newNoVoxelsX*newNoVoxelsY*newNoVoxelsZ);
  G4double newVoxDimX = (newMaxX-newMinX)/newNoVoxelsX;
  G4double newVoxDimY = (newMaxY-newMinY)/newNoVoxelsY;
  G4double newVoxDimZ = (newMaxZ-newMinZ)/newNoVoxelsZ;

  //----- Loop to voxels of new image
  for( G4int ixn = 0; ixn < newNoVoxelsX; ixn++ ) {
    G4double minXn = newMinX+ixn*newVoxDimX; // left wall of new voxel
    G4double maxXn = newMinX+(ixn+1)*newVoxDimX; // right wall of new voxel
    size_t im1stVoxXo = GmGenUtils::GetBelowInt((minXn-oldMinX)/oldVoxDimX,PRECISION); // image ID of old voxel corresponding to minXn 
    G4double imVoxMinX = 0;
    G4double imVoxMaxX = image->GetNoVoxelsX();
    //    G4cout << ixn << " im1stVoxXo " << im1stVoxXo << " " << int((minXn-oldMinX)/oldVoxDimX) << " " << (minXn-oldMinX)/oldVoxDimX << " ("<<minXn<<"-"<<oldMinX<<")/"<<oldVoxDimX<< G4endl; //GDEB
 
  /*t
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) ) {
      if( ixn < 10 ) G4cout << ixn << "DicomOperResize::Operate X minXn " << oldMinX+im1stVoxXo*oldVoxDimX << G4endl;
      if( ixn < 10000 ) G4cout << ixn << "/" << newNoVoxelsX << ": DicomOperResize::Operate X minXn " << minXn << " maxXn " << maxXn << " im1st " << im1stVoxXo << " imVoxMinMax " << imVoxMinX << " " << imVoxMaxX << " oldVoxDimX " << oldVoxDimX << G4endl;
    }
#endif
    */
    for( G4int iyn = 0; iyn < newNoVoxelsY; iyn++ ) {
      G4double minYn = newMinY+iyn*newVoxDimY; // left wall of new voxel
      G4double maxYn = newMinY+(iyn+1)*newVoxDimY; // right wall of new voxel
      size_t im1stVoxYo = GmGenUtils::GetBelowInt((minYn-oldMinY)/oldVoxDimY,PRECISION); // image ID of voxel corresponding to minYn      
      G4double imVoxMinY = 0;
      G4double imVoxMaxY = image->GetNoVoxelsY();
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(testVerb) ) {
	G4cout << iyn << "/" << newNoVoxelsY << ": DicomOperResize::Operate Y minYn " << minYn << " " << maxYn << " im1st " << im1stVoxYo << " imVoxMinMax " << imVoxMinY << " " << imVoxMaxY << G4endl;
      }
#endif
      for( G4int izn = 0; izn < newNoVoxelsZ; izn++ ) {
	G4double minZn = newMinZ+izn*newVoxDimZ; // left wall of new voxel
	G4double maxZn = newMinZ+(izn+1)*newVoxDimZ; // left wall of new voxel
	size_t im1stVoxZo = GmGenUtils::GetBelowInt((minZn-oldMinZ)/oldVoxDimZ,PRECISION); // image ID of voxel corresponding to minZn
	//	G4cout << " im1stVoxZo " << im1stVoxZo << " " << (minZn-oldMinZ)/oldVoxDimZ << " -64 " << (minZn-oldMinZ)/oldVoxDimZ -64 << " minZn " << minZn << " oldMinZ " <<oldMinZ << " newMinZ " << newMinZ <<  G4endl; //GDEB
	//-	G4cout << " im1stVoxZo " << im1stVoxZo << " " << minZn << " " << oldMinZ << " " << oldVoxDimZ << " : " << (minZn-oldMinZ) << " " << (minZn-oldMinZ)/oldVoxDimZ << G4endl;
	G4double imVoxMinZ = 0;
	G4double imVoxMaxZ = image->GetNoVoxelsZ();
	/*
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	  if( izn < 10 ) G4cout << izn << "/" << newNoVoxelsZ << ": DicomOperResize::Operate Z minZn " << minZn << " " << maxZn << " im1st " << im1stVoxZo << " imVoxMinMax " << imVoxMinZ << " " << imVoxMaxZ << G4endl;
	}
#endif
	*/
	//---- LOOP TO OLD IMAGE VOXELS COVERED BY NEW VOXEL
	size_t newCopyNo = ixn + newNoVoxelsX*iyn + newNoVoxelsXY*izn;
	G4double fracTOT = 0.;
	G4int nOldVox = 0; 
	//	G4cout << "@@ LOOP old VOXELS FOR " << ixn << " " << iyn << " " << izn << " X: " << minXn << " " << maxXn << " Y: " << minYn << " " << maxYn << " Z: " << minZn << " " << maxZn << G4endl; //GDEB
	for( size_t ix = im1stVoxXo;; ix++ ) {
	  if ( ix == im1stVoxXo ) {
	    imVoxMinX = std::max(minXn,oldMinX+im1stVoxXo*oldVoxDimX); // image left wall of old voxel
	    //	    G4cout << " imVoxMinX=" << imVoxMinX << " std::max("<<minXn<<","<<oldMinX+im1stVoxXo*oldVoxDimX<<")" <<G4endl; //GDEB
	    //	    imVoxMaxX = std::min(maxXn,imVoxMinX+newVoxDimX); // image right wall of old voxel
	    imVoxMaxX = std::min(maxXn,oldMinX+(im1stVoxXo+1)*oldVoxDimX); // image right wall of old voxel
	  }
	  G4double fracX = (imVoxMaxX-imVoxMinX)/newVoxDimX;
	  //	  G4cout << ix << " fracX " << fracX << " = (" << imVoxMaxX << "-" << imVoxMinX << ")/" << newVoxDimX << G4endl; //GDEB
	  for( size_t iy = im1stVoxYo;; iy++ ) {
	    if( iy == im1stVoxYo ) {
	      imVoxMinY = std::max(minYn,oldMinY+im1stVoxYo*oldVoxDimY); // image left wall of old voxel
	      imVoxMaxY = std::min(maxYn,oldMinY+(im1stVoxYo+1)*oldVoxDimY); // image right wall of old voxel
	    }
	    G4double fracXY = fracX*((imVoxMaxY-imVoxMinY)/newVoxDimY);
	    //	    G4cout << iy << " fracY " << fracXY/fracX << " = (" << imVoxMaxY << "-" << imVoxMinY << ")/" << newVoxDimY << " fracXY=" << fracXY << G4endl; //GDEB
	    for( size_t iz = im1stVoxZo;; iz++ ) {
	      nOldVox++;
	      if( iz == im1stVoxZo ) {
		imVoxMinZ = std::max(minZn,oldMinZ+im1stVoxZo*oldVoxDimZ); // image left wall of old voxel
		imVoxMaxZ = std::min(maxZn,oldMinZ+(im1stVoxZo+1)*oldVoxDimZ); // image right wall of old voxel
	      }
	      G4double fracXYZ = fracXY*(imVoxMaxZ-imVoxMinZ)/newVoxDimZ;
	      //	      G4cout << iz << " fracZ " << fracXYZ/fracXY << " = (" << imVoxMaxZ << "-" << imVoxMinZ << ")/" << newVoxDimZ << " fracXYZ=" << fracXYZ << G4endl; //GDEB
	      size_t oldCopyNo = ix + oldNVoxX*iy + oldNVoxXY*iz;
	      //	      G4cout << "@@ newCopyNo " << newCopyNo << " " << oldCopyNo << " : " << ixn << " " << iyn << " " << izn << " < " << image->GetData(oldCopyNo)*fracXYZ << " " << image->GetData(oldCopyNo)<< " * " << fracXYZ << G4endl; //GDEB
	      newData->at(newCopyNo) += image->GetData(oldCopyNo)*fracXYZ;
	      fracTOT += fracXYZ;
	      //	      G4cout << "@@ fracTOT " << fracTOT << " " << newCopyNo << ": " << newData->at(newCopyNo) << " " << oldCopyNo << " : " << ix << " " << iy << " " << iz << " oldData " << image->GetData(oldCopyNo) << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
	      if( DicomVerb(testVerb) ) {
		if( ixn < 10 && iyn < 10 && izn < 10) G4cout << ix << " " << iy << " " << iz << "DicomOperResize::Operate fractions " << fracX << " " << fracXY/fracX << " " << fracXYZ/fracXY << G4endl;
	      }
#endif
	      imVoxMinZ = imVoxMaxZ;
	      imVoxMaxZ = std::min( maxZn, imVoxMaxZ+oldVoxDimZ);
	      //	      G4cout << " Z BREAK? " << (imVoxMinZ - maxZn) << " =( " << imVoxMinZ << " - " << imVoxMaxZ << " >? " << " maxZn " << maxZn << " " << -PRECISION << G4endl; //GDEB
	      if( (imVoxMinZ - maxZn) >= -PRECISION ) break;
	    }
	    imVoxMinY = imVoxMaxY;
	    imVoxMaxY = std::min( maxYn, imVoxMaxY+oldVoxDimY);
	    if( (imVoxMinY - maxYn) >= -PRECISION ) break;
	  }
	  imVoxMinX = imVoxMaxX;
	  imVoxMaxX = std::min( maxXn, imVoxMaxX+oldVoxDimX);
	  if( (imVoxMinX - maxXn) >= -PRECISION ) break;
	}
	/*	if( fabs(fracTOT-1.) > 2.*PRECISION ) {
	  //	  G4cout << image << " OperResize PRECISION " << PRECISION << G4endl; //GDEB
	  G4Exception("DicomOperResize::Operate",
		      "",
		      FatalException,
		      ("voxel fraction sum -1. should be 0., while it is "+GmGenUtils::ftoa(fracTOT-1.)).c_str());
		      } */
	if( fracTOT < 0.99 ) {
	  G4cerr << "@@FINAL newData: " << newData->at(newCopyNo) << " " << newCopyNo << "    fracTot " << fracTOT << "nOldVox " << nOldVox << G4endl;
	  G4Exception("DicomOperResize::Operate",
		      "",
		      JustWarning,
		      ("Sum of old voxels volumes it is not 1., but "+GmGenUtils::ftoa(fracTOT)+", correct it").c_str());
	}
	newData->at(newCopyNo) /= fracTOT; // !!! FRACTOT SHOULD ALWAYS BE 1, sometimes it is e.g. 0.9913 (precision problem of multipying ~20 numbers?)
	//	G4cout << "@@FINAL newData: " << newData->at(newCopyNo) << " " << newCopyNo << "    fracTot " << fracTOT << " nOldVox " << nOldVox << G4endl; //GDEB
	
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
  //  G4cout << "DicomOperResize new image " << *image << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
//void DicomOperResize::BuildMaterials( DicomVImage* image )
void DicomOperResize::OperateStr( DicomVImageStr* imageStr )
{
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << imageStr << " DicomOperResize::OperateStr"  << *imageStr << G4endl;
#endif
  //----- Extract the new imageStr limits (if not changed, set it equal to imageStr limits)
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4double newMinX = paramMgr->GetNumericValue("minX",imageStr->GetMinX()); // minimum extension of voxels (position of wall)
  G4double newMaxX = paramMgr->GetNumericValue("maxX",imageStr->GetMaxX()); // maximum extension of voxels (position of wall)
  size_t newNoVoxelsX = paramMgr->GetNumericValue("nVoxX",imageStr->GetNoVoxelsX()); // number of voxels in each dimensions
  G4double newMinY = paramMgr->GetNumericValue("minY",imageStr->GetMinY()); 
  G4double newMaxY = paramMgr->GetNumericValue("maxY",imageStr->GetMaxY()); 
  size_t newNoVoxelsY = paramMgr->GetNumericValue("nVoxY",imageStr->GetNoVoxelsY()); 
  G4double newMinZ = paramMgr->GetNumericValue("minZ",imageStr->GetMinZ());
  G4double newMaxZ = paramMgr->GetNumericValue("maxZ",imageStr->GetMaxZ());
  size_t newNoVoxelsZ = paramMgr->GetNumericValue("nVoxZ",imageStr->GetNoVoxelsZ()); 
  G4bool bInterpolate = G4bool(paramMgr->GetNumericValue("bInterpolate",1));
  
  G4double PRECISION = std::min(1e-5,2*imageStr->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ)));
  //  G4cout << " 1PRECISION " << PRECISION << " " << imageStr->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ << G4endl; //GDEB

  G4bool bChangeNX = newNoVoxelsX == imageStr->GetNoVoxelsX();
  G4bool bChangeNY = newNoVoxelsY == imageStr->GetNoVoxelsY();
  G4bool bChangeNZ = newNoVoxelsZ == imageStr->GetNoVoxelsZ();
  //----- DO NOT INTERPOLATE: MOVE MIN/MAX TO VOXEL EDGES
  G4cout << " BINTERPOLATE= " << bInterpolate << G4endl; //GDEB
  if( !bInterpolate ) {
    if( bChangeNX ) {
      G4Exception("DicomOperResize::Operate",
		  "err2",
		  FatalException,
		  "No interpolation: nVoxX will be calculated as (newMaxX - newMinX)/stepX, it cannot be set by the user");
    }
    if( bChangeNY ) {
      G4Exception("DicomOperResize::Operate",
		  "err2",
		  FatalException,
		  "No interpolation: nVoxY will be calculated as (newMaxY - newMinY)/stepY, it cannot be set by the user");
    }
    if( bChangeNZ ) {
      G4Exception("DicomOperResize::Operate",
		  "err2",
		  FatalException,
		  "No interpolation: nVoxZ will be calculated as (newMaxZ - newMinZ)/stepZ, it cannot be set by the user");
    }
    
    //    if( DicomVerb(warningVerb) ) G4cout << "DicomOperResize Min/MaxX/Z may be changed to include the totality of the voxel that is cut by the new value, unless you change the number of voxels (use -resizeFitOld 0  if you do not want this change) " << G4endl; 
    //-- if nVoxX is not changed, use minX, maxX to cut the imageStr: if it cuts a voxel, include it also
    //  G4cout << " NEWMINX " << newMinX << " != " << imageStr->GetMinX() << " && " << paramMgr->IsParameterInScript("nVoxX") << G4endl; //GDEB
    if( newMinX != imageStr->GetMinX() && bChangeNX ) {
      int ibinX = GmGenUtils::GetBelowInt((newMinX-imageStr->GetMinX())/imageStr->GetVoxelDimX(),PRECISION);
      if( ibinX < 0 ) ibinX = 0;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MINX " << newMinX << " ->IBINX " << ibinX << " = (" << newMinX << " - " << imageStr->GetMinX() << " )/ " << imageStr->GetVoxelDimX() << " " << (newMinX-imageStr->GetMinX())/imageStr->GetVoxelDimX() << " " << GmGenUtils::GetBelowInt((newMinX-imageStr->GetMinX())/imageStr->GetVoxelDimX(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MINX " << imageStr->GetMinX()+ibinX*imageStr->GetVoxelDimX() << G4endl;
      }
#endif
      newMinX = imageStr->GetMinX()+ibinX*imageStr->GetVoxelDimX();
      if( ibinX >= 0 && bChangeNX ) newNoVoxelsX -= ibinX;
    }
    //  G4cout << " XLIM " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl; //GDEB
    
    if( newMaxX != imageStr->GetMaxX() && bChangeNX ) {
      int ibinX = GmGenUtils::GetAboveInt((newMaxX-imageStr->GetMinX())/imageStr->GetVoxelDimX(),PRECISION);
      if( ibinX >= G4int(imageStr->GetNoVoxelsX()) ) ibinX = imageStr->GetNoVoxelsX();
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MAXX " << newMaxX << " ->IBINX " << ibinX << " = (" << newMaxX << " - " << imageStr->GetMaxX() << " )/ " << imageStr->GetVoxelDimX() << " " << (newMaxX-imageStr->GetMaxX())/imageStr->GetVoxelDimX() << " " << GmGenUtils::GetBelowInt((newMaxX-imageStr->GetMaxX())/imageStr->GetVoxelDimX(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MAXX " << imageStr->GetMinX()+ibinX*imageStr->GetVoxelDimX() << G4endl;
      }
#endif
      newMaxX = imageStr->GetMinX()+ibinX*imageStr->GetVoxelDimX();
      if( ibinX < G4int(imageStr->GetNoVoxelsX()) && bChangeNX ) newNoVoxelsX -= imageStr->GetNoVoxelsX() - ibinX;
    }
    //  G4cout << " XLIM " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl; //GDEB
    
    if( newMinY != imageStr->GetMinY() && bChangeNY ) {
      int ibinY = GmGenUtils::GetBelowInt((newMinY-imageStr->GetMinY())/imageStr->GetVoxelDimY(),PRECISION);
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize newMINY " << newMinY << " ->IBINY " << ibinY << " = (" << newMinY << " - " << imageStr->GetMinY() << " )/ " << imageStr->GetVoxelDimY() << " " << (newMinY-imageStr->GetMinY())/imageStr->GetVoxelDimY() << " " << GmGenUtils::GetBelowInt((newMinY-imageStr->GetMinY())/imageStr->GetVoxelDimY(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MINY " << imageStr->GetMinY()+ibinY*imageStr->GetVoxelDimY() << G4endl;
      }
#endif
      newMinY = imageStr->GetMinY()+ibinY*imageStr->GetVoxelDimY();
      if( ibinY >= 0 && bChangeNY ) newNoVoxelsY -= ibinY;
    }
    //    if( DicomVerb(debugVerb) ) G4cout << " YLIM " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl; //GDEB
    
   //    if( newMaxY != imageStr->GetMaxY() && !paramMgr->IsParameterInScript("nVoxY") ) {
    if( newMaxY != imageStr->GetMaxY() && bChangeNY ) {
      int ibinY = GmGenUtils::GetAboveInt((newMaxY-imageStr->GetMinY())/imageStr->GetVoxelDimY(),PRECISION);
      //    G4cout << " IBINY " << ibinY << " " << newMaxY << " - " << imageStr->GetMinY() << " )/ " << imageStr->GetVoxelDimY() << " " << (newMaxY-imageStr->GetMinY())/imageStr->GetVoxelDimY() << " " << GmGenUtils::GetAboveInt((newMaxY-imageStr->GetMinY())/imageStr->GetVoxelDimY(),PRECISION);) << G4endl; //GDEB
      //    G4cout << " NEW MAY Y CHANGED " << newMaxY << " -> " << imageStr->GetMinY()+ibinY*imageStr->GetVoxelDimY()<< G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MAXY " << newMaxY << " ->IBINY " << ibinY << " = (" << newMaxY << " - " << imageStr->GetMaxY() << " )/ " << imageStr->GetVoxelDimY() << " " << (newMaxY-imageStr->GetMaxY())/imageStr->GetVoxelDimY() << " " << GmGenUtils::GetBelowInt((newMaxY-imageStr->GetMaxY())/imageStr->GetVoxelDimY(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MAXY " << imageStr->GetMinY()+ibinY*imageStr->GetVoxelDimY() << G4endl;
      }
#endif
      newMaxY = imageStr->GetMinY()+ibinY*imageStr->GetVoxelDimY();
      if( ibinY < G4int(imageStr->GetNoVoxelsY()) && bChangeNY ) newNoVoxelsY -= imageStr->GetNoVoxelsY() - ibinY;
    }
    //    if( DicomVerb(infoVerb) ) G4cout << " YLIM " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl; //GDEB
        
    if( newMinZ != imageStr->GetMinZ() && bChangeNZ ) {
      int ibinZ = GmGenUtils::GetBelowInt((newMinZ-imageStr->GetMinZ())/imageStr->GetVoxelDimZ(),PRECISION);
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MINZ " << newMinZ << " ->IBINZ " << ibinZ << " = (" << newMinZ << " - " << imageStr->GetMinZ() << " )/ " << imageStr->GetVoxelDimZ() << " " << (newMinZ-imageStr->GetMinZ())/imageStr->GetVoxelDimZ() << " " << GmGenUtils::GetBelowInt((newMinZ-imageStr->GetMinZ())/imageStr->GetVoxelDimZ(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MINZ " << imageStr->GetMinZ()+ibinZ*imageStr->GetVoxelDimZ() << G4endl;
      }
#endif
      newMinZ = imageStr->GetMinZ()+ibinZ*imageStr->GetVoxelDimZ();
      if( ibinZ >= 0 && bChangeNZ ) newNoVoxelsZ -= ibinZ;
    }
    //    G4cout << " ZLIM " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl; //GDEB
    
    if( newMaxZ != imageStr->GetMaxZ() && bChangeNZ ) {
      int ibinZ = GmGenUtils::GetAboveInt((newMaxZ-imageStr->GetMinZ())/imageStr->GetVoxelDimZ(),PRECISION);
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MAXZ " << newMaxZ << " ->IBINZ " << ibinZ << " = (" << newMaxZ << " - " << imageStr->GetMaxZ() << " )/ " << imageStr->GetVoxelDimZ() << " " << (newMaxZ-imageStr->GetMaxZ())/imageStr->GetVoxelDimZ() << " " << GmGenUtils::GetBelowInt((newMaxZ-imageStr->GetMaxZ())/imageStr->GetVoxelDimZ(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MAXZ " << imageStr->GetMinZ()+ibinZ*imageStr->GetVoxelDimZ() << G4endl;
      }
#endif
      newMaxZ = imageStr->GetMinZ()+ibinZ*imageStr->GetVoxelDimZ();
      if( ibinZ < G4int(imageStr->GetNoVoxelsZ()) && bChangeNZ ) newNoVoxelsZ -= imageStr->GetNoVoxelsZ() - ibinZ;
    }
    //    G4cout << " ZLIM " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl; //GDEB
  }
  
    //  G4cout << " NEWMINX " << newMinX << " != " << imageStr->GetMinX() << " && " << paramMgr->IsParameterInScript("nVoxX") << G4endl; //GDEB
      if( newMinX != imageStr->GetMinX() && bChangeNX ) {
	int ibinX = GmGenUtils::GetBelowInt((newMinX-imageStr->GetMinX())/imageStr->GetVoxelDimX(),PRECISION);
	if( ibinX < 0 ) ibinX = 0;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) {
	G4cout << " DicomOperResize MINX " << newMinX << " ->IBINX " << ibinX << " = (" << newMinX << " - " << imageStr->GetMinX() << " )/ " << imageStr->GetVoxelDimX() << " " << (newMinX-imageStr->GetMinX())/imageStr->GetVoxelDimX() << " " << GmGenUtils::GetBelowInt((newMinX-imageStr->GetMinX())/imageStr->GetVoxelDimX(),PRECISION) << G4endl;
	G4cout << " DicomOperResize NEW MINX " << imageStr->GetMinX()+ibinX*imageStr->GetVoxelDimX() << G4endl;
      }
#endif      
      newMinX = imageStr->GetMinX()+ibinX*imageStr->GetVoxelDimX();
      if( ibinX >= 0 && bChangeNX ) newNoVoxelsX -= ibinX;
    }
    //  G4cout << " XLIM " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl; //GDEB
				
  return OperateStr( imageStr, newNoVoxelsX, newMinX, newMaxX, newNoVoxelsY, newMinY, newMaxY, newNoVoxelsZ, newMinZ, newMaxZ);
  
}

//-----------------------------------------------------------------------------
void DicomOperResize::OperateStr( DicomVImageStr* imageStr, G4int newNoVoxelsX, G4double newMinX, G4double newMaxX, G4int newNoVoxelsY, G4double newMinY, G4double newMaxY, G4int newNoVoxelsZ, G4double newMinZ, G4double newMaxZ)
{ 
  //  G4cout << imageStr <<" 2DicomOperResize::OperateStr( " << imageStr->GetModalityStr() << G4endl; //GDEB
  G4double PRECISION = std::min(1e-5,2*imageStr->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ)));
  //- G4double PRECISION = 2*imageStr->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ));
  G4cout << " 1PRECISION " << PRECISION << " " << imageStr->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ << G4endl; //GDEB
  G4int newNoVoxelsXY = newNoVoxelsX*newNoVoxelsY;
  //----- Extract the old imageStr data
  size_t oldNVoxX = imageStr->GetNoVoxelsX();
  size_t oldNVoxY = imageStr->GetNoVoxelsY();
  size_t oldNVoxZ = imageStr->GetNoVoxelsZ();
  //  size_t oldNVoxXY = oldNVoxX*oldNVoxY;
  //  size_t oldNVoxZ = imageStr->GetNoVoxelsZ();
  G4double oldMinX = imageStr->GetMinX();
  G4double oldMinY = imageStr->GetMinY();
  G4double oldMinZ = imageStr->GetMinZ();
  G4double oldMaxX = imageStr->GetMaxX();
  G4double oldMaxY = imageStr->GetMaxY();
  G4double oldMaxZ = imageStr->GetMaxZ();
  //  G4double oldVoxDimX = imageStr->GetVoxelDimX();
  // G4double oldVoxDimY = imageStr->GetVoxelDimY();
  // G4double oldVoxDimZ = imageStr->GetVoxelDimZ();

#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(warningVerb) ) {
    G4cout << "IMAGESTR SIZE " << G4endl
	   << " X: " << oldNVoxX << " " << oldMinX << " " << oldMaxX << G4endl
	   << " Y: " << oldNVoxY << " " << oldMinY << " " << oldMaxY << G4endl
	   << " Z: " << oldNVoxZ << " " << oldMinZ << " " << oldMaxZ << G4endl;
    G4cout << "NEW REQUESTED SIZE " << G4endl
	   << " X: " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl
	   << " Y: " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl
	   << " Z: " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl;
    G4cout << "DIFFERENCE IN LIMITS (MUST BE <= 0) " << G4endl
	   << " X: " << -oldMinX + newMinX << " " << oldMaxX - newMaxX << G4endl
	   << " Y: " << -oldMinY + newMinY << " " << oldMaxY - newMaxY << G4endl
	   << " Z: " << -oldMinZ + newMinZ << " " << oldMaxZ - newMaxZ << G4endl;
    G4cout << "PRECISION " << PRECISION << G4endl;
  }
#endif

  //  G4double newVoxDimX = (newMaxX-newMinX)/newNoVoxelsX;
  //  G4double newVoxDimY = (newMaxY-newMinY)/newNoVoxelsY;
  //  G4double newVoxDimZ = (newMaxZ-newMinZ)/newNoVoxelsZ;

  //----- Check that new limits are inside old limits
  G4bool bInLimits = true;
  if( oldMinX - newMinX > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINX IMAGESTR LIMITS " << G4endl
	   << " X: " << oldMinX << " " << oldMaxX << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " X: " << newMinX << " " << newMaxX << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (MUST BE <= 0) " << G4endl
	   << " X: " << oldMinX - newMinX << " " << oldMaxX - newMaxX << G4endl;
    newMinX = oldMinX;
  }
  if( oldMinY - newMinY > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINY IMAGESTR LIMITS " << G4endl
	   << " Y: " << oldMinY << " " << oldMaxY << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Y: " << newMinY << " " << newMaxY << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (MUST BE <= 0) " << G4endl
	   << " Y: " << oldMinY - newMinY << " " << oldMaxY - newMaxY << G4endl;
    newMinY = oldMinY;
  }
  if( oldMinZ - newMinZ > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINZ IMAGESTR LIMITS " << oldMinZ - newMinZ << G4endl
	   << " Z: " << oldMinZ << " " << oldMaxZ << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Z: " << newMinZ << " " << newMaxZ << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (MUST BE <= 0) " << G4endl
	   << " Z: " << oldMinZ - newMinZ << " " << oldMaxZ - newMaxZ << G4endl;
    newMinZ = oldMinZ;
  }
  if( newMaxX - oldMaxX > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXX IMAGESTR LIMITS " << G4endl
	   << " X: " << oldMinX << " " << oldMaxX << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " X: " << newMinX << " " << newMaxX << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (MUST BE <= 0) " << G4endl
	   << " X: " << oldMinX - newMinX << " " << oldMaxX - newMaxX << G4endl;
    newMaxX = oldMaxX;
  }
  if( newMaxY - oldMaxY > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXY IMAGESTR LIMITS " << G4endl
	   << " Y: " << oldMinY << " " << oldMaxY << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Y: " << newMinY << " " << newMaxY << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " Y: " << oldMinY - newMinY << " " << oldMaxY - newMaxY << G4endl;
    newMaxY = oldMaxY;
  }
  if( newMaxZ - oldMaxZ > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXZ IMAGESTR LIMITS " << oldMaxZ - newMaxZ << G4endl
      //	   << " PREC " << PRECISION //GDEB
	   << " Z: " << oldMinZ << " " << oldMaxZ << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Z: " << newMinZ << " " << newMaxZ << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS " << G4endl
	   << " Z: " << oldMinZ - newMinZ << " " << oldMaxZ - newMaxZ << G4endl;
    newMaxZ = oldMaxZ;
  }
  if( !bInLimits ) {
    G4Exception("DicomOperResize::Operate",
		"",
		FatalException,
		(" New limits to resize are outside imageStr limits for imageStr: " + imageStr->GetName()).c_str());
  }

  //----- Check that new limits Min < Max
  if( newMinX >= newMaxX || 
      newMinY >= newMaxY || 
      newMinZ >= newMaxZ ) {
    G4cerr << "!! WRONG MIN/MAX NEW REQUESTED LIMITS  X: " << newMinX << " " << newMaxX << G4endl
	   << " Y: " << newMinY << " " << newMaxY << G4endl
	   << " Z: " << newMinZ << " " << newMaxZ << G4endl;
    G4Exception("DicomOperResize::Operate",
		"",
		FatalException,
		(" New limits Minimum is greater or equal than Maximum for imageStr "+imageStr->GetName()).c_str());
  }

  std::vector<G4double>* newData = new std::vector<G4double>(newNoVoxelsXY*newNoVoxelsZ);

  //----- Change imageStr
  imageStr->SetMinX(newMinX);
  imageStr->SetMaxX(newMaxX);
  imageStr->SetNoVoxelsX(newNoVoxelsX);
  imageStr->SetMinY(newMinY);
  imageStr->SetMaxY(newMaxY);
  imageStr->SetNoVoxelsY(newNoVoxelsY);
  imageStr->SetMinZ(newMinZ);
  imageStr->SetMaxZ(newMaxZ);
  imageStr->SetNoVoxelsZ(newNoVoxelsZ);
  imageStr->SetData(newData);
  //  G4cout << " N Readers(DRM_RTStruct 0 " << G4endl; //GDEB                             
 
  //----- G4StructID SET NEW VOXELS TO 0 OR REDO WITH RTSTRUCT                                                  
  DicomReaderMgr* theReaderMgr = DicomReaderMgr::GetInstance();
  std::vector<DicomVReader*> RTStructReaders = theReaderMgr->GetReaders(DRM_RTStruct, false);
  //  G4cout << " N Readers(DRM_RTStruct " << RTStructReaders.size() << G4endl; //GDEB                             
  std::vector<DicomVReader*> CTReaders = theReaderMgr->GetReaders(DRM_G4dcmCT, false);
  // G4cout << " N Readers(DRM_G4dcmCT " << CTReaders.size() << G4endl; //GDEB                             
  if( RTStructReaders.size() != 0 && CTReaders.size() != 0 ) {
    //-- REDO G4StructID WITH RTSTRUCT       
    //-    imageStr = new DicomVImageStr( imageStr, "RTStruct", DIM_RTStruct );
    DicomReaderRTStruct* structRead = (DicomReaderRTStruct*)(RTStructReaders[0]);
    DicomPolygonSet* polygonSet = structRead->GetPolygonSet();
    std::vector<DicomVLineList*> lineLists = polygonSet->GetLineLists();
    imageStr->SetDataStr( new std::vector<G4String>(imageStr->GetNoVoxels()));
    G4bool bVoxelsInUseClosest = DicomParameterMgr::GetInstance()->GetNumericValue("closestPolygon",1);
    //    G4cout << " FindVoxelsInXY " << bVoxelsInUseClosest << G4endl; //GDEB
    if( bVoxelsInUseClosest ) {
      DicomPolygonSet* polygonSetNew = new DicomPolygonSet(polygonSet, imageStr, polygonSet->GetOrientation() );
      structRead->FindVoxelsInXY( imageStr, polygonSetNew );
      delete polygonSetNew;
    } else {
      structRead->FindVoxelsInXY( imageStr, polygonSet );
    }
  } else {
    G4Exception("DicomOperResize::Operate(imageStr) ",
		"",
		JustWarning,
		("Operating on an image Structure with no -fRTStruct or -fVOIStruct "+imageStr->GetName()).c_str());
  }
  //  G4cout << "DicomOperResize new image " << *image << G4endl; //GDEB
}
