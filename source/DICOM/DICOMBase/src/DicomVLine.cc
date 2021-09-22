#include "DicomVLine.hh"
#include "DicomVLineList.hh"
#include "DicomMgr.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

#include "G4GeometryTolerance.hh"
#include "CLHEP/Vector/RotationZ.h"

//-----------------------------------------------------------------------------
DicomVLine::DicomVLine( G4String name, DPOrientation ori )
  : theName(name), theOrientation(ori)
{
  theOrientation = DPOrientXY;
  theNumber = UINT_MAX;
  theLineList = 0;
  //  G4cout << this << " " << theName << " DicomVLine::DicomVLine( ORIENTATION " << theOrientation << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
DicomVLine::DicomVLine( std::vector<G4ThreeVector> points, G4String name, DPOrientation ori )
  : theName(name), theOrientation(ori)
{
  theNumber = UINT_MAX;
  theLineList = 0;
  theOrientation = DPOrientXY;
  if( points.size() == 0 ) {
    G4Exception("DicomVLine::DicomVLine",
		"",
		JustWarning,
		("Building Line with 0 points: "+name).c_str());
  }
  SetPoints( points );
  BuildDirections();
  //  G4cout << this << " " << theName << " DicomVLine::DicomVLine(pts ORIENTATION " << theOrientation << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
DicomVLine::DicomVLine( std::vector<G4ThreeVector> points, std::vector<G4ThreeVector> dirs, G4String name, DPOrientation ori )
  : theName(name), theOrientation(ori)
{
  theNumber = UINT_MAX;
  theLineList = 0;
  theOrientation = DPOrientXY;
  if( points.size() == 0 ) {
    G4Exception("DicomVLine::DicomVLine",
		"",
		JustWarning,
		("Building Line with 0 points: "+name).c_str());
  }
  if( points.size() != dirs.size() ) {
    G4Exception("DicomVLine::DicomVLine",
		"",
		JustWarning,
		("Building Line with different number of points = " + GmGenUtils::itoa(points.size())+ " than of directions = "+ GmGenUtils::itoa(dirs.size())).c_str());
  }
  SetPoints( points );
  SetDirections( dirs );
  //  G4cout << this << " " << theName << " DicomVLine::DicomVLine(pts ORIENTATION " << theOrientation << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
size_t DicomVLine::GetNumber() const
{
  if( theNumber == UINT_MAX ) {
    if( !theLineList ) {
      G4Exception("DicomVLine::GetNumber",
		  "",
		  FatalException,
		  "No parent line list defined. Please check your line constructions, or contact GAMOS authors");
    }
    return theLineList->GetNumber();
  } else {
    return theNumber;
  }
}

//-----------------------------------------------------------------------------
G4double DicomVLine::GetID() const
{
  if( thePoints.size() == 0 ) {
    G4Exception("DicomVLine::GetID",
		 "",
		 FatalException,
		 ("No point is defined for DicomVLine " + theName + " in list " + theLineList->GetName()).c_str());
  }
  switch (theOrientation) {
  case DPOrientXY:
    return thePoints[0].z();
  case DPOrientXZ:
    return thePoints[0].y();
  case DPOrientYZ:
    return thePoints[0].x();
  case DPOrientNone:
    return 0.;
  }
  return 0.;
}

//-----------------------------------------------------------------------------
void DicomVLine::SetPoints( std::vector<G4ThreeVector> points )
{
  //--- do not count repeated points
  for( size_t ii = 0; ii < points.size(); ii++ ) {
    if( ii == 0 || points[ii] != points[ii-1] ) {
      thePoints.push_back(points[ii]);
    }
  }
  
  DetectOrientation();

  //  for(size_t ii = 0; ii < thePoints.size()-1; ii++ ) {
  //    G4cout << ii << " POINT " << thePoints[ii].x() << " " << thePoints[ii].y() << " " << thePoints[ii].z() << G4endl; //GDEB
  //  }

}
  
//-----------------------------------------------------------------------------
void DicomVLine::BuildDirections()
{
  size_t ii = 0;
  theDirections.clear();
  for( ; ii < thePoints.size()-1; ii++ ) {
    theDirections.push_back(thePoints[ii+1] - thePoints[ii]);
  }
  /*  std::cout << ii << " NDIRS " << theDirections.size() << " " << thePoints.size() << G4endl; //GDEB
  for( ii = 0; ii < thePoints.size(); ii++ ) {
    std::cout << " PTS " << thePoints[ii] << G4endl; //GDEB
    std::cout << " DIRS " << theDirections[ii] << G4endl; //GDEB
    }*/

}

//-----------------------------------------------------------------------------
void DicomVLine::DetectOrientation(G4int iOrder)
{
  if( thePoints.size() < 2 ) {
    G4Exception("DicomVLine::DetectOrientation",
		"",
		JustWarning,
		("LINE "+theName+" has less than 2 points="+GmGenUtils::itoa(thePoints.size())+"\n Orientation will be set to None").c_str());
    theOrientation = DPOrientNone;
    return;
  }
  
  typedef std::multimap<G4int,DPOrientation> mmio; 
  typedef std::map<DPOrientation,G4int> moi; 
  G4double PRECISION = 1E-9;
  moi nOrient;
  for( size_t iip = 0; iip < thePoints.size()-1; iip++ ) {
    G4ThreeVector pt1 = thePoints[iip];
    G4ThreeVector pt2 = thePoints[iip+1];
    G4bool bOrient = false;
    if( DicomVerb(testVerb) ) G4cout << iip << " DicomVLine::DetectOrientation: pt1 " << pt1 << " pt2 " << pt2 << G4endl; 
    if( fabs( pt1.x() - pt2.x() ) < PRECISION ) {
      nOrient[DPOrientYZ]++;
      bOrient = true;
      if( DicomVerb(testVerb) ) G4cout << iip << " DicomVLine::DetectOrientation: [DPOrientYZ " << pt1.x() << " == " << pt2.x() << " : " <<  nOrient[DPOrientYZ] << G4endl;
    }
    if( fabs( pt1.y() - pt2.y() ) < PRECISION ) {
      nOrient[DPOrientXZ]++;
      bOrient = true;
      if( DicomVerb(testVerb) ) G4cout << iip << " DicomVLine::DetectOrientation: [DPOrientXZ " << pt1.y() << " == " << pt2.y() << " : " <<  nOrient[DPOrientXZ] << G4endl;
    }
    if( fabs( pt1.z() - pt2.z() ) < PRECISION ) {
      nOrient[DPOrientXY]++;
      bOrient = true;
      if( DicomVerb(testVerb) ) G4cout << iip << " DicomVLine::DetectOrientation: [DPOrientXY " << pt1.z() << " == " << pt2.z() << " : " <<  nOrient[DPOrientXY] << G4endl;
    }
    if( !bOrient ) {
      if( DicomVerb(infoVerb) ) {
		G4Exception("DicomVLine::DetectOrientation",
		  "",
		    JustWarning,
		  " No orientation detected, use DPOrientXY");
	G4cerr << "X: " << pt1.x() << " != " << pt2.x() << " : " << pt1.x()-pt2.x() << G4endl
	       << "Y: " << pt1.y() << " != " << pt2.y() << " : " << pt1.y()-pt2.y() << G4endl
	       << "Z: " << pt1.z() << " != " << pt2.z() << " : " << pt1.z()-pt2.z() << G4endl;
      }
      nOrient[DPOrientXY]++; //theOrientation = DPOrientXY;
    }

  }
  //--- It may happen that two points coincide in two directions, so get the direction with more coincidences
  mmio nOrientInv;
  moi::const_iterator iter;
  for( iter = nOrient.begin(); iter != nOrient.end(); iter++ ) {
    nOrientInv.insert(mmio::value_type((*iter).second,(*iter).first));
  }
  mmio::reverse_iterator iterr = nOrientInv.rbegin();
  for( G4int ij = 0; ij < iOrder; ij++ ) { // get previous one if it has the same coincidences than last
    if( DicomVerb(testVerb) ) G4cout << " DicomVLine " << theName << " previous orientation? " << ij << " now " << (*iterr).second << G4endl;
    mmio::reverse_iterator iterr2 = iterr;
    iterr2++;
    if( iterr->first == iterr2->first ) {
      if( DicomVerb(testVerb) ) G4cout << " DicomVLine " << theName << " NOW orientation " << ij << " : " <<  (*iterr).second << G4endl;
      iterr++;
      if( DicomVerb(testVerb) ) G4cout << " DicomVLine " << theName << " TAKE previous orientation " << ij << " : " <<  (*iterr).second << G4endl;
    }
  }
  
  theOrientation = (*iterr).second;

  if( theOrientation == DPOrientXY ) {
    thePlanePosition = thePoints[0].z();
  } else if( theOrientation == DPOrientXZ ) {
    thePlanePosition = thePoints[0].y();
  } else if( theOrientation == DPOrientYZ ) {
    thePlanePosition = thePoints[0].x();
  }
  if( DicomVerb(testVerb) ) G4cout << " DicomVLine " << theName << " FINAL Orientation " << theOrientation << "=" << DicomVLine::GetOrientationName(theOrientation) << G4endl;

  if( theLineList ) theLineList->SetOrientation(theOrientation,false);
}
  
//-----------------------------------------------------------------------------
G4ThreeVector DicomVLine::GetPoint( size_t ipt )
{
  if( ipt >= thePoints.size() ) {
    G4Exception("DicomVLine::GetPoint",
		"",
		FatalException,
		("Asking for point number " + GmGenUtils::itoa(ipt) + ", which is outside number of points= " + GmGenUtils::itoa(thePoints.size())).c_str());
  }
  return thePoints[ipt];
}

//-----------------------------------------------------------------------------
G4ThreeVector DicomVLine::GetDirection( size_t idir )
{
  if( idir >= theDirections.size() ) {
    G4Exception("DicomVLine::GetDirection",
		"",
		FatalException,
		("Asking for direction number " + GmGenUtils::itoa(idir) + ", which is outside number of directions= " + GmGenUtils::itoa(theDirections.size())).c_str());
  }
  return theDirections[idir];
}

//-----------------------------------------------------------------------------
void DicomVLine::SetOrientation( DPOrientation ori )
{
  if( theLineList ) {
    if( theLineList->GetOrientation() == DPOrientNone ) {
      theLineList->SetOrientation(ori,false);
    } else {
      if( theLineList->GetOrientation() != ori ) {
	G4Exception("DicomVLine::SetOrientation",
		    "",
		    FatalException,
		    ("Setting Line orientation "+DicomVLine::GetOrientationName(ori)+" different than its LineList orientation "+ DicomVLine::GetOrientationName(theLineList->GetOrientation())).c_str());
      }
    }
  }
  theOrientation = ori;
}

//-----------------------------------------------------------------------------
void DicomVLine::SetLineList( DicomVLineList* ll )
{
  theLineList = ll;
  theLineList->SetOrientation(ll->GetOrientation(),false);
}

//-----------------------------------------------------------------------------
void DicomVLine::RotateXY180( G4double angle, G4ThreeVector rotPoint )
{
  //    G4cout << "RotateXY180: " << angle << " = " << rotPoint << G4endl; //GDEB
  if( theOrientation != DPOrientXY ) {
    G4Exception("DicomVLine::RotateXY180",
		"",
		JustWarning,
		//t FatalException,
		"Line does not have XY orientation");
  }

  CLHEP::HepRotationZ rotmZ(angle);
  for( size_t iip = 0; iip < thePoints.size(); iip++ ) {
    G4ThreeVector ptLocal = thePoints[iip] - rotPoint;
    //    G4cout << "RotateXY180 1 : " << ptLocal << " = " << thePoints[iip] << " - " << rotPoint << G4endl; //GDEB
    ptLocal *= rotmZ;
    //   G4cout << "RotateXY180 2 : " << ptLocal << " " << rotmZ << G4endl; //GDEB
    thePoints[iip] = ptLocal + rotPoint;
    //    G4cout << "RotateXY180 3 : " << thePoints[iip] << G4endl; //GDEB
  }
  BuildDirections();
  
}

//-----------------------------------------------------------------------------
G4String DicomVLine::GetOrientationName( DPOrientation orient )
{
  switch (orient) {
  case DPOrientXY:
    return "DPOrientXY";
  case DPOrientXZ:
    return "DPOrientXZ";
  case DPOrientYZ:
    return "DPOrientYZ";
  case DPOrientNone:
    return "DPOrientNone";
  }
  return "DPOrient_UNKNOWN:"+GmGenUtils::itoa(orient);
}

