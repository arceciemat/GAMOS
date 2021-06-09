#ifndef GmTimeStudyMgr_H
#define GmTimeStudyMgr_H

#include "G4RunManager.hh"
#include <vector>
#include "G4VSteppingVerbose.hh"
class G4SteppingVerbose;
class GmTimeStudyUA;

class GmTimeStudyMgr : public G4VSteppingVerbose
{
private:
  GmTimeStudyMgr();

public:
  ~GmTimeStudyMgr();

  static GmTimeStudyMgr* GetInstance();
  void AddTimeStudy( GmTimeStudyUA* ts );
  void InitManagers();

  virtual void NewStep();
  virtual void StepInfo();

  // Following methods are not used
  virtual void TrackBanner(){}
  virtual void AtRestDoItInvoked();
  virtual void AlongStepDoItAllDone();
  virtual void PostStepDoItAllDone();
  virtual void AlongStepDoItOneByOne();
  virtual void PostStepDoItOneByOne();
  virtual void TrackingStarted();
  virtual void DPSLStarted();
  virtual void DPSLUserLimit();
  virtual void DPSLPostStep();
  virtual void DPSLAlongStep();
  virtual void VerboseTrack();
  virtual void VerboseParticleChange();

private:
  static GmTimeStudyMgr* theInstance;

  std::vector<GmTimeStudyUA*> theTimeStudies;

  static G4SteppingVerbose* theG4Verbose;
  static int theG4VerboseLevel;

};

#endif
