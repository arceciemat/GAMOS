#ifndef GmHitList_HH
#define GmHitList_HH

#include "GamosCore/GamosSD/include/GmHit.hh"
#include <map>
#include <set>
class GmVDeadTimeDUList;
class G4RunManager;

typedef std::vector< GmHit* > hitVector;

enum MeasuringType{ bMTBackwards, bMTInterval, bMTTriggerGlobal, bMTTriggerIndependent };

class GmHitList : public std::vector<GmHit*>
{
public:
  GmHitList( G4String& sdtyp );
  ~GmHitList();

  void AddHit( GmHit* hit );
  const G4String GetSDType() const {
    return theSDType; }  

  void BuildHitsCompatibleInTime( G4double tim = -1. );
  void BuildHitsAll( G4double tim = -1. );
  void BuildHitsCompatibleInTimeBackwards( G4double tim = -1. );
  void BuildHitsCompatibleInTimeInterval( G4double tim );
  void BuildHitsCompatibleInTimeTriggerGlobal( G4double tim );
  void BuildHitsCompatibleInTimeTriggerIndependent( G4double tim );

  void AddHitToDeadTimeDetUnitList();

  void CleanHits( G4double tim = -1. );
  void CleanHitsBefore( G4double tim );
  void CleanHitsBefore( G4double tim, unsigned long long trigID );
  void CleanDeadTimeDetUnitList( G4double tim = 1.);

  void DeleteHit( iterator hit );

  const hitVector* GetHitsCompatibleInTime() const {
    return &theHitsCompatibleInTime; 
  }

  G4double GetTriggerTime( G4double hitTime, unsigned long long detUnitID );

  G4double GetMeasuringTime() const {
    return theMeasuringTime; 
  }

 
private:
  
  G4String theSDType;

  G4double theMeasuringTime;
  G4double theDeadTime;
  GmVDeadTimeDUList* theDeadTimeDetUnitList;

  hitVector theHitsCompatibleInTime;

  G4bool bParalyzable;

  MeasuringType theMeasuringType;
  G4double theCurrentIntervalStartTime;
  G4double thePreviousEventIntervalTime;
  G4RunManager* theRunManager;

  // for trigger independent 
  std::map<unsigned long long, G4double> theCurrentIntervalStartTimes;
  std::map<unsigned long long, G4double> thePreviousEventIntervalTimes;
  G4int theNShift;
  G4int theNTrigAncestors;
  std::set<unsigned long long> theHitTrigIDs;
};

typedef std::map< G4String, GmHitList* > gamosSDHitMap;

#endif
