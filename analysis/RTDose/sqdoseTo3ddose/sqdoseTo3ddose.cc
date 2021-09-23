#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4GeometryManager.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/Gm3ddose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) 
{

  if(argc!=3) { 
    G4Exception("sqdoseTo3ddose",
		"wrong argument",
		FatalErrorInArgument,
		"YOU MUST SUPPLY TWO ARGUMENTS: SQDDOSE_FILE_INPUT 3DDOSE_FILE_OUTPUT");
  }

  G4String fileListName = argv[1];

  G4String fNameOut = argv[2];
  std::ofstream* fout =  new std::ofstream(argv[2]);

  GmSqdose* sqdose;
  G4cout << "### sqdoseTo3ddose: reading sqdose file " << argv[1]  << G4endl;
  sqdose = new GmSqdose();
  sqdose->Read(argv[1]);

  Gm3ddose* dose3d = new Gm3ddose(*sqdose);

  G4cout << "### sqdoseTo3ddose: writing 3ddose file " << argv[2]  << G4endl;
  *fout << *dose3d;

  delete sqdose;
  delete dose3d;
  fout->close();

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

