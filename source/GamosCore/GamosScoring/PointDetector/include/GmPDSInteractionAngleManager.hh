#ifndef GmPDSInteractionAngleManager_h
#define GmPDSInteractionAngleManager_h
#include "globals.hh"
#include <map>
#include <vector>

class GmPDSInteractionAngleManager
{
  public:
  GmPDSInteractionAngleManager( const G4String& );
  ~GmPDSInteractionAngleManager(){};

  G4double GetHistoValue(const G4String& procname, const G4String& matname, G4double ener, const G4double cosang );

private:
  std::map<G4double,G4String>::iterator GetEnergyRange(G4double ener );

private:
  std::map<G4String, std::vector<G4double> > theHistoEntries;
  G4double theXmin;
  G4double theXmax;
  G4double theBinStep;
  std::map<G4double,G4String> theEnergies;
};
#endif
