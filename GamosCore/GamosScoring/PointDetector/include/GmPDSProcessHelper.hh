#ifndef GmPDSProcessHelper_h
#define GmPDSProcessHelper_h

#include "GmPDSNeutronProcess.hh"
#include "GmPDSGeantinoProcess.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include "G4Point3D.hh"
#include "G4EmCalculator.hh"

class GmPDSScore;
class GmPDSDetector;
class G4SteppingManager;
class GmPDSInteractionAngleManager;
class GmAnalysisMgr;
class GmVClassifier;
class GmVFilter;
class G4Navigator;
class G4VEmProcess;

#include <map>
#include <set>

struct Flux2Dose {
  G4double Hstar;
  G4double Hp0;
  G4double Hp15;
  G4double Hp30;
  G4double Hp45;
  G4double Hp60;
  G4double Hp75;
};

class GmPDSProcessHelper : public GmVHistoBuilder
{
  friend class GmPDSVProcess;
  friend class GmPDSGeantinoProcess;
public:
  
  GmPDSProcessHelper(G4bool bForNeutron );
  
  virtual ~GmPDSProcessHelper();
  
public :// with description
  void SetDetectors( std::map<G4int,GmPDSDetector*> det );

  void SetEnergies( std::set<G4double> eners ) 
  {
    theEnergies = eners;
  }

  void SetAnaMgr( GmAnalysisMgr* anamgr ) 
  {
    theAnaMgr = anamgr;
  }

  void InitScores(G4int index);
  void BookHistos(G4int index);
  void PrintResults( std::ostream& out = G4cout );

  void SetScores( std::map<G4String,GmPDSScore*>* tl ) 
  {
    theScores = tl;
  }
  
  GmPDSScore* GetScore( G4String name );

  GmPDSDetector* GetDetector( G4int detID ){
    return theDetectors[detID];
  }

  std::map<G4int,G4int> GetDetectorIDs() const {
    return theDetectorIDs;
  }

  G4double GetCrossSection( const G4Step* aStep );
  G4double GetCrossSectionForNeutron( const G4Step* aStep );
  G4double GetCrossSectionForGamma( const G4Step* aStep );

  G4bool IsForNeutron() const
  {
    return bIsForNeutron;
  }

  G4double GetDistanceToDetectorIncm( const G4ThreeVector pos, G4int detID );

  G4String GetOriginalParticleName() const {
    return theOriginalParticleName;
  }

  void SetClassifier( GmVClassifier* classf ) {
    theClassifier = classf;
  };

  void SetFilters( std::vector<GmVFilter*> filts ) {
    theFilters = filts;
  };

protected:
  void StoreNeutronProcesses();
  void StoreGammaProcesses();

  G4int DetectorReached(const G4Step& aStep);
  void FillScores( const G4Track& aTrack, G4bool bGeantino, G4int detID );
  void FillControlHistos( G4double ener, G4double wei, G4bool bGeantino, GmPDSScore* score, Flux2Dose flux2Dose, G4int index );

private:
  void ReadEnergyBinsForNeutrons();
  void ReadEnergyBinsForGammas();
  void BuildEnergies();
  void AddScore(const G4String& name, G4int detID );
  void CheckNewIndex( G4int index );
  Flux2Dose GetFlux2Dose( G4double ener );


protected:
  std::map<G4int,GmPDSDetector*> theDetectors;
  std::map<G4int,G4int> theDetectorIDs;
  std::map<G4int,G4Point3D> theDetectorCentres;

  GmAnalysisMgr* theAnaMgr;

  std::set<G4double> theEnergies;
  std::map<G4double,Flux2Dose> theFlux2Dose;

  std::map<G4String,G4int> theVolumes;

  G4double theMinimumWeightForSplitting;
  G4double theMaximumWeightForSplitting;
  G4double theMaximumSplitting;
  G4double theSplittingFactor;

  G4double theExclusionRadius;
  //  G4double theMinimumWeightForRR;

  std::map<G4String,GmPDSScore*>* theScores;

  G4SteppingManager* fpSteppingManager;

  G4bool bScoreTrueAndGeantino;
  G4bool bScorePerVolume;
  G4bool bScoreInSpheres;

  G4bool bIsForNeutron;

  std::map<G4String,G4HadronicProcess*> theNeutronProcesses;
  std::map<G4String,G4VEmProcess*> theGammaProcesses;
  //  std::vector<G4String> theGammaProcesses;

  G4String theOriginalParticleName;

  G4Navigator* theNavigator;

  G4EmCalculator* theEmCalculator;

public: 
  G4bool bControlHistos;

  GmVClassifier* theClassifier;
  G4bool bClassifierOnTrack;
  std::set<G4int> theIndices;
  std::vector<GmVFilter*> theFilters;

  std::set<G4String> theScorerHistos; // the scorers for which histograms have been defined
};

#endif
