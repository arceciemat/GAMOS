#ifndef __RTProdCutsStudyUA__
#define __RTProdCutsStudyUA__

#include "GamosCore/GamosPhysics/Cuts/include/GmProdCutsStudyUA.hh"

class RTProdCutsStudyUA : public GmProdCutsStudyUA
{
public:
  RTProdCutsStudyUA();
  ~RTProdCutsStudyUA(){}; 
  virtual void BuildFilter();

protected:

};

#endif
