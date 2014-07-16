#ifndef GmVPrimitiveScorer_hh
#define GmVPrimitiveScorer_hh 1
// class description:
//
// This is the base class of the GAMOS G4VPrimitiveScorer.
// It construct the G4VPrimitiveSensor object passing the corresponding arguments

#include "GmEnergySplitter.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <vector>
#include <map>
class GmVFilter;
class GmVPSPrinter;
class GmVClassifier;
class G4VPhysicalVolume;
class GmVData;
class GmVDistribution;
class GmConvergenceTester;
class GmGeometryUtils;

class GmVPrimitiveScorer : public G4VPrimitiveScorer
{
  friend class G4MultiFunctionalDetector;
  
public: // with description
  GmVPrimitiveScorer(G4String name);
  virtual ~GmVPrimitiveScorer();
  
protected: // with description
  virtual G4int GetIndex(G4Step*);
  // This is a function mapping from copy number(s) to an index of 
  // the hit collection. In the default implementation, just the
  // copy number of the physical volume is taken.

  void AddDefaultPrinter();
  void AddDefaultClassifier();
  
public:
  virtual void SetParameters( const std::vector<G4String>& ){
    G4cout << " GmVPrimitiveScorer::SetParameters: no parameters " << G4endl;
};
   
  virtual void DumpAll(G4THitsMap<G4double>* RunMap);
  
  void Initialize(G4HCofThisEvent* HCE);

  G4bool FillScorer(G4Step* aStep, G4double val, G4double wei);
  G4bool FillScorer(G4Step* aStep, G4int index, G4double val, G4double wei);
  void FillScorerAtPostCheckingRegular(G4Step* aStep, G4double val, G4double wei); 
  G4bool IsRegularScoring( G4Step*aStep );

  void ScoreNewEvent();

  void SetGmFilter(GmVFilter* f)
  { theFilter = f; }
  inline GmVFilter* GetGmFilter() const
  { return theFilter; }

  void AddPrinter( GmVPSPrinter* prt );

  GmVClassifier* GetClassifier() const{
    return theClassifier; 
  }
  void SetClassifier( GmVClassifier* idx );

  virtual void SetUseTrackWeight( G4bool val ){
    fWeighted = val;}
  virtual void SetScoreErrors( G4bool val ){
    bScoreErrors = val;}

  G4double GetSumV2( G4int index ) {
    if( theSumV2.find( index ) == theSumV2.end() ) return 0.;
    return theSumV2[index];
  }

  std::map<G4int,G4double>* GetSumV2() const {
    return const_cast<std::map<G4int,G4double>* >( &theSumV2 ); 
  }

  void SetSumV2( std::map<G4int,G4double>& sumw2 ) {
    theSumV2 = sumw2; }

  void CalculateErrors(G4THitsMap<G4double>* RunMap);
  G4double GetError( G4int index );
  G4double GetErrorRelative( G4int index, G4double sumWX, G4double nEvents );
  
  void SetNewEvent( G4bool val ){
    bNewEvent = val; }

  G4bool ScoreErrors() const { return bScoreErrors; }
  G4bool UseTrackWeight() const { return fWeighted;}
  G4double GetUnit() const { return theUnit;}
  G4String GetUnitName() const { return theUnitName;}

  virtual void SetScoreByEvent( G4bool val ){
    bScoreByEvent = val;}
  G4bool ScoreByEvent() const { return bScoreByEvent; }

  G4bool AcceptByFilter( G4Step*aStep );

  void SetUnit( const G4String& unitName, G4double val );
  void RegisterMFD( G4MultiFunctionalDetector * mfd );

  std::vector<G4LogicalVolume*> GetMFDVolumes();
  void CalculateTotalVolume();
  G4double GetVolume( const G4Step* aStep );

private:
  G4double GetError( G4int index, G4double sumWX, G4double nEvents );

protected:
  GmVFilter* theFilter;
  std::vector<GmVPSPrinter*> thePrinters;
  GmVClassifier* theClassifier;
  G4THitsMap<G4double>* EvtMap;
  G4int HCID;
  G4bool fWeighted;
  //--- Error calculations
  std::map<G4int,G4double> theSumV_tmp;
  std::map<G4int,G4double> theSumV2;
  std::map<G4int,G4double> theError;

  G4bool bNewEvent;

  G4bool bScoreErrors;
  G4double theUnit;
  G4String theUnitName;

  G4bool bScoreByEvent;

  G4double sumALL; 

  GmVData* theMultiplyingData;
  GmVDistribution* theMultiplyingDistribution;

  GmConvergenceTester* theConvergenceTester;

  GmEnergySplitter* theEnergySplitter;

  G4bool bSkipEqualMaterials;
  G4bool bUseClassifierIndex;

  GmRegularParamUtils* theRegularParamUtils;

  G4MultiFunctionalDetector* theMFD;
  GmGeometryUtils* theGeometryUtils;
  G4bool bIntegrateVolumes;
  G4double theTotalVolume;
  G4bool bUseVolume;
};

#endif
