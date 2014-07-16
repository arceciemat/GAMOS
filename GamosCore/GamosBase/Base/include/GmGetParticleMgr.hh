#ifndef GmGetParticleMgr_hh
#define GmGetParticleMgr_hh

#include "globals.hh"
#include <vector>
class G4ParticleDefinition;
class G4Ions;
class G4VProcess;

class GmGetParticleMgr
{
private:
  GmGetParticleMgr();

public:
  ~GmGetParticleMgr();

  static GmGetParticleMgr* GetInstance();

  std::vector<G4ParticleDefinition*> GetG4ParticleList( const G4String& particleName, G4bool bMustExist = 1 );
  G4ParticleDefinition* GetG4Particle(const G4String& particleName, G4bool bMustExist = 1);
  G4ParticleDefinition* GetG4Particle(const G4VProcess* proc, G4bool bMustExist = 1 );
  G4Ions* CreateIon( const G4String& newValues );
  G4Ions* CreateIon( G4int atomicNumber,
		     G4int atomicMass,
		     G4double energy,
		     G4int excitNumber);

  std::vector<G4VProcess*> GetG4ProcessList(const G4String& particleName, const G4String& processName, G4bool bMustExist = 1 );
  std::vector<G4VProcess*> GetG4ProcessList(const G4String& processName, G4bool bMustExist = 1 );
  G4String GetG4ProcessName(const G4String& particleName, G4String processName );

private:
  static GmGetParticleMgr* theInstance;
};

#endif
