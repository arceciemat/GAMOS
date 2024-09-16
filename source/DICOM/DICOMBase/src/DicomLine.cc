#include "DicomLine.hh"
#include "DicomLineList.hh"
#include "DicomMgr.hh"
#include "DicomVImageStr.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"
#include "G4Point3D.hh"
#include "G4Plane3D.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

#include "G4GeometryTolerance.hh"
typedef std::multimap<G4double,size_t> mmdi;

//-----------------------------------------------------------------------------
DicomLine::DicomLine( G4String name, DPOrientation ori )
  : DicomVLine(name,ori) 
{
  theOrientation = DPOrientXY;
}

//-----------------------------------------------------------------------------
DicomLine::DicomLine( G4ThreeVector point, G4ThreeVector dir, G4String name, DPOrientation ori )
  : DicomVLine(name,ori)
{
  G4cout << " DicomLine::DicomLine nPt " << thePoints.size() << " point " << point << G4endl; //GDEB
  thePoints.push_back(point);
  theDirections.push_back(dir);
}

//-----------------------------------------------------------------------------
std::map<G4double,G4double> DicomLine::FindValues( DicomVImage* image, G4double step )
{
  if ( step == -DBL_MAX ) {
    return FindValuesInVoxels( image );
  } else {
    return FindValuesInSteps( image, step );
  }
}

//-----------------------------------------------------------------------------
std::map<G4double,G4String> DicomLine::FindValuesStr( DicomVImageStr* image, G4double step )
{
  if ( step == -DBL_MAX ) {
    return FindValuesInVoxelsStr( image );
  } else {
    return FindValuesInStepsStr( image, step );
  }
}
     
//-----------------------------------------------------------------------------
std::map<G4double,G4double> DicomLine::FindValuesInVoxels( DicomVImage* image)
{
  G4cout << " DicomLine::FindValuesInVoxels nPt " << thePoints.size() << G4endl; //GDEB

  std::map<G4double,G4double> intersValues;
  
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInVoxels in image " << image->GetName() << " " << DicomVImage::GetModalityStr(image->GetModality()) << G4endl;
  //  double PRECISION = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  G4double PRECISION = 1.e-6;
  //G4double PRECISION = image->GetPrecision();

  G4ThreeVector pos = thePoints[0];
  G4ThreeVector dir = theDirections[0];
  std::vector<G4double>* data = image->GetData();
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInVoxels pos " << pos << " dir " << dir << G4endl;
 
  //--- Store XYZ variables in  vectors
  std::vector<size_t> nVoxelV = image->GetNoVoxelsV();
  std::vector<G4double> fMinV = image->GetMinV();
  std::vector<G4double> fMaxV = image->GetMaxV();
  std::vector<G4double> fVoxelDimV = image->GetVoxelDimV();
  std::vector<G4String> fAxisNameV;
  fAxisNameV.push_back("X");
  fAxisNameV.push_back("Y");
  fAxisNameV.push_back("Z");

  G4double distMax = -DBL_MAX;
  for( size_t ii=0; ii < 3; ii++ ){
    G4double phantomWall = fMinV[ii]+ (0.5 + 0.5*GmGenUtils::sign(dir[ii]))* (fMaxV[ii]-fMinV[ii]);
    G4double dist = -DBL_MAX;
    if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + phantomWall) / dir[ii];
    distMax = std::max(distMax,dist);
  }  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxels total distMax to one phantom border " << distMax << G4endl;
#endif
  
  //----- Check if it is out of phantom
  G4int isOut = CheckOutOfPhantom( pos, dir, fMinV, fMaxV, fAxisNameV );
  
  //--- If it is out, find the smallest distance to phantom, and move pos to it
  G4double distAccum = 0.;
  if( isOut != 0 ) {
    G4double dist = -DBL_MAX;
    distMax = -DBL_MAX;
    for( size_t ii=0; ii < 3; ii++ ){
      if( fabs(dir[ii]) < PRECISION ) continue;
      int io = G4int(isOut/std::pow(10,ii))%10;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInVoxels Distance to out check coord= " << ii << " isOut= " << io << G4endl;
#endif
      if( io == 1 ) { // 
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMinV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInVoxels Distance to out negative wall " << " dist= " << dist << " ==(" << (-pos[ii] + fMinV[ii]) << "/" << dir[ii] << ")" <<G4endl;
#endif
      } else if( io == 2 ) {
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMaxV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInVoxels Distance to out " << " dist= " << dist<< " ==(" << (-pos[ii] + fMaxV[ii]) << "/" << dir[ii] << G4endl;
#endif
      }      
      distMax = std::max(distMax,dist);
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInVoxels FINAL Distance to out " << distMax << G4endl;
#endif

    for( size_t ii=0; ii < 3; ii++ ){
      pos[ii] += dir[ii]*( distMax + PRECISION );
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInVoxels pos in phantom border " << ii << " = " << pos << G4endl;
#endif
    }
  }
  //--- Check it is indeed in phantom border
  for( size_t ii=0; ii < 3; ii++ ){
    if( (pos[ii] - fMaxV[ii]) > PRECISION || (-pos[ii] + fMinV[ii]) > PRECISION ) {
      G4cerr << "@@@@ FindValuesInVoxels pos " << " = " << pos << G4endl;
      G4cerr << "@@@@ FindValuesInVoxels dir " << " = " << dir << G4endl;
      G4cerr << " IMAGE " << *image << G4endl; 
      G4Exception("DicomLine::FindValuesInVoxels",
		  "FindValuesInVoxels",
		  JustWarning,
		  "line does not intersect phantom");
    }
  }
  
  //--- Get Voxel ID of initial position
  std::vector<G4int> voxelID = {0,0,0};
  //  std::vector<size_t> idMax;
  for( size_t ii=0; ii < 3; ii++ ){
    voxelID[ii] = GmGenUtils::GetBelowInt((pos[ii]-fMinV[ii])/fVoxelDimV[ii]+PRECISION);
    //  G4int idMaxXNew = GmGenUtils::GetAboveInt((theImageMaxX-phMinX)/voxelFWidthX);
    //  G4int phNVoxelXNew = idMaxXNew - idMinXNew;
  }
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInVoxels 1st voxelID " << voxelID[0] << " " << voxelID[1] << " " << voxelID[2] << G4endl;
#endif

  //--- FIND NEXT VOXEL
  mmdi dists;
  G4bool bIsFirstPoint = true;
  G4double distHis = 0.;
  G4bool bEnded = false;
  for( ;; ) {
    if( ! bIsFirstPoint ) {
      dists.clear();
      for( size_t ii=0; ii < 3; ii++ ){
	G4double voxelWall = fMinV[ii] + (voxelID[ii] + 0.5 + 0.5*GmGenUtils::sign(dir[ii]))*fVoxelDimV[ii];
	G4double dist = DBL_MAX;
	if( fabs(dir[ii]) > PRECISION ) {
	  dist = (-pos[ii] + voxelWall) / dir[ii];
	  dist = G4int(dist/PRECISION)*PRECISION;
	}
	dists.insert(mmdi::value_type(dist,ii));
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxels dist to voxelWall " << ii << " = " << dist << " voxelWall= " << voxelWall << G4endl;
#endif
      }
      G4double distMin = (*(dists.begin())).first; // minimal of distance along X, y and Z 
      distAccum += distMin;
      for( size_t ii=0; ii < 3; ii++ ){
	pos[ii] += dir[ii]*( distMin + PRECISION );
      }
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxels pos new " << pos << " distAccum " << distAccum << " distMin " << distMin <<  G4endl;
#endif
      std::pair<mmdi::const_iterator,mmdi::const_iterator> pite = dists.equal_range(distMin);
      mmdi::const_iterator ite;
      for( ite = pite.first; ite != pite.second; ite++ ) {
	size_t ii = (*ite).second;
	voxelID[ii] += GmGenUtils::sign(dir[ii]);
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxels new VoxelID " << ii << " : " << voxelID[ii] << G4endl;   
#endif
	if( voxelID[ii] < 0 || voxelID[ii] >= G4int(nVoxelV[ii]) ) {
	  bEnded = true;
	  break;
	} 
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxels voxelID new " << voxelID[0] << " " << voxelID[1] << " " << voxelID[2] << G4endl;
#endif
	if( bEnded ) break;
	distHis = distAccum + distMin/2.;
      }
    }
      
    bIsFirstPoint = false;
    if( bEnded ) break;
    size_t copyNo = voxelID[0] + voxelID[1]*nVoxelV[0] + voxelID[2]*nVoxelV[0]*nVoxelV[1];
    G4double value = data->at(copyNo);
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxels copyNo " << copyNo << " VALUE= " << value << " at dist " << distHis
#endif
				      << " voxel_centre (" << fMinV[0]+(voxelID[0]+0.5)*fVoxelDimV[0] << "," << fMinV[1]+(voxelID[1]+0.5)*fVoxelDimV[1] << "," << fMinV[2]+(voxelID[2]+0.5)*fVoxelDimV[2] << ") pos " << pos << G4endl;
    //    G4double maxValInFile = 100;
    //t    if( fabs(distHis) < maxValInFile )
    intersValues[distHis] = value;
    //    G4cout << " DicomLine FINAL VALUE " << value << " at " << pos << " dist= " << distHis <<  G4endl; //GDEB
  }

#ifndef GAMOS_NO_VERBOSE
  if(DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxels ended N intersections: " << intersValues.size() << G4endl;
#endif
  return intersValues;
}

//-----------------------------------------------------------------------------
std::map<G4double,G4double> DicomLine::FindValuesInSteps( DicomVImage* image, G4double step)
{
  std::map<G4double,G4double> intersValues;
  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInSteps in image " << image->GetName() << " " << DicomVImage::GetModalityStr(image->GetModality()) << G4endl;
#endif
  //  double PRECISION = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  //  G4double PRECISION = 1.e-6;
  G4double PRECISION = image->GetPrecision();

  G4ThreeVector pos = thePoints[0];
  G4ThreeVector dir = theDirections[0];
  std::vector<G4double>* data = image->GetData();
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInSteps pos " << pos << " dir " << dir << G4endl;
#endif
 
  //--- Store XYZ variables in  vectors
  std::vector<size_t> nVoxelV = image->GetNoVoxelsV();
  std::vector<G4double> fMinV = image->GetMinV();
  std::vector<G4double> fMaxV = image->GetMaxV();
  std::vector<G4double> fVoxelDimV = image->GetVoxelDimV();
  std::vector<G4String> fAxisNameV;
  fAxisNameV.push_back("X");
  fAxisNameV.push_back("Y");
  fAxisNameV.push_back("Z");

  G4double distMax = -DBL_MAX;
  for( size_t ii=0; ii < 3; ii++ ){
    G4double phantomWall = fMinV[ii]+ (0.5 + 0.5*GmGenUtils::sign(dir[ii]))* (fMaxV[ii]-fMinV[ii]);
    G4double dist = -DBL_MAX;
    if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + phantomWall) / dir[ii];
    distMax = std::max(distMax,dist);
  }  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInSteps total distMax to one phantom border " << distMax << G4endl;
#endif

  //----- Check if it is out of phantom
  G4int isOut = 0;
  for( size_t ii=0; ii < 3; ii++ ){
    if( pos[ii] > fMaxV[ii] ) {
      if( dir[ii] >= 0 ) {
	G4Exception("DicomLine::FindValuesInSteps",
		    "",
		    JustWarning,
		    ("Position in "+ fAxisNameV[ii] + " is bigger than phantom limits but direction is not negative").c_str());
      }
      isOut += 2*std::pow(10,ii);
    } else if( pos[ii] < fMinV[ii]) {
      if( dir[ii] <= 0 ) {
	G4Exception("DicomLine::FindValuesInSteps",
		    "",
		    //		    FatalException,
		    JustWarning,
		    ("Position in "+ fAxisNameV[ii] + " is smaller than phantom limits but direction is not positive").c_str());
      }
      isOut += std::pow(10,ii);
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInSteps IsOut?= " << ii << " : " << isOut << "   : xyz 2 if positive border, 1 if negative border " << G4endl;  
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) {
    G4cout << "@@@@ FindValuesInSteps IsOut= " << isOut
	   << " phantom min " << fMinV[0] << " " << fMinV[1] << " " << fMinV[2] 
	   << " phantom max " << fMaxV[0] << " " << fMaxV[1] << " " << fMaxV[2] << G4endl;
  }
#endif
  
  //--- If it is out, find the smallest distance to phantom, and move pos to it
  G4double distAccum = 0.;
  if( isOut != 0 ) {
    G4double dist = -DBL_MAX;
    distMax = -DBL_MAX;
    for( size_t ii=0; ii < 3; ii++ ){
      if( fabs(dir[ii]) < PRECISION ) continue;
      int io = G4int(isOut/std::pow(10,ii))%10;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInSteps Distance to out check coord= " << ii << " isOut= " << io << G4endl;
#endif
      if( io == 1 ) { // 
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMinV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInSteps Distance to out negative wall " << " dist= " << dist << " ==(" << (-pos[ii] + fMinV[ii]) << "/" << dir[ii] << ")" <<G4endl;
#endif
      } else if( io == 2 ) {
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMaxV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInSteps Distance to out " << " dist= " << dist<< " ==(" << (-pos[ii] + fMaxV[ii]) << "/" << dir[ii] << G4endl;
#endif
      }      
      distMax = std::max(distMax,dist);
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInSteps FINAL Distance to out " << distMax << G4endl;
#endif

    for( size_t ii=0; ii < 3; ii++ ){
      pos[ii] += dir[ii]*( distMax + PRECISION );
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInSteps pos in phantom border " << ii << " = " << pos << G4endl;
#endif
    }
  }
  //--- Check it is indeed in phantom border
  for( size_t ii=0; ii < 3; ii++ ){
    if( (pos[ii] - fMaxV[ii]) > PRECISION || (-pos[ii] + fMinV[ii]) > PRECISION ) {
      G4cerr << "@@@@ FindValuesInSteps pos " << " = " << pos << G4endl;
      G4cerr << "@@@@ FindValuesInSteps dir " << " = " << dir << G4endl;
      G4cerr << " IMAGE " << *image << G4endl; 
      G4Exception("DicomLine::FindValuesInSteps",
		  "FindValuesInSteps",
		  JustWarning,
		  "line does not intersect phantom");
    }
  }
  
  //--- Get Voxel ID of initial position
  std::vector<G4int> hvoxelID = {0,0,0};
  //  std::vector<size_t> idMax;
  /*  for( size_t ii=0; ii < 3; ii++ ){
    hvoxelID[ii] = GmGenUtils::GetBelowInt((pos[ii]-fMinV[ii])/fVoxelDimV[ii]*2.+PRECISION);  //in half vxo
    G4cout << " HV " << hvoxelID[ii] << " = " << (pos[ii]-fMinV[ii])/fVoxelDimV[ii]*2. << (pos[ii]-fMinV[ii])/fVoxelDimV[ii] << " " << (pos[ii]-fMinV[ii]) << " / " << fVoxelDimV[ii] << G4endl; //GDEB
      //  G4int idMaxXNew = GmGenUtils::GetAboveInt((theImageMaxX-phMinX)/voxelFWidthX);
    //  G4int phNVoxelXNew = idMaxXNew - idMinXNew;
  }
  if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInSteps 1st hvoxelID " << hvoxelID[0] << " " << hvoxelID[1] << " " << hvoxelID[2] << G4endl;
  */
  
  //--- FIND VOXEL OF NEXT POSITION
  mmdi dists;
  G4double nVoxelXY = nVoxelV[0]*nVoxelV[1];
  for( ;; ) {
    G4bool bEnded = false;
    std::vector<G4int> bPosXYZ; 
    for( size_t ii=0; ii < 3; ii++ ){
      hvoxelID[ii] = GmGenUtils::GetBelowInt((pos[ii]-fMinV[ii])/fVoxelDimV[ii]*2+PRECISION);
      if( hvoxelID[ii] < 0 || hvoxelID[ii] >= G4int(2*nVoxelV[ii]-1) ) bEnded = true; // line gets out of phantom
      if( hvoxelID[ii] == 0 ) {
	bPosXYZ.push_back(-1);
      } else if( hvoxelID[ii] == G4int(2*nVoxelV[ii]-1) ) {
	bPosXYZ.push_back(1);
      } else {
	bPosXYZ.push_back(0);
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) )  G4cout << bEnded << " HALF_VOXELID " << pos << " " << hvoxelID[0] << " " << hvoxelID[1] << " " << hvoxelID[2] << G4endl; //G
#endif
    if( bEnded ) break;

    //t treatment 1 and 2 corners separately, for the moment skip
    G4bool bNotNow = false;
    for( size_t ii=0; ii < 3; ii++ ){
      if( bPosXYZ[ii] != 0 ) {
	distAccum += step;
	pos += dir*step;
	bNotNow = true;
	G4cout << " SKIP NOW " << G4endl; //GDEB
	break;
      }
    }
    if( bNotNow ) continue;
    
    // get value interpolating values of 6 surrounding voxels
    G4double value = 0.;

    size_t ix = (hvoxelID[0]-1)/2;
    size_t iy = (hvoxelID[1]-1)/2;
    size_t iz = (hvoxelID[2]-1)/2;
    G4double XLocal = pos.x()-(fMinV[0]+(ix+0.5)*fVoxelDimV[0]);
    G4double YLocal = pos.y()-(fMinV[1]+(iy+0.5)*fVoxelDimV[1]);
    G4cout << pos << " YLocal " << YLocal << "= " << pos.y() << " - " << (fMinV[1]+(iy+0.5)*fVoxelDimV[1]) << "   " << fMinV[1] << "+"<<iy+0.5<<"*"<<fVoxelDimV[1]<< G4endl; //GDEB
    G4cout << " pos " << pos << " ixyz " << ix<<":"<<iy<<":"<< iz << " XYLocal " << XLocal << " " << YLocal << G4endl; //GDEB
    size_t copyNo;
    //---- BUILD TRIANGLE at Z- (from x,y=0,0 to x,y=widthX,widhtY)
    // corner down left
    copyNo = ix + iy*nVoxelV[0] + iz*nVoxelXY;
    G4Point3D pointN1( 0., 0., data->at(copyNo) );
    G4Point3D pointN2;
    // corner up right
    copyNo = (ix+1) + (iy+1)*nVoxelV[0] + iz*nVoxelXY;
    G4Point3D pointN3( fVoxelDimV[0], fVoxelDimV[1], data->at(copyNo) );
//---- BUILD TRIANGLE at Z+ (from x,y=0,0 to x,y=widthX,widhtY)
    // corner down left
    copyNo = ix + iy*nVoxelV[0] + (iz+1)*nVoxelXY;
    G4Point3D pointP1( 0., 0., data->at(copyNo) );
    //    G4cout << "pointP1 " << pointP1 << " " << copyNo << G4endl; //GDEB
    G4Point3D pointP2;
    // corner up right
    copyNo = (ix+1) + (iy+1)*nVoxelV[0] + (iz+1)*nVoxelXY;
    G4Point3D pointP3( fVoxelDimV[0], fVoxelDimV[1], data->at(copyNo) );
    //    G4cout << "pointP3 " << pointP3 << " " << copyNo << G4endl; //GDEB
    
    if( XLocal > YLocal ) { // USE LOWER TRIANGLE
      //---- BUILD TRIANGLE at Z- (from x,y=0,0 to x,y=widthX,widhtY)
      // corner down right
      copyNo = (ix+1) + iy*nVoxelV[0] + iz*nVoxelXY;
      pointN2 = G4Point3D( fVoxelDimV[0], 0., data->at(copyNo) );
      //      G4cout << "pointN2 " << pointN2 << " " << copyNo << G4endl; //GDEB
      //---- BUILD TRIANGLE at Z+ (from x,y=0,0 to x,y=widthX,widhtY)
      // corner down right
      copyNo = (ix+1) + iy*nVoxelV[0] + (iz+1)*nVoxelXY;
      pointP2 = G4Point3D( fVoxelDimV[0], 0., data->at(copyNo) );
      //      G4cout << "pointP2 " << pointP2 << " " << copyNo << G4endl; //GDEB
    } else { // USE UPPER TRIANGLE
      //---- BUILD TRIANGLE at Z- (from x,y=0,0 to x,y=widthX,widhtY)
      // corner up left
      copyNo = (ix) + (iy+1)*nVoxelV[0] + iz*nVoxelXY;
      pointN2 = G4Point3D( 0., fVoxelDimV[1], data->at(copyNo) );
      //      G4cout << "pointN2 " << pointN2 << " " << copyNo << G4endl; //GDEB
      // corner up right
      //---- BUILD TRIANGLE at Z+ (from x,y=0,0 to x,y=widthX,widhtY)
      // corner down right
      copyNo = (ix+1) + iy*nVoxelV[0] + (iz+1)*nVoxelXY;
      pointP2 = G4Point3D( fVoxelDimV[0], 0., data->at(copyNo) );
      //      G4cout << "pointP2 " << pointP2 << " " << copyNo << G4endl; //GDEB
    }
    //---- BUILD TRIANGLE at Z- (from x,y=0,0 to x,y=widthX,widhtY)
    // create plane from these three points
    G4Plane3D triangleN(pointN1, pointN2, pointN3);
    // projection of pos to triangle along Z
    //    G4cout << " triangleN " << triangleN<< G4endl; //GDEB
    G4double valueZN = (-triangleN.d()-triangleN.a()*XLocal-triangleN.b()*YLocal)/triangleN.c();
    //    G4cout << " valueZN " << valueZN << " " << (-triangleN.d()-triangleN.a()*XLocal-triangleN.c()*valueZN)/triangleN.b() << G4endl; //GDEB
    //---- BUILD TRIANGLE at Z+ (from x,y=0,0 to x,y=widthX,widhtY)
    // create plane from these three points
    G4Plane3D triangleP(pointP1, pointP2, pointP3);
    // projection of pos to triangle along Z
    //    G4cout << " triangleP " << triangleP<< G4endl; //GDEB
    G4double valueZP = (-triangleP.d()-triangleP.a()*XLocal-triangleP.b()*YLocal)/triangleP.c();
    //    G4cout << " valueZP " << valueZP << " " << (-triangleP.d()-triangleP.a()*XLocal-triangleP.c()*valueZP)/triangleP.b() << G4endl; //GDEB

    //---- INTERPOLATE VALUES AT Z- AND Z+
    G4double ZLocal = pos.z()-(fMinV[2]+(iz+0.5)*fVoxelDimV[2]);
    G4double slope = (valueZP-valueZN)/(fVoxelDimV[2]-0.);
    value = valueZN + slope * (ZLocal-0.);
    //    G4cout << " VOXEL_VALUE " << value << " slope=" << slope << " ZLocal=" << ZLocal << G4endl; //GDEB

    G4double distHis = distAccum;
    /*    if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInSteps VALUE= " << value << " at dist " << distHis
				      << " voxel_centre (" << fMinV[0]+(hvoxelID[0]+0.5)*fVoxelDimV[0] << "," << fMinV[1]+(voxelID[1]+0.5)*fVoxelDimV[1] << "," << fMinV[2]+(voxelID[2]+0.5)*fVoxelDimV[2] << ") pos " << pos << G4endl;
    */
    //    G4double maxValInFile = 100;
    //t    if( fabs(distHis) < maxValInFile )
    intersValues[distHis] = value;
    distAccum += step;
    pos += dir*step;
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) )  G4cout << " DicomLine FINAL VALUE " << value << " at " << pos << " dist= " << distHis << G4endl; //GDEB
#endif
   }

#ifndef GAMOS_NO_VERBOSE
  if(DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInSteps ended N intersections: " << intersValues.size() << G4endl;
#endif

  return intersValues;
										  
}

<<<<<<< HEAD
  //-----------------------------------------------------------------------------
std::map<G4double,G4String> DicomLine::FindValuesInVoxelsStr( DicomVImageStr* imageStr )
{
  G4cout << " DicomLine::FindValuesInVoxelsStr nPt " << thePoints.size() << G4endl; //GDEB

  std::map<G4double,G4String> intersValues;
  
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInVoxelsStr in imageStr " << imageStr->GetName() << " " << DicomVImage::GetModalityStr(imageStr->GetModality()) << G4endl;
  //  double PRECISION = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  G4double PRECISION = 1.e-6;
  //G4double PRECISION = imageStr->GetPrecision();

  G4ThreeVector pos = thePoints[0];
  G4ThreeVector dir = theDirections[0];
  std::vector<G4String>* dataStr = imageStr->GetDataStr();
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInVoxelsStr pos " << pos << " dir " << dir << G4endl;
 
  //--- Store XYZ variables in  vectors
  std::vector<size_t> nVoxelV = imageStr->GetNoVoxelsV();
  std::vector<G4double> fMinV = imageStr->GetMinV();
  std::vector<G4double> fMaxV = imageStr->GetMaxV();
  std::vector<G4double> fVoxelDimV = imageStr->GetVoxelDimV();
  std::vector<G4String> fAxisNameV;
  fAxisNameV.push_back("X");
  fAxisNameV.push_back("Y");
  fAxisNameV.push_back("Z");

  G4double distMax = -DBL_MAX;
  for( size_t ii=0; ii < 3; ii++ ){
    G4double phantomWall = fMinV[ii]+ (0.5 + 0.5*GmGenUtils::sign(dir[ii]))* (fMaxV[ii]-fMinV[ii]);
    G4double dist = -DBL_MAX;
    if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + phantomWall) / dir[ii];
    distMax = std::max(distMax,dist);
  }  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr total distMax to one phantom border " << distMax << G4endl;
#endif
  
  //----- Check if it is out of phantom
  G4int isOut = CheckOutOfPhantom( pos, dir, fMinV, fMaxV, fAxisNameV );

  //--- If it is out, find the smallest distance to phantom, and move pos to it
  G4double distAccum = 0.;
  if( isOut != 0 ) {
    G4double dist = -DBL_MAX;
    distMax = -DBL_MAX;
    for( size_t ii=0; ii < 3; ii++ ){
      if( fabs(dir[ii]) < PRECISION ) continue;
      int io = G4int(isOut/std::pow(10,ii))%10;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr Distance to out check coord= " << ii << " isOut= " << io << G4endl;
#endif
      if( io == 1 ) { // 
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMinV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr Distance to out negative wall " << " dist= " << dist << " ==(" << (-pos[ii] + fMinV[ii]) << "/" << dir[ii] << ")" <<G4endl;
#endif
      } else if( io == 2 ) {
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMaxV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr Distance to out " << " dist= " << dist<< " ==(" << (-pos[ii] + fMaxV[ii]) << "/" << dir[ii] << G4endl;
#endif
      }      
      distMax = std::max(distMax,dist);
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr FINAL Distance to out " << distMax << G4endl;
#endif

    for( size_t ii=0; ii < 3; ii++ ){
      pos[ii] += dir[ii]*( distMax + PRECISION );
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr pos in phantom border " << ii << " = " << pos << G4endl;
#endif
    }
  }  
  //--- Check it is indeed in phantom border
  for( size_t ii=0; ii < 3; ii++ ){
    if( (pos[ii] - fMaxV[ii]) > PRECISION || (-pos[ii] + fMinV[ii]) > PRECISION ) {
      G4cerr << "@@@@ FindValuesInVoxelsStr pos " << " = " << pos << G4endl;
      G4cerr << "@@@@ FindValuesInVoxelsStr dir " << " = " << dir << G4endl;
      G4cerr << " IMAGESTR " << *imageStr << G4endl; 
      G4Exception("DicomLine::FindValuesInVoxels",
		  "FindValuesInVoxels",
		  JustWarning,
		  "line does not intersect phantom");
    }
  }
  
  //--- Get Voxel ID of initial position
  std::vector<G4int> voxelID = {0,0,0};
  //  std::vector<size_t> idMax;
  for( size_t ii=0; ii < 3; ii++ ){
    voxelID[ii] = GmGenUtils::GetBelowInt((pos[ii]-fMinV[ii])/fVoxelDimV[ii]+PRECISION);
    //  G4int idMaxXNew = GmGenUtils::GetAboveInt((theImageMaxX-phMinX)/voxelFWidthX);
    //  G4int phNVoxelXNew = idMaxXNew - idMinXNew;
  }
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr 1st voxelID " << voxelID[0] << " " << voxelID[1] << " " << voxelID[2] << G4endl;
#endif

  //--- FIND NEXT VOXEL
  mmdi dists;
  G4bool bIsFirstPoint = true;
  G4double distHis = 0.;
  G4bool bEnded = false;
  for( ;; ) {
    if( ! bIsFirstPoint ) {
      dists.clear();
      for( size_t ii=0; ii < 3; ii++ ){
	G4double voxelWall = fMinV[ii] + (voxelID[ii] + 0.5 + 0.5*GmGenUtils::sign(dir[ii]))*fVoxelDimV[ii];
	G4double dist = DBL_MAX;
	if( fabs(dir[ii]) > PRECISION ) {
	  dist = (-pos[ii] + voxelWall) / dir[ii];
	  dist = G4int(dist/PRECISION)*PRECISION;
	}
	dists.insert(mmdi::value_type(dist,ii));
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr dist to voxelWall " << ii << " = " << dist << " voxelWall= " << voxelWall << G4endl;
#endif
      }
      G4double distMin = (*(dists.begin())).first; // minimal of distance along X, y and Z 
      distAccum += distMin;
      for( size_t ii=0; ii < 3; ii++ ){
	pos[ii] += dir[ii]*( distMin + PRECISION );
      }
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr pos new " << pos << " distAccum " << distAccum << " distMin " << distMin <<  G4endl;
#endif
      std::pair<mmdi::const_iterator,mmdi::const_iterator> pite = dists.equal_range(distMin);
      mmdi::const_iterator ite;
      for( ite = pite.first; ite != pite.second; ite++ ) {
	size_t ii = (*ite).second;
	voxelID[ii] += GmGenUtils::sign(dir[ii]);
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr new VoxelID " << ii << " : " << voxelID[ii] << G4endl;   
#endif
	if( voxelID[ii] < 0 || voxelID[ii] >= G4int(nVoxelV[ii]) ) {
	  bEnded = true;
	  break;
	} 
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr voxelID new " << voxelID[0] << " " << voxelID[1] << " " << voxelID[2] << G4endl;
#endif
	if( bEnded ) break;
	distHis = distAccum + distMin/2.;
      }
    }
      
    bIsFirstPoint = false;
    if( bEnded ) break;
    size_t copyNo = voxelID[0] + voxelID[1]*nVoxelV[0] + voxelID[2]*nVoxelV[0]*nVoxelV[1];
    G4String value = dataStr->at(copyNo);
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr copyNo " << copyNo << " VALUE= " << value << " at dist " << distHis
#endif
				      << " voxel_centre (" << fMinV[0]+(voxelID[0]+0.5)*fVoxelDimV[0] << "," << fMinV[1]+(voxelID[1]+0.5)*fVoxelDimV[1] << "," << fMinV[2]+(voxelID[2]+0.5)*fVoxelDimV[2] << ") pos " << pos << G4endl;
    //    G4double maxValInFile = 100;
    //t    if( fabs(distHis) < maxValInFile )
    intersValues[distHis] = value;
    //    G4cout << " DicomLine FINAL VALUE " << value << " at " << pos << " dist= " << distHis <<  G4endl; //GDEB
  }

#ifndef GAMOS_NO_VERBOSE
  if(DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInVoxelsStr ended N intersections: " << intersValues.size() << G4endl;
#endif
  return intersValues;
}

//-----------------------------------------------------------------------------
std::map<G4double,G4String> DicomLine::FindValuesInStepsStr( DicomVImageStr* imageStr, G4double step)
{
  std::map<G4double,G4String> intersValues;
  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInStepsStr in imageStr " << imageStr->GetName() << " " << DicomVImage::GetModalityStr(imageStr->GetModality()) << G4endl;
#endif
  //  double PRECISION = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  //  G4double PRECISION = 1.e-6;
  G4double PRECISION = imageStr->GetPrecision();

  G4ThreeVector pos = thePoints[0];
  G4ThreeVector dir = theDirections[0];
  std::vector<G4String>* dataStr = imageStr->GetDataStr();
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLine::FindValuesInStepsStr pos " << pos << " dir " << dir << G4endl;
#endif
 
  //--- Store XYZ variables in  vectors
  std::vector<size_t> nVoxelV = imageStr->GetNoVoxelsV();
  std::vector<G4double> fMinV = imageStr->GetMinV();
  std::vector<G4double> fMaxV = imageStr->GetMaxV();
  std::vector<G4double> fVoxelDimV = imageStr->GetVoxelDimV();
  std::vector<G4String> fAxisNameV;
  fAxisNameV.push_back("X");
  fAxisNameV.push_back("Y");
  fAxisNameV.push_back("Z");

  G4double distMax = -DBL_MAX;
  for( size_t ii=0; ii < 3; ii++ ){
    G4double phantomWall = fMinV[ii]+ (0.5 + 0.5*GmGenUtils::sign(dir[ii]))* (fMaxV[ii]-fMinV[ii]);
    G4double dist = -DBL_MAX;
    if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + phantomWall) / dir[ii];
    distMax = std::max(distMax,dist);
  }  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInStepsStr total distMax to one phantom border " << distMax << G4endl;
#endif

  //----- Check if it is out of phantom
  G4int isOut = 0;
  for( size_t ii=0; ii < 3; ii++ ){
    if( pos[ii] > fMaxV[ii] ) {
      if( dir[ii] >= 0 ) {
	G4Exception("DicomLine::FindValuesInSteps",
		    "",
		    JustWarning,
		    ("Position in "+ fAxisNameV[ii] + " is bigger than phantom limits but direction is not negative").c_str());
      }
      isOut += 2*std::pow(10,ii);
    } else if( pos[ii] < fMinV[ii]) {
      if( dir[ii] <= 0 ) {
	G4Exception("DicomLine::FindValuesInSteps",
		    "",
		    //		    FatalException,
		    JustWarning,
		    ("Position in "+ fAxisNameV[ii] + " is smaller than phantom limits but direction is not positive").c_str());
      }
      isOut += std::pow(10,ii);
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInStepsStr IsOut?= " << ii << " : " << isOut << "   : xyz 2 if positive border, 1 if negative border " << G4endl;  
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) {
    G4cout << "@@@@ FindValuesInStepsStr IsOut= " << isOut
	   << " phantom min " << fMinV[0] << " " << fMinV[1] << " " << fMinV[2] 
	   << " phantom max " << fMaxV[0] << " " << fMaxV[1] << " " << fMaxV[2] << G4endl;
  }
#endif
  
  //--- If it is out, find the smallest distance to phantom, and move pos to it
  G4double distAccum = 0.;
  if( isOut != 0 ) {
    G4double dist = -DBL_MAX;
    distMax = -DBL_MAX;
    for( size_t ii=0; ii < 3; ii++ ){
      if( fabs(dir[ii]) < PRECISION ) continue;
      int io = G4int(isOut/std::pow(10,ii))%10;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInStepsStr Distance to out check coord= " << ii << " isOut= " << io << G4endl;
#endif
      if( io == 1 ) { // 
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMinV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInStepsStr Distance to out negative wall " << " dist= " << dist << " ==(" << (-pos[ii] + fMinV[ii]) << "/" << dir[ii] << ")" <<G4endl;
#endif
      } else if( io == 2 ) {
	if( fabs(dir[ii]) > PRECISION ) dist = (-pos[ii] + fMaxV[ii]) / dir[ii];
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) G4cout << "@@@@ FindValuesInStepsStr Distance to out " << " dist= " << dist<< " ==(" << (-pos[ii] + fMaxV[ii]) << "/" << dir[ii] << G4endl;
#endif
      }      
      distMax = std::max(distMax,dist);
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInStepsStr FINAL Distance to out " << distMax << G4endl;
#endif

    for( size_t ii=0; ii < 3; ii++ ){
      pos[ii] += dir[ii]*( distMax + PRECISION );
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInStepsStr pos in phantom border " << ii << " = " << pos << G4endl;
#endif
    }
  }
  //--- Check it is indeed in phantom border
  for( size_t ii=0; ii < 3; ii++ ){
    if( (pos[ii] - fMaxV[ii]) > PRECISION || (-pos[ii] + fMinV[ii]) > PRECISION ) {
      G4cerr << "@@@@ FindValuesInStepsStr pos " << " = " << pos << G4endl;
      G4cerr << "@@@@ FindValuesInStepsStr dir " << " = " << dir << G4endl;
      G4cerr << " IMAGESTR " << *imageStr << G4endl; 
      G4Exception("DicomLine::FindValuesInSteps",
		  "FindValuesInSteps",
		  JustWarning,
		  "line does not intersect phantom");
    }
  }
  
  //--- Get Voxel ID of initial position
  std::vector<G4int> voxelID = {0,0,0};
  //  std::vector<size_t> idMax;
  /*  for( size_t ii=0; ii < 3; ii++ ){
    voxelID[ii] = GmGenUtils::GetBelowInt((pos[ii]-fMinV[ii])/fVoxelDimV[ii]*2.+PRECISION);  //in half vxo
    G4cout << " HV " << voxelID[ii] << " = " << (pos[ii]-fMinV[ii])/fVoxelDimV[ii]*2. << (pos[ii]-fMinV[ii])/fVoxelDimV[ii] << " " << (pos[ii]-fMinV[ii]) << " / " << fVoxelDimV[ii] << G4endl; //GDEB
      //  G4int idMaxXNew = GmGenUtils::GetAboveInt((theImageMaxX-phMinX)/voxelFWidthX);
    //  G4int phNVoxelXNew = idMaxXNew - idMinXNew;
  }
  if( DicomVerb(infoVerb) ) G4cout << "@@@@ FindValuesInStepsStr 1st voxelID " << voxelID[0] << " " << voxelID[1] << " " << voxelID[2] << G4endl;
  */
  
  //--- FIND VOXEL OF NEXT POSITION
  mmdi dists;
  for( ;; ) {
    G4bool bEnded = false;
    std::vector<G4int> bPosXYZ; 
    for( size_t ii=0; ii < 3; ii++ ){
      voxelID[ii] = GmGenUtils::GetBelowInt((pos[ii]-fMinV[ii])/fVoxelDimV[ii]*2+PRECISION);
      if( voxelID[ii] < 0 || voxelID[ii] >= G4int(2*nVoxelV[ii]-1) ) bEnded = true; // line gets out of phantom
      if( voxelID[ii] == 0 ) {
	bPosXYZ.push_back(-1);
      } else if( voxelID[ii] == G4int(2*nVoxelV[ii]-1) ) {
	bPosXYZ.push_back(1);
      } else {
	bPosXYZ.push_back(0);
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) )  G4cout << bEnded << " HALF_VOXELID " << pos << " " << voxelID[0] << " " << voxelID[1] << " " << voxelID[2] << G4endl; //G
#endif
    if( bEnded ) break;

    //t treatment 1 and 2 corners separately, for the moment skip
    G4bool bNotNow = false;
    for( size_t ii=0; ii < 3; ii++ ){
      if( bPosXYZ[ii] != 0 ) {
	distAccum += step;
	pos += dir*step;
	bNotNow = true;
	G4cout << " SKIP NOW " << G4endl; //GDEB
	break;
      }
    } 
    if( bNotNow ) continue;
    
    // get value
    size_t copyNo = voxelID[0] + voxelID[1]*nVoxelV[0] + voxelID[2]*nVoxelV[0]*nVoxelV[1];
    G4String value = dataStr->at(copyNo); 
    G4double distHis = distAccum;
    intersValues[distHis] = value;
    distAccum += step;
    pos += dir*step;
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(testVerb) )  G4cout << " DicomLine FINAL VALUE " << value << " at " << pos << " dist= " << distHis << G4endl; //GDEB
#endif
   }

#ifndef GAMOS_NO_VERBOSE
  if(DicomVerb(debugVerb) ) G4cout << "@@@@ FindValuesInStepsStr ended N intersections: " << intersValues.size() << G4endl;
#endif

  return intersValues;

}

//-----------------------------------------------------------------------------
G4int DicomLine::CheckOutOfPhantom( G4ThreeVector pos, G4ThreeVector dir, std::vector<G4double> fMinV,  std::vector<G4double> fMaxV, std::vector<G4String> fAxisNameV )
{
  G4int isOut = 0;
  for( size_t ii=0; ii < 3; ii++ ){
    if( pos[ii] > fMaxV[ii] ) {
      if( dir[ii] >= 0 ) {
	G4Exception("DicomLine::CheckOutOfPhantom",
		    "",
		    JustWarning,
		    ("Position in "+ fAxisNameV[ii] + " is bigger than phantom limits but direction is not negative").c_str());
      }
      isOut += 2*std::pow(10,ii);
    } else if( pos[ii] < fMinV[ii]) {
      if( dir[ii] <= 0 ) {
	G4Exception("DicomLine::CheckOutOfPhantom",
		    "",
		    //		    FatalException,
		    JustWarning,
		    ("Position in "+ fAxisNameV[ii] + " is smaller than phantom limits but direction is not positive").c_str());
      }
      isOut += std::pow(10,ii);
    }
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(debugVerb) ) G4cout << "@@@@ CheckOutOfPhantom IsOut?= " << ii << " : " << isOut << "   : xyz 2 if positive border, 1 if negative border " << G4endl;  
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) {
    G4cout << "@@@@ CheckOutOfPhantom IsOut= " << isOut
	   << " phantom min " << fMinV[0] << " " << fMinV[1] << " " << fMinV[2] 
	   << " phantom max " << fMaxV[0] << " " << fMaxV[1] << " " << fMaxV[2] << G4endl;
  }
#endif

  return isOut;

}
=======
>>>>>>> 889849c0 (GAMOS.7.0 v1)
