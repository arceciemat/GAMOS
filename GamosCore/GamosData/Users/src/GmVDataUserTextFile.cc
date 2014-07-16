#include "GmVDataUserTextFile.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4RunManager.hh"

#include <fstream>

//----------------------------------------------------------------
GmVDataUserTextFile::GmVDataUserTextFile()
{
}

//----------------------------------------------------------------
void GmVDataUserTextFile::WriteHeaderText()
{

  bWriteHeaderData = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":WriteHeaderData",0));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) )  G4cout << " bWriteHeaderData " << bWriteHeaderData << " " << theOutputType+":WriteHeaderData" << G4endl;
#endif

  if( !bWriteHeaderData ) return;

  unsigned int ndata = theData.size();
  *theFileOut << "HEADER: ";
  *theFileOut << ndata << ",";
  for( unsigned int ii = 0; ii < theData.size(); ii++) {
    G4String dataName = theData[ii]->GetName();
    *theFileOut << "\"" << dataName << "\"";
    if( ii != ndata-1) *theFileOut << ",";
  }

  *theFileOut << G4endl;
  
}
