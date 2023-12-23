#ifndef GmVPrimitiveScorer_hh
#define GmVPrimitiveScorer_hh 1
// class description:
//
// This is the base class of the GAMOS G4VPrimitiveScorer.
// It construct the G4VPrimitiveSensor object passing the corresponding arguments

#include "GmEnergySplitter.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include <vector>
#include <map>
#include <set>
class GmVFilter;
class GmVPSPrinter;
class GmVClassifier;
class G4VPhysicalVolume;
class GmVData;
class GmVDistribution;
class GmConvergenceTester;
class GmGeometryUtils;
enum ScoreNEventsType { SNET_ByRun, SNET_ByEvent, SNET_ByNFilled };
//## score spectrum
typedef std::map<G4int,G4double> mid;
typedef std::map<G4int, mid* > mimid;

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
  }
   
  virtual void DumpAll(G4THitsMap<G4double>* RunMap);
  
  void Initialize(G4HCofThisEvent* HCE);

  G4bool FillScorer(G4Step* aStep, G4double val, G4double wei);
  G4bool FillScorer(G4Step* aStep, G4int index, G4double val, G4double wei);
  G4bool IsRegularScoring( G4Step*aStep );

  virtual void SumEndOfEvent();

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

  void Normalize(G4THitsMap<G4double>* RunMap);
  virtual void CalculateErrors(G4THitsMap<G4double>* RunMap);
  G4double GetError( G4int index );
  G4double GetErrorRelative( G4int index, G4double sumWX );
  
  void SetNewEvent( G4bool val ){
    bNewEvent = val; }

  G4bool ScoreErrors() const { return bScoreErrors; }
  G4bool UseTrackWeight() const { return fWeighted;}
  G4double GetUnit() const { return theUnit;}
  G4String GetUnitName() const { return theUnitName;}

  virtual void SetScoreByEvent( G4bool val ){
    if( val ) {
      theNEventsType = SNET_ByEvent;
    } else {
      theNEventsType = SNET_ByRun;
    }
  }
  //  G4bool ScoreByEventType() const { return theNEventsType; }
  void SetNEventsType( ScoreNEventsType net ) {
    theNEventsType = net;
  }
  ScoreNEventsType GetNEventsType() const {
    return theNEventsType;
  }    
    
  virtual G4double GetNEvents( G4int index );
  virtual std::map<G4int,size_t> GetNFilled() const;
  G4int GetNFilled(size_t index) const {
    std::map<G4int,size_t>::const_iterator ite = theNFilled.find(index);
    if( ite != theNFilled.end() ) {
      return G4int((*ite).second);
    } else {
      return -1;
    }
  }
  std::set<size_t> GetNFilled_tmp() const {
    return theNFilled_tmp;
  }
  
  G4bool AcceptByFilter( G4Step*aStep );

  void SetUnit( const G4String& unitName, G4double val );
  void RegisterMFD( G4MultiFunctionalDetector * mfd );

  std::vector<G4LogicalVolume*> GetMFDVolumes();
  void CalculateTotalVolume();
  G4double GetVolume( const G4Step* aStep );
  
  void SetErrors( std::map<G4int,G4double> err ) {
    theError = err;
  }
  
  G4THitsMap<G4double>* GetEvtMap() {
    return EvtMap;
  }
  std::map<G4int,G4double> GetSumV_tmp(){
    return theSumV_tmp;
  }

  G4MultiFunctionalDetector* GetMFD() const {
    return theMFD;
  }

  virtual void PropagateMFDToSubScorers(){};
  virtual void PropagateFilterToSubScorers(){};
  virtual void PropagateClassifierToSubScorers(){};
  virtual void PropagatePrinterToSubScorers(GmVPSPrinter* ){};
  virtual void PropagateTrkWeightToSubScorers(){}; 
  virtual void PropagateScoreErrorsToSubScorers(){};

  void SetDefaultPrinter( G4bool bdf ) {
    bDefaultPrinter = bdf;
  }

  void SetSubScorer( G4bool bss ) {
    bSubScorer = bss;
  }
  G4bool IsSubScorer() const {
    return bSubScorer;
  }
  void ClearSumV_tmp();
  void ClearNFilled_tmp();

  virtual void PrintAll();
  void EndOfEvent(G4HCofThisEvent*);
  void DrawAll();

  void SetMultiplyingDistribution( GmVDistribution* dist ) {
    theMultiplyingDistribution = dist;
  }

  //## score spectrum
  G4bool IsSpectrum() const {
    return bSpectrum;
  }
  void SetSpectrum( G4bool sp );
  mimid GetSpectrumSumV( ) {
    return theSpectrumSumV;
  }
  mid* GetSpectrumSumV( G4int index ) {
    return theSpectrumSumV[index];
  }
  void SetSpectrumSumV( mimid& sumw ) {
    theSpectrumSumV = sumw; }

  mimid GetSpectrumSumV2( ) {
    return theSpectrumSumV2;
  }
  mid* GetSpectrumSumV2( G4int index ) {
    return theSpectrumSumV2[index];
  }
  void SetSpectrumSumV2( mimid& sumw2 ) {
    theSpectrumSumV2 = sumw2; }

//## score spectrum
  G4int GetSpectrumNBins() const {
    return theSpectrumNBins;
  }
  G4double GetSpectrumBinWidth() const {
    return theSpectrumBinWidth;
  }
  G4double GetSpectrumMinimum() const {
    return theSpectrumMinimum;
  }
  G4double GetSpectrumMaximum() const {
    return theSpectrumMaximum;
  } 
  G4bool GetSpectrumBinLog10() const {
    return bSpectrumBinLog10;
  }
  void SetSpectrumNBins( G4int nb ){
    theSpectrumNBins = nb;
  }
  void SetSpectrumBinWidth( G4double bw ) {
    theSpectrumBinWidth = bw;
  }
  void SetMinimum( G4double min ) {
    theSpectrumMinimum = min;
  }
  void SetSpectrumMaximum( G4double max ) {
    theSpectrumMaximum = max;
  }
  void SeSpectrumBinLog10( G4bool blog ) {
    bSpectrumBinLog10 = blog;
  }
  void CheckSpectrumDims();
  
private:
  G4double GetError( G4int index, G4double sumWX, G4double nEvents );

  //## score spectrum
public:  
  G4bool FillScorerSpectrum(G4Step* aStep, G4int index, G4double val, G4double wei);
  void SumEndOfEventSpectrum();
  void AddToScoresSpectrum();
  void NormalizeSpectrum();
  void CalculateErrorsSpectrum();
  G4double GetErrorSpectrum( G4int index, G4double sumWX, G4double sumWX2, G4double nEvents );
  G4double GetErrorRelativeSpectrum( G4int index, G4int ibin, G4double sumWX);
  G4double GetErrorSpectrum( G4int index, G4int ibin );

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

  ScoreNEventsType theNEventsType;
  std::map<G4int,size_t> theNFilled;
  std::set<size_t> theNFilled_tmp;
  
  G4double sumALL; 

  GmVData* theMultiplyingData;
  GmVDistribution* theMultiplyingDistribution;

  GmConvergenceTester* theConvergenceTester;

  GmEnergySplitter* theEnergySplitter;

  G4bool bSkipEqualMaterials;
  G4bool bUseClassifierIndex;

  G4MultiFunctionalDetector* theMFD;
  GmGeometryUtils* theGeomUtils;
  G4bool bIntegrateVolumes;
  G4double theTotalVolume;
  G4bool bUseVolume;
  G4bool bDefaultPrinter;
  G4bool bSubScorer;

  G4double theMinDensity;

  //## score spectrum
  G4bool bSpectrum;
  G4bool bSpectrum1Interaction;
  std::map<G4int, mid* > theSpectrumSumV_tmp;
  std::map<G4int, mid* > theSpectrumSumV;
  std::map<G4int, mid* > theSpectrumSumV2;
  std::map<G4int, mid* > theSpectrumError;
  G4int theSpectrumNBins;
  G4double theSpectrumBinWidth;
  G4double theSpectrumMinimum;
  G4double theSpectrumMaximum;
  G4bool bSpectrumBinLog10;

};

#endif
