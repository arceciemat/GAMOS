///////////////////////////////////////////////////////////////////////////////
// File: GmGeneratorMessenger.hh
//
// Description: Adds commands for listing, enabling and disabling user actions
// Modification: 
///////////////////////////////////////////////////////////////////////////////

#ifndef GmGeneratorMessenger_h
#define GmGeneratorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmGenerator;

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

class GmGeneratorMessenger: public G4UImessenger {
public:
  GmGeneratorMessenger(GmGenerator*);
  ~GmGeneratorMessenger();
  
  void SetNewValue(G4UIcommand * command,G4String newValues);

private:
  GmGenerator* myAction;
  
  GmUIcmdWithAString* AddSingleParticleCmd;
  GmUIcmdWithAString* AddIsotopeCmd;
  GmUIcmdWithAString* AddDoubleBackToBackParticleCmd;
  GmUIcmdWithAString* AddRTPlanCmd;
  GmUIcmdWithAString* AddRTIonPlanScanSpotCmd;
  GmUIcmdWithAString* AddFromTextFileCmd;

  GmUIcmdWithAString* DistTimeCmd;
  GmUIcmdWithAString* DistEnergyCmd;
  GmUIcmdWithAString* DistPositionCmd;
  GmUIcmdWithAString* DistDirectionCmd;
  GmUIcmdWithAString* DistPositionDirectionCmd;
  GmUIcmdWithAString* DirectionCmd;
  G4UIcmdWithADoubleAndUnit* PositionCmd;
  G4UIcmdWithADoubleAndUnit* EnergyCmd;
  G4UIcmdWithADoubleAndUnit* ActivityCmd;
  GmUIcmdWithAString* PositionAddVolumeCmd;
  GmUIcmdWithAString* DistributionCmd;
  GmUIcmdWithAString* createIonCmd;

 };

#endif
