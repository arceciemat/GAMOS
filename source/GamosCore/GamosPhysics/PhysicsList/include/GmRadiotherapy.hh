#ifndef GmRadiotherapy_h
#define GmRadiotherapy_h 1

#include "GmEMPhysics.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class GmRadiotherapyMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmRadiotherapy: public GmEMPhysics
{
public:
  GmRadiotherapy();
  virtual ~GmRadiotherapy();

private:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

