#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"
#include "globals.hh"

#include "GmGeneratorFromBinFile.hh"
#include "GmGenerVerbosity.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include <iostream>
#include <iomanip>

//------------------------------------------------------------------------
GmGeneratorFromBinFile::GmGeneratorFromBinFile()
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 
  G4String fname = parmgr->GetStringValue("GmGeneratorFromBinFile:FileName", "generator.bin");
  theFileIn = new std::ifstream(fname, std::ios::in | std::ios::binary);
  //   G4cout << " GmGeneratorFromBinFile::GmGeneratorFromBinFile() " << theFileIn << G4endl;

  theNExtraBytes = 0;

  G4bool bHeader = G4bool(parmgr->GetNumericValue("GmGeneratorFromBinFile:ReadHeader",1));
  if(bHeader) ReadHeader();

  theLastEventID = -1;
  theNEvents = -1;
}


//------------------------------------------------------------------------
GmGeneratorFromBinFile::~GmGeneratorFromBinFile()
{
  //  theFileIn->close();
}

//------------------------------------------------------------------------
void GmGeneratorFromBinFile::ReadHeader()
{ 
  int ndata;
  
  ReadHeaderCheck();
  
  int bHeaderNEvents;
  if( ! ( theFileIn->read((char*)&bHeaderNEvents, sizeof(int)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeader:",
		"Error",
		FatalException,
		"Problem reading boolean for number of events");
  }
  int bHeaderNSteps;
  if( ! ( theFileIn->read((char*)&bHeaderNSteps, sizeof(int)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeader",
		"Error",
		FatalException,
		"Problem reading boolean for number of steps");
  }
  int bHeaderData;
  if( ! ( theFileIn->read((char*)&bHeaderData, sizeof(int)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeader",
		"Error",
		FatalException,
		"Problem reading boolean for header data");
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader Info " 
	 << " NEvents " << bHeaderNEvents 
	 << " NSteps " << bHeaderNSteps
	 << " Data " << bHeaderData
	 << G4endl;
#endif

  if( bHeaderNEvents) {
    if( ! ( theFileIn->read((char*)&theNEvents, sizeof(int)) ) ) {
      G4Exception("GmGeneratorFromBinFile::ReadHeader",
		  "Error",
		  FatalException,
		  "Problem reading number of data");
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader  nEvents= " << theNEvents << G4endl;
#endif
  }

  if( bHeaderNSteps) {
    if( ! ( theFileIn->read((char*)&ndata, sizeof(int)) ) ) {
      G4Exception("GmGeneratorFromBinFile::ReadHeader",
		  "Error",
		  FatalException,
		  "Problem reading number of datae");
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader  nSteps= " << ndata << G4endl;
#endif
  }


  if( !bHeaderData) return;

  if( ! ( theFileIn->read((char*)&ndata, sizeof(int)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeader",
		"Error",
		FatalException,
		"Problem reading number of data");
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader  ndata= " << ndata << G4endl;
#endif

  int nchar;
  int nbytes;
  int nCtype;
  G4String dataNameS;
  G4String dataCTypeS;

  theNExtraBytes = 0;
  theExpectedDataNames.push_back("EventID");
  theExpectedDataNames.push_back("Particle"); 
  theExpectedDataNames.push_back("PosX"); 
  theExpectedDataNames.push_back("PosY");
  theExpectedDataNames.push_back("PosZ");
  theExpectedDataNames.push_back("MomX");
  theExpectedDataNames.push_back("MomY");
  theExpectedDataNames.push_back("MomZ");
  theExpectedDataNames.push_back("Time");
  theExpectedDataNames.push_back("Weight");

  char* charTemp = 0;

  for( int ii = 0; ii < ndata; ii++ ){
    if( ! (theFileIn->read ((char*)&nchar, sizeof(int))) ) {
      G4Exception("GmGeneratorFromBinFile::ReadHeader",
		  "Error",
		  FatalException,
		  "Problem reading number of data name characters");
    } 
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader " << ii << " nchar= " << nchar << G4endl;
#endif

    //    nchar = 7+ii;
    //    charTemp = new char[nchar+1];
    charTemp = new char[nchar];
    if( ! (theFileIn->read (charTemp, sizeof(char)*nchar)) ){
      G4Exception("GmGeneratorFromBinFile::ReadHeader",
		  "Error",
		  FatalException,
		  "Problem reading data name ");
    }
    dataNameS = G4String(charTemp).substr(0,nchar);
    delete [] charTemp;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader " << ii << " data name= " << charTemp << " " << G4String(charTemp).length() << " SS " << dataNameS << G4endl;
#endif

    if( ! (theFileIn->read ((char*)&nCtype, sizeof(int))) ) {
      G4Exception("GmGeneratorFromBinFile::ReadHeader",	
		  "Error",
		  FatalException,
		  "Problem reading data C type number of chars ");
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader " << ii << " C type number of chars= " << nCtype << G4endl;
#endif

    charTemp = new char[nCtype];
    if( ! (theFileIn->read (charTemp, sizeof(char)*nCtype)) ){
      G4Exception("GmGeneratorFromBinFile::ReadHeader",
		  "Error",
		  FatalException,
		  "Problem reading data name ");
    }
    dataCTypeS = G4String(charTemp).substr(0,nCtype);
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader " << ii << " data name= " << charTemp << " " << G4String(charTemp).length() << " SS " << dataCTypeS << G4endl;
#endif
    delete [] charTemp;

    if( ! (theFileIn->read ((char*)&nbytes, sizeof(int))) ) {
      G4Exception("GmGeneratorFromBinFile::ReadHeader",
		  "Error",
		  FatalException,
		  "Problem reading data number of bytes ");
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader " << ii << " nbytes= " << nbytes << G4endl;
#endif

    // check data names
    if( ii <= 9 ) {
      if( !CheckDataName( ii, dataNameS ) ){
	G4Exception("GmGeneratorFromBinFile::ReadHeader",
		    "Error reading data name",
		    FatalErrorInArgument,
		    G4String("Data name number " + GmGenUtils::itoa(ii) + " should be " + theExpectedDataNames[ii] + ", while it is " + dataNameS).c_str());
	
      }
    } else {
      theNExtraBytes += nbytes;
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeader " << ii << " " << nchar << " " << dataNameS << " " << nbytes << G4endl;
#endif
  }
  theExtraBytes = new char(theNExtraBytes);
  
}

//------------------------------------------------------------------------
void GmGeneratorFromBinFile::ReadHeaderCheck()
{
  char endianChar[1];
  if( ! ( theFileIn->read((char*)&endianChar, sizeof(char)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck",
		"Error",
		FatalException,
		"Problem reading header check endian");
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGeneratorFromBinFile::ReadHeaderCheck machine IsLittelEndian()= " << GmGenUtils::IsLittleEndian() << " little endian read= " << endianChar << G4endl;
#endif
  if( (GmGenUtils::IsLittleEndian() && endianChar[0] != 'L')
      || (!GmGenUtils::IsLittleEndian() && endianChar[0] != 'B') ) {
    G4String thisEndian = "big";
    if( GmGenUtils::IsLittleEndian() ) thisEndian = "little";
    G4String fileEndian = "big";
    if( endianChar[0] == 'L' ) fileEndian = "little";
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck()",
		"Wrong endian",
		FatalException,
		G4String("This machine is " + thisEndian + " endian, while file is " + fileEndian + " endian").c_str());
  }

  int dataint;
  if( ! ( theFileIn->read((char*)&dataint, sizeof(int)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck",
		"Error",
		FatalException,
		"Problem reading header check int");
  }
  if( dataint != 1234567890 )  {
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck",
		"Wrong header check int",
		FatalException,
		G4String("Code should read 1234567890, while it is reading" + GmGenUtils::itoa(dataint)).c_str());
  }
	
  float ftest = FLT_MAX;
  ftest = 3.40282e+38;

  float datafloat;
  if( ! ( theFileIn->read((char*)&datafloat, sizeof(float)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck",
		"Error",
		FatalException,
		"Problem reading header check float");
  }
  if( datafloat != ftest )  {
     G4cerr << " datafloat read " << datafloat << " datafloat read - test " << datafloat-ftest << G4endl;
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck",
		"Wrong header check double",
		FatalException,
		G4String("Code should read " + GmGenUtils::ftoa(ftest) + ", while it is reading" + GmGenUtils::ftoa(datafloat)).c_str());
  }

  double dtest = DBL_MAX;
  dtest = 1.79769e+308;

  double datadouble;
  if( ! ( theFileIn->read((char*)&datadouble, sizeof(double)) ) ) {
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck",
		"Error",
		FatalException,
		"Problem reading header check double");
  }
  if( datadouble != dtest )  {
    G4cerr << " datadouble read " << datadouble << " datadouble read - test " << datadouble-dtest << G4endl;
    G4Exception("GmGeneratorFromBinFile::ReadHeaderCheck",
		"Wrong header check double",
		FatalException,
		G4String("Code should read " + GmGenUtils::ftoa(dtest) + ", while it is reading" + GmGenUtils::ftoa(datadouble)).c_str());
  }  

}

//------------------------------------------------------------------------
G4bool GmGeneratorFromBinFile::CheckDataName( G4int ii, G4String dataName )
{
  if( ii == 0 || ii == 1 || ii == 9 ) {
    if( dataName != theExpectedDataNames[ii] ) return FALSE;
  } else {
    if( dataName.substr(dataName.length()-4,4) != theExpectedDataNames[ii] ) return FALSE;
  }

  return TRUE;
}

//------------------------------------------------------------------------
void GmGeneratorFromBinFile::GeneratePrimaries(G4Event* evt)
{
 
  // GenerFile gf;
  //  fwrite (&po, sizeof(struct PetOutput),1,theFileIn);

  GenerBinFileData gf;

  if ( theFileIn->eof() ){
    G4RunManager::GetRunManager()->AbortRun();  //  /abortrun
  }
 
  G4int eventID;
  if( theLastEventID == -1 ) {
    if( !( theFileIn->read((char*)&eventID, sizeof(G4int)) ) ) {
      G4Exception("GmGeneratorFromBinFile::GeneratePrimaries",
		  "Error",
		  FatalException,
		  "Problem reading event ID");
    }
    //    G4cout << " reading eventID " << eventID << G4endl;
    theLastEventID = eventID;
  }
  eventID = theLastEventID;

  while( eventID == theLastEventID ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::GeneratePrimaries eventID " << eventID << " lastEventID " << theLastEventID << G4endl;
#endif

    char* charTemp = new char[15];
    if( !( theFileIn->read(charTemp, sizeof(char)*15) ) ) {
      G4Exception("GmGeneratorFromBinFile::GeneratePrimaries",
		  "Error",
		  FatalException,
		  "Problem reading particle name");
    }
    G4String partName = G4String(charTemp).substr(0,15);
    //    G4cout << " reading partName " << partName << " " << charTemp << G4endl;
    delete [] charTemp;

    if( !( theFileIn->read((char*)&gf, sizeof(struct GenerBinFileData)) ) ) {
      G4Exception("GmGeneratorFromBinFile::GeneratePrimaries",
		  "Error",
		  FatalException,
		  "Problem reading particle");
    }
#ifndef GAMOS_NO_VERBOSE
    if ( GenerVerb(debugVerb) ) G4cout << " GmGeneratorFromBinFile::GeneratePrimaries "
	  << gf.posx << " " << gf.posy << " " << gf.posz
	  <<" "<< gf.momx << " " << gf.momy << " " << gf.momz << " " << gf.time << " " << gf.weight << G4endl;
#endif
    
    G4PrimaryVertex* vtx = new G4PrimaryVertex( G4ThreeVector(gf.posx,gf.posy,gf.posz), gf.time );
    evt->AddPrimaryVertex( vtx );
    G4ParticleDefinition* partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( partName, false );
    if( !partDef ) {
      GmGetParticleMgr::GetInstance()->CreateIon( partName );
      partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( partName, true );
    }

    //    if( !partDef ) G4cout << " partDef " << partDef << " partName " << partName << G4endl;
    G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, gf.momx, gf.momy, gf.momz);
    vtx->SetPrimary(particle);

    if( theNExtraBytes != 0 ) {
      if( !( theFileIn->read((char*)&theExtraBytes, sizeof(char)) ) ) {
	G4Exception("GmGeneratorFromBinFile::GeneratePrimaries",
		    "Error",
		    FatalException,
		    "Problem reading extra bytes");
      }
    }
    //----- Read next event ID
    if( !( theFileIn->read((char*)&eventID, sizeof(G4int)) ) ) {
      if ( theFileIn->eof() ){
	G4RunManager::GetRunManager()->AbortRun();  //  /abortrun
	return;
      }   
      G4Exception("GmGeneratorFromBinFile::GeneratePrimaries",
		  "Error",
		  FatalException,
		  "Problem reading event ID");
    }
    //    G4cout << " NEW reading eventID " << eventID << " last EventID " << eventID << G4endl;
  }

  theLastEventID = eventID;

}
