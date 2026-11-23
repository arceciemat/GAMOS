#include "GmGenerDistEnergyFromMultiFileE.hh"
#include "GmGenerDistEnergyFromFile.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
GmGenerDistEnergyFromMultiFileE::GmGenerDistEnergyFromMultiFileE()
{
  theInterpolationType = "histogram";
  BuildEnergyDists();
}

//-----------------------------------------------------------------------
void GmGenerDistEnergyFromMultiFileE::BuildEnergyDists()
{
  G4String fileListName = GmParameterMgr::GetInstance()->GetStringValue("GmGenerDistEnergyFromMultiFileE:FileListName","");
  if( fileListName == "" ) {
    G4Exception("GmGenerDistEnergyFromMultiFileE:BuildEnergyDists",
		"",
		FatalException,
		"No list of filenames, use parameter '/P GmGenerDistEnergyFromMultiFileE:FileListName <FILE_NAME>");
  }

  fileListName = GmGenUtils::FileInPath( fileListName );
  GmFileIn fin = GmFileIn::GetInstance(fileListName);

  std::vector<G4String> wl;
  G4int ii = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2) {
      G4Exception("GmGenerDistEnergyFromMultiFileE::ReadEnergyDist",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + fileListName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: ENERGY FILE_NAME").c_str());
    }
    theEnergyDists[ GmGenUtils::GetValue(wl[0]) ] = 0;
    theEnergyFileNames[ GmGenUtils::GetValue(wl[0]) ] = wl[1];
    
    ii++;
  }

}

//-----------------------------------------------------------------------
G4double GmGenerDistEnergyFromMultiFileE::GenerateEnergy( const GmParticleSource* source )
{
  return theCurrentDist->GenerateEnergy( source );
}

//---------------------------------------------------------------------
void GmGenerDistEnergyFromMultiFileE::SetParams( const std::vector<G4String>& params )
{  
  if( params.size() != 1 && params.size() != 2 ) {
    G4Exception("GmGenerDistEnergyFromMultiFileE::SetParams",
		"Wrong number of parameters",
		FatalErrorInArgument,
		G4String("There should be 1 or 2 parameters: ENERGY (INTERPOLATION_TYPE)" + GmGenUtils::itoa(params.size())).c_str());
  }

  G4double ener = GmGenUtils::GetValue(params[0]);
  std::map<G4double,GmGenerDistEnergyFromFile*>::iterator ite = theEnergyDists.find(ener);
  if( ite == theEnergyDists.end() ) {
    G4cerr << " Available Energies: " << G4endl;
    for( ite = theEnergyDists.begin(); ite != theEnergyDists.end(); ite++ ) {
      G4cerr << " ENER " << ite->first << G4endl;
    }
    G4Exception("GmGenerDistEnergyFromMultiFileE::SetParams",
		"",
		FatalErrorInArgument,
		  ("Energy not found "+GmGenUtils::ftoa(ener)).c_str());
  }
  G4String theCurrentFileName = "";
  if( ite->second == 0 ) {
    GmGenerDistEnergyFromFile* dist = new GmGenerDistEnergyFromFile();
    ite->second = dist;
  } 
  theCurrentFileName = theEnergyFileNames[ite->first];
  theCurrentDist = ite->second;
  std::vector<G4String> paramsNC;
  paramsNC.push_back(theCurrentFileName);
  //  G4cout << "Multi PARAMS2 N " << params.size() << G4endl; //GDEB
  if( params.size() == 2 ) {
    theInterpolationType = params[1];
    paramsNC.push_back(params[1]);
    //    G4cout << "Multi PARAMS2 " << params[0] << " " << params[1] << G4endl; //GDEB
    // G4cout << "Multi PARAMSNC2 " << paramsNC[0] << " " << paramsNC[1] << G4endl; //GDEB
  } else {
    paramsNC.push_back(theInterpolationType);
  }
  //  G4cout << "Multi PARAMSNC " << paramsNC[0] << " " << paramsNC[1] << G4endl; //GDEB
  
  theCurrentDist->SetParams(paramsNC);

}

