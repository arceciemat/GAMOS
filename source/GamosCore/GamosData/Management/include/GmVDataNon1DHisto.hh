#ifndef GmVDataNon1DHisto_hh
#define GmVDataNon1DHisto_hh

#include "GmVData.hh"

class GmVDataNon1DHisto : public GmVData
{
public:
  GmVDataNon1DHisto();
  ~GmVDataNon1DHisto();

  virtual void BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr ) = 0;

  virtual void FillHisto( const G4Step* , G4int ){};
  virtual void FillHisto( const G4Track* , G4int ){};
  virtual void FillHisto( const G4Track* , const G4Track* , G4int ){};
  virtual void FillHisto( const G4Event* , G4int ){};
  virtual void FillHisto( const G4Run* , G4int ){};

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

protected:

};
#endif
