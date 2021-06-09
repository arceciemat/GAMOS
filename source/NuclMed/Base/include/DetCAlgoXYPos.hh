#ifndef DetCAlgoXYPos_H
#define DetCAlgoXYPos_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoXYPos : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoXYPos();
  virtual ~DetCAlgoXYPos(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
