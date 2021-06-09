#ifndef DetCAlgoYPos_H
#define DetCAlgoYPos_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoYPos : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoYPos();
  virtual ~DetCAlgoYPos(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
