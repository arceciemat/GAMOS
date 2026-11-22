///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef RacetrackCoilCreator_HH
#define RacetrackCoilCreator_HH 1


#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;


class RacetrackCoilCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   RacetrackCoilCreator();
   ~RacetrackCoilCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* RacetrackCoilCmdDir;
   G4UIcmdWithADoubleAndUnit* SetRoutCmd;
   G4UIcmdWithADoubleAndUnit* SetD1Cmd;
   G4UIcmdWithADoubleAndUnit* SetD2Cmd;
   G4UIcmdWithADoubleAndUnit* SetL1Cmd;
   G4UIcmdWithADoubleAndUnit* SetL2Cmd;
   G4UIcmdWithADoubleAndUnit* SetICmd;
   
   G4double D1;
   G4double D2;
   G4double L1;
   G4double L2;
   G4double Rout;
   G4double I;
   
   

} ;

#endif
