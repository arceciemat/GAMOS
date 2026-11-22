#ifndef GmVDataInt_hh
#define GmVDataInt_hh

#include "GmVData.hh"

class GmVDataInt  : public GmVData
{
public:
  GmVDataInt();
  ~GmVDataInt();
  virtual void WriteText( const G4Step* aStep, G4bool bIsFirst );
  virtual void WriteText( const G4Track* aTrack, G4bool bIsFirst );
  virtual void WriteText( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst );
  virtual void WriteText( const G4Event* anEvent, G4bool bIsFirst );
  virtual void WriteText( const G4Run* aRun, G4bool bIsFirst );

  virtual void WriteBin( const G4Step* aStep );
  virtual void WriteBin( const G4Track* aTrack );
  virtual void WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 );
  virtual void WriteBin( const G4Event* anEvent );
  virtual void WriteBin( const G4Run* aRun );

  virtual void WriteCout( const G4Step* aStep, G4bool bIsFirst );
  virtual void WriteCout( const G4Track* aTrack, G4bool bIsFirst );
  virtual void WriteCout( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst );
  virtual void WriteCout( const G4Event* anEvent, G4bool bIsFirst );
  virtual void WriteCout( const G4Run* aRun, G4bool bIsFirst );

};
#endif
