///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef TransformationManager_h
#define TransformationManager_h 1

#include"globals.hh"
#include"G4ios.hh"
#include"G4AffineTransform.hh"
#include<vector> 


class TransManMessenger;

class TransformationManager 
{ public:
  
   TransformationManager();
   virtual ~TransformationManager();
   
   static TransformationManager* GetInstance();
   
 
  public: 
   
   G4AffineTransform* GetTransformation(G4String TransformationName);
   void SelectTransformation(G4String TransformationName);
   void ResetToUnitary();
   void AddRotationAroundX(G4double angle);
   void AddRotationAroundY(G4double angle);
   void AddRotationAroundZ(G4double angle);
   void AddRotationAroundAnAxis(G4double angle, G4ThreeVector anAxis);
   void AddTranslation(G4ThreeVector aVec);
   void MultiplyTransformations(G4String name1, G4String name2);
   void MultiplyBy(G4String name1);
   
   void SaveTransformation(G4String name);
   void ClearListOfTransformations();
  // void RemoveATransformation(G4String name);
  
  private:
 
   static TransformationManager* instance;
 
 //attributes
   
   //List of Transformation
   std::vector<G4String> listOfTransformationNames;
   std::vector<G4AffineTransform*> listOfTransformations;
   
   G4AffineTransform theTransformation;
 
   
   TransManMessenger* theMessenger;
  

   
   
};
#endif
