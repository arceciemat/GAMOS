#include "GmUserLimitsMessenger.hh"
#include "GmUserLimitsMessenger.hh"
#include "GmUserLimitsMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmRegionCutsMgr.hh"

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ProductionCutsTable.hh"

GmUserLimitsMessenger::GmUserLimitsMessenger() 
{
  theAction = GmUserLimitsMgr::GetInstance();

  regionCmd = new GmUIcmdWithAString("/gamos/geometry/createRegion",this);
  regionCmd->SetGuidance("Creates a Region from one of various logical volumes");
  regionCmd->SetParameterName("choice",true);
  regionCmd->AvailableForStates(G4State_PreInit);

  prodCutsCmd = new GmUIcmdWithAString("/gamos/physics/setCuts",this);
  prodCutsCmd->SetGuidance("Creates a Region from one of various logical volumes");
  prodCutsCmd->SetParameterName("choice",true);
  prodCutsCmd->AvailableForStates(G4State_PreInit);

  userLimitsCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/setUserLimits",this);
  userLimitsCmd->SetGuidance("Creates a G4UserLimits with the five limits. It must have as argument its name, logical volume, particle name, maxStep, maxTrkLen, maxTOF, minEKin, minRange");
  userLimitsCmd->SetParameterName("choice",true);
  userLimitsCmd->AvailableForStates(G4State_Idle);

  maxStepCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/setMaxStep",this);
  maxStepCmd->SetGuidance("Creates a G4UserLimits with maximum step size limit. It must have as argument its name, logical volume, particle name, maxStep");
  maxStepCmd->SetParameterName("choice",true);
  maxStepCmd->AvailableForStates(G4State_Idle);

  maxTrkLenCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/setMaxTrkLen",this);
  maxTrkLenCmd->SetGuidance("Creates a G4UserLimits with maximum track length limit. It must have as argument its name, logical volume, particle name, maxTrkLen");
  maxTrkLenCmd->SetParameterName("choice",true);
  maxTrkLenCmd->AvailableForStates(G4State_Idle);

  maxTOFCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/setMaxTOF",this);
  maxTOFCmd->SetGuidance("Creates a G4UserLimits with the maximum time of flight limit. It must have as argument its name, logical volume, particle name, maxTOF");
  maxTOFCmd->SetParameterName("choice",true);
  maxTOFCmd->AvailableForStates(G4State_Idle);

  minEKinCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/setMinEKin",this);
  minEKinCmd->SetGuidance("Creates a G4UserLimits with minimum kinetic energy limit. It must have as argument its name, logical volume, particle name, minEKin");
  minEKinCmd->SetParameterName("choice",true);
  minEKinCmd->AvailableForStates(G4State_Idle);

  minEKinByRangeCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/setMinEKinByRange",this);
  minEKinByRangeCmd->SetGuidance("Creates a G4UserLimits with minimum kinetic energy limit. It must have as argument its name, logical volume, particle name, minEKinByRange");
  minEKinByRangeCmd->SetParameterName("choice",true);
  minEKinByRangeCmd->AvailableForStates(G4State_Idle);

  minRangeCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/setMinRange",this);
  minRangeCmd->SetGuidance("Creates a G4UserLimits with minimum remaining range limit. It must have as argument its name, logical volume, particle name, minRange");
  minRangeCmd->SetParameterName("choice",true);
  minRangeCmd->AvailableForStates(G4State_Idle);

  addLVParticleCmd = new GmUIcmdWithAString("/gamos/physics/userLimits/addLVAndParticle",this);
  addLVParticleCmd->SetGuidance("Add a logical volume / particles to an existing G4UserLimits. It must have as argument the G4UserLimits name, the logical volume name, the particle name");
  addLVParticleCmd->SetParameterName("choice",true);
  addLVParticleCmd->AvailableForStates(G4State_Idle);

  printCmd = new G4UIcmdWithoutParameter("/gamos/physics/userLimits/print",this);
  printCmd->SetGuidance("Print list of user limits for each logical volume and particle");
  printCmd->AvailableForStates(G4State_Idle);

  prodCutsLimitsCmd = new GmUIcmdWithAString("/gamos/physics/prodCutsEnergyLimits",this);
  prodCutsLimitsCmd->SetGuidance("Changes the default values: MIN (1 keV) and MAX (100 GeV) for the limits of production cuts tables ");
  prodCutsLimitsCmd->SetParameterName("choice",true);
  prodCutsLimitsCmd->AvailableForStates(G4State_PreInit);

}

GmUserLimitsMessenger::~GmUserLimitsMessenger() 
{
  if (regionCmd) delete regionCmd;
  if (prodCutsCmd) delete prodCutsCmd;
  if (userLimitsCmd) delete userLimitsCmd;
  if (maxStepCmd) delete maxStepCmd;
  if (maxTrkLenCmd) delete maxTrkLenCmd;
  if (maxTOFCmd) delete maxTOFCmd;
  if (minEKinCmd) delete minEKinCmd;
  if (minEKinByRangeCmd) delete minEKinByRangeCmd;
  if (minRangeCmd) delete minRangeCmd;
  if (addLVParticleCmd) delete addLVParticleCmd;
  if (prodCutsLimitsCmd) delete prodCutsLimitsCmd;

}

void GmUserLimitsMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
  
  if (command == regionCmd) {
    GmRegionCutsMgr::GetInstance()->AddRegionData( wl );

  } else if (command == prodCutsCmd) {
    GmRegionCutsMgr::GetInstance()->AddRegionCuts( wl );

  } else if (command == userLimitsCmd) {
    GmGenUtils::CheckNWords(newValues,8,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 8 arguments: name, logical volume, particle name, maxStep, maxTrkLen, maxTOF, minEKin, minRange"); 

    theAction->CreateUserLimitsAll( wl );

  } else if (command == maxStepCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: name, logical volume, particle name, maxStep"); 

    theAction->CreateUserLimitsMaxStep( wl );

  } else if (command == maxTrkLenCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: name, logical volume, particle name, maxTrkLen"); 

    theAction->CreateUserLimitsMaxTrkLen( wl );

  } else if (command == maxTOFCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: name, logical volume, particle name, maxTOF"); 

    theAction->CreateUserLimitsMaxTOF( wl );

  } else if (command == minEKinCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: name, logical volume, particle name, minEKin"); 

    theAction->CreateUserLimitsMinEKin( wl );

  } else if (command == minEKinByRangeCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: name, logical volume, particle name, minRange"); 

    theAction->CreateUserLimitsMinEKinByRange( wl );

  } else if (command == minRangeCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: name, logical volume, particle name, minRange"); 

    theAction->CreateUserLimitsMinRange( wl );

  } else if (command == addLVParticleCmd) {
    GmGenUtils::CheckNWords(newValues,3,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: name, logical volume, particle name"); 

    theAction->AddLVAndParticle( wl );

  } else if (command == printCmd) {
    theAction->PrintUserLimits( G4cout );

  } else if (command == prodCutsLimitsCmd) {
    if( wl.size() != 2 ) {
      G4Exception("Command /gamos/physics/minCutEnergyLimit",
		  "Wrong number of arguments",
		  FatalErrorInArgument,
		  G4String("There must be two arguments: ENERGY_MIN ENERGY_MAX, while there are " + GmGenUtils::itoa(wl.size())).c_str());
    }

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(GmGenUtils::GetValue(wl[0]), GmGenUtils::GetValue(wl[1]));
  }
}
