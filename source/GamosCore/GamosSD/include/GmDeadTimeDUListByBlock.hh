#ifndef GmDeadTimeDUListByBlock_HH
#define GmDeadTimeDUListByBlock_HH

#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosSD/include/GmVDeadTimeDUList.hh"


class GmDeadTimeDUListByBlock : public GmVDeadTimeDUList
{
public:
  GmDeadTimeDUListByBlock(G4String sdtype);
  virtual ~GmDeadTimeDUListByBlock(){};

  virtual void AddDetUnit( GmHit* hit );

private:

};

#endif
