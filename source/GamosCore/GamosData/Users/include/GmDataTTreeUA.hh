#ifndef GAMOS_NO_ROOT
#ifndef __GmDataTTreeUA__
#define __GmDataTTreeUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"
#include "GamosCore/GamosData/Users/include/GmVDataUser.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "TTree.h" 
#include "TFile.h" 

#include "vector"

#define MAX_NUM_CHAR 20

class GmDataTTreeUA : public GmUserRunAction, 
		      public GmUserEventAction, 
		      public GmUserTrackingAction,
		      public GmUserSteppingAction,
		      public GmVDataUser
{
public:
  GmDataTTreeUA();
  ~GmDataTTreeUA(){};

  virtual void EndOfRunAction(const G4Run*);
  virtual void BeginOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event* );
  virtual void EndOfEventAction(const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);

  //redefinition of BuildData() since 2D and 3D data do not make sense in a TTree
  virtual void BuildData();
  virtual void BuildDataFromLoopOnDataList(std::vector<GmVData*> &myData, std::vector<G4String> &myDataList);

  //declaration of public method for creating a TTree. 
  //returns true if new TTree is created. 
  //returns false if a TTree is already in memory. 
  bool CreateTree(const G4String TreeName);

  //declaration of public 
  //method to save the tree.  to be called only at the end of run.
  void SaveTree();

  //declaration of public
  //method to fill the tree with current entries.
  //if the tree size gets larger than the limit
  //the tree is saved into the current file,
  //with the method TTree::ChangeFile that closes
  //the saved files, increment the file counter, and
  //opens a new file with the new incremented name.
  bool FillTree(); 

  //private:
  G4String theTreeFileName;
  GmAnalysisMgr* theAnaMgr;
  int test_var;

public: //to make private 
  //private pointer to TTree object; 
  //for now only one tree per run can be defined. 
  //TODO: add tree containers to allow user to define  
  //multiple ntuples simultaneously. 
  TTree* theTree;

  //private pointer to TFile object where theTree is saved 
  TFile *theTreeFile; 

  //private counter to keep track of number of files 
  //that contains the full tree for the given run. 
  G4int theTTreeStartingIndex; 

  G4int MAX_BYTES;

  std::map<std::string,std::vector<G4int> > int_Step_map;
  std::map<std::string,std::vector<G4double> > double_Step_map;
  std::map<std::string,std::vector<std::string> > string_Step_map;

  std::map<std::string,std::vector<G4int> > int_Track_map;
  std::map<std::string,std::vector<G4double> > double_Track_map;
  std::map<std::string,std::vector<std::string> > string_Track_map;

  std::map<std::string,std::vector<G4int> > int_SecondaryTrack_map;
  std::map<std::string,std::vector<G4double> > double_SecondaryTrack_map;
  std::map<std::string,std::vector<std::string> > string_SecondaryTrack_map;

  std::map<std::string,G4int> int_Event_map;
  std::map<std::string,G4double> double_Event_map;
  std::map<std::string,std::string> string_Event_map;

  // previous version with std::vector also for Event data
  // std::map<std::string,std::vector<G4int> > int_Event_map;
  // std::map<std::string,std::vector<G4double> > double_Event_map;
  // std::map<std::string,std::vector<std::string> > string_Event_map;

  // kept for future including Run Data in TTree
  // std::map<std::string,std::vector<G4int> > int_Run_map;
  // std::map<std::string,std::vector<G4double> > double_Run_map;
  // std::map<std::string,std::vector<std::string> > string_Run_map;


  std::vector<GmVData*> theStepData;
  std::vector<G4String> theStepDataList;
  std::vector<GmVData*> theTrackData;
  std::vector<G4String> theTrackDataList;
  std::vector<GmVData*> theSecondaryTrackData;
  std::vector<G4String> theSecondaryTrackDataList;
  std::vector<GmVData*> theEventData;
  std::vector<G4String> theEventDataList;
  // std::vector<GmVData*> theRunData;
  // std::vector<G4String> theRunDataList;

};

#endif
#endif
