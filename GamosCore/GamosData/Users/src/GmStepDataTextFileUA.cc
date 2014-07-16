#include "GmStepDataTextFileUA.hh"
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
GmStepDataTextFileUA::GmStepDataTextFileUA()
{
  theDataType = DTStep;
}

//----------------------------------------------------------------
void GmStepDataTextFileUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

  theOutputType = theName;

  //  G4cout << " theOutputType " << theOutputType << " filters " << theFilters.size() << G4endl;
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
 
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " param " << "GmStepDataTextFileUA"+theName+":DataList" << theDataList.size() << G4endl;
#endif

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->SetFileOutText( theFileOut );
    }

  WriteHeaderText();

}


//----------------------------------------------------------------
void GmStepDataTextFileUA::UserSteppingAction(const G4Step* aStep )
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->WriteText( aStep, ite == theData.begin() );
    }
  *theFileOut << G4endl;  

}


//----------------------------------------------------------------
void GmStepDataTextFileUA::EndOfRunAction( const G4Run* )
{
  theFileOut->close();
}
