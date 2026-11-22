#include"TransformationManager.hh"
#include"G4RotationMatrix.hh"
#include"TransManMessenger.hh"
#include "CLHEP/Geometry/BasicVector3D.h"

TransformationManager* TransformationManager::instance = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
//
TransformationManager::TransformationManager()
{ theTransformation =G4AffineTransform();
  listOfTransformations.clear();
  listOfTransformationNames.clear();
  
  theMessenger = new TransManMessenger(this);
}
///////////////////////////////////////////////////////////////////////
TransformationManager::~TransformationManager()
{; 
}
////////////////////////////////////////////////////////////////////////////////
//
TransformationManager* TransformationManager::GetInstance()
{ 
  if (!instance) instance = new TransformationManager();
  return instance;
}
////////////////////////////////////////////////////////////////////////////////
//
G4AffineTransform*  TransformationManager::GetTransformation(G4String TransformationName)
{ G4AffineTransform* aTransformation=0;
  for (size_t i=0; i<listOfTransformations.size(); i++){
  	if (listOfTransformationNames[i] == TransformationName){
		aTransformation = listOfTransformations[i];
		break;
	}
  
  }
  if (!aTransformation){
  	G4cout<<"The coordinate transformation that you have selected does not exist!"<<std::endl; 
  }
  return aTransformation;
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::SelectTransformation(G4String TransformationName)
{ G4AffineTransform* aTransformation=0;
  for (size_t i=0; i<listOfTransformations.size(); i++){
  	if (listOfTransformationNames[i] == TransformationName){
		aTransformation = listOfTransformations[i];
		break;
	}
  
  }
  if (!aTransformation){
  	G4cout<<"The coordinate transformation that you have selected does not exist!"<<std::endl; 
  }
  else {
  	theTransformation = *(aTransformation);
  }
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::ResetToUnitary()
{ 
  theTransformation =G4AffineTransform();
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::AddRotationAroundX(G4double angle)
{ 
  G4RotationMatrix aRot = G4RotationMatrix();//To check the directiom of transformation
  aRot.set(G4ThreeVector(1.,0.,0.),-angle);
  theTransformation *= G4AffineTransform(aRot);
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::AddRotationAroundY(G4double angle)
{
  G4RotationMatrix aRot = G4RotationMatrix();//To check the direction of transformation
  aRot.set(G4ThreeVector(0.,1.,0.),-angle);
  theTransformation *= G4AffineTransform(aRot);
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::AddRotationAroundZ(G4double angle)
{ 
  G4RotationMatrix aRot = G4RotationMatrix();//To check the direction of transformation
  aRot.set(G4ThreeVector(0.,0.,1.),-angle);
  theTransformation *= G4AffineTransform(aRot);
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::AddRotationAroundAnAxis(G4double angle, G4ThreeVector anAxis)
{ 
  G4RotationMatrix aRot = G4RotationMatrix();//To check the direction of transformation
  aRot.set(anAxis,-angle);
  theTransformation *= G4AffineTransform(aRot);
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::AddTranslation(G4ThreeVector aVec) 
{ theTransformation *= G4AffineTransform(-aVec); //To check the direction of transformation
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::MultiplyTransformations(G4String name1, G4String name2)
{ G4AffineTransform* trans1= GetTransformation(name1);
  G4AffineTransform* trans2= GetTransformation(name2);
  if (trans1 && trans2)	theTransformation.Product(*trans1, *trans2);  
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::MultiplyBy(G4String name)
{ G4AffineTransform* trans= GetTransformation(name);
  if (trans){
  	theTransformation *= *(trans);
  }
  
}
////////////////////////////////////////////////////////////////////////////////
//
   
void  TransformationManager::SaveTransformation(G4String name)
{ G4bool did_find =false;
  for (size_t i=0; i<listOfTransformations.size(); i++){
  	if (listOfTransformationNames[i] == name){
		did_find =true;
		break;
	}
  
  }
  if (did_find){
  	G4cout<<"A transformation with the name that you have specified exist already!"<<std::endl; 
  }
  else {
  	G4AffineTransform* trans = new G4AffineTransform();
	(*trans) = theTransformation;
	//G4cout<<name<<std::endl;
	listOfTransformationNames.push_back(name);
	listOfTransformations.push_back(trans);
	
  }
  
}
////////////////////////////////////////////////////////////////////////////////
//
void  TransformationManager::ClearListOfTransformations()
{for (size_t i=0; i<listOfTransformations.size(); i++){
  	G4AffineTransform* trans = listOfTransformations[i];
	delete trans;
  
 }
 listOfTransformations.clear();
 listOfTransformationNames.clear();
}
////////////////////////////////////////////////////////////////////////////////
//
/*void  TransformationManager::RemoveATransformation(G4String name)
{;
}
*/
////////////////////////////////////////////////////////////////////////////////
//
