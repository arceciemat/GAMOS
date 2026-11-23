#include "GmStackDataHistosUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmStackDataHistosUA::GmStackDataHistosUA()
{
  theDataType = DTTrack;

}


//----------------------------------------------------------------
void GmStackDataHistosUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("GmStackDataHistosUA",theFilters,theClassifier);
}

//----------------------------------------------------------------
void GmStackDataHistosUA::BeginOfRunAction(const G4Run* )
{
  SetUserName( theName );

  theOutputType = theName;

  theDataList.clear(); 
  theDataList.push_back("FinalPosX");
  theDataList.push_back("FinalPosY");
  theDataList.push_back("FinalPosZ");
  theDataList.push_back("FinalMomX");
  theDataList.push_back("FinalMomY");
  theDataList.push_back("FinalMomZ");
  theDataList.push_back("AccumulatedEnergyLost");
  theDataList.push_back("AccumulatedEnergyDeposited");

  BuildData();
}

//----------------------------------------------------------------
G4ClassificationOfNewTrack GmStackDataHistosUA::ClassifyNewTrack(const G4Track* aTrack)
{
  G4int index;
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ ){
    if (theClassifier ) {
      index = theClassifier->GetIndexFromTrack(aTrack);
    } else {
      index = 0;
    }
    if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier );
    (*ite)->FillHisto( aTrack, index );
  }

  return fUrgent;

}


//----------------------------------------------------------------
void GmStackDataHistosUA::EndOfRunAction(const G4Run* )
{
  EndOfRun();
}
