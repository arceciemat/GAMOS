#ifndef GmDeadTimeDUListByCrystal_HH
#define GmDeadTimeDUListByCrystal_HH

#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosSD/include/GmVDeadTimeDUList.hh"

class GmDeadTimeDUListByCrystal : public GmVDeadTimeDUList
{
public:
  GmDeadTimeDUListByCrystal(G4String sdtype);
  ~GmDeadTimeDUListByCrystal(){};

  virtual void AddDetUnit( GmHit* hit );

private:

};

#endif
