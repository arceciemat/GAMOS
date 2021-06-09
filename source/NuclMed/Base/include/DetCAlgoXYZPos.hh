#ifndef DetCAlgoXYZPos_H
#define DetCAlgoXYZPos_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoXYZPos : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoXYZPos();
  virtual ~DetCAlgoXYZPos(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
