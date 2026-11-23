#include "G4Run.hh"

#include "GmCutsEnergy2RangeUA.hh"
#include "GmCutsEnergy2Range.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

#include "G4MaterialTable.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4ProcessManager.hh"

#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForPositron.hh"

//-----------------------------------------------------------------------
GmCutsEnergy2RangeUA::GmCutsEnergy2RangeUA()
{
  cutsCmd = new GmUIcmdWithAString("/gamos/physics/ECuts2RangeCuts",this);
  cutsCmd->SetGuidance("Get the range cut corresponding to an energy cut in a material (or ALL materials)");
  cutsCmd->SetParameterName("choice",true);
  cutsCmd->AvailableForStates(G4State_Idle);

  theAction = new GmCutsEnergy2Range;
}

//-----------------------------------------------------------------------
GmCutsEnergy2RangeUA::~GmCutsEnergy2RangeUA()
{
  if (cutsCmd) delete cutsCmd;
}

//-----------------------------------------------------------------------
void GmCutsEnergy2RangeUA::SetNewValue(G4UIcommand * command, G4String newValues)
{
  if (command == cutsCmd) {
  //--- Initialise
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() != 3 ) {
      G4Exception("GmCutsEnergy2RangeUA::SetNewValue",
		  "Error in arguments",FatalErrorInArgument,G4String("Arguments must be  MATERIAL_NAME  CUT_VALUE  PARTICLE_NAME, it is " + newValues).c_str());
    }
    
    if( wl[2] != "gamma" && wl[2] != "e-" && wl[2] != "e+" && wl[2] != "*" && wl[2] != "e*") {
      G4Exception("GmCutsEnergy2RangeUA::SetNewValue",
		  "Error in arguments",FatalErrorInArgument,G4String("Third argument must be gamma, e-, e+, e* or *, it is " + wl[2]).c_str());
    }
    
    std::vector<G4Material*> mates = GmGeometryUtils::GetInstance()->GetMaterials(wl[0]);
    G4double enerCut = GmGenUtils::GetValue( wl[1] );
    G4double rcut;

    for( unsigned int ii = 0; ii < mates.size(); ii++) {
      G4Material* mate = mates[ii];
      G4MaterialCutsCouple * cutsCouple = new G4MaterialCutsCouple(mate);
      G4ProductionCuts* prodCuts = new G4ProductionCuts();
      prodCuts->SetProductionCut(1.*CLHEP::mm,"gamma");
      prodCuts->SetProductionCut(1.*CLHEP::mm,"e-");
      prodCuts->SetProductionCut(1.*CLHEP::mm,"e+");
      cutsCouple->SetProductionCuts( prodCuts ); 
      //---- Build list of gamma processes and G4Track for gamma
      if( wl[2] == "*" || wl[2] == "gamma" ) {
	rcut = theAction->ConvertSlow( enerCut, const_cast<G4Gamma*>(G4Gamma::Gamma()),  cutsCouple );
	G4cout << "GmCutsEnergy2RangeUA:  MATERIAL:  " << mate->GetName() << "  PART: gamma  ENERGY CUT: " << enerCut << " (MeV)  = RANGE CUT: "<< rcut << G4endl;
      }
      if( wl[2] == "*" || wl[2] == "e*" || wl[2] == "e-" ) {
	rcut = theAction->ConvertSlow( enerCut,const_cast<G4Electron*>(G4Electron::Electron()), cutsCouple );
	G4cout << "GmCutsEnergy2RangeUA:  MATERIAL:  " << mate->GetName() << "  PART: e-  ENERGY CUT: " << enerCut << " (MeV)  = RANGE CUT: "<< rcut << G4endl;
      }
      if( wl[2] == "*" || wl[2] == "e*" || wl[2] == "e+" ) {
	rcut = theAction->ConvertSlow( enerCut, const_cast<G4Positron*>(G4Positron::Positron()),  cutsCouple );
	G4cout << "GmCutsEnergy2RangeUA:  MATERIAL:  " << mate->GetName() << "  PART: e+  ENERGY CUT: " << enerCut << " (MeV)  = RANGE CUT: "<< rcut << G4endl;
      }
    }
  }

}
