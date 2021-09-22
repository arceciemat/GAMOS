#include "DicomROI.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

//-----------------------------------------------------------------------------
DicomROI::DicomROI(int ROINumber, OFString ROIName) : theNumber(ROINumber+1), theName(ROIName)
{
  if( DicomVerb(debugVerb) ) G4cout << " CREATED  DicomROI "  << theNumber << " : " << theName << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
void DicomROI::AddContour( DicomROIContour* cont )
{
  theContours.push_back( cont );
  /*  // check if it already exists a contour with same Z
  G4bool bOK = false;
  size_t ii; 
  for( ii = 0; ii < theContours.size(); ii++ ){
    if( cont->GetZ() == theContours[ii]->GetZ() ) {
      bOK = true;
      G4cout << cont << " " << ii << " DicomROI::AddContour (-34.6685,-139.511,-1058.69) " << cont->GetZ() - theContours[ii]->GetZ() << " OK " << bOK << G4endl; //GDEB
      break;
    }
  }
  if( !bOK ) { 
    theContours.push_back( cont );
  } else {
    theContours[ii]->AddPoints( cont->GetPoints() );
    delete cont;
  }
  */
}

//-----------------------------------------------------------------------------
void DicomROI::Print( std::ostream& out )
{
  out << "@@@@@ ROI: " << theNumber << " " << theName << G4endl;

  out <<"@@@@ NUMBER OF ContourSequences " << theContours.size() << G4endl;
  for( size_t ii = 0; ii < theContours.size(); ii++ ) {
    out <<"@@@@ CONTOUR " << ii << G4endl;
    theContours[ii]->Print(out);
  }
}
