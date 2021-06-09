#ifndef GmClassifierByNumericData_hh
#define GmClassifierByNumericData_hh 1

#include <vector>
#include "globals.hh"
class G4Step;
class G4Track;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GmVDataUser.hh"

class GmClassifierByNumericData  : public GmVClassifier,
				   public GmVDataUser
{
public: // with description
  GmClassifierByNumericData(G4String);
  virtual ~GmClassifierByNumericData();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);
  virtual G4int GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2);
  G4int GetIndexFromValue(const G4double val );

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4double,G4int> theIndexMap;
  G4double theMin, theMax, theStep;

  G4bool bAllowOutOfLimits;
};

#endif
