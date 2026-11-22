#ifndef DetCClassifXPosMin_H
#define DetCClassifXPosMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifXPosMin : public DetVComptClassificationVariable
{

public:
  DetCClassifXPosMin();
  virtual ~DetCClassifXPosMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
