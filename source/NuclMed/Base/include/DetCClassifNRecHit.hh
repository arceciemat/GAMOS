#ifndef DetCClassifNRecHit_H
#define DetCClassifNRecHit_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifNRecHit : public DetVComptClassificationVariable
{

public:
  DetCClassifNRecHit();
  virtual ~DetCClassifNRecHit();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
