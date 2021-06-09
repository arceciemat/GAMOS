#ifndef GmHitsEventMgr_HH
#define GmHitsEventMgr_HH

#include "GamosCore/GamosSD/include/GmHit.hh"
#include <vector>
#include <map>
class GmHit;
class GmVDeadTimeDUList;
class G4Event;
class GmVDigitizer;
class GmVRecHitBuilder;
#include "GamosCore/GamosSD/include/GmHitList.hh"
#include "GamosCore/GamosSD/include/GmVDeadTimeDUList.hh"
#include "GamosCore/GamosSD/include/GmDigit.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
class GmVEventTimeExtractor;

class GmHitsEventMgr{

private:
  GmHitsEventMgr();

public:
  static GmHitsEventMgr* GetInstance();
  ~GmHitsEventMgr();

  void AddDigitizer( G4String& name, G4String& sdtype );
  void AddRecHitBuilder( G4String& name, G4String& sdtype );

  GmHitList* CreateHitList( G4String sdtype );
  void AddHit( GmHit* hit, G4String sdtype );

  void BuildHitsCompatibleInTime();
  void DigitizeAndReconstructHits();

  void AddHitToDeadTimeDetUnitList( const G4String& sdtype );

  void CleanHits();
  void CleanDeadTimeDetUnitList();
  void CleanDigitsAndRecHits();

  void DeleteHits( GmRecHit* rhit );

public:
  gamosSDHitMap GetHitLists() const { return theHitLists; }
  std::vector<GmHit*> GetHitsInEvent( G4int evtID );
  //  std::vector<GmHit*> GetAllHits();
  //-  gamosSDHitMap GetHitsCompatibleInTime() const { return theHitsCompatibleInTime;}
  gamosSDDigitMap GetDigits() const { return theDigits; }
  gamosSDRecHitMap GetRecHits() const { return theRecHits; }

  GmHitList* GetHitList( const G4String& sdType, G4bool bMustExist = true );
  std::vector<GmDigit*> GetDigits( const G4String& sdType, G4bool bMustExist = true  );
  std::vector<GmRecHit*> GetRecHits( const G4String& sdType, G4bool bMustExist = true );
  std::vector<GmRecHit*> GetAllRecHits();

  const std::map< G4String, GmVDigitizer* > GetDigitizers() const {
    return theDigitizers; }
  const std::map< G4String, GmVRecHitBuilder* > GetRecHitBuilders() const {
    return theRecHitBuilders; }

  GmVEventTimeExtractor* GetEventTimeExtractor() const {
    return theEventTimeExtractor; }
  void SetEventTimeExtractor(GmVEventTimeExtractor* ext ) {
    theEventTimeExtractor = ext; }

  void DeleteHit( GmHit* hit );

  void BuildEventTime( const G4Event* evt );

  G4int GetHitsTimeType() const {
    return theHitsTimeType;
  }

private:
  static GmHitsEventMgr* theInstance;

  gamosSDHitMap theHitLists;
  gamosSDHitMap theHitsCompatibleInTime;
  gamosSDDigitMap theDigits;
  gamosSDRecHitMap theRecHits;

  std::map< G4String, GmVDeadTimeDUList > theDeadTimeDetUnitLists; // one map for each SD type

  G4double theEventTime;

  std::map< G4String, GmVDigitizer* > theDigitizers; //one digitizer by SD type
  std::map< G4String, GmVRecHitBuilder* > theRecHitBuilders; //one digitizer by SD type

  GmVEventTimeExtractor* theEventTimeExtractor;

  G4int theHitsTimeType;
};

#endif
