// ----------------------------------------------------------------------------
//                 GEANT 4 - Hadrontherapy example
// ----------------------------------------------------------------------------
// Code developed by:
//
// G.A.P. Cirrone(a)*, F. Di Rosa(a), S. Guatelli(b), G. Russo(a)
// 
// (a) Laboratori Nazionali del Sud 
//     of the National Institute for Nuclear Physics, Catania, Italy
// (b) National Institute for Nuclear Physics Section of Genova, genova, Italy
// 
// * cirrone@lns.infn.it
// ----------------------------------------------------------------------------

#ifndef GmEmDNAPhysicsMessenger_HH
#define GmEmDNAPhysicsMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmEmDNAPhysics;
class G4UIdirectory;
class GmUIcmdWithAString;

class GmEmDNAPhysicsMessenger: public G4UImessenger {

public:
  
  GmEmDNAPhysicsMessenger(GmEmDNAPhysics* physList);
  
  ~GmEmDNAPhysicsMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);
  
private:
  
  GmEmDNAPhysics* theGmPhysicsList;   
  G4UIdirectory* listDir;
  GmUIcmdWithAString* theGmPhysicsListCmd;
  GmUIcmdWithAString* theGmPhysicsListCmdOld;
};

#endif
