#ifndef GmBiasingMgr_h
#define GmBiasingMgr_h

/*---------------------------------------------------------------------------   
ClassName:   GmBiasingMgr    
Author:      P. Arce
Changes:     03/2016: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*! Manager for biasing */

#include "G4ThreeVector.hh"

#include <vector>
#include <map>
class GmVBiasingOperator;
class GmGenericBiasingPhysics;

//------------------------------------------------------------------------
class GmBiasingMgr 
{ 
private:
  GmBiasingMgr();
 public:
  ~GmBiasingMgr();
  static GmBiasingMgr* GetInstance();

  void CreateOperator(std::vector<G4String> params );
  void AssocOper2LogVol(std::vector<G4String> params );
  void AddProcesses2Oper(std::vector<G4String> params );
  void AddParticles2Oper(std::vector<G4String> params );
  void AddParticleProcesses2Oper(std::vector<G4String> params );
  void AddSplittingProcess( std::vector<G4String> params);
  
private:
  static GmBiasingMgr* theInstance;
  std::map<G4String,GmVBiasingOperator*> theOperators;
  GmGenericBiasingPhysics* theBiasingPhysics;
};

#endif
