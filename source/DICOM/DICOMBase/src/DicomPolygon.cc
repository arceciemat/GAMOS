#include "DicomPolygon.hh"
#include "DicomPolygonList.hh"
#include "DicomMgr.hh"
#include "DicomVImageStr.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

#include "G4GeometryTolerance.hh"

#include "G4ExtrudedSolid.hh"

//-----------------------------------------------------------------------------
DicomPolygon::DicomPolygon( G4String name, DPOrientation ori )
  : DicomVLine(name,ori)
{
  theOrientation = DPOrientXY;
}

//-----------------------------------------------------------------------------
DicomPolygon::DicomPolygon( std::vector<G4ThreeVector> points, G4String name, DPOrientation ori )
  : DicomVLine(name, ori)
{
  SetPoints( points );
  BuildDirections();
  //  G4cout << this << " " << theName << " DicomPolygon::DicomPolygon(pts ORIENTATION " << theOrientation << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
DicomPolygon::DicomPolygon( std::vector<G4ThreeVector> points, std::vector<G4ThreeVector> dirs, G4String name, DPOrientation ori )
  : DicomVLine(name, ori)
{
  if( points.size() != dirs.size() ) {
    G4Exception("DicomPolygon::DicomPolygon",
		"",
		JustWarning,
		("Building Line with different number of points = " + GmGenUtils::itoa(points.size())+ " than of directions = "+ GmGenUtils::itoa(dirs.size())).c_str());
  }
  SetPoints( points );
  SetDirections( dirs );
  //  G4cout << this << " " << theName << " DicomPolygon::DicomPolygon(pts ORIENTATION " << theOrientation << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
void DicomPolygon::SetPoints( std::vector<G4ThreeVector> points )
{
  //--- do not count repeated points
  for( size_t ii = 0; ii < points.size(); ii++ ) {
    if( ii == 0 || points[ii] != points[ii-1] ) {
      thePoints.push_back(points[ii]);
    }
  }
  std::vector<G4ThreeVector>::iterator ite = points.begin();
  std::vector<G4ThreeVector>::reverse_iterator rite = points.rbegin();
  if( *ite != *rite) {
    if( DicomVerb(infoVerb) ) {
      G4Exception("DicomPolygon::SetPoints",
		  "",
		  JustWarning,
		  "First and last point are not the same, adding a last point");
      /*    for( size_t ii = 0; ii < points.size(); ii++ ) {
	    G4cerr << ii << " POINT " << points[ii] << G4endl; //GDEB
	    } */
    }
    thePoints.push_back(*ite);
    //    theDirections.push_back(G4ThreeVector());
  }
  
  DetectOrientation();

  //  for(size_t ii = 0; ii < thePoints.size()-1; ii++ ) {
  //    G4cout << ii << " POINT " << thePoints[ii].x() << " " << thePoints[ii].y() << " " << thePoints[ii].z() << G4endl; //GDEB
  //  }

}
  
//-----------------------------------------------------------------------------
void DicomPolygon::BuildDirections()
{
  size_t ii = 0;
  theDirections.clear();
  for( ; ii < thePoints.size()-1; ii++ ) {
    theDirections.push_back(thePoints[ii+1] - thePoints[ii]);
  }
  theDirections.push_back(thePoints[0]-thePoints[ii]);
  /*  std::cout << ii << " NDIRS " << theDirections.size() << " " << thePoints.size() << G4endl; //GDEB
  for( ii = 0; ii < thePoints.size(); ii++ ) {
    std::cout << " PTS " << thePoints[ii] << G4endl; //GDEB
    std::cout << " DIRS " << theDirections[ii] << G4endl; //GDEB
    }*/

}

//-----------------------------------------------------------------------------
void DicomPolygon::FindVoxelsInXY( DicomVImageStr* imageStr, G4double extraZ )
{
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomPolygon::FindVoxelsInXY in image " << imageStr->GetName() << " " << DicomVImage::GetModalityStr(imageStr->GetModality()) << " polygon_name " << theName <<  " from " << theLineList->GetName()  << " NPoints " << thePoints.size() << " Z= " << thePoints[0] << " - " << extraZ << G4endl;
  double PRECISION = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();

  if( theOrientation != DPOrientXY ) {
    G4Exception("DicomPolygon::FindVoxelsInXY",
		"",
		FatalException,
		("Only XY polygon orientations are supported, this is orientation " + DicomVLine::GetOrientationName(theOrientation) + " Please contact GAMOS authors").c_str());
  }
  std::set<double> distInters;

  //----- Extract the old image data
  size_t imNoVoxelsX = imageStr->GetNoVoxelsX();
  size_t imNoVoxelsY = imageStr->GetNoVoxelsY();
  size_t imNoVoxelsXY = imNoVoxelsX*imNoVoxelsY;
  size_t imNoVoxelsZ = imageStr->GetNoVoxelsZ();
  G4double imMinX = imageStr->GetMinX();
  G4double imMinY = imageStr->GetMinY();
  G4double imMinZ = imageStr->GetMinZ();
  G4double imMaxX = imageStr->GetMaxX();
  G4double imMaxY = imageStr->GetMaxY();
  G4double imVoxelDimX = imageStr->GetVoxelDimX();
  G4double imVoxelDimY = imageStr->GetVoxelDimY();
  G4double imVoxelDimZ = imageStr->GetVoxelDimZ();

  //--- Get contour Zplane ID and check it is in limits
  G4int iPolygonZ = GetPolygonZIndex(imageStr, extraZ);
  if( iPolygonZ < 0 || iPolygonZ >= G4int(imNoVoxelsZ) ) {
    return;
  }

  if( DicomVerb(debugVerb) ) G4cout << " INTERS CONTOUR WITH Z SLICE " << thePoints[0].z() << " zMin " << imMinZ << " zMax " << imMinZ+imVoxelDimZ*imageStr->GetNoVoxelsZ() << G4endl;
  if( DicomVerb(debugVerb) ) G4cout << " NPOINTS " << thePoints.size() << G4endl;

  //--- Set min/max extensions
  double minXc = DBL_MAX;
  double maxXc = -DBL_MAX;
  double minYc = DBL_MAX;
  double maxYc = -DBL_MAX;
  for( size_t ll = 0; ll < thePoints.size(); ll++ ){
    minXc = std::min(minXc,thePoints[ll].x());
    maxXc = std::max(maxXc,thePoints[ll].x());
    minYc = std::min(minYc,thePoints[ll].y());
    maxYc = std::max(maxYc,thePoints[ll].y());
  }
  if( minXc < imMinX || maxXc > imMaxX || minYc < imMinY || maxYc > imMaxY ){
    G4cerr << " minXc= " << minXc << " < " << imMinX
	   << " maxXc= " << maxXc << " > " << imMaxX
	   << " minYc= " << minYc << " < " << imMinY
	   << " maxYc= " << maxYc << " > " << imMaxY << G4endl;
    G4Exception("DicomPolygon::FindVoxelsInXY",
		"",
		JustWarning,
			  "Contour limits exceed Z slice extent");
  }
  //--- Set contour min/max voxel IDs 
  int idMinX = std::max(0,int((minXc-imMinX)/imVoxelDimX));
  int idMaxX = std::min(int(imNoVoxelsX-1),int((maxXc-imMinX)/imVoxelDimX+1));
  int idMinY = std::max(0,int((minYc-imMinY)/imVoxelDimY));
  int idMaxY = std::min(int(imNoVoxelsY-1),int((maxYc-imMinY)/imVoxelDimY+1));
  if( DicomVerb(debugVerb) )
    G4cout << " OK CONTOUR INSIDE X/Y OF Z SLICE "
	   << " minXc " << minXc << " < " << imMinX
	   << " maxXc " << maxXc << " > " << imMaxX
	   << " minYc " << minYc << " < " << imMinY
	   << " maxYc " << maxYc << " > " << imMaxY << G4endl;
  if( DicomVerb(debugVerb) )
    G4cout << " VOXEL ID LIMITS OF CONTOUR : idMinX " << idMinX 
	   << " idMaxX " << idMaxX 
	   << " idMinY " << idMinY 
	   << " idMaxY " << idMaxY << G4endl;
  //for each voxel: build 4 lines from the corner towards the center and check how many contour segments it crosses, and the minimum distance to a segment

  //  if( DicomVerb(debugVerb) ) G4cout << " LOOPING POLYGON " << theName << " from " << theLineList->GetName() << G4endl; 
  for( int ix = idMinX; ix <= idMaxX; ix++ ) {
    for( int iy = idMinY; iy <= idMaxY; iy++ ) {
      // check if center is in
      double c0x = imMinX + imVoxelDimX * (ix+0.5); // CENTRE X
      double c0y = imMinY + imVoxelDimY * (iy+0.5); // CENTRE Y
      
      //      G4double halfDiagonal = 0.5*imVoxelDimX;
      G4int NLoopInitial = 2;
      G4int NLoopPlus = 1;
      G4int NLoopMax = 6;
      G4int loopResult = 0;
      G4double iniAngle = 43*CLHEP::deg; // any direction
      G4double stepAngle = 153*CLHEP::deg;
      for( int iloop = 1; ; iloop++ ) {
	distInters.clear();
	if( DicomVerb(debugVerb) ) G4cout << " START LOOP " << iloop << G4endl;
	double v0x = cos(iniAngle+iloop*stepAngle);
	double v0y = sin(iniAngle+iloop*stepAngle);
	if( DicomVerb(debugVerb) ) G4cout << ix << " " << iy << "@@@@@ CHECKING VOXEL AT (" << c0x << "," << c0y << ")" << " DIR= (" << v0x << "," << v0y << ") " << G4endl;
	for( size_t ll = 0; ll < thePoints.size()-1; ll++ ){ // NO last point, as it is the same as first point => direction=0
	  double pc0x = thePoints[ll].x() - c0x;
	  double pc0y = thePoints[ll].y() - c0y;
	  double d0x = theDirections[ll].x();
	  double d0y = theDirections[ll].y();
	  //		    if( ix == 332 && iy == 274 )  G4cout << ll << " POINT (" << pc0x+c0x << "," << pc0y+c0y << ") " << " DIR= (" << d0x << "," << d0y << ") " << G4endl; //GDEB
	  double fac1 = d0x*v0y - d0y*v0x;
	  double fac2 = pc0x*v0y - pc0y*v0x;
	  double fac3 = pc0y*d0x - pc0x*d0y;
	  double lambdap = fac3/fac1;
	  /*	if( fabs(lambdap) < PRECISION ) { // if segment line passes by voxel centre , consider the voxel in
		distInters.clear();
		distInters.insert(lambdap);
		if( DicomVerb(testVerb) ) G4cout << " SEGMENT LINE BY VOXEL CENTER " << lambdap << " facs " << fac1 << " " << fac2 << " " << fac3 << " d0 " << d0x << " " << d0y << " v0 " << v0x << " " << v0y << " pc0 " << pc0x << " " << pc0y << " " << -fac2/fac1 << G4endl; 
		break;
		}*/
	  double lambdaq = 0;
	  //	if( fac1 != 0 ) { // if not parallel lines
	  if( fabs(fac1) > PRECISION ) { // if not parallel lines
	    lambdaq = -fac2/fac1;
	  } else {
	    if( DicomVerb(testVerb) ) G4cout << " PARALLEL LINES  LAMBDAP " << lambdap << " LAMBDAQ " << lambdaq << " facs " << fac1 << " " << fac2 << " " << fac3 << " d0 " << d0x << " " << d0y << " v0 " << v0x << " " << v0y << " pc0 " << pc0x << " " << pc0y << G4endl; 
	  }
	  if( DicomVerb(testVerb) ) G4cout << ll << " " << thePoints[ll] << " " << theDirections[ll] << " INTERS LAMBDAQ " << lambdaq << " facs " << fac1 << " " << fac2 << " " << fac3 << G4endl; 
	  //--- Intersection is = c0+lambdap*v0 and = thePoints[ll]+lambdaq*theDirections[ll]
	  if( lambdaq < PRECISION || lambdaq-1. >= PRECISION ) continue; // BAD: intersection further than segment length
	  if( lambdap > PRECISION ) { 
	    distInters.insert(lambdap);
	    if( DicomVerb(testVerb) ) G4cout << " !! GOOD INTERS " << lambdaq << "  (" << thePoints[ll].x()+lambdaq*d0x << "," << thePoints[ll].y()+lambdaq*d0y << ")  =  (" << c0x+lambdap*v0x << "," << c0y+lambdap*v0y << ") " << " N " << distInters.size() << G4endl;		      
	  } else {
	    if( DicomVerb(testVerb) ) G4cout << " !! BAD INTERS: OPPOSITE DIRECTION " << lambdap << G4endl;		      
	  }
	    if( DicomVerb(testVerb) ) G4cout << " INTERS LAMBDAQ " << lambdaq << " P " << lambdap << G4endl;
	  if( DicomVerb(testVerb) ) G4cout << " INTERS POINT FROM CENTRE= " << ll << "  (" << thePoints[ll].x()+lambdaq*d0x-c0x << "," << thePoints[ll].y()+lambdaq*d0y-c0y << ")  =  (" << c0x+lambdap*v0x-(thePoints[ll].x()+lambdaq*d0x) << "," << c0y+lambdap*v0y-(thePoints[ll].y()+lambdaq*d0y) << ") " << G4endl; 
	  if( DicomVerb(debugVerb) ) G4cout << " INTERS POINT FROM CENTRE= " << ll << " (" << thePoints[ll].x()+lambdaq*d0x << "," <<  thePoints[ll].y()+lambdaq*d0y << ")  =  (" << c0x+lambdap*v0x << "," << c0y+lambdap*v0y << ") " << G4endl;
	} // end thePoints loop
	if( distInters.size() % 2 == 1 ) { // search for odd number of segment intersections
	  loopResult++;
	} else {
	  loopResult--;
	}
	if( DicomVerb(debugVerb) ) G4cout << iloop << " LOOP RESULT " << (distInters.size() % 2) << " -> " << loopResult << G4endl; 
	if( iloop == NLoopInitial
	    || (iloop>NLoopInitial &&(iloop-NLoopInitial)%NLoopPlus == 0)
	    || iloop == NLoopMax
	    || iloop == NLoopMax + 1 ) {
	  if( DicomVerb(debugVerb) ) G4cout << " CHECK LOOP RESULT " << loopResult << " || " << iloop << " == " << NLoopMax+1 << G4endl; 
	  if( loopResult != 0 || iloop == NLoopMax + 1 ) break;
	  if( DicomVerb(debugVerb) ) G4cout << "!! CONTRADICTORY RESULT AFTER LOOP " << " (" << imMinX + imVoxelDimX*(ix+0.5) << "," << imMinY + imVoxelDimY*(iy+0.5) << "," << thePoints[0].z() << ")" << loopResult << " || " << iloop << " == " << NLoopMax+1 << G4endl; 
	}
      } //ENDED the NLOOPS 
      if( loopResult > 0 )  {
	if( DicomVerb(debugVerb) ) G4cout << "@@@@@ CENTRE OK  for Polygon " << GetName() << " " << ix << " " << iy << " (" << imMinX + imVoxelDimX*(ix+0.5) << "," << imMinY + imVoxelDimY*(iy+0.5) << "," << thePoints[0].z() << ")" << G4endl;
	// extract previous ID value
	G4String roiStr = imageStr->GetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY);
	//		roival = 2 + NMAXID*3 + NMAXID*NMAXID*15;
	size_t theID = theLineList->GetNumber();
	G4String theIDStr = GmGenUtils::itoa(theID);
	if(roiStr != "" ) { // add new roiStr
	  if( roiStr.find(theIDStr) != std::string::npos ) {
	    roiStr += ":"+theIDStr;
	  }
	  if( DicomVerb(debugVerb) ){
	    G4cout << ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY << " FINAL Struct ID WITH PREVIOUS ID's IN VOXEL " << ix << " " << iy << " : " << roiStr << G4endl;
	  }
	}
	imageStr->SetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY, theIDStr);

	/*    G4ThreeVector pos = imageStr->GetPosition(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY);
    G4double posZ = pos.z(); //GDEB
    G4double posY = pos.y(); //GDEB
    G4cout << " posZ " << posZ << " posY " << posY << G4endl; //GDEB
    if( posZ < -867 && posZ > -871 && posY < -100 && posY > -140 )
      G4cout << pos << " fStructure " << ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY << " : " << imageStr->GetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY) << " i " << ix << " " << iy << " " << iPolygonZ << " N " <<imNoVoxelsX << " " << imNoVoxelsXY << G4endl; //GDEB
	// 	G4cout << " fStructure minZ " << imMinZ + imVoxelDimZ*(ix) << " " << ix+iy*imNoVoxelsX << " : " << imageStr->GetData(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY) << " i " << ix << " " << iy << G4endl; //GDEB
	*/
      } 
      
    }
  }
}


//-----------------------------------------------------------------------------
G4bool DicomPolygon::PointIsInXY( G4ThreeVector point )
{
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomPolygon::FindVoxelsInXY in point " << point << " polygon_name " << theName << " NPoints " << thePoints.size() << G4endl;
  double PRECISION = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();

  if( theOrientation != DPOrientXY ) {
    G4Exception("DicomPolygon::FindVoxelsInXY",
		"",
		FatalException,
		("Only XY polygon orientations are supported, this is orientation " + DicomVLine::GetOrientationName(theOrientation) + " Please contact GAMOS authors").c_str());
  }

  std::set<double> distInters;

  //--- Set min/max extensions
  double minXc = DBL_MAX;
  double maxXc = -DBL_MAX;
  double minYc = DBL_MAX;
  double maxYc = -DBL_MAX;
  for( size_t ll = 0; ll < thePoints.size(); ll++ ){
    minXc = std::min(minXc,thePoints[ll].x());
    maxXc = std::max(maxXc,thePoints[ll].x());
    minYc = std::min(minYc,thePoints[ll].y());
    maxYc = std::max(maxYc,thePoints[ll].y());
  }

  //for each voxel: build 4 lines from the corner towards the center and check how many contour segments it crosses, and the minimum distance to a segment

  //  if( DicomVerb(debugVerb) ) G4cout << " LOOPING POLYGON " << theName << " from " << theLineList->GetName() << G4endl; 
  double c0x = point.x(); // CENTRE X
  double c0y = point.y(); // CENTRE Y
      
  //      G4double halfDiagonal = 0.5*imVoxelDimX;
  G4int NLoopInitial = 2;
  G4int NLoopPlus = 1;
  G4int NLoopMax = 6;
  G4int loopResult = 0;
  G4double iniAngle = 43*CLHEP::deg; // any direction
  G4double stepAngle = 153*CLHEP::deg;
  for( int iloop = 1; ; iloop++ ) {
    distInters.clear();
    if( DicomVerb(debugVerb) ) G4cout << " START LOOP " << iloop << G4endl;
    double v0x = cos(iniAngle+iloop*stepAngle);
    double v0y = sin(iniAngle+iloop*stepAngle);
    if( DicomVerb(debugVerb) ) G4cout << " @@@@@ CHECKING POINT AT (" << c0x << "," << c0y << ")" << " DIR= (" << v0x << "," << v0y << ") " << G4endl;
    for( size_t ll = 0; ll < thePoints.size()-1; ll++ ){ // NO last point, as it is the same as first point => direction=0
      double pc0x = thePoints[ll].x() - c0x;
      double pc0y = thePoints[ll].y() - c0y;
      double d0x = theDirections[ll].x();
      double d0y = theDirections[ll].y();
      //		    if( ix == 332 && iy == 274 )  G4cout << ll << " POINT (" << pc0x+c0x << "," << pc0y+c0y << ") " << " DIR= (" << d0x << "," << d0y << ") " << G4endl; //GDEB
      double fac1 = d0x*v0y - d0y*v0x;
      double fac2 = pc0x*v0y - pc0y*v0x;
      double fac3 = pc0y*d0x - pc0x*d0y;
      double lambdap = fac3/fac1;
      double lambdaq = 0;
      //	if( fac1 != 0 ) { // if not parallel lines
      if( fabs(fac1) > PRECISION ) { // if not parallel lines
	lambdaq = -fac2/fac1;
      } else {
	if( DicomVerb(testVerb) ) G4cout << " PARALLEL LINES  LAMBDAP " << lambdap << " LAMBDAQ " << lambdaq << " facs " << fac1 << " " << fac2 << " " << fac3 << " d0 " << d0x << " " << d0y << " v0 " << v0x << " " << v0y << " pc0 " << pc0x << " " << pc0y << G4endl; 
      }
      if( DicomVerb(testVerb) ) G4cout << ll << " " << thePoints[ll] << " " << theDirections[ll] << " INTERS LAMBDAQ " << lambdaq << " facs " << fac1 << " " << fac2 << " " << fac3 << G4endl; 
      //--- Intersection is = c0+lambdap*v0 and = thePoints[ll]+lambdaq*theDirections[ll]
      if( lambdaq < PRECISION || lambdaq-1. >= PRECISION ) continue; // BAD: intersection further than segment length
      if( lambdap > PRECISION ) { 
	distInters.insert(lambdap);
	if( DicomVerb(testVerb) ) G4cout << " !! GOOD INTERS " << lambdaq << "  (" << thePoints[ll].x()+lambdaq*d0x << "," << thePoints[ll].y()+lambdaq*d0y << ")  =  (" << c0x+lambdap*v0x << "," << c0y+lambdap*v0y << ") " << " N " << distInters.size() << G4endl;		      
      } else {
	if( DicomVerb(testVerb) ) G4cout << " !! BAD INTERS: OPPOSITE DIRECTION " << lambdap << G4endl;	 
      }
      if( DicomVerb(testVerb) ) G4cout << " INTERS LAMBDAQ " << lambdaq << " P " << lambdap << G4endl;
      if( DicomVerb(testVerb) ) G4cout << " INTERS POINT FROM CENTRE= " << ll << "  (" << thePoints[ll].x()+lambdaq*d0x-c0x << "," << thePoints[ll].y()+lambdaq*d0y-c0y << ")  =  (" << c0x+lambdap*v0x-(thePoints[ll].x()+lambdaq*d0x) << "," << c0y+lambdap*v0y-(thePoints[ll].y()+lambdaq*d0y) << ") " << G4endl; 
      if( DicomVerb(debugVerb) ) G4cout << " INTERS POINT FROM CENTRE= " << ll << " (" << thePoints[ll].x()+lambdaq*d0x << "," <<  thePoints[ll].y()+lambdaq*d0y << ")  =  (" << c0x+lambdap*v0x << "," << c0y+lambdap*v0y << ") " << G4endl;
    } // end thePoints loop
    if( distInters.size() % 2 == 1 ) { // search for odd number of segment intersections
      loopResult++;
    } else {
      loopResult--;
    }
    if( DicomVerb(debugVerb) ) G4cout << iloop << " LOOP RESULT " << (distInters.size() % 2) << " -> " << loopResult << G4endl; 
    if( iloop == NLoopInitial
	|| (iloop>NLoopInitial &&(iloop-NLoopInitial)%NLoopPlus == 0)
	|| iloop == NLoopMax
	|| iloop == NLoopMax + 1 ) {
      if( DicomVerb(debugVerb) ) G4cout << " CHECK LOOP RESULT " << loopResult << " || " << iloop << " == " << NLoopMax+1 << G4endl; 
      if( loopResult != 0 || iloop == NLoopMax + 1 ) break;
      //      if( DicomVerb(debugVerb) ) G4cout << "!! CONTRADICTORY RESULT AFTER LOOP " << " (" << imMinX + imVoxelDimX*(ix+0.5) << "," << imMinY + imVoxelDimY*(iy+0.5) << "," << thePoints[0].z() << ")" << loopResult << " || " << iloop << " == " << NLoopMax+1 << G4endl; 
    }
  } //ENDED the NLOOPS 
  if( loopResult > 0 )  {
    if( DicomVerb(debugVerb) ) G4cout << "@@@@@ CENTRE INSIDE Polygon " << GetName() << " " << point << G4endl;
    return true;
  } else {
    if( DicomVerb(debugVerb) ) G4cout << "@@@@@ CENTRE OUTSIDE Polygon " << GetName() << " " << point << G4endl;
    return false;
  }
}


//-----------------------------------------------------------------------------
G4int DicomPolygon::GetPolygonZIndex( DicomVImage* image, G4double extraZ )
{ 
  size_t imNoVoxelsZ = image->GetNoVoxelsZ();
  G4double imMinZ = image->GetMinZ();
  G4double imVoxelDimZ = image->GetVoxelDimZ();

  G4double polyZ = thePoints[0].z()+extraZ;
  G4int iPolygonZ = ( polyZ - imMinZ ) / imVoxelDimZ;
  if( iPolygonZ < 0 ) {
    G4cerr << "DicomPolygon has Z smaller than image "
	   << " Polygon Z= " << thePoints[0].z()
	   << " Image min Z= " << imMinZ << " diff " << thePoints[0].z()-imMinZ << G4endl;
    //-- check if it is a  precision problem
    if( polyZ - imMinZ  > -image->GetPrecision() ) {
      iPolygonZ = 0;
      G4Exception(" DicomPolygon::GetPolygonZIndex",
		  "",
		  JustWarning,
		  "Polygon Z is smaller than image minimum Z, probably due to precision. It will be set to image minimim Z");
    } else {
      G4Exception(" DicomPolygon::GetPolygonZIndex",
		  "",
		  JustWarning,
		  "Polygon Z is smaller than image minimum Z, it will not be used");
    }
  }

  if( iPolygonZ >= G4int(imNoVoxelsZ) ) {
    G4double imMaxZ = imMinZ+imVoxelDimZ*image->GetNoVoxelsZ();
    G4cerr << "DicomPolygon has Z bigger than image "
	     << "Polygon Z= " << thePoints[0].z() << "+" << extraZ << " Image max Z= " << imMaxZ << " diff " << polyZ-imMaxZ << G4endl;
    if( polyZ - imMaxZ < image->GetPrecision() ) {
      iPolygonZ = imNoVoxelsZ-1;
      G4Exception(" DicomPolygon::GetPolygonZIndex",
		"",
		  JustWarning,
		  "Polygon Z is bigger than image maximum Z, probably due to precision. It will be set to image maximim Z");
    } else {
      G4Exception(" DicomPolygon::GetPolygonZIndex",
		  "",
		  JustWarning,
		  "Polygon Z is bigger than image maximum Z, it will not be used");
    }
  }

  return iPolygonZ;
}

//-----------------------------------------------------------------------------
G4ExtrudedSolid* DicomPolygon::BuildExtrudedSolid(G4double dimZ)
{
  std::vector<G4TwoVector> polygonList;
  std::vector<G4ExtrudedSolid::ZSection> zsectionList;
  for( size_t ii = 0; ii < thePoints.size(); ii++ ) {
    polygonList.push_back( G4TwoVector(thePoints[ii].x(),thePoints[ii].y()) );
  }
  G4TwoVector offset(0.,0.);
  zsectionList.push_back( G4ExtrudedSolid::ZSection(-dimZ*0.5,offset,1.) );
  zsectionList.push_back( G4ExtrudedSolid::ZSection(+dimZ*0.5,offset,1.) );
  G4ExtrudedSolid* solid = new G4ExtrudedSolid( "EXTRUDED", polygonList, zsectionList );

  return solid;
}
