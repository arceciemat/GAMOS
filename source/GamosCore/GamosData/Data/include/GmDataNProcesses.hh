#ifndef GmDataNProcesses_hh
#define GmDataNProcesses_hh

#include "GamosCore/GamosData/Management/include/GmVDataAccumulated.hh"
class G4EventManager;
class GmDataNProcesses : public GmVDataAccumulated
{
public:
  GmDataNProcesses();
  ~GmDataNProcesses();

  void Accumulate( const G4Step* aStep, G4int index = 0);
  virtual G4double GetValueFromStep( const G4Step* aStep, G4int index = 0 );

private:
  std::vector<G4String> theProcessesNames;
  G4int thePrevTrackID;
  G4int thePrevEventID;
  std::map<G4int,G4int> theNSteps;
  G4EventManager* theEventManager;
};
#endif
