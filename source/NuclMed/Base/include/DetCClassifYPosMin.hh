#ifndef DetCClassifYPosMin_H
#define DetCClassifYPosMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifYPosMin : public DetVComptClassificationVariable
{

public:
  DetCClassifYPosMin();
  virtual ~DetCClassifYPosMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
