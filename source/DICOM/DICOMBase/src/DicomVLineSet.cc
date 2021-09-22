#include "DicomVLineSet.hh"
#include "DicomPolygonList.hh"
#include "DicomVImage.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomVLineSet::DicomVLineSet(G4String name, DPOrientation ori)
  : theName(name), theOrientation(ori)
{

  bHasBeenRotatedXY180 = false;
}

//-----------------------------------------------------------------------------
DicomVLineSet::~DicomVLineSet()
{
  for( size_t ii = 0; ii < theLineLists.size(); ii++ ) {
    delete theLineLists[ii];
  }
}

//-----------------------------------------------------------------------------
void DicomVLineSet::SetLineLists( std::vector<DicomVLineList*> lineLists )
{
  theLineLists = lineLists;
}

//-----------------------------------------------------------------------------
void DicomVLineSet::AddLineList( DicomVLineList* linel )
{
  // check orientation
  if( theLineLists.size() != 0
      && theLineLists[0]->GetLines().size() != 0 ) {  // if no lines, a lineList empty has been defined
    if( theOrientation != linel->GetOrientation() ) {
      G4Exception(" DicomVLineSet::AddLineList",
		  "",
		  FatalException,
		  ("Adding a line list with Orientation="+DicomVLine::GetOrientationName(linel->GetOrientation())+" while previous line lists have Orientation="+DicomVLine::GetOrientationName(theOrientation)).c_str());
    }
  } else {
    theOrientation = linel->GetOrientation();
  }
  if( linel ) theLineLists.push_back(linel);
  linel->SetLineSet(this);

  /*  G4double posZ = 0.;
  std::vector<DicomVLine*> lines = linel->GetLines();
  if( lines.size() != 0 ) {
    if( lines[0]->GetPoints().size() != 0 ) {
      posZ = lines[0]->GetPoints()[0].z(); 
    }
  }
  //  G4cout << posZ << "  DicomVLineSet::AddLineList " << theName << " " << linel->GetName() << " " << theLineLists.size() << " ORIENT=" << theOrientation << G4endl; //GDEB
  */
}

//-----------------------------------------------------------------------------
std::map<G4int,G4String> DicomVLineSet::GetLineListNames()
{
  std::map<G4int,G4String> names;
  std::vector<DicomVLineList*>::const_iterator itep;
  for( itep = theLineLists.begin(); itep != theLineLists.end(); itep++ ) {
    DicomVLineList* plist = *itep;
    names[plist->GetNumber()] = plist->GetName();
  }

  return names;
}

//-----------------------------------------------------------------------------
void DicomVLineSet::SetOrientation( DPOrientation ori, G4bool bChildren )
{
  theOrientation = ori;

  if( bChildren ) {
    for( size_t ii = 0; ii < theLineLists.size(); ii++ ) {
      theLineLists[ii]->SetOrientation(ori,false);
    }
  }
}

//-----------------------------------------------------------------------------
void DicomVLineSet::RotateXY180( G4double angle, G4ThreeVector rotPoint )
{
  std::vector<DicomVLineList*>::const_iterator itep;
  for( itep = theLineLists.begin(); itep != theLineLists.end(); itep++ ) {
    (*itep)->RotateXY180( angle, rotPoint );
  }

}

//-----------------------------------------------------------------------------
DicomVLineList* DicomVLineSet::GetLineList(size_t num ) const
{
  DicomVLineList* lineList = 0;
  std::vector<DicomVLineList*>::const_iterator itep;
  for( itep = theLineLists.begin(); itep != theLineLists.end(); itep++ ) {
    if( (*itep)->GetNumber() == num ) {
      lineList = *itep;
      break;
    }
  }
  if( !lineList ) {
    for( itep = theLineLists.begin(); itep != theLineLists.end(); itep++ ) {
      G4cerr << " LINELIST: " << (*itep)->GetNumber() << " : " << (*itep)->GetName() << G4endl;
    }
    G4Exception("DicomVLineSet::GetLineList",
		"",
		FatalException,
		("LineList number "+GmGenUtils::itoa(num)+" does not exist").c_str());
  }

  return lineList;
  
}
