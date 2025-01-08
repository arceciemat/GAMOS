#include "DicomPolygonList.hh"
#include "DicomPolygon.hh"
#include "DicomVImageStr.hh"
#include "DicomVerbosity.hh"
#include "DicomParameterMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------------
DicomPolygonList::DicomPolygonList(G4String name, size_t num, DPOrientation ori )
  : DicomVLineList(name, num, ori)
{
  bVoxelsInUseClosest = DicomParameterMgr::GetInstance()->GetNumericValue("closestPolygon",1);
  bOrderedLineListBuilt = false;
  if( DicomVerb(infoVerb) ) G4cout << " DicomPolygonList::DicomPolygonList " << theName << " " << num << G4endl; 
}

//-----------------------------------------------------------------------------
DicomPolygonList::DicomPolygonList(DicomPolygonList* polyListOld, DicomVImage* image, DPOrientation ori ) : DicomVLineList(ori)
{
  bOrderedLineListBuilt = false;
  if( polyListOld->GetLines().size() == 0 ) return;
  
  std::map<DicomVLine*,G4bool> linesUsed;
  
  SetName( polyListOld->GetName() );
  SetNumber( polyListOld->GetNumber() );
  if( DicomVerb(infoVerb) ) G4cout << " DicomPolygonList::DicomPolygonList " << theName << " " << theNumber << G4endl; 
  
  bVoxelsInUseClosest = DicomParameterMgr::GetInstance()->GetNumericValue("closestPolygon",1);
  if( !bVoxelsInUseClosest ) {
    G4Exception("DicomPolygonList::DicomPolygonList (DicomPolygonList* polyListOld, DicomVImage* image ) ",
		"",
		FatalException,
		"This should not be called if parameter closestPolygon is not 1");
  }
  
  mmddp posOrderedLines = polyListOld->BuildPosOrderedLines();  
  pmmddpi polygonsInPlane;
  pmmddpi pitet;
  //  G4cout << " DicomPolygonList bVoxelsInUseClosest " << bVoxelsInUseClosest << " Old nLines " << polyListOld->GetLines().size() << " " << polyListOld->GetName() << G4endl; //GDEB
  if( DicomVerb(debugVerb) ) {
    G4cout << "DicomPolygonList bVoxelsInUseClosest N " << posOrderedLines.size() << G4endl;
  }
  // line_range=line+-1/2*distance_between_lines
  G4double imMinZ = image->GetMinZ();
  G4double imMaxZ = image->GetMaxZ();
  //  G4double imWidthZ = image->GetVoxelDimZ();

  G4double line_range = 0;
  mmddp::const_iterator iteDOWN = posOrderedLines.begin();
  mmddp::const_iterator iteUP = posOrderedLines.end(); iteUP--;
  if( DicomVerb(debugVerb) ) {
    G4cout << imMinZ << " " << theName << " posOrderedLines BOTTOM " << iteDOWN->first << " TOP " << iteUP->first << G4endl;
  }
  G4bool bOnePolygonZ = false;
  pitet = posOrderedLines.equal_range((*iteDOWN).first);
  if( pitet.first->first == pitet.second->first ) bOnePolygonZ = true;
  // all polygons in one unique Z
  // lines are all below image: check if adding line_range is in
  if( iteUP->first < imMinZ ) {
    //    G4cout << " iteUP->first < imMinZ " << iteUP->first << " " << imMinZ << G4endl; //GDEB
    if( bOnePolygonZ ) { // all polygons in one unique Z
      std::vector<DicomVLine*> lines = polyListOld->GetLines();
      for( size_t il = 0; il < lines.size(); il++ ) {
	DicomVLine* newPoly = lines[il];
	linesUsed[newPoly] = true;
	AddLine( newPoly );
      }
      return;
    } else {
      pitet = posOrderedLines.equal_range((*iteUP).first);
      mmddpi ite1 = pitet.first; ite1--;
      line_range = (ite1->first-pitet.first->first)/2.; // check adding 1/2 line separations
      if( iteUP->first+line_range < imMinZ ) {
	//	G4cout << " All lines have Z below min " << imMinZ << G4endl; //GDEB 
	return;
      } else {
	polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	  DicomVLine* newPoly = AddDisplacedLine( (*item).second, line_range );
	  linesUsed[newPoly] = true;
	}
	// FindVoxelsInXY(image,polygonsInPlane, line_range);
	//	  (*iteUP).second->FindVoxelsInXY(image,+line_range);
      }
    }
    // lines are all above image: check if subtracting line_range is in
  } else if( iteDOWN->first > imMaxZ ) {
    //    G4cout << " iteDOWN->first > imMaxZ " << iteDOWN->first << " " << imMaxZ << G4endl; //GDEB
    if( bOnePolygonZ ) { // all polygons in one unique Z
      std::vector<DicomVLine*> lines = polyListOld->GetLines();
      for( size_t il = 0; il < lines.size(); il++ ) {
	DicomVLine* newPoly = lines[il];
	linesUsed[newPoly] = true;
	AddLine( newPoly );
      }
      return;

    } else {
      pitet = posOrderedLines.equal_range((*iteDOWN).first);
      mmddpi ite1 = pitet.second; ite1++; 
      line_range = -(pitet.first->first-ite1->first)/2.;// check adding -1/2 line separations
      if( iteDOWN->first-line_range > imMaxZ ) {
	return;
      } else {	
	polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	  DicomVLine* newPoly = AddDisplacedLine( (*item).second, line_range );
	  linesUsed[newPoly] = true;
	}
	//FindVoxelsInXY(image,polygonsInPlane, line_range);
	// (*iteDOWN).second->FindVoxelsInXY(image,-line_range);
      }
    } 
  } else {
    //    G4cout << " INSIDEDicomPolygonList bOnePolygonZ " << bOnePolygonZ << G4endl; //GDEB
    if( bOnePolygonZ ) { // all polygons in one unique Z
      std::vector<DicomVLine*> lines = polyListOld->GetLines();
      for( size_t il = 0; il < lines.size(); il++ ) {
	DicomVLine* newPoly = lines[il];
	linesUsed[newPoly] = true;
	AddLine( newPoly );
      }
      return;
      // (*iteUP).second->FindVoxelsInXY(image);
    } else {
      std::multimap<G4int,DicomVLine*> iZPlanes;
      // List Z planes that each line is in
      std::vector<DicomVLine*> linesOld = polyListOld->GetLines();
      //      G4cout << theName << " lineName " << polyListOld->GetName() << " linesOld " << linesOld.size() << G4endl; //GDEB
      for( size_t ii = 0; ii < linesOld.size(); ii++ ) {
	DicomPolygon* line = dynamic_cast<DicomPolygon*>(linesOld[ii]);
	G4double planeZ = line->GetPlanePosition();
	G4int iPolygonZ = GetPolygonZIndex(image, planeZ);
	if (iPolygonZ < 0 || iPolygonZ >= G4int(image->GetNoVoxelsZ()) ) continue;
	iZPlanes.insert(std::multimap<G4int,DicomVLine*>::value_type(iPolygonZ,line));
	//	G4cout << GetName() << " DicomPolygonList::DicomPolygonList( interpolate add iZPlanes " << iPolygonZ << " " << planeZ << G4endl; //GDEB
      }
      if( iZPlanes.size() == 0 ) return;
      // get smaller and bigger index of > plane
      std::multimap<G4int,DicomVLine*>::iterator itezp = iZPlanes.begin();
      G4int iSmallerZ = itezp->first;
      if( iSmallerZ < 0 ) iSmallerZ = 0; // using only a fraction of the CT image
      std::multimap<G4int,DicomVLine*>::reverse_iterator ritezp = iZPlanes.rbegin();
      G4int iBiggerZ = ritezp->first;
      G4double imVoxelDimZ = image->GetVoxelDimZ();
      //      G4cout << " DicomPolygonList::DicomPolygonList( interpolate iSmallerZ " << iSmallerZ << " iBiggerZ " << iBiggerZ <<" imMinZ " << imMinZ << " imVoxelDimZ " << imVoxelDimZ << G4endl; //GDEB
      for( G4int iz = iSmallerZ; iz <= iBiggerZ; iz++ ) {
	auto itePair = iZPlanes.equal_range(iz);
	if( itePair.first != iZPlanes.end() ) {
	  for( std::multimap<G4int,DicomVLine*>::iterator itezp1 = itePair.first; itezp1 != itePair.second; itezp1++ ) {
	    //    G4cout << " DicomPolygonList::DicomPolygonList( interpolate find plane " << iz << G4endl; //GDEB
	    DicomPolygon* line = dynamic_cast<DicomPolygon*>(itezp1->second);
	    DicomVLine* lineNew = AddDisplacedLine( line, 0. );
	    linesUsed[lineNew] = true;
	    //	    G4cout << theName << " " << iz << " DicomPolygonList::DicomPolygonList( interpolate FOUND plane " << iz << " " << line->GetPoints()[0].z() << " NLINE " << theLines.size() << G4endl; //GDEB
	  }
	} else {
	  // get closer line
	  G4double planeZ = imMinZ+(iz+0.5)*imVoxelDimZ;
	  G4double minZdist = DBL_MAX;
	  G4int iMinZdist = -1;
	  for( std::multimap<G4int,DicomVLine*>::iterator itezp1 = iZPlanes.begin(); itezp1 != iZPlanes.end(); itezp1++ ) {
	    G4double lineZ = itezp1->second->GetPlanePosition();;
	    if( fabs(planeZ-lineZ) < minZdist ) {
	      minZdist =  fabs(planeZ-lineZ);
	      iMinZdist = itezp1->first;
	    }
	  }
	  auto itezp2 =iZPlanes.equal_range(iMinZdist);
	  for (auto ite = itezp2.first; ite != itezp2.second; ++ite) {
	    DicomPolygon* line = dynamic_cast<DicomPolygon*>(ite->second);
	    std::vector<G4ThreeVector> points = line->GetPoints();
	    G4double plusZ = 0.;
	    if( points.size() > 0 ) plusZ = planeZ-points[0].z();
	    //      DicomVLine* lineNew =
	    AddDisplacedLine( line, plusZ );
	    //	    G4cout << theName << " DicomPolygonList::DicomPolygonList( interpolate NOT FOUND plane " << iz << " ptsZ " << points[0].z() << " planeZ " << planeZ << " minZdist " << minZdist << " iMinZdist " <<  iMinZdist << " NLINE " << theLines.size() << G4endl; //GDEB
	  }
	}
      }
      
      // If 2*line_range > voxelBinWidth: a voxel lower edge and upper edge will have the same lineList as upper
      // If 2*line_range < voxelBinWidth: two or more polygons are in the same voxel: take the one closest to the voxel center
      // loop to all voxels from the ones of the minimum polygon to the maximum one
      /*  G4int iPolygonZMin = std::max(iteDOWN->second->GetPolygonZIndex( image ),0);
      G4int iPolygonZMax = std::min(iteUP->second->GetPolygonZIndex( image ),G4int(image->GetNoVoxelsZ())-1);
      if( DicomVerb(debugVerb) ) {	
	G4cout << " DicomPolygonList  iPolygonZMin/Max " << iPolygonZMin << " " << iPolygonZMax << G4endl;
      }
      mmddp::const_iterator mite;
      for( int ipz = iPolygonZMin; ipz <= iPolygonZMax; ipz++ ) {	
	line_range = 0.;
	G4double poszUP = imMinZ+(ipz+1)*imWidthZ; // upper position of voxels in Z plane
	iteUP = posOrderedLines.upper_bound(poszUP);
	if( DicomVerb(debugVerb) ) {
	  G4cout << iteUP->first << " FROM poszUP " << poszUP << " = " << imMinZ << " + " << (ipz+1) << " * " << imWidthZ << G4endl;
	}
	G4double poszDOWN = imMinZ+ipz*imWidthZ; // lower position of voxels in Z plane
	iteDOWN = posOrderedLines.upper_bound(poszDOWN);
	if( DicomVerb(debugVerb) ) {
	  G4cout << iteDOWN->first << " FROM poszDOWN " << poszDOWN << " = " << imMinZ << " + " << ipz << " * " << imWidthZ << G4endl;
	}
	std::set<G4double> positions; // how many different Z pos in this Z plane
	for( mite = iteDOWN; mite != iteUP; mite++ )  {
	  positions.insert((*mite).first);
	  if( DicomVerb(debugVerb) ) {
	    G4cout << " INSERT POSITION " << (*mite).first << G4endl;
	  }
	}
	positions.insert((*mite).first); // iteUP
	
	G4int distance = positions.size()-1;
	//	G4cout << "DicomPolygonList  distance " << distance << G4endl; //GDEB
	if( distance == 0 ) { // if upper_bound of poszDOWN and poszUP is equal, both are above
	  // No polygon: use the one above or the one below (the one that is closest to the voxel boundaries) subtracting/adding line_range
	  pitet = posOrderedLines.equal_range((*iteUP).first);
	  //	  mmddpi ite1 = pitet.first;  ite1--; // the one before the first has Z smaller
	  //	  line_range = (pitet.first->first-ite1->first)/2.;
	  G4double dist2up = iteUP->first - poszUP;
	  iteDOWN = pitet.first; iteDOWN--; // polygon below poszDOWN
	  G4double dist2down = poszDOWN - iteDOWN->first;
	  line_range = ((*iteUP).first-(*iteDOWN).first)/2.;
	  if( DicomVerb(debugVerb) ) {
	    G4cout << " DISTANCE 0  UP: " << dist2up << " = " << iteUP->first << " - " << poszUP << " DOWN: " << dist2down << " = " << poszDOWN << " -  " << iteDOWN->first << " line_range " << line_range << G4endl;
	  }
	  if(dist2up <= dist2down) {
	    polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	    line_range *= -1;
	    for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	      DicomVLine* newPoly = AddDisplacedLine( (*item).second, +line_range );
	      linesUsed[newPoly] = true;
	    }	  
	  } else {
	    polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	    for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	      DicomVLine* newPoly = AddDisplacedLine( (*item).second, +line_range );
	      linesUsed[newPoly] = true;
	    }	  
	    //	    FindVoxelsInXY(image,polygonsInPlane, line_range);
	    //	      line_range *= +1;
	  }
	  //-- loop to all Polygon's with same Z
	} else if( distance == 1 ) {
	  // OK: loop to all Polygon's with  Z = the one below
	  polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	  for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	    DicomVLine* newPoly = AddDisplacedLine( (*item).second, +line_range );
	    linesUsed[newPoly] = true;
	  }	  
	  //	  FindVoxelsInXY(image,polygonsInPlane, line_range);
	  //pmite = posOrderedLines.equal_range((*iteDOWN).first);
	  if( DicomVerb(debugVerb) ) {
	    G4cout << " DISTANCE 1  USE DOWN : " << iteDOWN->first << G4endl;
	  }
	} else {
	  // find the one closest to voxel center
	  G4double poszCENTRE = imMinZ+(ipz+0.5)*imWidthZ;
	  std::map<G4double, DicomPolygon*>::const_iterator iteoC;
	  std::map<G4double, DicomPolygon*>::const_iterator iteoClosest;
	  G4double distMin = DBL_MAX; 
	  for( iteoC = iteDOWN; iteoC != iteUP; iteoC++ ) {
	    G4double dist = fabs( (*iteoC).first - poszCENTRE );
	    if( DicomVerb(debugVerb) ) {
	      G4cout << " DISTANCE >1  DIST " << dist << " = fabs " << (*iteoC).first << " - " << poszCENTRE << G4endl;
	    }
	    if( dist < distMin ) {
	      dist = distMin;
	      iteoClosest = iteoC;
	    }
	  }
	  polygonsInPlane = posOrderedLines.equal_range(iteoClosest->first);
	  for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	    DicomVLine* newPoly = AddDisplacedLine( (*item).second, +line_range );
	    linesUsed[newPoly] = true;
	  }	  
	  //	  FindVoxelsInXY(image,polygonsInPlane, line_range);
	  }
      }
    */
    }
  }

  /*  // Delete lines that have not been used to intersect image Z plane
  std::vector<DicomVLine*>::reverse_iterator itel = theLines.rbegin();
  G4cout << " linesUsed N lines " << theLines.size() << G4endl; //GDEB
  for( ; itel != theLines.rend(); itel++ ) {
    DicomVLine* line = *itel;      
    if( linesUsed.find(line) == linesUsed.end() ) {
      G4cout << line << " " << GetName() << " linesUsed NOT, delete " << line->GetPoints()[0] << G4endl; //GDEB
      theLines.erase( itel.base() );
      delete *itel;
    }
    } */ 
  
}


//-----------------------------------------------------------------------------
mmddp DicomPolygonList::BuildPosOrderedLines()
{
  //built list of polygons ordered by position
  if( DicomVerb(debugVerb) ) {
    G4cout << this << " " << GetName() << " build thePosOrderedLines " << bOrderedLineListBuilt << " " << theLines.size() << G4endl;
  }
  if( bOrderedLineListBuilt == 0) {
    for( size_t ii = 0; ii < theLines.size(); ii++ ) {
      DicomVLine* line = theLines[ii];
      G4double planePos = line->GetPlanePosition();
      /*      if( thePosOrderedLines.find(planePos) != thePosOrderedLines.end() ) {
	G4cerr << "PolygonList: " << theName << " " << planePos << G4endl;
	G4Exception("DicomPolygonList::BuildPosOrderedLines",
		    "",
		    FatalException,
		    ("Two lines with the same plane position " + GetName()+":"+line->GetName()).c_str());
		    } */ // possible: struct split in two CHECK
      thePosOrderedLines.insert(mmddp::value_type(planePos,static_cast<DicomPolygon*>(line)));
    }
    bOrderedLineListBuilt = true;

  }
  if( DicomVerb(debugVerb) ) {
    G4cout << this << " Built thePosOrderedLines " << thePosOrderedLines.size() << G4endl;
    for( mmddp::const_iterator ite = thePosOrderedLines.begin(); ite != thePosOrderedLines.end(); ite++ ) {
      G4cout << GetName() << " : " << (*ite).second->GetName() <<  " thePosOrderedLines " << (*ite).first << " " << G4endl; //GDEG
    }
  }

  return thePosOrderedLines;
}


//-----------------------------------------------------------------------------
void DicomPolygonList::FindVoxelsInXY( DicomVImageStr* imageStr )
{
  // interpolate lines to cover all intermediate image Z planes
  // get smaller and bigger Z
  //  G4cout << " DicomPolygonList::FindVoxelsInXY( interpolate " <<  GetName() << " N= " << theLines.size() << G4endl; //GDEB
    
  BuildPosOrderedLines();
  if( DicomVerb(debugVerb) ) {
    G4cout << GetName() << " DicomPolygonList::FindVoxelsInXY  thePosOrderedLines N " << thePosOrderedLines.size() << G4endl;
  }

  for( mmddpi ite = thePosOrderedLines.begin(); ite != thePosOrderedLines.end(); ite++ ) {
    pmmddpi polygonsInPlane = thePosOrderedLines.equal_range((*ite).first);
    if( DicomVerb(infoVerb) ) {
      G4cout << GetName() << " DicomPolygonList::FindVoxelsInXY " << (*ite).first << " polygonsInPlane N " << std::distance(polygonsInPlane.first,polygonsInPlane.second)   << G4endl;
    }
    FindVoxelsInXY(imageStr,polygonsInPlane);
  }
  
}


//-----------------------------------------------------------------------------
void DicomPolygonList::FindVoxelsInXY( DicomVImageStr* imageStr, pmmddpi polygonsInPlane)
{
  if( DicomVerb(infoVerb) ) {
    G4cout << theName << " : " << theNumber << " CALLED FindVoxelsInXY  N LINES= " << std::distance(polygonsInPlane.first,polygonsInPlane.second) << " Z= " << polygonsInPlane.first->second->GetPoints()[0].z() << G4endl;
  }
  // DicomVerb.SetFilterLevel( testVerb ); //GDEB
  G4double planeZ = polygonsInPlane.first->second->GetPoints()[0].z();
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomPolygonList::FindVoxelsInXY in image " << imageStr->GetName() << " " << DicomVImage::GetModalityStr(imageStr->GetModality()) << " polygon_name " << theName <<  " from " << GetName() << G4endl;
  double PRECISION = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
  
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
  G4int iPolygonZ = GetPolygonZIndex(imageStr, planeZ);
  if( iPolygonZ < 0 || iPolygonZ >= G4int(imNoVoxelsZ) ) {
    return;
  }

  if( DicomVerb(debugVerb) ) G4cout << " INTERS CONTOUR WITH Z SLICE " << planeZ << " zMin " << imMinZ << " zMax " << imMinZ+imVoxelDimZ*imageStr->GetNoVoxelsZ() << " iPolygonZ " <<iPolygonZ << G4endl; 

  //--- Set min/max extensions
  double minXc = DBL_MAX;
  double maxXc = -DBL_MAX;
  double minYc = DBL_MAX;
  double maxYc = -DBL_MAX;
  for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
    DicomPolygon* polygon = item->second;
    std::vector<G4ThreeVector> thePoints = polygon->GetPoints();
    //	  std::vector<G4ThreeVector> theDirections = pÃolygone>GetDirections();
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
      G4Exception("DicomPolygonList::FindVoxelsInXY",
		  "",
		  JustWarning,
			  "Contour limits exceed Z slice extent");
    }
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
  //  G4bool bPrint = false; //GDEB
  //  if( DicomVerb(debugVerb) ) G4cout << " LOOPING POLYGON " << theName << " from " << theLineList->GetName() << G4endl; 
  for( int ix = idMinX; ix <= idMaxX; ix++ ) {
    for( int iy = idMinY; iy <= idMaxY; iy++ ) {
      /*	 G4ThreeVector pos = imageStr->GetPosition(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY);
	 G4double posZ = pos.z(); //GDEB
	 G4double posY = pos.y(); //GDEB
	 if( posZ < -867 && posZ > -872 && posY < -120 && posY > -122 ) {
	   bPrint = true;
	   G4cout << " DicomVerb.SetFilterLevel test "<< G4endl; //GDEB
	   DicomVerb.SetFilterLevel( testVerb );
	 } else {
	   DicomVerb.SetFilterLevel( warningVerb );
	   bPrint = false;
	 }
      */
      
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

	//--- Loop to polygons
	for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	  DicomPolygon* polygon = item->second;
	  std::vector<G4ThreeVector> thePoints = polygon->GetPoints();
	  std::vector<G4ThreeVector> theDirections = polygon->GetDirections();
	 if( ix == idMinX ) {
	   //	   for( size_t ll = 0; ll < thePoints.size()-1; ll++ ){ // NO last point, as it is the same as first point => direction=0
	   /*	   for( size_t ll = 0; ll < 5; ll++ ){ // NO last point, as it is the same as first point => direction=0
	     G4cout << ll << " POINTS= " << thePoints[ll] << G4endl; //GDEB
	     } */
	    //	 G4cout << " 00posZ " << posZ << " posY " << posY << G4endl; //GDEB
	 }
	    
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
	  
	} // end polygon loop
	
	if( distInters.size() % 2 == 1 ) { // search for odd number of segment intersections with polygonsInPlane
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
	  if( DicomVerb(debugVerb) ) G4cout << "!! CONTRADICTORY RESULT AFTER LOOP " << " (" << imMinX + imVoxelDimX*(ix+0.5) << "," << imMinY + imVoxelDimY*(iy+0.5) << "," << planeZ << ")" << loopResult << " || " << iloop << " == " << NLoopMax+1 << G4endl; 
	}
      } //ENDED the NLOOPS 
      if( loopResult > 0 )  {
 	if( DicomVerb(debugVerb) ) G4cout << "@@@@@ CENTRE OK  for Polygon " << GetName() << " " << ix << " " << iy << " (" << imMinX + imVoxelDimX*(ix+0.5) << "," << imMinY + imVoxelDimY*(iy+0.5) << "," << planeZ << ")" << G4endl;
	// extract previous ID value
	G4String roiStr = imageStr->GetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY);
	//	G4cout << " fStructure " << ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY << " roiStrOLD : " << roiStr << G4endl; //GDEB
	//		roival = 2 + NMAXID*3 + NMAXID*NMAXID*15;
	size_t theID = GetNumber();
	G4String theIDStr = GmGenUtils::itoa(theID);
	if(roiStr != "" ) { // add new roiStr
	  if( roiStr.find(theIDStr) == std::string::npos ) {
	    roiStr += ":"+theIDStr;
	  }
	  //	  G4cout << " fStructure " << ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY << " roiStrNEW : " << roiStr << " <- " << theIDStr << " " << theID << G4endl; //GDEB
	} else {
	  roiStr = theIDStr;
	}
	if( DicomVerb(debugVerb) ){
	  G4cout << imageStr << " : " <<ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY << " FINAL Struct ID WITH PREVIOUS ID's IN VOXEL " << ix << " " << iy << " " << iPolygonZ << " : " << roiStr << " Z=" << planeZ << G4endl;
	}
	imageStr->SetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY, roiStr);

	/*	 //	 G4cout << " posZ " << posZ << " posY " << posY << G4endl; //GDEB
	 if( posZ < -867 && posZ > -872 && posY < -100 && posY > -140 )
	   G4cout << pos << " fStructure " << ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY << " : " << imageStr->GetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY) << " ixyz " << ix << " " << iy << " " << iPolygonZ << G4endl; //GDEB
	*/
	//	G4cout << " fStructure " << ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY << " : " << imageStr->GetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY) << " i " << ix << " " << iy << " " << iPolygonZ << " N " <<imNoVoxelsX << " " << imNoVoxelsXY << G4endl; //GDEB
	//	size_t nsts = std::count(roiStr.begin(), roiStr.end(), ':'); //GDEB
	//	if( nsts > 6 ) G4cout << " nSts " << nsts+1 << " " << roiStr << G4endl; //GDEB
	// 	G4cout << " fStructure minZ " << imMinZ + imVoxelDimZ*(ix) << " " << ix+iy*imNoVoxelsX << " : " << imageStr->GetDataStr(ix+iy*imNoVoxelsX+iPolygonZ*imNoVoxelsXY) << " i " << ix << " " << iy << G4endl; //GDEB
      } 
      
    }
  }

}
  
//-----------------------------------------------------------------------------
DicomPolygon* DicomPolygonList::AddDisplacedLine( DicomPolygon* line, G4double disp )
{
  std::vector<G4ThreeVector> points = line->GetPoints();
  //  G4cout << this << "  DicomPolygonList::AddDisplacedLine " << theName << " Z= " << points[0].z() << " -> " <<points[0].z()+disp << " NPOINTS " << line->GetPoints().size() << " N_LINES " << theLines.size() << G4endl; //GDEB
  std::vector<G4ThreeVector> dirs = line->GetDirections();
  for( size_t ip = 0; ip < points.size(); ip++ ) {
    if( line->GetOrientation() == DPOrientXY ) {
      points[ip].setZ( points[ip].z() + disp );
    } else if( line->GetOrientation() == DPOrientXZ ) {
      points[ip].setY( points[ip].y() + disp );
    } else if( line->GetOrientation() == DPOrientYZ ) {
      points[ip].setX( points[ip].x() + disp );
    }
  }
  DicomPolygon* newPoly = new DicomPolygon(points,dirs,GetName(),theOrientation);
  AddLine(newPoly);
  //  G4cout << "  DicomPolygonList::AddDisplacedLine  DONE " << theName << " Z= " << points[0].z()-disp << " -> " <<points[0].z() << " N_LINES " << theLines.size() << G4endl; //GDEB

  return newPoly;
}
 
//-----------------------------------------------------------------------------
G4int DicomPolygonList::GetPolygonZIndex( DicomVImage* image, G4double planeZ )
{ 
  size_t imNoVoxelsZ = image->GetNoVoxelsZ();
  G4double imMinZ = image->GetMinZ();
  G4double imVoxelDimZ = image->GetVoxelDimZ();

  G4double polyZ = planeZ;
  G4int iPolygonZ = ( polyZ - imMinZ ) / imVoxelDimZ;
  G4cout << " DicomPolygonList::GetPolygonZIndex " <<iPolygonZ << " = ( " << polyZ << " - " << imMinZ << " ) / " <<imVoxelDimZ << G4endl;//GDEB
  if( DicomVerb(debugVerb) ) {
    if( iPolygonZ < 0 ) {
      G4cerr << " iPolygonZ= " << iPolygonZ
	     << " Polygon Z= " << planeZ << " Image min Z= " << imMinZ << " diff " << planeZ-imMinZ << G4endl;
    }
  }
  //-- check if it is a  precision problem
  if( iPolygonZ < 0 ) {
    if( polyZ - imMinZ  < -image->GetPrecision() ) {
      iPolygonZ = 0;
      if( DicomVerb(debugVerb) ) {
	G4Exception(" DicomPolygon::GetPolygonZIndex",
		    "",
		    JustWarning,
		    "Polygon Z is smaller than image minimum Z, probably due to precision. It will be set to image minimim Z");
      }
    } else {
      if( DicomVerb(debugVerb) ) {
	G4Exception(" DicomPolygon::GetPolygonZIndex",
		    "",
		    JustWarning,
		    "Polygon Z is smaller than image minimum Z, it will not be used");
      }
    }
  }

  if( iPolygonZ >= G4int(imNoVoxelsZ) ) {
    G4double imMaxZ = imMinZ+imVoxelDimZ*image->GetNoVoxelsZ();
    if( DicomVerb(infoVerb) ) {
      G4cerr << " iPolygonZ= " << iPolygonZ 
	     << "Polygon Z= " << planeZ << " Image max Z= " << imMaxZ << " diff " << polyZ-imMaxZ << G4endl;
    }
    if( polyZ - imMaxZ < image->GetPrecision() ) {
      iPolygonZ = imNoVoxelsZ-1;
      /*t      G4Exception(" DicomPolygon::GetPolygonZIndex",
		"",
		  JustWarning,
		  "Polygon Z is bigger than image maximum Z, probably due to precision. It will be set to image maximim Z"); *///GDEB
    } else {
      /*   G4Exception(" DicomPolygon::GetPolygonZIndex",
		  "",
		  JustWarning,
		  "Polygon Z is bigger than image maximum Z, it will not be used");*///GDEB
    } 
  }

  return iPolygonZ;
}
