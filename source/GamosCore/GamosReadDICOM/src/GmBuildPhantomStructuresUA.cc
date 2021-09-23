#include "G4Run.hh"
#include "G4Event.hh"

#include "GmReadDICOMVerbosity.hh"
#include "GmBuildPhantomStructuresUA.hh"
#include "Gm3ddoseHeader.hh"
#include "GmRegularParamUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4PVParameterised.hh"
#include "G4ExtrudedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4AffineTransform.hh"

//-----------------------------------------------------------------------
GmBuildPhantomStructuresUA::GmBuildPhantomStructuresUA()
{
}

//-----------------------------------------------------------------------
void GmBuildPhantomStructuresUA::BeginOfRunAction( const G4Run* )
{
  //--- Read structure file, with contours at each Z plane
  std::vector<G4String> fileNames;
  fileNames = GmParameterMgr::GetInstance()->GetVStringValue(theName+":FileNames",fileNames);
  if( fileNames.size() == 0 ) {
    G4Exception("GmBuildPhantomStructuresUA::BeginOfRunAction",
		"",
		FatalException,
		"No file name provided, please use command /gamos/setParam GmBuildPhantomStructuresUA:FileNames FILE_NAME_1 FILE_NAME_2 ...");
  }

  G4bool bEGS = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":IsEGS",1));

  std::vector<PStPlaneData*>* fPlanes;
  PStPlaneData* plane;
  std::vector<std::vector<G4double>* > fPointV;
  std::vector<G4double>* points;
  //  EAxis theAxis = kUndefined;
  //  EAxis theOldAxis = kUndefined;

  std::set<PStructureData*> thePSData;
  PStructureData* psData = 0;
  std::vector<G4String> wl;
  for( size_t ii = 0; ii < fileNames.size(); ii++ ) {
    GmFileIn fin = GmFileIn::GetInstance(fileNames[ii]);
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
    G4cout << " BPV  open file " << fileNames[ii] << G4endl;
#endif
    for(;;){
      if( !fin.GetWordsInLine(wl) ) break;
      if( wl[0] == "STRUCTURE" ) {
	if( wl.size() != 2 ) {
	  G4Exception("GmBuildPhantomStructuresUA::BeginOfRunAction",
		      "",
		      FatalException,
		      (G4String("Line starting with STRUCTURE may have only two words, while it has ") + GmGenUtils::itoa(wl.size())).c_str());
	}	
	psData = new PStructureData;
	psData->theName = wl[1];
	psData->theNumber = thePSData.size();	
	thePSData.insert(psData);
	fPlanes = new std::vector<PStPlaneData*>;
	psData->thePlanes = fPlanes;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	G4cout << " BPV new struct " << psData << " " << wl[1] << " NPL " << fPlanes->size() << G4endl;
#endif
      } else if( wl[0] == "ans" ) {
	//--- reading new plan
	if( fPointV.size() == 0 || fPointV.size() == 3 ) {
	  plane = new PStPlaneData;
	  fPlanes->push_back(plane);
	  fPointV.clear();
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	  G4cout << " BPV ans new plane, plsize " << fPlanes->size() << G4endl;
#endif
	}
	points = new std::vector<G4double>;
	fPointV.push_back(points);
	if( fPointV.size() == 3 ) { // fill old plane
	  plane->thePointV = fPointV;
	}
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(debugVerb) ) 
	  G4cout << " BPV ans psize " << points->size() << G4endl;
#endif
      } else if( wl[0] == "REPEATZ" ) {
#ifndef GAMOS_NO_VERBOSE
	if( ReadDICOMVerb(debugVerb) ) 
	  G4cout << " start REPEATZ " << wl[1] << " " << wl[2] << G4endl;
#endif
	G4double Zorig = GmGenUtils::GetValue(wl[1]);
	G4double Znew = GmGenUtils::GetValue(wl[2]);
	G4int nPlanes = fPlanes->size();
	for( G4int ii2 = 0; ii2 < nPlanes; ii2++ ){
	  G4double Z = (*((*fPlanes)[ii2]->thePointV[2]))[0]; // first value of third set of points
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	  G4cout << ii2 << " check Z " << Z << G4endl;
#endif
	  if( Z == Zorig ) {
	    plane = new PStPlaneData;
	    fPlanes->push_back(plane);
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	    G4cout << ii2 << " REPEATZ " << Z << " nPlanes " << fPlanes->size() << G4endl;
#endif
	    std::vector< std::vector<G4double>* > pointVnew;
	    pointVnew.push_back( (*fPlanes)[ii2]->thePointV[0] ); 
	    pointVnew.push_back( (*fPlanes)[ii2]->thePointV[1] ); 
	    std::vector<G4double>* pointsZ = new std::vector<G4double>;
	    size_t nPoints = pointVnew[0]->size();
	    for( size_t ip = 0; ip < nPoints; ip++ ){
	      pointsZ->push_back(Znew);
	    } 
	    pointVnew.push_back(pointsZ);
	    plane->thePointV = pointVnew;
	  } 
	}

      } else {
	// reading data
	for( size_t ii2 = 0; ii2 < wl.size(); ii2++ ){
	  points->push_back(GmGenUtils::GetValue(wl[ii2]));
	}
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	G4cout << wl.size() << " BPV FILL DATA " << points->size() << G4endl;
#endif

      }

    }

#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
    G4cout << " psData ended nPlanes " << psData->thePlanes->size() << G4endl;
#endif
  }

  /*  //--- Now interpolate planes backward
  for( iteps = thePSData.begin(); iteps != thePSData.end(); iteps++ ) {
    PStructureData* psData = *iteps;
    std::vector<PStPlaneData*>* fPlanes = psData->thePlanes;
    G4int nPlanes = fPlanes->size();
    G4cout << psData->theName << " NPLANES " << nPlanes << G4endl;
    //order planes by z
    std::map<G4double,PStPlaneData*> planeMap;
    for( G4int ii = 0; ii < nPlanes; ii++ ){
      G4double Z = (*((*fPlanes)[ii]->thePointV[2]))[0]; // first value of third set of points
      planeMap[Z] = (*fPlanes)[ii];
    }
  }
  std::vector<PStPlaneData*>* fPlanesNew = new std::vector<PStPlaneData*>;
  G4double oldZ = (*((*fPlanes)[0]->thePointV[2]))[0]; 
  G4double diffZ = (*((*fPlanes)[1]->thePointV[2]))[0] - (*((*fPlanes)[0]->thePointV[2]))[0]; 
  std::map<G4double,PStPlaneData*>::iterator itep;
  for( itep = planeMap.begin(); itep != planeMap.end(); itep++ ) {
    G4double Z = (*((*fPlanes)[1]->thePointV[2]))[0] - (*((*fPlanes)[0]->thePointV[2]))[0]; 
    
  }
  }
  }*/


  //----- Find phantom parameterisation
  G4bool phantomRegular = FALSE;
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  theRegularParam = 0;
  G4AffineTransform thePhantomTransform;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
      G4cout << " PV " << (*cite)->GetName() << " " << (*cite)->GetTranslation() << G4endl;
#endif
    theRegularParam = GmRegularParamUtils::GetInstance()->GetPhantomParam( *cite, FALSE );
    if( theRegularParam != 0 ){
      if( phantomRegular ) G4Exception("GmBuildPhantomStructuresUA::BeginOfRunAction",
				       "Error",
				       FatalException,
				       "Two G4PhantomParameterisation's found ");
      phantomRegular = TRUE;
      thePhantomStructure = *cite;
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) 
	G4cout << thePhantomStructure->GetName() << " PV TRANS " << thePhantomStructure->GetTranslation() << G4endl;
#endif
      std::vector<G4VPhysicalVolume*> pvsv = GmGeometryUtils::GetInstance()->GetPhysicalVolumes( thePhantomStructure->GetMotherLogical()->GetName() );
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
      G4cout << pvsv[0]->GetName() << " PARENT PV TRANS " << pvsv[0]->GetTranslation() << G4endl;
#endif
      thePhantomTransform = G4AffineTransform( pvsv[0]->GetRotation(), pvsv[0]->GetTranslation());
    }
  }
  if( !phantomRegular ) G4Exception("GmBuildPhantomStructuresUA::BeginOfRunAction",
				    "Error",
				    FatalException,
				    "No G4PhantomParameterisation found ");

  //--- Loop to structures
  G4ExtrudedSolid* theExtrudedSolid;
  std::set<PStructureData*>::const_iterator iteps;
  G4int nVoxX = theRegularParam->GetNoVoxelsX();
  G4int nVoxY = theRegularParam->GetNoVoxelsY();
  G4int nVoxZ = theRegularParam->GetNoVoxelsZ();
  G4double pMinZ = theRegularParam->GetTranslation(0).z()-theRegularParam->GetVoxelHalfZ();
  G4double pMaxZ = theRegularParam->GetTranslation(nVoxX*nVoxY*nVoxZ-1).z()+theRegularParam->GetVoxelHalfZ();
  G4double voxHX = theRegularParam->GetVoxelHalfX();
  G4double voxHY = theRegularParam->GetVoxelHalfY();
  G4double voxHZ = theRegularParam->GetVoxelHalfZ(); 
  G4ThreeVector theCornerMin( -nVoxX * voxHX, -nVoxY * voxHY, -nVoxZ * voxHZ );
  G4ThreeVector theCornerMax( -theCornerMin );
  thePhantomTransform.ApplyPointTransform(theCornerMin);
  thePhantomTransform.ApplyPointTransform(theCornerMax);
  
  //- G4cout << " PHANTOM LIMITS " << pMinX << " " << pMaxX << " Y " << pMinY << " " << pMaxY << " Z " << pMinZ << " " << pMaxZ << G4endl;
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) {
    G4cout << " PHANTOM CORNERS " << theCornerMin << " " << theCornerMax << G4endl;
    G4cout << " PHANTOM VOXEL HALF SIZES " << voxHX << " " << voxHY << " " << voxHZ << G4endl;
    G4cout << " PHANTOM N VOXELS " <<nVoxX << " " << nVoxY << " " << nVoxZ << G4endl;
  }
#endif
  
  std::map<G4int,G4int> theIDStruct;
  //  G4cout << " PSDATA " << thePSData.size() << G4endl;
  for( iteps = thePSData.begin(); iteps != thePSData.end(); iteps++ ) {
    PStructureData* psData2 = *iteps;
    fPlanes = psData2->thePlanes;
    G4int nPlanes = fPlanes->size();
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
      G4cout << psData2->theName << " NPLANES " << nPlanes << G4endl;
#endif
    for( G4int ii = 0; ii < nPlanes; ii++ ){
      PStPlaneData* plane2 = (*fPlanes)[ii];
      std::vector<G4double>* xPoints = plane2->thePointV[0];
      std::vector<G4double>* yPoints = plane2->thePointV[1];
      std::vector<G4double>* zPoints = plane2->thePointV[2];
      //--- Build G4ExtrudedSolid
      std::vector<G4TwoVector> polygon;
      std::vector<G4ExtrudedSolid::ZSection> zsections;
      for( size_t jj = 0; jj < xPoints->size(); jj++ ){
	G4TwoVector twovec((*xPoints)[jj], (*yPoints)[jj]);
	G4bool bRepeated = FALSE;
	for( size_t ip = 0; ip < polygon.size(); ip++ ) {
	  //	  G4cout << " TEST " << twovec << " " << polygon[ip] << " " <<  twovec == polygon[ip] << G4endl;
	  if( twovec == polygon[ip] ) {
	    bRepeated = TRUE;
	    break;
	  }
	}
	if( !bRepeated ) {
	  polygon.push_back( twovec );
	}
      }
      //      zsections.push_back( G4ExtrudedSolid::ZSection( -fabs((*zPoints)[0]), G4TwoVector(), 1) ); // all z are the same in a plane
      //      zsections.push_back( G4ExtrudedSolid::ZSection( fabs((*zPoints)[0]), G4TwoVector(), 1) ); // all z are the same in a plane
      G4double z1 = -fabs((*zPoints)[0]);
      G4double z2 = fabs((*zPoints)[0]);
      if( z1 < pMinZ ) {
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	G4cout << " ZSECTIONS Z1 RECALCULATED " << z1 << " -> " <<  z2-3*voxHZ << " MINZ " << pMinZ << G4endl;
#endif
	z1 = z2-3*voxHZ;
      }
      if( z2 > pMaxZ ) {
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
      G4cout << " ZSECTIONS Z1 RECALCULATED " << z2 << " -> " <<  z1+3*voxHZ << " MAXZ " << pMaxZ << G4endl;
#endif
	z2 = z1+3*voxHZ;
      }
      zsections.push_back( G4ExtrudedSolid::ZSection( z1, G4TwoVector(), 1) ); // all z are the same in a plane
      zsections.push_back( G4ExtrudedSolid::ZSection( z2, G4TwoVector(), 1) ); // all z are the same in a plane
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
      G4cout << " ZSECTIONS " << z1 << " " << z2 << G4endl;
#endif
      theExtrudedSolid = new G4ExtrudedSolid( psData2->theName + "_" + GmGenUtils::itoa(ii), polygon, zsections);
      theExtrudedSolid->DumpInfo();

      //--- Find for this structure (extruded solid) which voxels are inside 
      //--- First find Z
      G4double zStruct = (*zPoints)[0]; // all have same Z
      G4int iz = G4int( ( zStruct - thePhantomTransform.NetTranslation().z()-pMinZ ) / (2*voxHZ) );
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
      G4cout << " IZ " << iz << " zStruct " << zStruct << " pMinZ " << pMinZ << " 2*voxHZ " << 2*voxHZ << G4endl;
#endif
      for( G4int ix = 0; ix < nVoxX; ix++ ) {
	for( G4int iy = 0; iy < nVoxY; iy++ ) {
	  G4int copyNo = iz*nVoxX*nVoxY + iy*nVoxX + ix;
	  //find if any of the four corners is inside extruded solid (= inside structure)
	  G4ThreeVector trans = theRegularParam->GetTranslation(copyNo);
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	  G4cout << " CHECK COPYNO " << copyNo << " trans " << trans << G4endl;
#endif
	  thePhantomTransform.ApplyPointTransform(trans);
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
	  G4cout << " CHECK COPYNO TRANFORMED " << copyNo << " trans " << trans << G4endl;
#endif
	  G4double transZ = trans.z();
	  trans = G4ThreeVector( trans.x(), trans.y(), 0 );
	  G4bool isInside = FALSE;
	  for( G4double xo = -voxHX; xo < voxHX; xo+=2*voxHX){
	    for( G4double yo = -voxHY; yo < voxHY; yo+=2*voxHY){
	      G4ThreeVector corner = trans + G4ThreeVector(xo,yo);
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) 
      G4cout << xo << " " << yo << " CHECK CORNER " << corner << G4endl;
#endif
	      if( theExtrudedSolid->Inside(corner) == kInside ) {
		isInside = TRUE;
#ifndef GAMOS_NO_VERBOSE
		if( ReadDICOMVerb(debugVerb) ) 
		  G4cout << copyNo << " " << ix << " " << iy << " " << iz << " corner inside " << corner << " VOXEL CENTRE " << trans.x() << "," << trans.y() << "," << transZ << G4endl;
#endif

		break;
	      }
	    }
	  }
	  if( isInside ) theIDStruct[copyNo] = psData2->theNumber;
	}
      }
    }

  }

  //----- Dump IDstruct to file
  G4String fileNameOut = GmParameterMgr::GetInstance()->GetStringValue("GmBuildPhantomStructuresUA:FileNameOut","struct_out.egsphant");
  std::ofstream fout(fileNameOut);
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) 
    G4cout << " theIDStruct " << theIDStruct.size() << G4endl;
#endif
  G4int nVox = theRegularParam->GetNoVoxels();
  for( int ii = 0; ii < nVox; ii++ ) {
    std::map<G4int,G4int>::const_iterator ites = theIDStruct.find(ii);
    if( ites == theIDStruct.end() ) {
      fout << "-1";
    } else {
      G4int structId = (*ites).second;
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) 
	G4cout << ii << " STRUCTID PRINT " << structId << G4endl;
#endif
      if( bEGS ) {
	if( structId < 10 ) {
	  fout << "0"; // print 01 instead of 1
	}
      }
      fout << structId;
    }
    if( !bEGS ) fout << " ";
    if( ii%nVoxX == nVoxX-1 ) fout << G4endl;
  }
  for( iteps = thePSData.begin(); iteps != thePSData.end(); iteps++ ) {
    fout << (*iteps)->theNumber << " " << (*iteps)->theName << G4endl;
  }
    
}
