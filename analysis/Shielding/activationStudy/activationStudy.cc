#include "G4ions.hh"
#include <fstream>
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "CLHEP/Units/SystemOfUnits.h"
int verbose;
std::vector<G4Ions*> theIons;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

int main(int argc,char** argv) 
{
  verbose = 3;

  if(argc != 3 ) { 
    G4Exception("!!! FATAL ERROR: YOU MUST SUPPLY TWO ARGUMENT: INPUT_FILE  OUTPUT_FILE");
  }

  //Read the file
  G4String fileName = argv[1];
   
  G4String fNameOut = argv[2];
  std::ofstream fout(fNameOut);
  
  std::vector<G4String> wl;
  std::vector<G4String> w2;
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  G4cout << "@@@@@@@ Reading FILE: " << wl[0]  << G4endl;
      
  G4double nevents = 0.;
  //G4double theNEvents ;
  
  G4int ii;
  for( ii = 0;; ii++)  {
    if( ! fin.GetWordsInLine(wl) ) break;      

    if( wl.size != 2 ) {
      G4cerr << " !! ERROR reading line: ";
      for( G4int jj = 0; jj < wl.size(); jj++ ) {
	G4cerr << wl[jj] << " ";
      }
      G4Exception("Wrong line, it must contain two words: ION_NAME ION_INTENSITY");
    }

    G4Ion* ion = CreateIon( wl[0] );    
    theIons[ion] = GetValue( wl[1] );  
  
  }
    

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....


//------------------------------------------------------------------------
G4Ions* GmGeneratorMessenger::CreateIon( const G4String& name )
{
  G4int atomicNumber;
  G4int atomicMass;
  G4double energy;
  // no excitation number supported  G4int excitNumber;
  size_t ipar = name.find("[");
  G4String isotName = name;
  if( ipar != std::string::npos ) {
    energy = 0.;
  } else {
    size_t ipar2 = name.find("]");
    if( ipar2 == std::string::npos ) {
      G4Exception("CreateIon",
		  "Error in ion name, energy must be between square brackets",
		  FatalErrorInArgument,
		  G4String("ION= " + name).c_str());
    }
    energy = GetValue(name.substr(ipar+1,ipar2-ipar-1));
    isotName = name.substr(0,ipar);
  }

  G4int ii = 0;
  for( ; ii < isotName.length(); ii++ ){
    if( G4tgrUtils::IsNumber(isotName[ii] ) ) {
      std::map<G4String,G4int>::const_iterator ite = theAtomicNumbers.find(isotName.substr(0,ii));
      if( ite == theAtomicNumbers.end() ){
	G4Exception("CreateIon",
		    "Error in ion name, isotope name not found",
		    FatalErrorInArgument,
		    G4String("ION= " + name).c_str());
      }
      atomicNumber = (*ite).second;
      atomicMass = GetValue( isotName.substr(ii,isotName.length()));
    }
  }
  if( ii == isotName.length() ) {
    G4Exception("CreateIon",
		"Error in ion name, isotope mass not found",
		FatalErrorInArgument,
		G4String("ION= " + name).c_str());
  }

  G4IonTable * iontab=
    (G4IonTable *)(G4ParticleTable::GetParticleTable()->GetIonTable());
  G4ParticleDefinition* partnew = iontab->GetIon(atomicNumber,atomicMass,energy);

  G4Ions* ionnew = dynamic_cast<G4Ions*>(partnew);
#ifndef GAMOS_NO_VERBOSE
  if( verbose > 1 ) G4cout << " GmGeneratorMessenger::CreateIon " << ionnew->GetParticleName() 
				   << " atomic number= " << ionnew->GetAtomicNumber() 
				   << " atomic mass= " << ionnew->GetAtomicMass() 
				   << " excitation energy= " << ionnew->GetExcitationEnergy()/CLHEP::keV 
    //				   << " spin= " << ionnew->GetPDGiSpin() 
				   << G4endl;
#endif
  
  return ionnew;

}
