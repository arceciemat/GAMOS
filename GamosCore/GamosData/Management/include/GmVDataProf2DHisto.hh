#ifndef GmVDataProf2DHisto_hh
#define GmVDataProf2DHisto_hh

#include "GmVDataNon1DHisto.hh"

class GmVDataProf2DHisto : public GmVDataNon1DHisto
{
public:
  GmVDataProf2DHisto();
  ~GmVDataProf2DHisto();

  virtual void BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr );

  virtual void FillHisto( const G4Step* aStep, G4int index );
  virtual void FillHisto( const G4Track* aTrack, G4int index );
  virtual void FillHisto( const G4Track* aTrack1, const G4Track* aTrack2, G4int index );
  virtual void FillHisto( const G4Event* anEvent, G4int index );
  virtual void FillHisto( const G4Run* aRun, G4int index );

protected:

};
#endif
