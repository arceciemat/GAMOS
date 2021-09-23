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
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);
  int64_t GetIndexFromValue(const G4double val );

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices(std::vector<G4String, std::allocator<G4String> >){};

private:
  std::map<int64_t,G4String> theIndexNamesMap;
  GmVDistribution* theDistribution;
};

#endif
