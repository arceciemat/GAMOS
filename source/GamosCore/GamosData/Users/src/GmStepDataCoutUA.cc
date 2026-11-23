#include "GmStepDataCoutUA.hh"
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
GmStepDataCoutUA::GmStepDataCoutUA()
{
  theDataType = DTStep;
}

//----------------------------------------------------------------
void GmStepDataCoutUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

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
  if( DataVerb(infoVerb) ) G4cout << " param " << "GmStepDataCoutUA"+theName+":DataList " << theDataList.size() << G4endl;
#endif
 
}


//----------------------------------------------------------------
void GmStepDataCoutUA::UserSteppingAction(const G4Step* aStep )
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->WriteCout( aStep, ite == theData.begin() );
    }
  G4cout << G4endl;  

}


//----------------------------------------------------------------
void GmStepDataCoutUA::EndOfRunAction( const G4Run* )
{
}
