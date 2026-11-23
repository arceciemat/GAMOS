#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
//#include "GamosCore/GamosBase/include/GmVisManager.hh"
#include "G4VisExecutive.hh"
#endif

#include "G4GeometryManager.hh"
#include "GamosCore/GamosReadDICOM/include/Gm3ddose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {


  if(argc!=3) { 
    G4Exception("!!! FATAL ERROR: YOU MUST SUPPLY TWO ARGUMENTS: FILE_WITH_LIST_OF_3DDOSE_FILES FILE_OUTPUT");
  }

  G4String fileListName = argv[1];

  G4String fNameOut = argv[2];
  std::ofstream* fout = new std::ofstream(fNameOut);

  std::vector<G4String> wl;
  GmFileIn finlis = GmFileIn::GetInstance(fileListName);

  Gm3ddose dose, dose2;
  G4int ii;
  for( ii = 0;; ii++){
    if( ! finlis.GetWordsInLine(wl) ) break;      
    G4cout << "### Merge3ddoseUA: reading dose file " << wl[0]  << G4endl;
    if( ii == 0 ) {
      dose = Gm3ddose();
      dose.Read(wl[0]);
    } else {
      dose2 = Gm3ddose();
      dose2.Read(wl[0]);
      G4cout << " adding doses " << G4endl;
      dose += dose2;
    }
  }

  G4cout << "$$$$$ MERGED " << ii-1 << " files into " << fNameOut << G4endl;
  *fout << dose;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

