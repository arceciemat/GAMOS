#ifndef __RTZRLimitsUA__
#define __RTZRLimitsUA__

#include "RTVZRLimitsUA.hh"

class RTZRLimitsUA : public RTVZRLimitsUA
{
public:
  RTZRLimitsUA();
  ~RTZRLimitsUA(){}; 
  virtual void EndOfRunAction( const G4Run* aRun );

protected:
  virtual void BuildXYLimits();

};

#endif
