///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef JCurrentCalculator_h
#define JCurrentCalculator_h 1
#include"globals.hh"
#include"G4ios.hh"
#include"G4MagneticField.hh"
#include<vector>
#include "G4ThreeVector.hh" 


class MagFieldModManMessenger;



class JCurrentCalculator 
{ public:
  
   JCurrentCalculator();
   virtual ~JCurrentCalculator();
   
   static JCurrentCalculator* GetInstance();
   
   
  public: 
  
  
  //field
   
  void SetField(G4MagneticField* aField);
  G4ThreeVector CalculateJCurrent(G4ThreeVector pos,G4double t,G4double ds);
  void CalculateJCurrentBetweenTwoPositions(G4ThreeVector pos1,G4ThreeVector pos2,G4double t, G4int npos,G4double ds, G4String output_file_name);
  void CalculateJCurrentOnACartesianGrid(G4double x0,G4double  dx, G4int nX,
					 G4double y0,G4double  dy, G4int nY,
					 G4double z0,G4double  dz, G4int nZ,G4double ds,G4double t,
					 G4String file_output);

 
 
 private:
 
   static JCurrentCalculator* instance;
 
 //attributes
   
   G4MagneticField* theField;
   
  

   
   
};
#endif
