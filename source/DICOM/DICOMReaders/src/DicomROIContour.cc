#include "DicomROIContour.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

//-----------------------------------------------------------------------------
DicomROIContour::DicomROIContour()
{
}

//-----------------------------------------------------------------------------
void DicomROIContour::SetData( std::vector<G4ThreeVector> points )
{
  for( size_t ii = 0; ii < points.size(); ii++ ) {
    if( ii == 0 || points[ii] != points[ii-1] ) {
      thePoints.push_back(points[ii]);
    }
  }

  // theDirections.clear();
  if( theGeomType == "CLOSED_PLANAR" ){
    //add a last point to close the circle
    //    if( thePoints[0] != thePoints[thePoints.size()-1] ) {
      thePoints.push_back( thePoints[0] );
      //    }
    for( size_t ii = 1; ii < thePoints.size(); ii++ ){
      theDirections.push_back( thePoints[ii] - thePoints[ii-1] );
      //      G4cout << this << " " << ii <<"DicomROIContour ADD DIRECTION " <<theDirections[theDirections.size()-1] << " " <<  thePoints[ii] - thePoints[ii-1] << " " << thePoints[ii] << " - " << thePoints[ii-1]  << G4endl; //GDEB
    }
    theDirections.push_back( thePoints[0] - thePoints[thePoints.size()-1] ); // (0,0)
  } else {
#ifndef WIN32
    G4Exception("DicomROIContour::SetData",
		"DRC001",
		JustWarning,
		("Contour is not of type CLOSED_PLANAR, but " + theGeomType + "  It will not be drawn").c_str());
#endif
  }
}

//-----------------------------------------------------------------------------
G4double DicomROIContour::GetZ()
{
  if( thePoints.size() == 0 ) {
    G4Exception("DicomROIContour::GetZ",
		"DRC001",
		FatalException,
		"No Point in contour");
    if( DicomVerb(debugVerb) ) Print(G4cout);
    return DBL_MAX;
  }
  return thePoints[0].z();
}

//-----------------------------------------------------------------------------
void DicomROIContour::AddPoints( std::vector<G4ThreeVector> points )
{
  points.push_back( points[0] );
  size_t npold = thePoints.size();
  for( size_t ii = 0; ii < points.size(); ii++ ) {
    thePoints.push_back( points[ii] );
  }

  for( size_t ii = npold+1; ii < thePoints.size(); ii++ ){
    theDirections.push_back( thePoints[ii] - thePoints[ii-1] );
  }
  theDirections.push_back( thePoints[npold] - thePoints[thePoints.size()-1] );
  
}

//-----------------------------------------------------------------------------
void DicomROIContour::Print( std::ostream& out )
{

  out << "@@@ NUMBER OF ContourImageSequences " << theImageIUIDs.size() << G4endl;
  for( size_t ii = 0; ii < theImageIUIDs.size(); ii++ ) {
    out << "ContourImageSequence I= " << theImageIUIDs[ii] << G4endl;
  }
  out << "@@@ GeomType " << theGeomType << "  NPOINTS " << thePoints.size() << G4endl;
  for( size_t ii = 0; ii < thePoints.size(); ii++ ) {
    out << "(" << thePoints[ii].x()<< "," << thePoints[ii].y()<< "," << thePoints[ii].z() << ")" << G4endl;
  }

}
