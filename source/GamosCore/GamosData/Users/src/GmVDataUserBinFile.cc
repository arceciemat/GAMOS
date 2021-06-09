#include "GmVDataUserBinFile.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "G4RunManager.hh"

#include <fstream>

//----------------------------------------------------------------
GmVDataUserBinFile::GmVDataUserBinFile()
{
  theNCallsWritten = 0;
  theNEvents = 0;
}

//----------------------------------------------------------------
void GmVDataUserBinFile::WriteHeaderBin()
{
  bWriteHeaderData = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":WriteHeaderData",0));
  //  bWriteHeaderCheck = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":WriteHeaderCheck",0));
  bWriteHeaderNEvents = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":WriteHeaderNEvents",0));
  bWriteHeaderNCalls = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":WriteHeaderNCalls",0));
  bWriteHeaderCheck = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":WriteHeaderCheck",0));

  if( bWriteHeaderCheck != 0 ) WriteHeaderCheck(theFileOutBin);

  if( bWriteHeaderNEvents != 0 ) WriteHeaderNEvents( TRUE );

  if( bWriteHeaderNCalls != 0 ) WriteHeaderNCalls( TRUE );

  if( bWriteHeaderData != 0 ) WriteHeaderData();


  if( bWriteHeaderNEvents != 0 || 
      bWriteHeaderNCalls != 0 || 
      bWriteHeaderData != 0 || 
      bWriteHeaderCheck != 0) {
    WriteHeaderInfo();
  }

}


//----------------------------------------------------------------
void GmVDataUserBinFile::WriteHeaderInfo()
{
  theFileOutBin->write( (char*)&bWriteHeaderCheck, sizeof(int) );
  theFileOutBin->write( (char*)&bWriteHeaderData, sizeof(int) );
  theFileOutBin->write( (char*)&bWriteHeaderNEvents, sizeof(int) );
  theFileOutBin->write( (char*)&bWriteHeaderNCalls, sizeof(int) );

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmVDataUserBinFile::WriteHeaderInfo " 
				  << " NEvents " << bWriteHeaderNEvents 
				  << " NCalls " << bWriteHeaderNCalls
				  << " Check" << bWriteHeaderCheck
				  << " Data " << bWriteHeaderData
	 << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataUserBinFile::WriteHeaderNEvents(G4bool bIsFirst)
{
  if( !bIsFirst ) {
    theFileOutBin->seekp(0, std::fstream::beg);   // move to the start of the file
    WriteHeaderCheck(theFileOutBin);
    WriteHeaderInfo();
  }

  theNEvents = GmNumberOfEvent::GetNumberOfEvent();

  theFileOutBin->write((char*)&theNEvents, sizeof(G4int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmStepDataBinFileUA::EndOfRunAction  write NEvents " << theNEvents <<G4endl; 
#endif
 }

//----------------------------------------------------------------
void GmVDataUserBinFile::WriteHeaderNCalls(G4bool bIsFirst)
{
  if( !bIsFirst ) {
    if( bWriteHeaderNEvents == 0 ) {
      theFileOutBin->seekp(0, std::fstream::beg);   // move to the start of the file
      WriteHeaderCheck(theFileOutBin);
      WriteHeaderInfo();
    }
  }

  theFileOutBin->write((char*)&theNCallsWritten, sizeof(G4int)); 
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << "GmStepDataBinFileUA::EndOfRunAction  write NCalls " << theNCallsWritten << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataUserBinFile::WriteHeaderData()
{
  int ndata = theData.size();
  theFileOutBin->write( (char*)&ndata, sizeof(int) );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << "GmVDataUserBinFile::WriteHeader ndata " << ndata << G4endl;
#endif

  for( unsigned int ii = 0; ii < theData.size(); ii++) 
    {
      G4String dataName = theData[ii]->GetName();
      ndata = dataName.length();
      theFileOutBin->write( (char*)&ndata, sizeof(int) );
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(infoVerb) ) G4cout << "GmVDataUserBinFile::WriteHeader ndata " <<ii << " data_length " << ndata << G4endl;
#endif

      //      const char* dataS = new char[ndata+1];
      //   dataS = dataName.c_str();
      theFileOutBin->write( dataName.c_str(), sizeof(char)*ndata );
      //      delete [] dataS;
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(infoVerb) ) G4cout << "GmVDataUserBinFile::WriteHeaderData ndata " << ii << " data_name   " << dataName << G4endl;
#endif
      //      theFileOutBin->close();
      //      exit(1);

      G4String dataCType = theData[ii]->GetCType();
      ndata = dataCType.length();
      theFileOutBin->write( (char*)&ndata, sizeof(int) );
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(infoVerb) ) G4cout << "GmVDataUserBinFile::WriteHeaderData ndata " <<ii << " data_Ctype_length " << ndata << G4endl;
#endif

      theFileOutBin->write( dataCType.c_str(), sizeof(char)*ndata );
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(infoVerb) ) G4cout << "GmVDataUserBinFile::WriteHeaderData ndata " <<ii << " data_Ctype " << dataCType << G4endl;
#endif

      ndata = theData[ii]->GetNBytes();
      theFileOutBin->write( (char*)&ndata, sizeof(int) );
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(infoVerb) ) G4cout << "GmVDataUserBinFile::WriteHeaderData ndata " << ii << " data_nbytes " << ndata << G4endl;
#endif
    }
}

//----------------------------------------------------------------
void GmVDataUserBinFile::EndOfRun()
{
  if( bWriteHeaderNEvents != 0 ) WriteHeaderNEvents();
  if( bWriteHeaderNCalls != 0 ) WriteHeaderNCalls();

  theFileOutBin->close();
}

/*//----------------------------------------------------------------
void GmVDataUserBinFile::EndOfEvent()
{
  theNEvents++;
}
*/
