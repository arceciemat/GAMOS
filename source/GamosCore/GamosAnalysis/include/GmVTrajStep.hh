#ifndef GmVTrajStep_H
#define GmVTrajStep_H

#include <vector>
#include "globals.hh"

enum TSPUse {TSPUPre, TSPUPost, TSPUPrePost};
  
class GmVTrajStep
{
public:
  GmVTrajStep(){};
  virtual ~GmVTrajStep(){};
  
protected:
  TSPUse thePointToUse;
};

#endif
