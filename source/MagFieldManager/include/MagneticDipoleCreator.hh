///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef MagneticDipoleCreator_HH
#define MagneticDipoleCreator_HH 1
#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWith3VectorAndUnit;
class G4UIdirectory;


class MagneticDipoleCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   MagneticDipoleCreator();
   ~MagneticDipoleCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
   
  private :
   
   G4UIdirectory* MagneticDipoleCmdDir;
   G4UIcmdWith3VectorAndUnit* SetDipoleMomentCmd;
   G4UIcmdWith3VectorAndUnit* SetDipoleCenterCmd;
   G4ThreeVector theDipoleMoment;
   G4ThreeVector theDipoleCenter; 
   
   
 
   
} ;

#endif
