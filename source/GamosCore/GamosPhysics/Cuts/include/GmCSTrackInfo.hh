#ifndef GmCSTrackInfo_H
#define GmCSTrackInfo_H 1
#include "GmCutsEnergy2Range.hh"

class G4Region;
class G4VProcess;
class G4ParticleDefinition;
class G4Track;


class GmCSTrackInfo
{
  //  friend std::ostream& operator<<(std::ostream& out, const GmCSTrackInfo&);

  public:
  GmCSTrackInfo(){};
  GmCSTrackInfo(const G4Track* );
  virtual ~GmCSTrackInfo();

  public:
  virtual void Print(std::ostream& out = G4cout ) const;

  G4int GetTrackID() const { return theTrackID;}
  GmCSTrackInfo* GetParent() const { return theParent;}
  G4Region* GetRegion() const { return theRegion;}
  G4VProcess* GetProcess() const { return theProcess;}
  G4ParticleDefinition* GetParticle() const { return theParticle;}
  G4double GetRange() const { return theRange;}
  G4double GetEnergy() const { return theEnergy;}
  G4double GetSafety() const { return theSafety;}
  G4bool IsAccepted() const { return bAccepted;}

  void SetAccepted(G4bool acc ) { bAccepted = acc;}
  void SetParent( GmCSTrackInfo* par ){ theParent = par;}

  static void CreateEnergy2Range() {
    theEnergy2Range = new GmCutsEnergy2Range;
  }
  static GmCutsEnergy2Range* GetEnergy2Range() {
    return theEnergy2Range;
  }

protected:
  G4int theTrackID;
  GmCSTrackInfo* theParent;
  G4Region* theRegion;
  G4VProcess* theProcess;
  G4ParticleDefinition* theParticle;
  G4double theRange;
  G4double theEnergy;
  G4double theSafety;
  G4bool bAccepted;

private:
  static GmCutsEnergy2Range* theEnergy2Range;

};

#endif
