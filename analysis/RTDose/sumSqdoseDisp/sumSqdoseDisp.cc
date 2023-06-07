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

  if(argc != 3 && argc != 4 ) { 
    G4Exception("sumSqdoseDisp",
		"wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO OR THREE ARGUMENTS: FILE_WITH_LIST_OF_SQDDOSE_FILES FILE_OUTPUT FILE_OUTPUT_TYPE");
  }

  G4String fileListName = argv[1];

  G4String fNameOut = argv[2];
  FILE* fout  = fopen(fNameOut,"wb");

  std::vector<G4String> wl;
  GmFileIn finlis = GmFileIn::GetInstance(fileListName,1);
  G4double nevents = 0.;

  GmSqdose* dose;
  GmSqdose* dose2;
  G4int ii;
  for( ii = 0;; ii++){
    if( ! finlis.GetWordsInLine(wl) ) break;      
    G4cout << "### sumSqdoseDisp: reading dose file " << wl[0] << G4endl;
    G4double multFactor = 1.; 
    G4ThreeVector theDisp;
    if( wl.size() > 1 ) multFactor = GmGenUtils::GetValue(wl[1]);
    if( wl.size() > 2 ) {
      if(wl.size() != 5 ) {
	G4Exception("sumSqdoseDisp",
		    "wrong argument",
		    FatalErrorInArgument,
		    ("FILE_WITH_LIST LINES CANNOT HAVE "+GmGenUtils::itoa(wl.size())+" WORDS, ONLY 1, 2 or 5").c_str());
      }
      theDisp = G4ThreeVector(GmGenUtils::GetValue(wl[2]),GmGenUtils::GetValue(wl[3]),GmGenUtils::GetValue(wl[4]));
      G4cout << "sumSqdoseDisp theDisp " << theDisp << G4endl; //GDEB
    }
    
    if( ii == 0 ) {
      dose = new GmSqdose();
      dose->Read(wl[0]);
      if( theDisp != G4ThreeVector() ) {
	dose->Displace(theDisp);
      }
      if( multFactor != 1. ) *dose *= multFactor;
      nevents += dose->GetHeader()->GetNumberOfEvents();
      if( argc == 4 ) {
	if( std::string(argv[3]) == "ALL" ) {
	  dose->SetSqdoseType(SqTALL);
	} else if( std::string(argv[3]) == "FILLED" ) {
	  dose->SetSqdoseType(SqTFILLED);
	} else {
	  G4Exception("sumSqdoseDisp",
		      "Wrong type of output file (third argument)",
		      FatalErrorInArgument,
		      G4String("It can only be 'ALL' or 'FILLED', it is " + G4String(argv[3])).c_str());
	}
      }
    } else {
      dose2 = new GmSqdose();
      dose2->Read(wl[0]);
      if( multFactor != 1. ) *dose2 *= multFactor;
      nevents += dose->GetHeader()->GetNumberOfEvents();
      if( theDisp == G4ThreeVector() ) {
	*dose += *dose2;
      } else {
	dose2->Displace(theDisp);
	dose->SumDisplaced(dose2);
      }
      //      G4cout << " adding doses " << nevents << G4endl;
      delete dose2;
    }
    G4cout << "$$$$$ MERGED " << ii+1 << " files into " << fNameOut <<  " NEVENTS= "<< nevents << G4endl;
  }
  
  //  G4cout << "$$$$$ MERGED " << ii << " files into " << fNameOut <<  " NEVENTS= "<< nevents << G4endl;
  if( ii != 0 ) dose->Print(fout);

  delete dose;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

