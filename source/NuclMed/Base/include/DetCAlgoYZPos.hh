#ifndef DetCAlgoYZPos_H
#define DetCAlgoYZPos_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoYZPos : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoYZPos();
  virtual ~DetCAlgoYZPos(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
