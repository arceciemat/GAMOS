#include "GmMovementMessenger.hh"
#include "GmMovementDisplacement.hh"
#include "GmMovementRotation.hh"
#include "GmMovementMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UImanager.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

//------------------------------------------------------------------------
GmMovementMessenger::GmMovementMessenger() 
{
  MoveEachTimeCmd = new GmUIcmdWithAString("/gamos/movement/moveEachTime",this);
  MoveEachTimeCmd->SetGuidance("Rotate or displace each N seconds");
  MoveEachTimeCmd->SetParameterName("'rotate/displace' volume_name value axis_x axis_y axis_z time_interval (offset=0) (number_of_intervals=infinite)",false);
  MoveEachTimeCmd->AvailableForStates(G4State_Idle);

  MoveEachNEventsCmd = new GmUIcmdWithAString("/gamos/movement/moveEachNEvents",this);
  MoveEachNEventsCmd->SetGuidance("Rotate or displace each N events");
  MoveEachNEventsCmd->SetParameterName("'rotate/displace' volume_name value axis_x axis_y axis_z NEvents_interval (offset=0) (number_of_intervals=infinite)",false);
  MoveEachNEventsCmd->AvailableForStates(G4State_Idle);
  
  MoveFromFileCmd = new GmUIcmdWithAString("/gamos/movement/moveFromFile",this);
  MoveFromFileCmd->SetGuidance("Execute the movements contained in file");
  MoveFromFileCmd->SetParameterName("file_name",false);
  MoveFromFileCmd->AvailableForStates(G4State_Idle);
}

//------------------------------------------------------------------------
GmMovementMessenger::~GmMovementMessenger() 
{
  if (MoveEachTimeCmd) delete MoveEachTimeCmd;
  if (MoveEachNEventsCmd) delete MoveEachNEventsCmd;
  if (MoveFromFileCmd) delete MoveFromFileCmd;

}

//------------------------------------------------------------------------
void GmMovementMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  std::vector<G4String> words = GmGenUtils::GetWordsInString(newValues);
    
  if (command == MoveEachTimeCmd || command == MoveEachNEventsCmd) {
    if( words.size() < 5 ){
      G4Exception("GmMovementMessenger::SetNewValue",
		  "Wrong number of arguments",
		  FatalErrorInArgument,
		  G4String(command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 7 arguments (plus two optional): 'rotate/displace' VOLUME_NAME VALUE AXIS_X AXIS_Y AXIS_Z NEVENTS_INTERVAL OFFSET NUMBER_OF_INTERVALS").c_str());
    }
    if( words[0] == "displace" ){
      if( command == MoveEachTimeCmd ){
	new GmMovementDisplacement( words, AfterTime ); 
      } else if( command == MoveEachNEventsCmd ){
	new GmMovementDisplacement( words, AfterNEvents ); 
      }
    } else if( words[0] == "rotate" ){
      if( command == MoveEachTimeCmd ){
	new GmMovementRotation( words, AfterTime ); 
      } else if( command == MoveEachNEventsCmd ){
	new GmMovementRotation( words, AfterNEvents );
      }
    } else {
      G4Exception("GmMovementMessenger::SetNewValue","ERROR",FatalErrorInArgument,"second argument must be rotate/displace");
    }
  } else if (command == MoveFromFileCmd ) {
    if( words.size() != 1 ){
      G4Exception("GmMovementMessenger::SetNewValue",
		  "Wrong number of arguments",
		  FatalErrorInArgument,
		  G4String(command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs only 1 argument: FILE_NAME").c_str());
    }

    GmMovementMgr::GetInstance()->ReadMovementFile( newValues ); 

  }
}
