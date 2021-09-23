#ifndef GAMOS_NO_ROOT
#include "GmDataTTreeUA.hh"

GmDataTTreeUA::GmDataTTreeUA()
{


  MAX_BYTES = 500000000; //this is to limit the size of a single root file
}

void GmDataTTreeUA::BeginOfRunAction(const G4Run*)
{
  SetUserName( theName );
  //getting TTree file name
  //same name will be used as the name of the TTree to be saved in the file.
  //future development can forsee multiple TTrees in the same UA, therefore
  //changing the TTree name convention.
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  this->theTreeFileName = paramMgr->GetStringValue(theUserName+":TreeFileName","GmDataTTree");

  BuildData();

  //The TTree is filled event by event.
  //The same TTree can contain Event, Step, Track, and SecondaryTrack GAMOS Data.
  //The user must specify a list of GAMOS Data for each category.

  //Reading user preferred starting index for ttrees 
  theTTreeStartingIndex = GmParameterMgr::GetInstance()->GetNumericValue(theUserName+":TreeStartingIndex",1000);

  //Initializing this->theTree and this->theTreeFile pointers to null
  this->theTreeFile = 0;
  this->theTree = 0;

  //function to open the TFile and create a correspondent empty TTree 
  CreateTree(this->theTreeFileName);

  //declare iterators for maps
  std::vector<GmVData*>::const_iterator ite;
  std::map<std::string,std::vector<G4int> >::iterator int_iter;
  std::map<std::string,std::vector<G4double> >::iterator double_iter;
  std::map<std::string,std::vector<std::string> >::iterator string_iter;
  std::map<std::string,G4int>::iterator int_evt_iter;
  std::map<std::string,G4double>::iterator double_evt_iter;
  std::map<std::string,std::string>::iterator string_evt_iter;

  //create and initialize Event_map entry for each EventData
  for( ite = theEventData.begin(); ite != theEventData.end(); ite++ ){
    if((*ite)->GetCType()=="int") {
      int_Event_map[(*ite)->GetName()] = -999;
    }
    else if((*ite)->GetCType()=="char") {
      string_Event_map[(*ite)->GetName()] = "init";
    }
    else {
      double_Event_map[(*ite)->GetName()] = -999.999;
    }
  }

  //Branching for EventData
  for(int_evt_iter = int_Event_map.begin();int_evt_iter!=int_Event_map.end();int_evt_iter++) {
    this->theTree->Branch(("Event_"+(*int_evt_iter).first).c_str(),&((*int_evt_iter).second));
  }
  for(double_evt_iter = double_Event_map.begin();double_evt_iter!=double_Event_map.end();double_evt_iter++) {
    this->theTree->Branch(("Event_"+(*double_evt_iter).first).c_str(),&((*double_evt_iter).second));
  }
  for(string_evt_iter = string_Event_map.begin();string_evt_iter!=string_Event_map.end();string_evt_iter++) {
    this->theTree->Branch(("Event_"+(*string_evt_iter).first).c_str(),&((*string_evt_iter).second));
  }

  //create and initialize Step_map entry for each StepData
  for( ite = theStepData.begin(); ite != theStepData.end(); ite++ ){
    if((*ite)->GetCType()=="int") {
      int_Step_map[(*ite)->GetName()].push_back(-999);
    }
    else if((*ite)->GetCType()=="char") {
      string_Step_map[(*ite)->GetName()].push_back("init");
    }
    else {
      double_Step_map[(*ite)->GetName()].push_back(-999.999);
    }
  }
  //Branching for StepData
  for(int_iter = int_Step_map.begin();int_iter!=int_Step_map.end();int_iter++) {
    this->theTree->Branch(("Step_"+(*int_iter).first).c_str(),&((*int_iter).second));
  }
  for(double_iter = double_Step_map.begin();double_iter!=double_Step_map.end();double_iter++) {
    this->theTree->Branch(("Step_"+(*double_iter).first).c_str(),&((*double_iter).second));
  }
  for(string_iter = string_Step_map.begin();string_iter!=string_Step_map.end();string_iter++) {
    this->theTree->Branch(("Step_"+(*string_iter).first).c_str(),&((*string_iter).second));
  }


  //create and initialize Track_map entry for each TrackData
  for( ite = theTrackData.begin(); ite != theTrackData.end(); ite++ ){
    if((*ite)->GetCType()=="int") {
      int_Track_map[(*ite)->GetName()].push_back(-999);
    }
    else if((*ite)->GetCType()=="char") {
      string_Track_map[(*ite)->GetName()].push_back("init");
    }
    else {
      double_Track_map[(*ite)->GetName()].push_back(-999.999);
    }
  }
  //Branching for TrackData
  for(int_iter = int_Track_map.begin();int_iter!=int_Track_map.end();int_iter++) {
    this->theTree->Branch(("Track_"+(*int_iter).first).c_str(),&((*int_iter).second));
  }
  for(double_iter = double_Track_map.begin();double_iter!=double_Track_map.end();double_iter++) {
    this->theTree->Branch(("Track_"+(*double_iter).first).c_str(),&((*double_iter).second));
  }
  for(string_iter = string_Track_map.begin();string_iter!=string_Track_map.end();string_iter++) {
    this->theTree->Branch(("Track_"+(*string_iter).first).c_str(),&((*string_iter).second));
  }


  //create and initialize SecondaryTrack_map entry for each SecondaryTrackData
  for( ite = theSecondaryTrackData.begin(); ite != theSecondaryTrackData.end(); ite++ ){
    if((*ite)->GetCType()=="int") {
      int_SecondaryTrack_map[(*ite)->GetName()].push_back(-999);
    }
    else if((*ite)->GetCType()=="char") {
      string_SecondaryTrack_map[(*ite)->GetName()].push_back("init");
    }
    else {
      double_SecondaryTrack_map[(*ite)->GetName()].push_back(-999.999);
    }
  }
  //Branching for SecondaryTrackData
  for(int_iter = int_SecondaryTrack_map.begin();int_iter!=int_SecondaryTrack_map.end();int_iter++) {
    this->theTree->Branch(("SecondaryTrack_"+(*int_iter).first).c_str(),&((*int_iter).second));
  }
  for(double_iter = double_SecondaryTrack_map.begin();double_iter!=double_SecondaryTrack_map.end();double_iter++) {
    this->theTree->Branch(("SecondaryTrack_"+(*double_iter).first).c_str(),&((*double_iter).second));
  }
  for(string_iter = string_SecondaryTrack_map.begin();string_iter!=string_SecondaryTrack_map.end();string_iter++) {
    this->theTree->Branch(("SecondaryTrack_"+(*string_iter).first).c_str(),&((*string_iter).second));
  }


  // Following is commented: no Run Data implemented for this TTree at the moment.

  // //create and initialize Run_map entry for each RunData
  // for( ite = theRunData.begin(); ite != theRunData.end(); ite++ ){
  //   if((*ite)->GetCType()=="int") {
  //     int_Run_map[(*ite)->GetName()].push_back(-999);
  //   }
  //   else if((*ite)->GetCType()=="char") {
  //     string_Run_map[(*ite)->GetName()].push_back("init");
  //   }
  //   else {
  //     double_Run_map[(*ite)->GetName()].push_back(-999.999);
  //   }
  // }
  // //Branching for RunData
  // for(int_iter = int_Run_map.begin();int_iter!=int_Run_map.end();int_iter++) {
  //   this->theTree->Branch(("Run_"+(*int_iter).first).c_str(),&((*int_iter).second));
  // }
  // for(double_iter = double_Run_map.begin();double_iter!=double_Run_map.end();double_iter++) {
  //   this->theTree->Branch(("Run_"+(*double_iter).first).c_str(),&((*double_iter).second));
  // }
  // for(string_iter = string_Run_map.begin();string_iter!=string_Run_map.end();string_iter++) {
  //   this->theTree->Branch(("Run_"+(*string_iter).first).c_str(),&((*string_iter).second));
  // }

}




void GmDataTTreeUA::BeginOfEventAction(const G4Event* evt)
{

  std::vector<GmVData*>::const_iterator ite;
  std::map<std::string,std::vector<G4int> >::iterator int_iter;
  std::map<std::string,std::vector<G4double> >::iterator double_iter;
  std::map<std::string,std::vector<std::string> >::iterator string_iter;
  std::map<std::string,G4int>::iterator int_evt_iter;
  std::map<std::string,G4double>::iterator double_evt_iter;
  std::map<std::string,std::string>::iterator string_evt_iter;

  for(int_evt_iter = int_Event_map.begin();int_evt_iter!=int_Event_map.end();int_evt_iter++) {
    (*int_evt_iter).second = -999;
  }
  for(double_evt_iter = double_Event_map.begin();double_evt_iter!=double_Event_map.end();double_evt_iter++) {
    (*double_evt_iter).second = -99.99;
  }
  for(string_evt_iter = string_Event_map.begin();string_evt_iter!=string_Event_map.end();string_evt_iter++) {
    (*string_evt_iter).second = "init";
  }

  for(int_iter = int_Step_map.begin();int_iter!=int_Step_map.end();int_iter++) {
    (*int_iter).second.clear();
  }
  for(double_iter = double_Step_map.begin();double_iter!=double_Step_map.end();double_iter++) {
    (*double_iter).second.clear();
  }
  for(string_iter = string_Step_map.begin();string_iter!=string_Step_map.end();string_iter++) {
    (*string_iter).second.clear();
  }

  for(int_iter = int_Track_map.begin();int_iter!=int_Track_map.end();int_iter++) {
    (*int_iter).second.clear();
  }
  for(double_iter = double_Track_map.begin();double_iter!=double_Track_map.end();double_iter++) {
    (*double_iter).second.clear();
  }
  for(string_iter = string_Track_map.begin();string_iter!=string_Track_map.end();string_iter++) {
    (*string_iter).second.clear();
  }

  for(int_iter = int_SecondaryTrack_map.begin();int_iter!=int_SecondaryTrack_map.end();int_iter++) {
    (*int_iter).second.clear();
  }
  for(double_iter = double_SecondaryTrack_map.begin();double_iter!=double_SecondaryTrack_map.end();double_iter++) {
    (*double_iter).second.clear();
  }
  for(string_iter = string_SecondaryTrack_map.begin();string_iter!=string_SecondaryTrack_map.end();string_iter++) {
    (*string_iter).second.clear();
  }
  // for(int_iter = int_Run_map.begin();int_iter!=int_Run_map.end();int_iter++) {
  //   (*int_iter).second.clear();
  // }
  // for(double_iter = double_Run_map.begin();double_iter!=double_Run_map.end();double_iter++) {
  //   (*double_iter).second.clear();
  // }
  // for(string_iter = string_Run_map.begin();string_iter!=string_Run_map.end();string_iter++) {
  //   (*string_iter).second.clear();
  // }


  for( ite = theEventData.begin(); ite != theEventData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	if((*ite)->GetCType()=="int") {
	  int_Event_map[(*ite)->GetName()] = (*ite)->GetValueFromEvent(evt,0);
	}
	else if((*ite)->GetCType()=="char") {
	  string_Event_map[(*ite)->GetName()] = (*ite)->GetStringValueFromEvent(evt);
	} 
	else {
	  double_Event_map[(*ite)->GetName()] = (*ite)->GetValueFromEvent(evt,0);
	}
      } else {
        (*ite)->Initialise();
      }
    }

}



void GmDataTTreeUA::PreUserTrackingAction(const G4Track* aTrack)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theTrackData.begin(); ite != theTrackData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	if((*ite)->GetCType()=="int") {
	  int_Track_map[(*ite)->GetName()].push_back((*ite)->GetValueFromTrack(aTrack,0));
	}
	else if((*ite)->GetCType()=="char") {
	  string_Track_map[(*ite)->GetName()].push_back((*ite)->GetStringValueFromTrack(aTrack));
	} 
	else {
	  double_Track_map[(*ite)->GetName()].push_back((*ite)->GetValueFromTrack(aTrack,0));
	}
      } else {
        (*ite)->Initialise();
      }
    }
}

void GmDataTTreeUA::PostUserTrackingAction(const G4Track* aTrack)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theTrackData.begin(); ite != theTrackData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	if((*ite)->GetCType()=="int") {
	  int_Track_map[(*ite)->GetName()].push_back((*ite)->GetValueFromTrack(aTrack,0));
	}
	else if((*ite)->GetCType()=="char") {
	  string_Track_map[(*ite)->GetName()].push_back((*ite)->GetStringValueFromTrack(aTrack));
	} 
	else {
	  double_Track_map[(*ite)->GetName()].push_back((*ite)->GetValueFromTrack(aTrack,0));
	}
      }
    }
}

//----------------------------------------------------------------
void GmDataTTreeUA::UserSteppingAction(const G4Step* aStep )
{

  std::vector<GmVData*>::const_iterator ite;
  //accumulating Event Data
  for( ite = theEventData.begin(); ite != theEventData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }
  //accumulating Track Data
  for( ite = theTrackData.begin(); ite != theTrackData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }

  //getting Step Data
  for( ite = theStepData.begin(); ite != theStepData.end(); ite++ )
    {
      if((*ite)->GetCType()=="int") {
        int_Step_map[(*ite)->GetName()].push_back((*ite)->GetValueFromStep(aStep,0));
      }
      else if((*ite)->GetCType()=="char") {
        string_Step_map[(*ite)->GetName()].push_back((*ite)->GetStringValueFromStep(aStep));
      }
      else {
        double_Step_map[(*ite)->GetName()].push_back((*ite)->GetValueFromStep(aStep,0));
      }
    }



  //getting Secondary Track Data
  const G4Track* aTrack = aStep->GetTrack();
  G4TrackVector secondaries = GetStepSecondaries();
  unsigned int nSeco = secondaries.size();
  for(unsigned int ii = 0; ii < nSeco; ii++ ){
    G4Track* secoTrack = secondaries[ii];
    for( ite = theSecondaryTrackData.begin(); ite != theSecondaryTrackData.end(); ite++ )
      {
	if((*ite)->GetCType()=="int") {
	  int_SecondaryTrack_map[(*ite)->GetName()].push_back((*ite)->GetValueFromSecoTrack(aTrack,secoTrack,0));
	}
	else if((*ite)->GetCType()=="char") {
	  string_SecondaryTrack_map[(*ite)->GetName()].push_back((*ite)->GetStringValueFromSecoTrack(aTrack,secoTrack));
	} 
	else {
	  double_SecondaryTrack_map[(*ite)->GetName()].push_back((*ite)->GetValueFromSecoTrack(aTrack,secoTrack,0));
	}
      }
  }


}



void GmDataTTreeUA::EndOfEventAction(const G4Event* evt)
{

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theEventData.begin(); ite != theEventData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	if((*ite)->GetCType()=="int") {
	  int_Event_map[(*ite)->GetName()] = (*ite)->GetValueFromEvent(evt,0);
	}
	else if((*ite)->GetCType()=="char") {
	  string_Event_map[(*ite)->GetName()] = (*ite)->GetStringValueFromEvent(evt);
	} 
	else {
	  double_Event_map[(*ite)->GetName()] = (*ite)->GetValueFromEvent(evt,0);
	}
      }
    }

  FillTree();
}


void GmDataTTreeUA::EndOfRunAction(const G4Run*)
{
  //saving the TTree
  if( DataVerb(infoVerb) ) G4cout<<"saving TTree"<<G4endl;
  SaveTree();
}



//----------------------------------------------------------------------------
//method to create a new TTree object.
//the method returns true if a new object is created.
//false if this->theTree object was already in memory.
bool GmDataTTreeUA::CreateTree(const G4String TreeName) 
{
  if(!this->theTree) {
    if( DataVerb(infoVerb) ) G4cout<<"*** GmDataTTreeUA::CreateTree ***"<<G4endl;
    G4String FileName = Form("%s_tree_%d.root",this->theTreeFileName.c_str(),theTTreeStartingIndex);
    this->theTreeFile = new TFile(FileName.c_str(),"RECREATE");
    this->theTree = new TTree(TreeName.c_str(),TreeName.c_str());
    this->theTree->SetDirectory(this->theTreeFile);
    this->theTree->SetFileNumber(theTTreeStartingIndex);
    //    TotEntriesCurrentTree = 0;
    return true;
  }
  else {
    if( DataVerb(infoVerb) ) G4cout<<"GmSDTTreeUA::CreateTree  tree already in memory"<<G4endl;
    return false;
  }
  if( DataVerb(infoVerb) ) G4cout<<"GmSDTTreeUA::CreateTree end"<<G4endl;
}


//----------------------------------------------------------------------
//method to save the tree.  to be called only at the end of run.
void GmDataTTreeUA::SaveTree()
{
  if(this->theTree) {
    if( DataVerb(infoVerb) ) G4cout<<"GmSDTTreeUA::SaveTree"<<G4endl;
    this->theTreeFile = this->theTree->GetCurrentFile(); 
    this->theTreeFile->cd(); //mandatory to avoid overlap with other open files
    this->theTree->Write();
    if( DataVerb(infoVerb) ) G4cout<<"treeFile Address = "<<this->theTreeFile<<G4endl;
    if( DataVerb(infoVerb) ) G4cout<<"treeFile Name = "<<this->theTreeFile->GetName()<<G4endl;
    delete this->theTree;
    this->theTreeFile->Close();
    delete this->theTreeFile;
  }
}



//----------------------------------------------------------------------------
//method to fill the tree with current entries.
//if the tree size gets larger than the limit
//the tree is saved into the current file,
//with the method TTree::ChangeFile that closes
//the saved files, increment the file counter, and
//opens a new file with the new incremented name.
bool GmDataTTreeUA::FillTree() 
{
  if(this->theTree) {
    this->theTree->Fill();
    //    TotEntriesCurrentTree++;
    if(this->theTreeFile->GetSize() > MAX_BYTES) {
      this->theTreeFile = this->theTree->ChangeFile(this->theTreeFile);
      //      this->theTree->SetDirectory(this->theTreeFile);
      if( DataVerb(infoVerb) ) G4cout<<"new file name = "<<this->theTree->GetCurrentFile()->GetName()<<G4endl;
    }
    return true;
  }
  else {
    return false;
  }
}


//----------------------------------------------------------------
void GmDataTTreeUA::BuildData()
{
  bUseAtInitial = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theUserName+":UseAtInitial",0));

  //Reading list for Event Data
  theEventDataList = GmParameterMgr::GetInstance()->GetVStringValue(theUserName+":EventDataList",theEventDataList);
  BuildDataFromLoopOnDataList(theEventData,theEventDataList);
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":EventDataList " << theEventDataList.size() << G4endl;
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":EventData " << theEventData.size() << G4endl;
#endif

  //Reading list for Step Data
  theStepDataList = GmParameterMgr::GetInstance()->GetVStringValue(theUserName+":StepDataList",theStepDataList);
  BuildDataFromLoopOnDataList(theStepData,theStepDataList);
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":StepDataList " << theStepDataList.size() << G4endl;
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":StepData " << theStepData.size() << G4endl;
#endif

  //Reading list for Track Data
  theTrackDataList = GmParameterMgr::GetInstance()->GetVStringValue(theUserName+":TrackDataList",theTrackDataList);
  BuildDataFromLoopOnDataList(theTrackData,theTrackDataList);
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":TrackDataList " << theTrackDataList.size() << G4endl;
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":TrackData " << theTrackData.size() << G4endl;
#endif

  //Reading list for SecondaryTrack Data
  theSecondaryTrackDataList = GmParameterMgr::GetInstance()->GetVStringValue(theUserName+":SecondaryTrackDataList",theSecondaryTrackDataList);
  BuildDataFromLoopOnDataList(theSecondaryTrackData,theSecondaryTrackDataList);
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":SecondaryTrackDataList" << theSecondaryTrackDataList.size() << G4endl;
  if( DataVerb(infoVerb) ) G4cout << " param " << theUserName+":SecondaryTrackData" << theSecondaryTrackData.size() << G4endl;
#endif

}


void GmDataTTreeUA::BuildDataFromLoopOnDataList(std::vector<GmVData*> &myData, std::vector<G4String> &myDataList)
{
  for( unsigned int ii = 0; ii < myDataList.size(); ii++) {
    // check if it is single or double data
    G4int ivs = myDataList[ii].find(".vs.");
    G4int iprof = myDataList[ii].find(".prof.");

    //      G4cout << " ivs " << ivs << " iprof " << iprof << " " << myDataList[ii] << G4endl;
    if( ivs == G4int(std::string::npos) && iprof == G4int(std::string::npos) )
      {
        G4String dataName = myDataList[ii];
        GmVData* data = Build1Data(dataName);
#ifndef GAMOS_NO_VERBOSE 
        if( DataVerb(infoVerb) ) G4cout <<  "GmDataTTreeUA::BuildData 1D " << dataName << G4endl;
#endif
        data->SetName( myDataList[ii] );
        myData.push_back( data );
      }
    else  if( ivs != G4int(std::string::npos) && iprof == G4int(std::string::npos) )
      {
        G4Exception("GmDataTTreeUA::GmDataTTreeUA",
                    "No 2D Data available for TTree",
                    FatalErrorInArgument,
                    G4String("Asking for Data: " + myDataList[ii]).c_str());
      }
    else  if( ivs == G4int(std::string::npos) && iprof != G4int(std::string::npos) )
      {
        G4Exception("GmDataTTreeUA::GmDataTTreeUA",
                    "No Prof1D Data available for TTree",
                    FatalErrorInArgument,
                    G4String("Asking for Data: " + myDataList[ii]).c_str());
      }
    else  if( ivs != G4int(std::string::npos) && iprof != G4int(std::string::npos) )
      {
        G4Exception("GmDataTTreeUA::GmDataTTreeUA",
                    "No Prof2D Data available for TTree",
                    FatalErrorInArgument,
                    G4String("Asking for Data: " + myDataList[ii]).c_str());
      }
  }

}
#endif
