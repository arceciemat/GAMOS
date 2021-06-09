#include "GmChangeEnergies.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4tgrFileIn.hh"
#include "CLHEP/Units/SystemOfUnits.h"

//------------------------------------------------------------------
GmChangeEnergies::GmChangeEnergies()
{
}

//------------------------------------------------------------------
void GmChangeEnergies::GetEnergyParameters(G4String name)
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();

  theMinE = parMgr->GetNumericValue(name+":minE",1.E-12*CLHEP::MeV);
  theMaxE = parMgr->GetNumericValue(name+":maxE",1.E+2*CLHEP::MeV);
  theNStepsE = (unsigned int)(parMgr->GetNumericValue(name+":nStepsE",15));
  bLogE = G4bool(parMgr->GetNumericValue(name+":logE",1));

  theEnergyFileName = parMgr->GetStringValue(name+":EnergiesFileName","");
  if( theEnergyFileName == "" ) {
    bEnergyFromFile = false;
  } else {
    bEnergyFromFile = true;
  }

  BuildEnergies();

}

//------------------------------------------------------------------
void GmChangeEnergies::BuildEnergies()
{

  if( bEnergyFromFile ) {
    G4tgrFileIn fin = G4tgrFileIn::GetInstance(theEnergyFileName);
    std::vector<G4String> wl;
    for(G4int ii = 0; ;ii++){
      if( !fin.GetWordsInLine(wl) ) break;
      if( wl.size() != 1 ) {
	G4Exception("GmGeneratroChangeEnergyAndMaterial::BuildSources",
		    "Too many columns in energy file",
		    FatalErrorInArgument,
		    "Only one energy per line is allowed");
      }
      theEnergies.push_back( GmGenUtils::GetValue(wl[0]) );
      //-      G4cout << " ENER " <<  wl[0]  << G4endl;
    }

    theNStepsE = theEnergies.size();

  } else {
    if( !bLogE ) {
      if( theNStepsE == 1 ) {
	theStepE = 0.;
      } else {
	theStepE = (theMaxE-theMinE)/(theNStepsE-1);
      }
      for( unsigned int ii = 0; ii < theNStepsE; ii++) {
	theEnergies.push_back( theMinE + ii*theStepE );
	//-	G4cout << " ENER " <<  theMinE + ii * theStepE  << G4endl;
      }
    } else {
      if( theNStepsE == 1 ) {
	theStepE = 1.;
      } else {
	theStepE = pow(theMaxE/theMinE, 1./(theNStepsE-1));
      }
      for( unsigned int ii = 0; ii < theNStepsE; ii++) {
	theEnergies.push_back( theMinE * pow( theStepE, int(ii) ) );
	//-	G4cout << " ENER " <<  theMinE * pow( theStepE, ii )  << G4endl;
      }
    }
  }

}

