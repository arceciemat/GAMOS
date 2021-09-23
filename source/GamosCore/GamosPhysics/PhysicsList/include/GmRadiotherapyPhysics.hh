#ifndef GmRadiotherapyPhysics_h
#define GmRadiotherapyPhysics_h 1

#include "GmEMPhysics.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class GmRadiotherapyPhysicsMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmRadiotherapyPhysics: public GmEMPhysics
{
public:
  GmRadiotherapyPhysics();
  virtual ~GmRadiotherapyPhysics();

private:

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

