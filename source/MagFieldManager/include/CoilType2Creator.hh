///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef CoilType2Creator_HH
#define CoilType2Creator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWithADoubleAndUnit;
class G4UIdirectory;


class CoilType2Creator : public ParametrizedFieldCreator 
{ 
  public:
  
   CoilType2Creator();
   ~CoilType2Creator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* CoilType2CmdDir;
   G4UIcmdWithADoubleAndUnit* SetRoutCornerCmd;
   G4UIcmdWithADoubleAndUnit* SetD1Cmd;
   G4UIcmdWithADoubleAndUnit* SetD2Cmd;
   G4UIcmdWithADoubleAndUnit* SetLVertLeftCmd;
   G4UIcmdWithADoubleAndUnit* SetLVertRightCmd;
   G4UIcmdWithADoubleAndUnit* SetLHorizUpCmd;
   G4UIcmdWithADoubleAndUnit* SetLHorizDownCmd;
   
   G4UIcmdWithADoubleAndUnit* SetICmd;
   
   G4double D1;
   G4double D2;
   G4double LVertLeft;
   G4double LVertRight;
   G4double LHorizUp;
   G4double LHorizDown;
   G4double RoutCorner;
   G4double I;
   
   
   
   

} ;

#endif
