#include "DicomOperResize.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomOperResize::DicomOperResize( G4int iOrder, G4String name)
  : DicomVOperator( iOrder, name )
{  
  bAutomaticOperate = true;
}

//-----------------------------------------------------------------------------
//void DicomOperResize::BuildMaterials( DicomVImage* image )
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
  
  G4double PRECISION = std::min(1e-4,2*image->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ)));
  G4cout << " 1PRECISION " << PRECISION << " " << image->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ <<  " bInterpolate " << bInterpolate << G4endl; //GDEB

  G4bool bChangeNX = (newNoVoxelsX != image->GetNoVoxelsX());
  G4bool bChangeNY = (newNoVoxelsY != image->GetNoVoxelsY());
  G4bool bChangeNZ = (newNoVoxelsZ != image->GetNoVoxelsZ());
  //----- DO NOT INTERPOLATE: MOVE MIN/MAX TO VOXEL EDGES
  /*  if( !bInterpolate ) {
    if( bChangeNX ) {
      G4Exception("DicomOperResize::Operate",
		  "",
		  FatalException,
		  "No interpolation: nVoxX will be calculated as (newMaxX - newMinX)/stepX, it cannot be set by the user");
    }
    if( bChangeNY ) {
      G4Exception("DicomOperResize::Operate",
		  "",
		  FatalException,
		  "No interpolation: nVoxY will be calculated as (newMaxY - newMinY)/stepY, it cannot be set by the user");
    }
    if( bChangeNZ ) {
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
    //  G4cout << " XLIM " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl; //GDEB
    
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
    */				
  return Operate( image, newNoVoxelsX, newMinX, newMaxX, newNoVoxelsY, newMinY, newMaxY, newNoVoxelsZ, newMinZ, newMaxZ);
  
}

//-----------------------------------------------------------------------------
void DicomOperResize::Operate( DicomVImage* image, G4int newNoVoxelsX, G4double newMinX, G4double newMaxX, G4int newNoVoxelsY, G4double newMinY, G4double newMaxY, G4int newNoVoxelsZ, G4double newMinZ, G4double newMaxZ)
{ 
  G4double PRECISION = 2*image->GetPrecision()*std::min(newNoVoxelsX,std::min(newNoVoxelsY,newNoVoxelsZ));
  //  G4cout << " 1PRECISION " << PRECISION << " " << image->GetPrecision() << " " << newNoVoxelsX << " " << newNoVoxelsY << " " << newNoVoxelsZ << G4endl; //GDEB
  G4int newNoVoxelsXY = newNoVoxelsX*newNoVoxelsY;
 
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
  if( DicomVerb(warningVerb) ) {
    G4cout << "IMAGE SIZE " << G4endl
	   << " X: " << imNVoxX << " " << imMinX << " " << imMaxX << G4endl
	   << " Y: " << imNVoxY << " " << imMinY << " " << imMaxY << G4endl
	   << " Z: " << imNVoxZ << " " << imMinZ << " " << imMaxZ << G4endl;
    G4cout << "NEW REQUESTED SIZE " << G4endl
	   << " X: " << newNoVoxelsX << " " << newMinX << " " << newMaxX << G4endl
	   << " Y: " << newNoVoxelsY << " " << newMinY << " " << newMaxY << G4endl
	   << " Z: " << newNoVoxelsZ << " " << newMinZ << " " << newMaxZ << G4endl;
    G4cout << "DIFFERENCE IN LIMITS (must be >=0)" << G4endl
	   << " X: " << -imMinX + newMinX << " " << imMaxX - newMaxX << G4endl
	   << " Y: " << -imMinY + newMinY << " " << imMaxY - newMaxY << G4endl
	   << " Z: " << -imMinZ + newMinZ << " " << imMaxZ - newMaxZ << G4endl;
    G4cout << "PRECISION " << PRECISION << G4endl;
  }
#endif

  G4double theVoxelDimX = (newMaxX-newMinX)/newNoVoxelsX;
  G4double theVoxelDimY = (newMaxY-newMinY)/newNoVoxelsY;
  G4double theVoxelDimZ = (newMaxZ-newMinZ)/newNoVoxelsZ;

  //----- Check that new limits are inside old limits
  G4bool bInLimits = true;
  if( imMinX - newMinX > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINX IMAGE LIMITS " << G4endl
	   << " X: " << imMinX << " " << imMaxX << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " X: " << newMinX << " " << newMaxX << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (must be >=0) " << G4endl
	   << " X: " << imMinX - newMinX << " " << imMaxX - newMaxX << G4endl;
    newMinX = imMinX;
  }
  if( imMinY - newMinY > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINY IMAGE LIMITS " << G4endl
	   << " Y: " << imMinY << " " << imMaxY << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Y: " << newMinY << " " << newMaxY << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (must be >=0) " << G4endl
	   << " Y: " << imMinY - newMinY << " " << imMaxY - newMaxY << G4endl;
    newMinY = imMinY;
  }
  if( imMinZ - newMinZ > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MINZ IMAGE LIMITS " << imMinZ - newMinZ << G4endl
	   << " Z: " << imMinZ << " " << imMaxZ << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Z: " << newMinZ << " " << newMaxZ << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (must be >=0) " << G4endl
	   << " Z: " << imMinZ - newMinZ << " " << imMaxZ - newMaxZ << G4endl;
    newMinZ = imMinZ;
  }
  if( newMaxX - imMaxX > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXX IMAGE LIMITS " << G4endl
	   << " X: " << imMinX << " " << imMaxX << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " X: " << newMinX << " " << newMaxX << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (must be >=0) " << G4endl
	   << " X: " << imMinX - newMinX << " " << imMaxX - newMaxX << G4endl;
    newMaxX = imMaxX;
  }
  if( newMaxY - imMaxY > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXY IMAGE LIMITS " << G4endl
	   << " Y: " << imMinY << " " << imMaxY << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Y: " << newMinY << " " << newMaxY << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (must be >=0) " << G4endl
	   << " Y: " << imMinY - newMinY << " " << imMaxY - newMaxY << G4endl;
    newMaxY = imMaxY;
  }
  if( newMaxZ - imMaxZ > PRECISION ) {
    bInLimits = false;
    G4cerr << "!! MAXZ IMAGE LIMITS " << imMaxZ - newMaxZ << G4endl
      //	   << " PREC " << PRECISION //GDEB
	   << " Z: " << imMinZ << " " << imMaxZ << G4endl;
    G4cerr << "!! NEW REQUESTED LIMITS " << G4endl
	   << " Z: " << newMinZ << " " << newMaxZ << G4endl;
    G4cerr << "!! DIFFERENCE IN LIMITS (must be >=0) " << G4endl
	   << " Z: " << imMinZ - newMinZ << " " << imMaxZ - newMaxZ << G4endl;
    newMaxZ = imMaxZ;
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

  //----- Loop to voxels of new image
  for( G4int ixn = 0; ixn < newNoVoxelsX; ixn++ ) {
    G4double minXn = newMinX+ixn*theVoxelDimX; // left wall of new voxel
    G4double maxXn = newMinX+(ixn+1)*theVoxelDimX; // right wall of new voxel
    size_t im1stVoxX = GmGenUtils::GetBelowInt((minXn-imMinX)/imVoxDimX,PRECISION); // image ID of voxel corresponding to minXn
    G4double imVoxMinX = 0;
    G4double imVoxMaxX = image->GetNoVoxelsX();
    /*t
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) ) {
      if( ixn < 10 ) G4cout << ixn << "DicomOperResize::Operate X minXn " << imMinX+im1stVoxX*imVoxDimX << G4endl;
      if( ixn < 10000 ) G4cout << ixn << "/" << newNoVoxelsX << ": DicomOperResize::Operate X minXn " << minXn << " maxXn " << maxXn << " im1st " << im1stVoxX << " imVoxMinMax " << imVoxMinX << " " << imVoxMaxX << " imVoxDimX " << imVoxDimX << G4endl;
    }
#endif
    */
    for( G4int iyn = 0; iyn < newNoVoxelsY; iyn++ ) {
      G4double minYn = newMinY+iyn*theVoxelDimY; // left wall of new voxel
      G4double maxYn = newMinY+(iyn+1)*theVoxelDimY; // right wall of new voxel
      size_t im1stVoxY = GmGenUtils::GetBelowInt((minYn-imMinY)/imVoxDimY,PRECISION); // image ID of voxel corresponding to minYn      
      G4double imVoxMinY = 0;
      G4double imVoxMaxY = image->GetNoVoxelsY();
      /*t
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(testVerb) ) {
	if( iyn < 10000 ) G4cout << iyn << "/" << newNoVoxelsY << ": DicomOperResize::Operate Y minYn " << minYn << " " << maxYn << " im1st " << im1stVoxY << " imVoxMinMax " << imVoxMinY << " " << imVoxMaxY << G4endl;
      }
#endif
      */
      for( G4int izn = 0; izn < newNoVoxelsZ; izn++ ) {
	G4double minZn = newMinZ+izn*theVoxelDimZ; // left wall of new voxel
	G4double maxZn = newMinZ+(izn+1)*theVoxelDimZ; // left wall of new voxel
	size_t im1stVoxZ = GmGenUtils::GetBelowInt((minZn-imMinZ)/imVoxDimZ,PRECISION); // image ID of voxel corresponding to minZn
	//	G4cout << " im1stVoxZ " << im1stVoxZ << " " << (minZn-imMinZ)/imVoxDimZ << " -64 " << (minZn-imMinZ)/imVoxDimZ -64 << " minZn " << minZn << " imMinZ " <<imMinZ << " newMinZ " << newMinZ <<  G4endl; //GDEB
	//-	G4cout << " im1stVoxZ " << im1stVoxZ << " " << minZn << " " << imMinZ << " " << imVoxDimZ << " : " << (minZn-imMinZ) << " " << (minZn-imMinZ)/imVoxDimZ << G4endl;
	G4double imVoxMinZ = 0;
	G4double imVoxMaxZ = image->GetNoVoxelsZ();
	/*t 
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	  if( izn < 10 ) G4cout << izn << "/" << newNoVoxelsZ << ": DicomOperResize::Operate Z minZn " << minZn << " " << maxZn << " im1st " << im1stVoxZ << " imVoxMinMax " << imVoxMinZ << " " << imVoxMaxZ << G4endl;
	}
#endif
	*/
	G4double fracTOT = 0.;
	for( size_t ix = im1stVoxX;; ix++ ) {
	  if ( ix == im1stVoxX ) {
	    imVoxMinX = std::max(minXn,imMinX+im1stVoxX*imVoxDimX); // image left wall of old voxel
	    //	    imVoxMaxX = std::min(maxXn,imVoxMinX+theVoxelDimX); // image right wall of old voxel
	    imVoxMaxX = std::min(maxXn,imMinX+(im1stVoxX+1)*imVoxDimX); // image right wall of old voxel
	  }
	  G4double fracX = (imVoxMaxX-imVoxMinX)/theVoxelDimX;
	  //	  G4cout << ix << " fracX " << fracX << " = (" << imVoxMaxX << "-" << imVoxMinX << ")/" << theVoxelDimX << G4endl; //GDEB
	  for( size_t iy = im1stVoxY;; iy++ ) {
	    if( iy == im1stVoxY ) {
	      imVoxMinY = std::max(minYn,imMinY+im1stVoxY*imVoxDimY); // image left wall of old voxel
	      imVoxMaxY = std::min(maxYn,imMinY+(im1stVoxY+1)*imVoxDimY); // image right wall of old voxel
	    }
	    G4double fracXY = fracX*((imVoxMaxY-imVoxMinY)/theVoxelDimY);
	    //	    G4cout << iy << " fracY " << fracXY/fracX << " = (" << imVoxMaxY << "-" << imVoxMinY << ")/" << theVoxelDimY << G4endl; //GDEB
	    for( size_t iz = im1stVoxZ;; iz++ ) {
	      if( iz == im1stVoxZ ) {
		imVoxMinZ = std::max(minZn,imMinZ+im1stVoxZ*imVoxDimZ); // image left wall of old voxel
		imVoxMaxZ = std::min(maxZn,imMinZ+(im1stVoxZ+1)*imVoxDimZ); // image right wall of old voxel
	      }
	      G4double fracXYZ = fracXY*(imVoxMaxZ-imVoxMinZ)/theVoxelDimZ;
	      //	      G4cout << iz << " fracZ " << fracXYZ/fracXY << " = (" << imVoxMaxZ << "-" << imVoxMinZ << ")/" << theVoxelDimZ << G4endl; //GDEB
	      size_t copyNo = ixn + newNoVoxelsX*iyn + newNoVoxelsXY*izn;
	      size_t imCopyNo = ix + imNVoxX*iy + imNVoxXY*iz;
	      //	      G4cout << "@@ copyNo " << copyNo << " " << imCopyNo << " : " << ixn << " " << iyn << " " << izn << " < " << image->GetData(imCopyNo)*fracXYZ << " " << image->GetData(imCopyNo)<< " * " << fracXYZ << G4endl; //GDEB
	      //	      G4cout << "@@ imCopyNo " << imCopyNo << " : " << ix << " " << iy << " " << iz  << " < " << image->GetNoVoxels() << G4endl; //GDEB
	      newData->at(copyNo) += image->GetData(imCopyNo)*fracXYZ;
	      fracTOT += fracXYZ;
	      //	      G4cout << "@@ fracTOT " << fracTOT << " " << fracXYZ << " " << copyNo << ": " << newData->at(copyNo) << " " << imCopyNo << " : " << ix << " " << iy << " " << iz << " " << image->GetData(imCopyNo) << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
	      if( DicomVerb(testVerb) ) {
		if( ixn < 10 && iyn < 10 && izn < 10) G4cout << ix << " " << iy << " " << iz << "DicomOperResize::Operate fractions " << fracX << " " << fracXY/fracX << " " << fracXYZ/fracXY << G4endl;
	      }
#endif
	      imVoxMinZ = imVoxMaxZ;
	      imVoxMaxZ = std::min( maxZn, imVoxMaxZ+imVoxDimZ);
	      //	      G4cout << " Z BREAK? " << (imVoxMinZ - maxZn) << " =( " << imVoxMinZ << " - " << imVoxMaxZ << " >? " << " maxZn " << maxZn << " " << -PRECISION << G4endl; //GDEB
	      if( (imVoxMinZ - maxZn) >= -PRECISION ) break;
	    }
	    imVoxMinY = imVoxMaxY;
	    imVoxMaxY = std::min( maxYn, imVoxMaxY+imVoxDimY);
	    if( (imVoxMinY - maxYn) >= -PRECISION ) break;
	  }
	  imVoxMinX = imVoxMaxX;
	  imVoxMaxX = std::min( maxXn, imVoxMaxX+imVoxDimX);
	  if( (imVoxMinX - maxXn) >= -PRECISION ) break;
	}
	/*	if( fabs(fracTOT-1.) > 2.*PRECISION ) {
	  //	  G4cout << image << " OperResize PRECISION " << PRECISION << G4endl; //GDEB
	  G4Exception("DicomOperResize::Operate",
		      "",
		      FatalException,
		      ("voxel fraction sum -1. should be 0., while it is "+GmGenUtils::ftoa(fracTOT-1.)).c_str());
		      } */
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
