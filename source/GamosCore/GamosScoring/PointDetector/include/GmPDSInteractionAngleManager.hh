#ifndef GmPDSInteractionAngleManager_h
#define GmPDSInteractionAngleManager_h
#include "globals.hh"
#include <map>
#include <vector>
typedef std::map<G4double,G4String> mds;
typedef std::map<G4String, std::map<G4double,G4String>*> msmds;

class GmPDSInteractionAngleManager
{
  public:
  GmPDSInteractionAngleManager( const G4String& );
  ~GmPDSInteractionAngleManager(){};

  G4double GetHistoValue(const G4String& procname, const G4String& matname, G4double ener, const G4double cosang );

private:
  std::map<G4double,G4String>::iterator GetEnergyRange(mds* energies, G4double ener );

private:
  std::map<G4String, std::vector<G4double> > theHistoEntries;
  G4double theXmin;
  G4double theXmax;
  G4double theBinStep;
  msmds theEnergies;
  G4bool bEnergyLog;
};
#endif
