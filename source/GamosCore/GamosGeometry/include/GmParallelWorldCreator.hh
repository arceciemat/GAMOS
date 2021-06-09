#ifndef GmParallelWorldCreator_h
#define GmParallelWorldCreator_h

#include "G4tgbVParallelWorldCreator.hh"


//----------------------------------------------------------------------------  
class GmParallelWorldCreator : public G4tgbVParallelWorldCreator
{
  public:
    GmParallelWorldCreator(){};

    ~GmParallelWorldCreator(){};

    virtual G4VUserParallelWorld* CreateParallelWorld( const G4String& parallelWorldName, const G4int index );

};

#endif
