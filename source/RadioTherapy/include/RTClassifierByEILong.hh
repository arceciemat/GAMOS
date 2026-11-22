#ifndef RTClassifierByEILong_hh
#define RTClassifierByEILong_hh 1

#include <map>
#include "globals.hh"
#include "RTVClassifierByExtraInfo.hh"

class RTClassifierByEILong  : public RTVClassifierByExtraInfo
{
public: // with description
  RTClassifierByEILong(G4String);
  virtual ~RTClassifierByEILong();
  
  virtual int64_t GetIndexFromStep(const G4Step*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual std::set<int64_t> GetIndexSet() const;

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<int64_t,G4String> theIndexNames;
  std::map<int64_t,int64_t> theIndexMap;
  G4int theFirstBit;
  G4int theNBits;
  G4int theLastBit;
  G4int theMaxNFound;
};

#endif
