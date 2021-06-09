#ifndef GmGeometryMessenger_h
#define GmGeometryMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class GmUIcmdWithAString;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmGeometryMessenger: public G4UImessenger
{
public:
  GmGeometryMessenger();
  ~GmGeometryMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);
  
private:
  void SetMagField(const G4String& newValues);
  void SetElecField(const G4String& newValues);
  void SetEMField(const G4String& newValues);
  void SetMagFieldLocal(const G4String& newValues);
  void SetRefractionIndex(const G4String& newValues);
  void PrintTouchables(const G4String& newValues); 
  void PrintVolumeTree(const G4String& newValues);
  void PrintMaterials(const G4String& newValues);

private:
  G4UIdirectory*  theMagFieldDir;
  GmUIcmdWithAString* theMagFieldCmd;
  GmUIcmdWithAString* theElecFieldCmd;
  GmUIcmdWithAString* theEMFieldCmd;
  GmUIcmdWithAString* theMagFieldLocalCmd;

  G4UIdirectory*  theGeometryDir;
  GmUIcmdWithAString* theParallelToMassCmd;
  GmUIcmdWithAString* theRefracIndexCmd;
  GmUIcmdWithAString* thePrintTouchablesCmd;
  G4UIcmdWithAnInteger* thePrintVolumeTreeCmd;
  G4UIcmdWithAnInteger* thePrintMaterialsCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
