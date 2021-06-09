#ifndef GmVDeadTimeDUList_HH
#define GmVDeadTimeDUList_HH

#include "GamosCore/GamosSD/include/GmHit.hh"
#include <map>
struct DTHitInfo{
  G4double time;
  G4int eventID;
};

class GmVDeadTimeDUList : public std::map<unsigned long long,DTHitInfo>
{
public:
  GmVDeadTimeDUList(){};
  virtual ~GmVDeadTimeDUList(){};

  virtual void AddDetUnit( GmHit* ){}; //t = 0
  void Clean(G4double time);

  virtual G4bool FindDetUnit( GmHit*, G4double deadTime );

protected:
  G4int theNShift;

};

#endif
