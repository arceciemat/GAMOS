#include "DicomVDrawer.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"
#include "DicomPolygonSet.hh"
#include "DicomPolygonList.hh"
#include "DicomPolygon.hh"
#include <set>
#include <cfloat>

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomVDrawer::DicomVDrawer()
{
  bAverageSurroundingBig = false;
  theBackgroundImage = 0;
}

//-----------------------------------------------------------------------------
DicomPolygonSet* DicomVDrawer::VoxelsInUseClosest( DicomPolygonSet* polySet, DicomVImage* image )
{
  DicomPolygonSet* newPolySet = new DicomPolygonSet(polySet->GetName(),polySet->GetOrientation());
  std::vector<DicomVLineList*> lineLists = polySet->GetLineLists();
  std::vector<DicomVLineList*> newLineLists;
  // build lines in polylist, taking closest to voxel
  G4double imMinZ = image->GetMinZ();
  G4double imMaxZ = image->GetMaxZ();
  G4double imWidthZ = image->GetVoxelDimZ();
  for( size_t ipl = 0; ipl < lineLists.size(); ipl++ ) {
    DicomPolygonList* polyList = (DicomPolygonList*)(lineLists[ipl]);
    DicomPolygonList* newPolyList = new DicomPolygonList(polyList->GetName(),polyList->GetNumber(),polySet->GetOrientation());
    if( DicomVerb(debugVerb) ) {
      G4cout << " bVoxelsInUseClosest polylist " << polyList->GetName() << " N " << polyList->GetLines().size() << G4endl;
    }
    std::vector<DicomVLine*> lines = polyList->GetLines();
    if( lines.size() == 0 ) continue;
    
    mmddp posOrderedLines = polyList->BuildPosOrderedLines();
    pmmddpi polygonsInPlane;
    pmmddpi pitet;
    if( DicomVerb(debugVerb) ) {
      G4cout << "DicomPolygonList bVoxelsInUseClosest N " << posOrderedLines.size() << G4endl;
    }
    // line_range=line+-1/2*distance_between_lines
    G4double line_range = 0;
    mmddp::const_iterator iteDOWN = posOrderedLines.begin();
    mmddp::const_iterator iteUP = posOrderedLines.end(); iteUP--;
    if( DicomVerb(debugVerb) ) {
      G4cout << imMinZ << " posOrderedLines BOTTOM " << iteDOWN->first << " TOP " << iteUP->first << G4endl; 
    }
    G4bool bOnePolygonZ = false;
    pitet = posOrderedLines.equal_range((*iteDOWN).first);
    if( pitet.first->first == pitet.second->first ) bOnePolygonZ = true;
    // all polygons in one unique Z
    // lines are all below image: check if adding line_range is in
    if( iteUP->first < imMinZ ) {
      if( bOnePolygonZ ) { // all polygons in one unique Z
	return polySet;
      } else {
	pitet = posOrderedLines.equal_range((*iteUP).first);
	mmddpi ite1 = pitet.first; ite1--;
	line_range = (ite1->first-pitet.first->first)/2.;
	if( iteUP->first+line_range < imMinZ ) {
	  return polySet;
	} else {
	  polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	  for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	    newPolyList->AddDisplacedLine( (*item).second, +line_range );
	  }
	}
      }
      // lines are all above image: check if subtracting line_range is in
    } else if( iteDOWN->first > imMaxZ ) {
      if( bOnePolygonZ ) { // all polygons in one unique Z
	return polySet;
      } else {
	pitet = posOrderedLines.equal_range((*iteDOWN).first);
	mmddpi ite1 = pitet.second; ite1++; 
	line_range = -(pitet.first->first-ite1->first)/2.;
	if( iteDOWN->first-line_range > imMaxZ ) {
	  return polySet;
	} else {	
	  polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	  for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	    newPolyList->AddDisplacedLine( (*item).second, +line_range );
	  }
	}
      }
    } else {
      if( bOnePolygonZ ) { // all polygons in one unique Z
	polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	  newPolyList->AddDisplacedLine( (*item).second, +line_range );
	}
      } else {
	// If 2*line_range > voxelBinWidth: a voxel lower edge and upper edge will have the same lineList as upper
	// If 2*line_range < voxelBinWidth: two or more polygons are in the same voxel: take the one closest to the voxel center
	// loop to all voxels from the ones of the minimum polygon to the maximum one
	G4int iPolygonZMin = std::max(iteDOWN->second->GetPolygonZIndex( image ),0);
	G4int iPolygonZMax = std::min(iteUP->second->GetPolygonZIndex( image ),G4int(image->GetNoVoxelsZ())-1);
	if( DicomVerb(debugVerb) ) {
	  G4cout << " DicomPolygonList  iPolygonZMin/Max " << iPolygonZMin << " " << iPolygonZMax << G4endl;
	}
	mmddp::const_iterator mite;
	for( int ipz = iPolygonZMin; ipz <= iPolygonZMax; ipz++ ) {
	  line_range = 0.;
	  G4double poszUP = imMinZ+(ipz+1)*imWidthZ; // upper position of voxels in Z plane
	  iteUP = posOrderedLines.upper_bound(poszUP);
	  G4double poszDOWN = imMinZ+ipz*imWidthZ; // lower position of voxels in Z plane
	  iteDOWN = posOrderedLines.upper_bound(poszDOWN);
	  if( DicomVerb(debugVerb) ) {
	    G4cout << iteUP->first << " FROM poszUP " << poszUP << " = " << imMinZ << " + " << (ipz+1) << " * " << imWidthZ << G4endl;
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
	    mmddpi ite1 = pitet.first;  ite1--;
	    line_range = (pitet.first->first-ite1->first)/2.;
	    G4double dist2up = iteUP->first - poszUP;
	    iteDOWN = pitet.first; iteDOWN--; // polygon below poszDOWN
	    G4double dist2down = poszDOWN - iteDOWN->first;
	    line_range = ((*iteUP).first-(*iteDOWN).first)/2.;
	    if( DicomVerb(debugVerb) ) {
	      G4cout << " DISTANCE 0  UP: " << dist2up << " = " << iteUP->first << " - " << poszUP << " DOWN: " << dist2down << " = " << poszDOWN << " -  " << iteDOWN->first << " line_range " << line_range << G4endl;
	    }
	    if(dist2up <= dist2down) {
	      polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	      for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
		newPolyList->AddDisplacedLine( (*item).second, +line_range );
	      }
	      line_range *= -1;
	    } else {
	      polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	      for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
		newPolyList->AddDisplacedLine( (*item).second, +line_range );
	      }
	      //	      line_range *= +1;
	    }
	  } else if( distance == 1 ) {
	    if( DicomVerb(debugVerb) ) {
	      G4cout << " DISTANCE 1  USE DOWN : " << iteDOWN->first << G4endl;
	    }
	    // OK: loop to all Polygon's with  Z = the one below
	    polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	    for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	      newPolyList->AddDisplacedLine( (*item).second, +line_range );
	    }
	    //pmite = posOrderedLines.equal_range((*iteDOWN).first);
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
	      newPolyList->AddDisplacedLine( (*item).second, +line_range );
	    }
	  }
	}
      }
    }
    newLineLists.push_back(newPolyList);
    if( DicomVerb(debugVerb) ) {
      G4cout << " bVoxelsInUseClosest newLineLists ADD " << newPolyList->GetName() << " N " << newLineLists.size() << G4endl;
    }
  } // end of old poly list loop
  newPolySet->SetLineLists(newLineLists);

  return newPolySet;

}

//-----------------------------------------------------------------------------
G4double DicomVDrawer::GetValueSurrounding(size_t ix, size_t iy, size_t iz, G4double value, DicomVImage* image, std::vector<G4double>* imgDatap ) 
{
  G4double valueAve = 0.;
  G4int nVals = 0;
  if( ix > 0 ) {
    nVals++;
    valueAve += imgDatap->at(ix-1+iy*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iy > 0 ) {
    nVals++;
    valueAve += imgDatap->at(ix+(iy-1)*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iz > 0 ) {
    nVals++;
    valueAve += imgDatap->at(ix+iy*image->GetNoVoxelsX()+(iz-1)*image->GetNoVoxelsXY());
  }
  if( ix < image->GetNoVoxelsX()-1 ) {
    nVals++;
    valueAve += imgDatap->at(ix+1+iy*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iy < image->GetNoVoxelsY()-1 ) {
    nVals++;
    valueAve += imgDatap->at(ix+(iy+1)*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iz < image->GetNoVoxelsZ()-1 ) {
    nVals++;
    valueAve += imgDatap->at(ix+iy*image->GetNoVoxelsX()+(iz+1)*image->GetNoVoxelsXY());
  }

  if( value > valueAve*theOverSurrounding ) {
    value = valueAve*theOverSurrounding;
  }
  return value;
}

