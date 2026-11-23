///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef ParametrizedFieldCreator_HH
#define ParametrizedFieldCreator_HH 1 


#include "globals.hh"
#include"G4ios.hh"
#include "G4MagneticField.hh"
#include "G4UIcommand.hh"
#include "G4UImessenger.hh"

class ParametrizedFieldCreator 
{ 
  public:
  
   ParametrizedFieldCreator(G4String ModelName);
   ~ParametrizedFieldCreator();
  
  public: //methods
   
   
   virtual void CreateMacroCommands(G4String cmd_dir_name, G4UImessenger* aMessenger)=0;
   virtual G4bool DefineParameters(G4UIcommand* aCmd , G4String parameter_string)=0;
   virtual G4MagneticField* CreateNewField()=0;
   inline G4int GetNbFieldsOfSameModel(){return nb_fields_of_same_model;};
   inline G4String GetModelName(){return theModelName;}
  
  protected:
   G4int  nb_fields_of_same_model; 
   G4String theModelName; 
   
 
};
#endif
