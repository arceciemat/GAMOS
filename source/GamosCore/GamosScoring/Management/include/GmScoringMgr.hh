#ifndef GmScoringMgr_H
#define GmScoringMgr_H

#include "G4RunManager.hh"
class G4MultiFunctionalDetector;
class GmVPrimitiveScorer;
class GmVPrimitiveScorerSpectrum;
class GmVFilter;
class GmVPSPrinter;
class GmVClassifier;
class GmScoringRun;
#include "GmVPrimitiveScorer.hh"
#include <map>


class GmScoringMgr
{
private:
  GmScoringMgr();

public:
  ~GmScoringMgr();

  static GmScoringMgr* GetInstance();

  void CreateMFD( const std::vector<G4String>& wl );
  void CreateScorer( const G4String& scorerName, const G4String& scorerClass, std::vector<G4String>& params );
  void AddScorerToMFD( std::vector<G4String>& wl );
  GmVPrimitiveScorerSpectrum* AddScorerSpectrumToMFD( std::vector<G4String>& wl );
  void AddFilterToScorer( std::vector<G4String> params ); 
  void AddPrinterToScorer(std::vector<G4String> params ); 
  void AssignClassifierToScorer( std::vector<G4String>& params ); 
  void AddTrkWeightToScorer( const G4String& scorerName, G4String& val );
  void AddScoreErrorsToScorer( const G4String& scorerName, G4String& val );
  void AddPrintByEventToScorer( const G4String& scorerName, G4String& val );
  void AddPrintNEventsTypeToScorer( const G4String& scorerName, G4String& val );
  void ChangeScorerUnit( const G4String& scorerName, const G4String& unitName, G4String& unitValue );

  void PrintAllScorers();

  void AddScorer( G4String scorerName, GmVPrimitiveScorer* scorer ) {
    theScorers[scorerName] = scorer;
  }
  std::map<G4String,GmVPrimitiveScorer*> GetScorers() const{ 
    return theScorers; }
  std::map<G4String,G4MultiFunctionalDetector*> GetMFDs() const {
    return theMFDs; }

  G4THitsMap<G4double>* GetRunMap(GmVPrimitiveScorer* scorer );

  void SetScoringRun( GmScoringRun* sr ) {
    theScoringRun = sr;
  }

  GmVPrimitiveScorer* GetScorer( G4String, G4bool bMustExists ) const;

private:
  static GmScoringMgr* theInstance;

  GmScoringRun* theScoringRun;
  std::map<G4String,G4MultiFunctionalDetector*> theMFDs;
  std::map<G4String,GmVPrimitiveScorer*> theScorers;
  //  std::map<G4String,GmVFilter*> theFilters;
  std::map<G4String,GmVPSPrinter*> thePrinters;
  //  std::map<G4String,GmVClassifier*> theClassifiers;

  G4bool bPrintingUA;

};

#endif
