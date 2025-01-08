
#include "DicomPolygonSet.hh"
#include "DicomPolygonList.hh"
#include "DicomVImage.hh"
#include "DicomVImageStr.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomPolygonSet::DicomPolygonSet(G4String name, DPOrientation ori )
  : DicomVLineSet(name, ori)
{

}

//-----------------------------------------------------------------------------
DicomPolygonSet::DicomPolygonSet(DicomPolygonSet* polySet, DicomVImage* image, DPOrientation ori )
  : DicomVLineSet("", ori)
{
  SetName( polySet->GetName() );
  std::vector<DicomVLineList*> lineLists = polySet->GetLineLists();
  for( size_t ipl = 0; ipl < lineLists.size(); ipl++ ) {
    //    G4cout << ipl << " : " << lineLists.size() << " DicomPolygonSet AddLineList N " << lineLists[ipl]->GetLines().size() << G4endl; //GDEB
    AddLineList(new DicomPolygonList( (DicomPolygonList*)(lineLists[ipl]), image, ori ) );
  }

}

//-----------------------------------------------------------------------------
void DicomPolygonSet::FindVoxelsInXY( DicomVImageStr* imageStr )
{
  if( DicomVerb(infoVerb) ) G4cout << imageStr << " " << theName << " DicomPolygonSet::FindVoxelsInXY in image " << imageStr->GetName() << " " << DicomVImage::GetModalityStr(imageStr->GetModality()) << G4endl; 
  for( size_t ii = 0; ii < theLineLists.size(); ii++ ) {
    if( DicomVerb(debugVerb) ) G4cout << GetName() << "@@ DicomPolygonSet::FindVoxelsInXY " << theLineLists[ii]->GetName() << G4endl; 
    (static_cast<DicomPolygonList*>(theLineLists[ii]))->FindVoxelsInXY(imageStr);
  }

}

