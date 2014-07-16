#ifndef GmVDataEquivalentDose_hh
#define GmVDataEquivalentDose_hh

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include <map>
class G4Step;

struct Flux2Dose {
  G4double Hstar;
  G4double Hp0;
  G4double Hp15;
  G4double Hp30;
  G4double Hp45;
  G4double Hp60;
  G4double Hp75;
};
enum equivDose {edHstar, edHp0, edHp15, edHp30, edHp45, edHp60, edHp75 };

class GmVDataEquivalentDose : public GmVData
{
public:
  GmVDataEquivalentDose();
  ~GmVDataEquivalentDose();

protected:
  G4double DoseFromEnergy( G4double ener, const G4Step* aStep );
  G4double EnergyToDoseFactor( G4double ener );
  std::map<G4double,Flux2Dose>* ReadEnergyBinsForNeutrons();
  std::map<G4double,Flux2Dose>* ReadEnergyBinsForGammas();
  void SetEquivDoseType( const G4String& doseName );

protected:
  std::map<G4double,Flux2Dose>* theFlux2Dose;
  equivDose theEquivDose;

};
#endif
