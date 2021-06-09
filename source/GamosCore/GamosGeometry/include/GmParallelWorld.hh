#ifndef GmParallelWorld_hh
#define GmParallelWorld_hh

#include "G4tgbParallelWorld.hh"

class GmParallelWorld : public G4tgbParallelWorld {
public:
  GmParallelWorld(G4String worldName, G4int index);
  ~GmParallelWorld();
  
private:
  void Construct();

};

#endif
