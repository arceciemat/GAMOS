#ifndef SHRadDecayScorerFromFlux_h
#define SHRadDecayScorerFromFlux_h 1

#include "G4THitsMap.hh"
class G4ParticleHPProduct;
class G4Ions;
class G4IonTable;
class G4Step;
class G4Isotope;
class G4EmCalculator;

///////////////////////////////////////////////////////////////////////////////
class SHRadDecayScorerFromFlux 
{
 
public: // with description
  SHRadDecayScorerFromFlux();
  ~SHRadDecayScorerFromFlux();
  void ProcessHits(const G4Step*);

  G4bool FillScorer(const G4Step* aStep, G4int index, G4double val, G4double wei);
  void SaveEventToRun();
  void CalculateErrors(G4THitsMap<G4double>* RunMap);
  G4double GetError( G4int index );
  G4double GetError( G4int index, G4double sumWX, G4double nEvents );
  G4double GetErrorRelative( G4int index, G4double sumWX, G4double nEvents );

  G4int GetIndexFromProduct(G4ParticleHPProduct* product);
  G4int GetIndexAdjustingIon( G4int AZ, const G4Isotope* isotope );

  G4Ions* GetIon(G4int index) const;

  G4THitsMap<G4double>* GetRunMap() const {
    return theRunMap;
  }
  

private:
  G4bool bAdjustPHP;
  G4bool bAdjustHP;
  G4bool bInitialised;
  G4double theEnergyInterpStep;
  G4int theNInterpStep;

  G4IonTable* theIonTable;
  std::map<G4int,G4int> theHPProductIndices; // isotope_mass_code - index. If isomers are present this is not valid
  std::map<G4int,G4int> theAZindices;
  std::map<G4int,G4Ions*> theIonIndices;

  // use hits as if it were a scorer
  G4THitsMap<G4double>* theEvtMap;
  G4THitsMap<G4double>* theRunMap;

  //--- Error calculations
  std::map<G4int,G4double> theSumV_tmp;
  std::map<G4int,G4double> theSumV2;
  std::map<G4int,G4double> theError;
  G4double sumALL; 

  G4EmCalculator* theEmCalculator;
};

#endif
