#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4GeometryManager.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/Gm3ddose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

int main(int argc,char** argv) 
{

  if(argc!=3) { 
    G4Exception("3ddoseToSqdose",
		"wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO ARGUMENTS: 3DDDOSE_FILE_INPUT SQDOSE_FILE_OUTPUT");
  }

  G4String fNameIn = argv[1];

  G4String fNameOut = argv[2];
  FILE* fout  = fopen(fNameOut,"wb");

  Gm3ddose* dose3d = new Gm3ddose;
  G4cout << "### 3ddoseToSqdose: reading 3ddose file " << argv[1]  << G4endl;
  dose3d->Read(fNameIn);

  GmSqdose* dosesq = new GmSqdose(*dose3d);

  G4cout << "### 3ddoseToSqdose: writing Sqdose file " << argv[2]  << G4endl;
  dosesq->Print(fout);

  delete dosesq;
  delete dose3d;
  fclose(fout);

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

