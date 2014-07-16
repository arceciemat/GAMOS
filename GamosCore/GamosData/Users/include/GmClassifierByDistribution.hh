#ifndef GmClassifierByDistribution_hh
#define GmClassifierByDistribution_hh 1

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include <vector>
class G4Step;
class G4Track;
class GmVDistribution;

class GmClassifierByDistribution  : public GmVClassifier
{
public: // with description
  GmClassifierByDistribution(G4String);
  virtual ~GmClassifierByDistribution();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);
  G4int GetIndexFromValue(const G4double val );

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices(std::vector<G4String, std::allocator<G4String> >){};

private:
  std::map<G4int,G4String> theIndexNamesMap;
  GmVDistribution* theDistribution;
};

#endif
