#include "DicomLineSet.hh"
#include "DicomPolygonList.hh"
#include "DicomVImageStr.hh"
#include "DicomParameterMgr.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//-----------------------------------------------------------------------------
DicomLineSet::DicomLineSet(G4String name, DPOrientation ori )
: DicomVLineSet(name,ori)
{

}

//-----------------------------------------------------------------------------
void DicomLineSet::FindVoxelsInXY( DicomVImageStr* imageStr )
{
  if( DicomVerb(infoVerb) ) G4cout << theName << " DicomLineSet::FindVoxelsInXY in image " << imageStr->GetName() << " " << DicomVImage::GetModalityStr(imageStr->GetModality()) << G4endl;
  for( size_t ii = 0; ii < theLineLists.size(); ii++ ) {
    if( DicomVerb(debugVerb) ) G4cout << "@@ DicomLineSet::FindVoxelsInXY " << theName << G4endl; 
    (static_cast<DicomPolygonList*>(theLineLists[ii]))->FindVoxelsInXY(imageStr);
  }

}

