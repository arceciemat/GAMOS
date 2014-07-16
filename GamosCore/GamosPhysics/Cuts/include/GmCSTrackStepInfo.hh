#ifndef GmCSTrackStepInfo_H
#define GmCSTrackStepInfo_H 1
#include "GmCSTrackInfo.hh"
#include "G4ThreeVector.hh"


class GmCSTrackStepInfo : public GmCSTrackInfo
{
  //  friend std::ostream& operator<<(std::ostream& out, const GmCSTrackStepInfo&);

  public:
    GmCSTrackStepInfo(const G4Track* );
    virtual ~GmCSTrackStepInfo();

  public:
  G4ThreeVector GetPosition() const {
    return  thePosition; }
  void SetPosition( G4ThreeVector pos ) {
    thePosition = pos; }
  G4double GetTrackLength() const {
    return  theTrackLength; }
  void SetTrackLength( G4double tl ){
    theTrackLength = tl; }

  virtual void Print(std::ostream& out  = G4cout) const;

private:
  G4ThreeVector thePosition;
  G4double theTrackLength;
};

#endif
