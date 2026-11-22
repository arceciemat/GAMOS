#ifndef GmGenerDistTimeDecay_HH
#define GmGenerDistTimeDecay_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistTime.hh"
#include "G4Types.hh"

class GmParticleSource;
class GmGenerator;

class GmGenerDistTimeDecay : public GmVGenerDistTime
{
public:
  GmGenerDistTimeDecay();
  virtual ~GmGenerDistTimeDecay(){};

  virtual G4double GenerateTime( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

  void SetActivity( const G4double act );
  void SetLifeTime( const G4double lf );

public:
  // Windows: exported through CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS, no declspec needed
  static G4double GetCurrentActivity();

protected:
	static G4ThreadLocal G4double theCurrentActivity;
	G4double theActivity;
  G4double theLifeTime;
  GmGenerator* theGenerator;
};

#endif
