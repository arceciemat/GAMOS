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

#include "GamosCore/GamosRunManager/include/GmRunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) 
{

  G4String theFileListName = "";
  G4String theFileNameOut = "";
  G4String theSqdoseType = "";
  G4bool bNo0Dose = false;
  GmRunManager* runManager = new GmRunManager();
  
  if( argc == 2 ) {
    if( G4String(argv[1]) == "-help" ) {
      G4cout << "  -fl    list of sqdose files" << G4endl
	     << "  -fOut    output file name " << G4endl
	     << "  -sqdoseType   sets the sqdose type: ALL / FILLED" << G4endl
	     << "  -bNo0Dose    do not take into account dose counts that are 0 (designed for LET sqdose files)" << G4endl
	     << "  -verb    verbosity: it sets the GmReadVerbosity. Default is warning, that will print the above lines; debug, that will print each particle read form the phase space file" << G4endl
	     << "  -help    prints the set of arguments" << G4endl;
      return 0;
    } else {
      G4Exception("sumSqdose",
		  "wrong argument",
		  FatalErrorInArgument,
		  "YOU MUST SUPPLY AT LEAST TWO ARGUMENTS: FILE_WITH_LIST_OF_SQDDOSE_FILES SQDOSE_TYPE ");
    }

  } else if( argc == 3 ) {
    theFileListName = argv[1];
    theFileNameOut = argv[2];
  } else if( argc == 4 ) {
    theFileListName = argv[1];
    theFileNameOut = argv[2];
    theSqdoseType = argv[3];
  } else {
    if(argc%2 != 1) { 
      G4Exception("analyseSqdose",
		  "Wrong argument",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-fl" ) {
	theFileListName = argv[ii+1];
      } else if( argvstr == "-fOut" ) {
	theFileNameOut = argv[ii+1];
      } else if( argvstr == "-sqdoseType" ) {
	theSqdoseType = argv[ii+1];
      } else if( argvstr == "-bNo0Dose" ) {
	bNo0Dose = G4bool(argv[ii+1]);
	GmSqdose::SetNo0Dose(bNo0Dose);
      } else if( argvstr == "-verb" ) {
	G4String verbstr = "GmReadDICOMVerbosity " + G4String(argv[ii+1]);
	runManager->SelectVerbosity(verbstr);
      } else {
	G4Exception("sumSqdose",
		    "Wrong argument",
		    FatalErrorInArgument,
		    G4String("Argument can be -ff -fOut -sqdoseType -bNo0Dose -verb, it is"+argvstr).c_str());
      }
    }
  }
  
  FILE* fout  = fopen(theFileNameOut,"wb");

  std::vector<G4String> wl;
  GmFileIn finlis = GmFileIn::GetInstance(theFileListName,1);
  G4double nevents = 0.;

  GmSqdose* dose;
  GmSqdose* dose2;
  G4int ii;
  for( ii = 0;; ii++){
    if( ! finlis.GetWordsInLine(wl) ) break;      
    G4cout << "### MergeSqdoseUA: reading dose file " << wl[0] << G4endl;
    G4double multFactor = 1.; 
    if( wl.size() == 2 ) multFactor = GmGenUtils::GetValue(wl[1]);
    if( ii == 0 ) {
      dose = new GmSqdose();
      dose->Read(wl[0]);
      if( multFactor != 1. ) *dose *= multFactor;
      nevents += dose->GetHeader()->GetNumberOfEvents();
      if( argc == 4 ) {
	if( theSqdoseType == "ALL" ) {
	  dose->SetSqdoseType(SqTALL);
	} else if( theSqdoseType == "FILLED" ) {
	  dose->SetSqdoseType(SqTFILLED);
	} else {
	  G4Exception("sumSqdose",
		      "Wrong type of output file (third argument)",
		      FatalErrorInArgument,
		      G4String("It can only be 'ALL' or 'FILLED', it is " + theSqdoseType).c_str());
	}
      }
    } else {
      dose2 = new GmSqdose();
      dose2->Read(wl[0]);
      if( multFactor != 1. ) *dose2 *= multFactor;
      nevents += dose->GetHeader()->GetNumberOfEvents();
      *dose += *dose2;
      //      G4cout << " adding doses " << nevents << G4endl;
      delete dose2;
    }
    G4cout << "$$$$$ MERGED " << ii+1 << " files into " << theFileNameOut <<  " NEVENTS= "<< nevents << G4endl;
  }
  
  //  G4cout << "$$$$$ MERGED " << ii << " files into " << theFileNameOut <<  " NEVENTS= "<< nevents << G4endl;
  if( ii != 0 ) dose->Print(fout);

  delete dose;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

