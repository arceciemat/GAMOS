#ifndef GmMinRangeCutOutsideVoxelFilter_h
#define GmMinRangeCutOutsideVoxelFilter_h 1

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
class G4Track;

class GmMinRangeCutOutsideVoxelFilter : public GmVFilter 
{

public: // with description
  GmMinRangeCutOutsideVoxelFilter(G4String name);
  virtual ~GmMinRangeCutOutsideVoxelFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  
  void SetParameters( std::vector<G4String>& params);
  
  virtual G4bool CheckProcess(const G4VProcess*);

  virtual G4double GetTrackEnergy( const G4Track* aTrack );
  virtual G4bool CheckTrackNumber( G4int nstep );

protected:
  std::set<G4int> theParticleIDs;
  G4int theCreationVoxel;
  std::map<G4int, std::set<const G4Track*> > theSecoNotCut;
  std::map<G4Material*, G4double> theGammaCuts;
  std::map<G4Material*, G4double> theElectronCuts;
  std::map<G4Material*, G4double> thePositronCuts;

  G4int theOldEventID;
  G4int theNewEventID;

};

#endif
