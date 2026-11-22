///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef UniformFieldCreator_HH
#define UniformFieldCreator_HH 1


#include"globals.hh"
#include"G4ios.hh"
#include"ParametrizedFieldCreator.hh"

class G4UIcmdWith3VectorAndUnit;
class G4UIdirectory;


class UniformFieldCreator : public ParametrizedFieldCreator 
{ 
  public:
  
   UniformFieldCreator();
   ~UniformFieldCreator();
  
  public: //methods
  
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger);
   G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string);
   virtual G4MagneticField* CreateNewField();
  
  private:
   
   G4UIcmdWith3VectorAndUnit* theCmd;
   G4UIdirectory* UniformFieldDir;
   G4ThreeVector B_vec;
} ;

#endif
