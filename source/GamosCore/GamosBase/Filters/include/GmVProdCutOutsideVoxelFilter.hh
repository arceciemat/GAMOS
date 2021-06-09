#ifndef GmVProdCutOutsideVoxelFilter_h
#define GmVProdCutOutsideVoxelFilter_h 1

class G4Track;
class G4VProcess;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
class G4Material;
#include <map>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class is the base for prod cuts filter
// 
///////////////////////////////////////////////////////////////////////////////

#include <set>

class GmVProdCutOutsideVoxelFilter : public GmVFilter 
{

public: // with description
  GmVProdCutOutsideVoxelFilter(G4String name);
  virtual ~GmVProdCutOutsideVoxelFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  
  void SetParameters( std::vector<G4String>& params);
  
  virtual G4bool CheckProcess(const G4VProcess*) = 0;

  virtual G4double GetTrackEnergy( const G4Track* aTrack );
  virtual G4bool CheckStepNumber( G4int nstep );

protected:
  std::set<G4int> theParticleIDs;
  std::map<G4int,G4int> theCreationVoxels;
  std::map<G4Material*, G4double> theGammaCuts;
  std::map<G4Material*, G4double> theElectronCuts;
  std::map<G4Material*, G4double> thePositronCuts;

  G4int theOldEventID;
  G4int theNewEventID;
};

#endif
