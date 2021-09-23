///////////////////////////////////////////////////////////////////////////////
// File: RTPlanMessenger.hh
//
///////////////////////////////////////////////////////////////////////////////

#ifndef RTPlanMessenger_h
#define RTPlanMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RTVPlanSource;

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

class RTPlanMessenger: public G4UImessenger {
public:
  RTPlanMessenger(RTVPlanSource*);
  ~RTPlanMessenger();
  
  void SetNewValue(G4UIcommand * command,G4String newValues);

private:
  RTVPlanSource* myAction;
  
  GmUIcmdWithAString* AssociatePlanVolumesCmd;
  GmUIcmdWithAString* DefineAcceleratorVolumeCmd;

};

#endif
