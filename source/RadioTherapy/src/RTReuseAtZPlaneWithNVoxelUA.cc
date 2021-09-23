#include "RTReuseAtZPlaneWithNVoxelUA.hh"
#include "RTReuseAtZPlaneMessenger.hh"
#include "RTReuseAtZPlaneTrackInfo.hh"
#include "G4MultiFunctionalDetector.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4Gamma.hh"
#include "G4RunManager.hh"

//---------------------------------------------------------------------
RTReuseAtZPlaneWithNVoxelUA::RTReuseAtZPlaneWithNVoxelUA()
{
  new RTReuseAtZPlaneMessenger();

}

//---------------------------------------------------------------------
RTReuseAtZPlaneWithNVoxelUA::~RTReuseAtZPlaneWithNVoxelUA()
{

}

//---------------------------------------------------------------------
void RTReuseAtZPlaneWithNVoxelUA::BeginOfRunAction(const G4Run* )
{
  theZPlane = GmParameterMgr::GetInstance()->GetNumericValue("RTReuseAtZPlane:ZReusePlane",-1);
  G4String fGVName = GmParameterMgr::GetInstance()->GetStringValue("RTReuseAtZPlane:GoodVoxelsFileName","goodVoxels.lis");
  GmFileIn fin = GmFileIn::GetInstance(fGVName);
  std::vector<G4String> wl;
  for(;;) {
    if( !fin.GetWordsInLine(wl) ) break;
    theGoodVoxels.insert(GmGenUtils::GetInt(wl[1]));
  }
}

//---------------------------------------------------------------------
void RTReuseAtZPlaneWithNVoxelUA::PreUserTrackingAction(const G4Track* aTrack )
{
  //  G4cout << " PreUserTrackingAction Z " <<  fabs(aTrack->GetVertexPosition().z()-theZPlane) << " " << aTrack->GetVertexPosition().z() << " - " << theZPlane << G4endl; //GDEB
  if( aTrack->GetDefinition() == G4Gamma::Gamma() &&
      fabs(aTrack->GetVertexPosition().z()-theZPlane) < 1.e-6 ) {
    theReuseTrackID = aTrack->GetTrackID();
  }
}

//---------------------------------------------------------------------
void RTReuseAtZPlaneWithNVoxelUA::UserSteppingAction(const G4Step* aStep)
{
  G4VUserTrackInformation* trkInfo = aStep->GetTrack()->GetUserInformation();
  if( trkInfo ) {
    RTReuseAtZPlaneTrackInfo* trkInfoS = (RTReuseAtZPlaneTrackInfo*)trkInfo;
    if( trkInfoS->IsReuse() ){
      G4TrackVector secondariesTrack = GetStepSecondaries();
      G4TrackVector::const_iterator ite;
      for( ite = secondariesTrack.begin(); ite != secondariesTrack.end(); ite++ ){
	if( (*ite)->GetUserInformation() ) {
	  delete  (*ite)->GetUserInformation();
	}
	RTReuseAtZPlaneTrackInfo * ti = new RTReuseAtZPlaneTrackInfo; 
 	(*ite)->SetUserInformation( ti );
      }
    }
  }

  G4VPhysicalVolume* physVol = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if( physVol ) {
    G4VSensitiveDetector* SD = physVol->GetLogicalVolume()->GetSensitiveDetector();
    G4MultiFunctionalDetector* MFD = dynamic_cast<G4MultiFunctionalDetector*>(SD);
    if( MFD ) {
      G4VPrimitiveScorer* PS = MFD->GetPrimitive(0);
      if( PS ) {
	//--- Get parent track created by ReuseAtZPlane
	G4int copyNo = aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();
	if( theGoodVoxels.find(copyNo) != theGoodVoxels.end() &&
	    theReuseTrackID != -1 ) {
	  std::map<G4int,std::set<G4int> >::iterator item;
	  item = theNVoxelList.find(copyNo);
	  if( item == theNVoxelList.end() ) {
	    theNVoxelList[copyNo] = std::set<G4int>();
	    item = theNVoxelList.find(copyNo);
	  }
	  (*item).second.insert(theReuseTrackID);
	  theNEvents.insert(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
	  
	  //	  G4cout << G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() << " " << copyNo << " INSERT nv " << theReuseTrackID << " N= " << (*item).second.size() << " TOTNVOX " << theNVoxelList.size() << G4endl; //GDEB
	}
      }
    }
  }
  
}

//---------------------------------------------------------------------
void RTReuseAtZPlaneWithNVoxelUA::EndOfRunAction(const G4Run* )
{
  G4int nev = theNEvents.size();
  G4double ratio = theRatioSum / nev;
  G4double ratioErr = sqrt((theRatioSum2-theRatioSum*theRatioSum/nev)/(nev*nev));
  G4cout << " RTReuseAtZPlaneWithNVoxelUA RATIO " << ratio << " +- " << ratioErr << " nev " << nev << G4endl;

}

//---------------------------------------------------------------------
void RTReuseAtZPlaneWithNVoxelUA::EndOfEventAction(const G4Event* )
{
  std::map<G4int,std::set<G4int> >::iterator item;
  G4int nVox = 0;
  for( item = theNVoxelList.begin(); item != theNVoxelList.end(); item++ ) {
    nVox += (*item).second.size();
  }
  G4double ratio = 0;
  if( theNVoxelList.size() != 0 ) {
    ratio = float(theNVoxelList.size())/nVox;
  }
  theRatioSum += ratio;
  theRatioSum2 += ratio*ratio;
  //  G4cout << " NVOX " << nVox << " nv " << theNVoxelList.size() << G4endl; //GDEB
  //  G4cout << " RATIO " << ratio << " " << theRatioSum << " " << theRatioSum2 << G4endl; //GDEB

  theNVoxelList.clear();

}
