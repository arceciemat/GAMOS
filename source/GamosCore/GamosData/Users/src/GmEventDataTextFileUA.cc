#include "GmEventDataTextFileUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

//----------------------------------------------------------------
GmEventDataTextFileUA::GmEventDataTextFileUA()
{
  theDataType = DTTrack;
}

//----------------------------------------------------------------
void GmEventDataTextFileUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

  theOutputType = theName;

  //  G4cout << " theOutputType " << theOutputType << " filters " << theFilters.size() << G4endl;
  Init();
  OpenFileOut();

  theDataList.clear();
  theDataList.push_back("EventID");
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
void GmEventDataTextFileUA::BeginOfEventAction(const G4Event* evt)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( bUseAtInitial && (*ite)->IsInitial() ){
	(*ite)->WriteText( evt, ite == theData.begin() );
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmEventDataTextFileUA::UserSteppingAction(const G4Step* aStep )
{

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep );
    }

}


//----------------------------------------------------------------
void GmEventDataTextFileUA::EndOfEventAction(const G4Event* evt )
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( !bUseAtInitial || !(*ite)->IsInitial() ){
	(*ite)->WriteText( evt, ite == theData.begin() );// print theDataAccumulated
      }
    }

  *theFileOut << G4endl;
}

//----------------------------------------------------------------
void GmEventDataTextFileUA::EndOfRunAction( const G4Run* )
{
  theFileOut->close();
}
