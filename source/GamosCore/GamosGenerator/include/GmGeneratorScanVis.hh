#ifndef GmGeneratorScanVis_HH
#define GmGeneratorScanVis_HH

class G4Event;

#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "GmGeneratorScanVisUA.hh"
#include <vector>

class GmGeneratorScanVis: public G4VUserPrimaryGeneratorAction
{
public:
  GmGeneratorScanVis();
  ~GmGeneratorScanVis();
  void GeneratePrimaries(G4Event* evt);

  G4double GetPosTransv() const {
    return thePosTransv;
  }
  void Initialise();

private:
  void GenerateParticle(G4ThreeVector pos, G4Event* evt );
  void SetDir( G4double angle );
  G4bool IncrementPosTransv(); 
  
  G4bool bInitialised;
  G4double theStepScan;
  G4double theStepFig;
  G4double thePosTransv;
  G4double theScanWorldMinX;
  G4double theScanWorldMaxX;
  G4double theScanWorldMinY;
  G4double theScanWorldMaxY;
  G4double theScanWorldMinZ;
  G4double theScanWorldMaxZ;
  planeFig thePlaneFig;
  std::vector<G4double> theAngles;
  G4ThreeVector theDir;
  GmGeneratorScanVisUA* theGeneratorUA;  
};
#endif
