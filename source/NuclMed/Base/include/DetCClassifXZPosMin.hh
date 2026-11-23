#ifndef DetCClassifXZPosMin_H
#define DetCClassifXZPosMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifXZPosMin : public DetVComptClassificationVariable
{

public:
  DetCClassifXZPosMin();
  virtual ~DetCClassifXZPosMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
