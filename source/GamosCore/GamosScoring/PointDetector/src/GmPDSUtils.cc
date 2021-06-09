#include "GmPDSUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------
G4double GetWeight( const G4Track* aTrack, G4bool bWeightMustExist )
{
  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* trkInfow = dynamic_cast<GmTrackInfo*>(trkInfo);

  if( !trkInfow){
    if( bWeightMustExist ) {
      G4Exception("GmPDSUtils::GetWeight",
                  "Wrong argument",
                  FatalErrorInArgument,
                  "Track has no GmTrackInfo");
    } else {
      return aTrack->GetWeight();
    }
  } else if( trkInfow->GetType() != "GmPDSTrackInfo" ) { 
    return aTrack->GetWeight();
  }
  

  return trkInfow->GetWeight();
}


//------------------------------------------------------------------
void SetWeight( G4Track* aTrack, G4double wei )
{
  G4VUserTrackInformation* trkInfo = aTrack->GetUserInformation();
  GmTrackInfo* trkInfow = dynamic_cast<GmTrackInfo*>(trkInfo);

  if(!trkInfow){
    G4Exception("GmPDSUtils::SetWeight",
		"Wrong argument",
		FatalErrorInArgument,
		"Track has no GmTrackInfo");
  }
  
  trkInfow->SetWeight(wei);
  aTrack->SetWeight( wei );
}

//------------------------------------------------------------------
G4double GetPDSNumericParameter(const G4String& param, G4String partName, G4double defVal)
{
  if( partName == "G4Neutron" ) {
    partName = "neutron";
  } else if( partName == "G4Gamma" ) {
    partName = "gamma";
  }

  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  G4String paramFull = "GmPDS:"+partName+":"+param;
  //  G4cout << "  GetPDSNumericParameter " << param << " " << partName << " " << defVal << " FULL " << paramFull << G4endl; 
  if( theParamMgr->IsParameterInScript(paramFull) ) {
    return theParamMgr->GetNumericValue(paramFull,defVal);
  } else {
    paramFull = "GmPDS:"+param;
    return theParamMgr->GetNumericValue(paramFull,defVal);
  }

}

//------------------------------------------------------------------
G4String GetPDSStringParameter(const G4String& param, G4String partName, G4String defVal)
{
  if( partName == "G4Neutron" ) {
    partName = "neutron";
  } else if( partName == "G4Gamma" ) {
    partName = "gamma";
  }

  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  G4String paramFull = "GmPDS:"+partName+":"+param;
  //  G4cout << "  GetPDSStringParameter " << param << " " << partName << " " << defVal << " FULL " << paramFull << G4endl;
  if( theParamMgr->IsParameterInScript(paramFull) ) {
    return theParamMgr->GetStringValue(paramFull,defVal);
  } else {
    paramFull = "GmPDS:"+param;
    return theParamMgr->GetStringValue(paramFull,defVal);
  }

}


