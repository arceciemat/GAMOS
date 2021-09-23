#ifndef RTVClassifierByExtraInfo_hh
#define RTVClassifierByExtraInfo_hh 1

#include <vector>
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
class RTExtraInfoMgr;

class RTVClassifierByExtraInfo : public GmVClassifier
{
public: // with description
  RTVClassifierByExtraInfo(G4String);
  virtual ~RTVClassifierByExtraInfo(){};
  
  virtual int64_t GetIndexFromStep(const G4Step*) = 0;

  virtual void SetParameters( std::vector<G4String>& ){};

  virtual G4String GetIndexName(int64_t);

protected:

  RTExtraInfoMgr* theExtraInfoMgr;
};

#endif
