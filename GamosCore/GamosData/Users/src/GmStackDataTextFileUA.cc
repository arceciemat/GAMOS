#include "GmStackDataTextFileUA.hh"
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
GmStackDataTextFileUA::GmStackDataTextFileUA()
{
  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmStackDataTextFileUA::BeginOfRunAction( const G4Run* )
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
G4ClassificationOfNewTrack GmStackDataTextFileUA::ClassifyNewTrack(const G4Track* aTrack)
{

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ ) {
    (*ite)->WriteText( aTrack, ite == theData.begin() );
  }

  *theFileOut << G4endl;  

  return fUrgent;
}

//----------------------------------------------------------------
void GmStackDataTextFileUA::EndOfRunAction( const G4Run* )
{
  theFileOut->close();
}
