#include "GmTrackDataTextFileUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmTrackDataTextFileUA::GmTrackDataTextFileUA()
{
  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmTrackDataTextFileUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

  theOutputType = theName;

  Init();
  OpenFileOut();

  theDataList.clear();
  theDataList.push_back("EventID");
  theDataList.push_back("TrackID");
  theDataList.push_back("Particle");
  theDataList.push_back("FinalPosX");
  theDataList.push_back("FinalPosY");
  theDataList.push_back("FinalPosZ");
  theDataList.push_back("FinalMomX");
  theDataList.push_back("FinalMomY");
  theDataList.push_back("FinalMomZ");
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();
 
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->SetFileOutText( theFileOut );
    }

  WriteHeaderText();

}


//----------------------------------------------------------------
void GmTrackDataTextFileUA::PreUserTrackingAction(const G4Track* aTrack)
{
  //t  if( !bUseAtInitial ) return;

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ ) {
    if( bUseAtInitial && (*ite)->IsInitial() ){
      (*ite)->WriteText( aTrack, ite == theData.begin() );
    } else {
      (*ite)->Initialise();
    }
  }
  if( bUseAtInitial && (*ite)->IsInitial() ){
    *theFileOut << G4endl;  
  }
}

//----------------------------------------------------------------
void GmTrackDataTextFileUA::UserSteppingAction(const G4Step* aStep )
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }
}


//----------------------------------------------------------------
void GmTrackDataTextFileUA::PostUserTrackingAction(const G4Track* aTrack )
{
  if( bUseAtInitial ) return;

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ ) {
    (*ite)->WriteText( aTrack, ite == theData.begin() );
  }
  *theFileOut << G4endl;  

}

//----------------------------------------------------------------
void GmTrackDataTextFileUA::EndOfRunAction( const G4Run* )
{
  theFileOut->close();
}
