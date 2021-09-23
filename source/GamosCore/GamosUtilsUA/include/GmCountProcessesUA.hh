///////////////////////////////////////////////////////////////////////////////
// File : GmCountProcessesUA.h
// Author: P.Arce  12.09.01
// Description:
// Modifications:
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef GmCountProcessesUA_h
#define GmCountProcessesUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include "globals.hh"

#include <string>
#include <map>
#include "G4ParticleDefinition.hh"

typedef std::multimap< G4String, G4String > mss;
typedef std::pair<G4String,G4String> pss;
typedef std::map <pss, int> mpssi;
typedef std::map<long int, mpssi* > mlmpssi;
typedef std::map<G4String,int > msi;
typedef std::map<long int, msi* > mlmsi;

class G4RRun;
class G4Track;
class G4Step;

class GmCountProcessesUA : public GmUserRunAction, 
			   public GmUserTrackingAction, 
			   public GmUserSteppingAction
{

 public:
  GmCountProcessesUA();
  ~GmCountProcessesUA();

 public:
 virtual void BeginOfRunAction(const G4Run* );
 virtual void EndOfRunAction(const G4Run* );
 virtual void PreUserTrackingAction(const G4Track* aTrack);
 virtual void UserSteppingAction(const G4Step* aStep);

  //---- Dump list of processes for each particle. printNsteps = 1 print in how many step the process was called, print only those processes with this number <> 0
  void DumpProcessList( std::ostream& out = G4cout);
  void DumpProcessCount( std::ostream& out = G4cout);
  void DumpCreatorProcessCount( std::ostream& out = G4cout);
  void DumpParticleCount( std::ostream& out = G4cout);

 private:
  mss theProcessList; // list of processes 
  mlmpssi theProcessCount;  
  mlmpssi theCreatorProcessCount;

  mlmsi theParticleCount;

  G4String theNameSuffix;
  G4bool bCreatorModel;
};

#endif
