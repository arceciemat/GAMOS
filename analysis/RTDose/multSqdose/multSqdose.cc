//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4GeometryManager.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) 
{

  if(argc != 4 && argc != 5 ) { 
    G4Exception("sumSqdose",
		"wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO OR THREE ARGUMENTS: FILE_OUTPUT FILE_INPUT_1 FILE_INPUT_2 FILE_OUTPUT_TYPE");
  }

  G4String fNameOut = argv[1];
  FILE* fout  = fopen(fNameOut,"wb");

  G4double nevents = 0.;

  GmSqdose* sqdose = 0;
  GmSqdose* sqdose2 = 0;
  for( G4int ii = 2; ii < std::min(argc,4); ii++){
    G4cout << "### MergeSqdoseUA: reading sqdose file " << argv[ii] << G4endl;
    G4double multFactor = 1.; 
    if( ii == 2 ) {
      sqdose = new GmSqdose();
      sqdose->Read(argv[ii]);
      if( multFactor != 1. ) *sqdose *= multFactor;
      nevents += sqdose->GetHeader()->GetNumberOfEvents();
      if( argc == 5 ) {
	if( std::string(argv[3]) == "ALL" ) {
	  sqdose->SetSqdoseType(SqTALL);
	} else if( std::string(argv[3]) == "FILLED" ) {
	  sqdose->SetSqdoseType(SqTFILLED);
	} else {
	  G4Exception("sumSqdose",
		      "Wrong type of output file (third argument)",
		      FatalErrorInArgument,
		      G4String("It can only be 'ALL' or 'FILLED', it is " + G4String(argv[3])).c_str());
	}
      }
    } else {
      sqdose2 = new GmSqdose();
      sqdose2->Read(argv[ii]);
      if( multFactor != 1. ) *sqdose2 *= multFactor;
      nevents += sqdose->GetHeader()->GetNumberOfEvents();
      *sqdose *= *sqdose2;
      //      G4cout << " adding sqdoses " << nevents << G4endl;
      delete sqdose2;
    }
    G4cout << "$$$$$ MERGED " << ii+1 << " files into " << fNameOut <<  " NEVENTS= "<< nevents << G4endl;
  }
  
  //  G4cout << "$$$$$ MERGED " << ii << " files into " << fNameOut <<  " NEVENTS= "<< nevents << G4endl;
  sqdose->Print(fout);

  delete sqdose;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

