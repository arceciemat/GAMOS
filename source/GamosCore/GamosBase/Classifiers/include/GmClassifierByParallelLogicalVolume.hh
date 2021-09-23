#ifndef GmClassifierByParallelLogicalVolume_hh
#define GmClassifierByParallelLogicalVolume_hh 1

#include <vector>
#include <map>
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
class G4ParallelWorldScoringProcess;
typedef std::vector<G4ParallelWorldScoringProcess*> vpwsp;
class G4ParticleDefinition;
typedef std::map< G4ParticleDefinition*, vpwsp > mvpwsp;
class G4LogicalVolume;
class G4VPhysicalVolume;

class GmClassifierByParallelLogicalVolume  : public GmVClassifier
{
public: // with description
  GmClassifierByParallelLogicalVolume(G4String);
  virtual ~GmClassifierByParallelLogicalVolume();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::vector<G4ParallelWorldScoringProcess*> GetParticleParallelWorldProcesses(G4ParticleDefinition* particle );
  void InitializeMaps();
  void SetIndexMaps( G4LogicalVolume* childLV, G4VPhysicalVolume* PV, G4VPhysicalVolume* PVGhostWorld, G4int& pwID, G4int idWorld );
  
  std::map<G4LogicalVolume*,int64_t> theIndexMap;
  std::map<int64_t,G4String> theIndexNameMap;

  mvpwsp theParallelWorldProcesses;

  G4int theNShift;
  G4bool bInitialized;

};

#endif
