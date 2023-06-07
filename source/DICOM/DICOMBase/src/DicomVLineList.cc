#include "DicomVLineList.hh"
#include "DicomVLineSet.hh"
#include "DicomPolygon.hh"
#include "DicomVImage.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomVLineList::DicomVLineList(DPOrientation ori) :
  theOrientation(ori)
{
  theLineSet = 0;
}

//-----------------------------------------------------------------------------
DicomVLineList::DicomVLineList(G4String name, size_t num,DPOrientation ori)
: theName(name), theNumber(num), theOrientation(ori)
{
  theLineSet = 0;
}

//-----------------------------------------------------------------------------
DicomVLineList::~DicomVLineList()
{
  for( size_t ii = 0; ii < theLines.size(); ii++ ) {
    delete theLines[ii];
  }
}

//-----------------------------------------------------------------------------
void DicomVLineList::AddLine( DicomVLine* line )
{
  // check orientation
  if( theLines.size() != 0 
      && theLines[0]->GetPoints().size() != 0 ) {  // if no points, an empty line has been defined
    if( theOrientation != line->GetOrientation() ) {
      line->DetectOrientation(1);
      if( line->GetOrientation() != DPOrientNone && theOrientation != line->GetOrientation() ) {
	G4Exception(" DicomVLineList::AddLine",
		  "",
		    FatalException,
		    ("Adding a line with Orientation="+DicomVLine::GetOrientationName(line->GetOrientation())+" while previous lines have Orientation="+DicomVLine::GetOrientationName(theOrientation)).c_str());
      }
    }
  } else {
    theOrientation = line->GetOrientation();
  }
  theLines.push_back(line);
  line->SetLineList( this );
  /*  if( theName.find("Bolus") != std::string::npos ) { // == "Bolus Couch" ) {
    G4cout << line->GetPoints()[0].z() << "  DicomVLineList::AddLine " << theName << " " << theLines.size() << " ORIENT=" << theOrientation << G4endl; //GDEB
    }*/
}

//-----------------------------------------------------------------------------
void DicomVLineList::SetLineSet( DicomVLineSet* ls )
{
  theLineSet = ls;
  theLineSet->SetOrientation(ls->GetOrientation(), false);
}

//-----------------------------------------------------------------------------
void DicomVLineList::SetOrientation( DPOrientation ori, G4bool bChildren )
{
  if( theLineSet ) {
    if( theLineSet->GetOrientation() == DPOrientNone ) {
      theLineSet->SetOrientation(ori,false); // this will set of line's belonging to it
    } else {
      if( theLineSet->GetOrientation() != ori ) {
	G4Exception("DicomVLineList::SetOrientation",
		    "",
		    FatalException,
		    ("Setting LineList orientation "+DicomVLine::GetOrientationName(ori)+" different than its LineSet orientation "+ DicomVLine::GetOrientationName(theLineSet->GetOrientation())).c_str());
      }
    }
  }
  theOrientation = ori;
    
  if( !bChildren ) {
    for( size_t ii = 0; ii < theLines.size(); ii++ ) {
      theLines[ii]->SetOrientation(ori);
    }
  }
}
  
//-----------------------------------------------------------------------------
void DicomVLineList::RotateXY180( G4double angle, G4ThreeVector rotPoint )
{
  std::vector<DicomVLine*>::const_iterator itep;
  for( itep = theLines.begin(); itep != theLines.end(); itep++ ) {
    //    G4cout << " ROTATE LINE " << (*itep)->GetName() << G4endl; //GDEB
    (*itep)->RotateXY180( angle, rotPoint );
  }

}
