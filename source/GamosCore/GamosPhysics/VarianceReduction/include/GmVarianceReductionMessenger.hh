#ifndef GmVarianceReductionMessenger_HH
#define GmVarianceReductionMessenger_HH

#include "globals.hh"
#include "G4UImessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include <map>

class GmPSEMPhysics;
typedef std::pair<G4String,G4double> psd;
typedef std::multimap< G4String, psd > mmspsd;

class GmVarianceReductionMessenger: public G4UImessenger {
public:
  GmVarianceReductionMessenger();
  ~GmVarianceReductionMessenger();
  
  void SetNewValue(G4UIcommand * command,G4String newValues);
  
private:
  void SetImportanceSampling(std::vector<G4String> params );
  //  void XSBiasing(std::vector<G4String> params );

private:
  GmUIcmdWithAString* importanceSamplingCmd;
  //  GmUIcmdWithAString* XSBiasingCmd;
};

#endif
