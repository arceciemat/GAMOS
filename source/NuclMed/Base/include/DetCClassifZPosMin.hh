#ifndef DetCClassifZPosMin_H
#define DetCClassifZPosMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifZPosMin : public DetVComptClassificationVariable
{

public:
  DetCClassifZPosMin();
  virtual ~DetCClassifZPosMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
