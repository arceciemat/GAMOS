#ifndef GmVDataInt_hh
#define GmVDataInt_hh

#include "GmVData.hh"

class GmVDataInt  : public GmVData
{
public:
  GmVDataInt();
  ~GmVDataInt();

  virtual void WriteBin( const G4Step* aStep );
  virtual void WriteBin( const G4Track* aTrack );
  virtual void WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 );
  virtual void WriteBin( const G4Event* anEvent );
  virtual void WriteBin( const G4Run* aRun );

};
#endif
