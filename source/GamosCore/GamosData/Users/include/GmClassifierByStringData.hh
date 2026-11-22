#ifndef GmClassifierByStringData_hh
#define GmClassifierByStringData_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4Step;
class G4Track;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GmVDataUser.hh"

class GmClassifierByStringData  : public GmVClassifier, 
				  public GmVDataUser
{
public: // with description
  GmClassifierByStringData(G4String);
  virtual ~GmClassifierByStringData();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);
  virtual int64_t GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2);
 int64_t GetIndexFromValue(const G4String val );

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4String,int64_t> theIndexMap;
};

#endif
