///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef CurvedSegmentCreator_HH
#define CurvedSegmentCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;


class CurvedSegmentCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   CurvedSegmentCreator();
   ~CurvedSegmentCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* CurvedSegmentCmdDir;
   G4UIcmdWithADoubleAndUnit* SetD1Cmd;
   G4UIcmdWithADoubleAndUnit* SetRINCmd;
   G4UIcmdWithADoubleAndUnit* SetROUTCmd;
   
   G4UIcmdWithADoubleAndUnit* SetPHI0Cmd;
   G4UIcmdWithADoubleAndUnit* SetDPHICmd;
   G4UIcmdWithADoubleAndUnit* SetICmd;
   
   G4double D1,RIN,ROUT,DPHI,PHI0;
   G4double I;
   
   

} ;

#endif
