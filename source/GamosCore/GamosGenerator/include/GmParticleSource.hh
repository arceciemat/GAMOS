#ifndef GmParticleSource_H
#define GmParticleSource_H
//
#include "globals.hh"
#include <vector>
#include <map>
#include "G4ThreeVector.hh"
class G4PrimaryVertex;
class GmVGenerDist;
class GmVGenerDistEnergy;
class GmVGenerDistTime;
class GmVGenerDistPosition;
class GmVGenerDistDirection;
class GmVNumericDistribution;

class GmParticleSource 
{
public:
  GmParticleSource( const G4String& name );
  virtual ~GmParticleSource(){}

  void LastTimeActive( G4double tim = 0.);
  void LastTimeNotActive( G4double tim );
  virtual G4double GenerateTime();
  virtual G4PrimaryVertex* GenerateVertex( G4double time ) = 0;

  virtual G4String GetParticleName() const = 0;

  void ReInitialiseDistributions();

  // Get and Set methods
  void SetDistributionTime( const G4String& distName, const std::vector<G4String>& wl );
  void SetDistributionEnergy( const G4String& distName, const std::vector<G4String>& wl );
  void SetDistributionPosition( const G4String& distName, const std::vector<G4String>& wl );
  void SetDistributionDirection( const G4String& distName, const std::vector<G4String>& wl );

  void SetDistributionTime( GmVGenerDistTime* dist ) {
    theTimeDistribution = dist;
  }
  void SetDistributionEnergy( GmVGenerDistEnergy* dist ) {
    theEnergyDistribution = dist;
  } 
  void SetDistributionPosition( GmVGenerDistPosition* dist ) {
    thePositionDistribution = dist;
  }  
  void SetDistributionDirection( GmVGenerDistDirection* dist ) {
    theDirectionDistribution = dist;
  } 

  G4double GetLastDecayTimeAnySource() const { return theLastDecayTimeAnySource; }
  void SetLastDecayTimeAnySource( G4double tim ) { theLastDecayTimeAnySource = tim; }

  G4String GetName() const { return theName; }
  G4String GetType() const { return theType; }

  G4double GetActivity() const { return theActivity; }
  void SetActivity( G4double act ) { theActivity = act; }

  G4double GetEnergy() const{ return theEnergy; }
  void SetEnergy( G4double ener ){ theEnergy = ener; }

  G4double GetTime() const{ return theTime; }
  void SetTime( G4double tim ){ theTime = tim; }

  G4ThreeVector GetPosition() const{ return thePosition; }
  void SetPosition( G4ThreeVector& pos ){ thePosition = pos; }

  G4ThreeVector GetDirection() const{ return theDirection; }
  void SetDirection( G4ThreeVector& dir ){ theDirection = dir; }

  GmVGenerDistEnergy* GetEnergyDistribution() const {
    return theEnergyDistribution; }
  GmVGenerDistTime* GetTimeDistribution() const { 
    return theTimeDistribution; }
  GmVGenerDistPosition* GetPositionDistribution() const {
    return  thePositionDistribution;  }
  GmVGenerDistDirection* GetDirectionDistribution() const {
    return  theDirectionDistribution; } 

  void AddBiasDistribution( const G4String& varName, const G4String& distName );

  std::map<G4String,GmVNumericDistribution*> GetBiasDistributions() const {
    return theBiasDistributions;
  }

  void BiasPosition();
  void BiasDirection();
  void BiasEnergy();

  G4double GetWeight() const {
    return theWeight;
  }
  void SetWeight( G4double wei ) {
    theWeight = wei;
  }

  G4int GetMaxBiasIterations() const {
    return theMaxBiasIterations; 
  }

protected:
  void CheckDistributionsExist();

protected:
  G4String theName;
  G4String theType;

  GmVGenerDistEnergy* theEnergyDistribution;
  GmVGenerDistTime* theTimeDistribution;
  GmVGenerDistPosition* thePositionDistribution; 
  GmVGenerDistDirection* theDirectionDistribution; 

  G4bool bUsedLastTime;
  G4double theTime;
  G4double theLastDecayTimeAnySource;

  G4double theActivity;
  G4double theEnergy;
  G4ThreeVector thePosition;
  G4ThreeVector theDirection;

  std::map<G4String,GmVNumericDistribution*> theBiasDistributions;
  G4int theMaxBiasIterations;
  G4double theWeight;
  G4bool bBiasDistributions;

};
#endif // GmParticleSource_H
