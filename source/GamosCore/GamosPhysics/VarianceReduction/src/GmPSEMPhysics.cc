#include "GmPSEMPhysics.hh"

#include "GmPhysicsGammaEWPSLowEner.hh"
#include "GmPhysicsGammaEWPSStandard.hh"
#include "GmPhysicsPositronUBSStandard.hh"
#include "GmPhysicsPositronZBSStandard.hh"
#include "GmPhysicsPositronEWPSStandard.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsGammaStandard.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsPositronStandard.hh"
#include "GmPhysicsElectronEWPSLowEner.hh"
#include "GmPhysicsElectronUBSLowEner.hh"
#include "GmPhysicsElectronZBSLowEner.hh"

#include "GmPhysicsElectronLowEnerDeexSplit.hh"
#include "GmPhysicsElectronPenelopeDeexSplit.hh"
#include "GmPhysicsGammaLowEnerDeexSplit.hh"
#include "GmPhysicsGammaPenelopeDeexSplit.hh"

#include "GmPhysicsElectronLowEnerUBSDeexSplit.hh"
#include "GmPhysicsElectronLowEnerZBSDeexSplit.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

GmPSEMPhysics::GmPSEMPhysics(): GmEMPhysics()
{
  // The threshold of production of secondaries is fixed to 10. mm
  // for all the particles, in all the experimental set-up
  defaultCutValue = 0.1 * mm;
  SetVerboseLevel(1);
  ConstructParticles();

}

GmPSEMPhysics::~GmPSEMPhysics()
{ 
}


void GmPSEMPhysics::ConstructParticles()
{
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
}

G4bool GmPSEMPhysics::ReplacePhysicsList(const G4String& name)
{
  G4bool bFound = GmEMPhysics::ReplacePhysicsList(name);

  if( bFound ) return bFound; 

  bFound = true;
  G4cout << "GmpSEMPhysics::Replacing PhysicsList component " << name << G4endl;
  
  if( name == "electron-lowener-UBS" ){
    if (electronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- electron List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsElectronUBSLowEner("electron-lowener-UBS") );
      electronIsRegistered = true;
    }
  } else if( name == "electron-lowener-ZBS" ){
    if (electronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- electron List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsElectronZBSLowEner("electron-lowener-ZBS") );
      electronIsRegistered = true;
    }
  } else if( name == "electron-lowener-EWPS" ){
    if (electronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- electron List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsElectronEWPSLowEner("electron-lowener-EWPS") );
      electronIsRegistered = true;
    }
  } else if( name == "electron-lowener-DeexSplit" ){
    if (electronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- electron List already existing"                  
	     << G4endl;
    }  
    else 
      {
	G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	       << " is registered" << G4endl;
	RegisterPhysics( new GmPhysicsElectronLowEnerDeexSplit("electron-lowener-DeexSplit") );
      electronIsRegistered = true;
      } 
  } else if( name == "electron-penelope-DeexSplit" ){
    if (electronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- electron List already existing"                  
	     << G4endl;
    }  
    else 
      {
	G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	       << " is registered" << G4endl;
	RegisterPhysics( new GmPhysicsElectronPenelopeDeexSplit("electron-penelope-DeexSplit") );
      electronIsRegistered = true;
      } 
  } else if( name == "electron-lowener-UBS-DeexSplit" ){
    if (electronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- electron List already existing"                  
	     << G4endl;
    }  
    else 
      {
	G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	       << " is registered" << G4endl;
	RegisterPhysics( new GmPhysicsElectronLowEnerUBSDeexSplit("electron-lowener-DeexSplit") );
      electronIsRegistered = true;
      } 
  } else if( name == "electron-lowener-ZBS-DeexSplit" ){
    if (electronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- electron List already existing"                  
	     << G4endl;
    }  
    else 
      {
	G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	       << " is registered" << G4endl;
	RegisterPhysics( new GmPhysicsElectronLowEnerZBSDeexSplit("electron-lowener-DeexSplit") );
      electronIsRegistered = true;
      } 

  } else if( name == "positron-standard-UBS" ){
    if (positronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- positron List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsPositronUBSStandard("positron-standard-EWPS") );
      positronIsRegistered = true;
    }
  } else if( name == "positron-standard-ZBS" ){
    if (positronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- positron List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsPositronZBSStandard("positron-standard-EWPS") );
      positronIsRegistered = true;
    }
  } else if( name == "positron-standard-EWPS" ){
    if (positronIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- positron List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsPositronEWPSStandard("positron-standard-EWPS") );
      positronIsRegistered = true;
    }
  } else if( name == "gamma-standard-EWPS" ){
    if (gammaIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- gamma List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsGammaEWPSStandard("gamma-standard-EWPS") );
      gammaIsRegistered = true;
    }

  } else if( name == "gamma-lowener-EWPS" ){
    if (gammaIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- gamma List already existing"                  
	     << G4endl;
    } 
    else 
    {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " is registered" << G4endl;
      RegisterPhysics( new GmPhysicsGammaEWPSLowEner("gamma-lowener-EWPS") );
      gammaIsRegistered = true;
    }
    
  } else if( name == "gamma-lowener-DeexSplit" ){
    if (gammaIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- gamma List already existing"                  
	     << G4endl;
    } 
    else 
      {
	G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	       << " is registered" << G4endl;
	RegisterPhysics( new GmPhysicsGammaLowEnerDeexSplit("gamma-lowener-DeexSplit") );
	gammaIsRegistered = true;
      }
  } else if( name == "gamma-penelope-DeexSplit" ){
    if (gammaIsRegistered) {
      G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	     << " cannot be registered ---- gamma List already existing"                  
	     << G4endl;
    } 
    else 
      {
	G4cout << "GmPSEMPhysics::ReplacePhysicsList: " << name 
	       << " is registered" << G4endl;
	RegisterPhysics( new GmPhysicsGammaPenelopeDeexSplit("gamma-penelope-DeexSplit") );
	gammaIsRegistered = true;
      }

  } else {
    bFound = false;
  }

  return bFound;

}
                                                                                
