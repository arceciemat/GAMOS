// ------------------------------------------------------------
//        GAMOS class header file 
//
// Class Description
//
// This class wraps a process invoking NSplit times the PostStepDoIt method// ------------------------------------------------------------

#ifndef GmSplitWrapperProcess_h 
#define GmSplitWrapperProcess_h 1

#include "G4WrapperProcess.hh"

class GmSplitWrapperProcess : public G4WrapperProcess
{
  public: // with description
  //  constructor requires the process name and type
      GmSplitWrapperProcess(const G4String& aName =  "Wrapped",
                 G4ProcessType   aType = fNotDefined );

  public: 
  //  destructor 
      virtual ~GmSplitWrapperProcess();

  public: 
      virtual G4VParticleChange* PostStepDoIt(
                             const G4Track& track,
                             const G4Step&  stepData
                            );

  G4int GetNSplit() const
  {
    return theNSplit;
  }
  void SetNSplit( G4int nspl ) 
  {
    theNSplit = nspl;
  }


private:
  G4int theNSplit;
};

#endif
