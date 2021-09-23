#ifndef GmVParallelGeomCondition_h
#define GmVParallelGeomCondition_h 1

#include "globals.hh"
#include <vector>
#include <map>
class G4ParticleDefinition;
class G4ParallelWorldScoringProcess;
class G4VTouchable;
class G4Track;
class G4Step;
class G4StepPoint;

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  Base class to manage geometry conditions for GmVolumeConditionFilter
// 
///////////////////////////////////////////////////////////////////////////////
typedef std::vector<G4ParallelWorldScoringProcess*> vpwsp;
typedef std::map< G4ParticleDefinition*, vpwsp > mvpwsp;

class GmVParallelGeomCondition 
{
public: // with description
  GmVParallelGeomCondition(){};
  
  virtual ~GmVParallelGeomCondition(){};
  
public: // with description
  virtual const std::vector<const G4VTouchable*> GetTouchableFromTrack(const G4Track*) = 0;
  virtual const std::vector<const G4VTouchable*> GetTouchableFromStep(const G4Step*) = 0;
  const G4VTouchable* GetTouchableFromStepPoint(const G4StepPoint*);

protected:
  void FillParallelWorldProcesses();
  vpwsp GetParticleParallelWorldProcesses(G4ParticleDefinition*);

protected:
  mvpwsp theParallelWorldProcesses;

};

#endif
