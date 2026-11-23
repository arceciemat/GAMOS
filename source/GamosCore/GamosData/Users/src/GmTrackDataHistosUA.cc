#include "GmTrackDataHistosUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmTrackDataHistosUA::GmTrackDataHistosUA()
{
  theDataType = DTTrack;

}


//----------------------------------------------------------------
void GmTrackDataHistosUA::InitialiseHistoNames()
{
  SetHistoNameAndNumber("GmTrackDataHistosUA",theFilters,theClassifier);
}

//----------------------------------------------------------------
void GmTrackDataHistosUA::BeginOfRunAction(const G4Run* )
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

  SetUserName( theName );

  BuildData();
}

//----------------------------------------------------------------
void GmTrackDataHistosUA::PreUserTrackingAction(const G4Track* aTrack)
{
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    { 
      if( bUseAtInitial && (*ite)->IsInitial() ){
	G4int index;
	if (theClassifier ) {
	  index = theClassifier->GetIndexFromTrack(aTrack);
	} else {
	  index = 0;
	}
	if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier );
	(*ite)->FillHisto( aTrack, index );
      } else {
	(*ite)->Initialise();
      }
    }
}

//----------------------------------------------------------------
void GmTrackDataHistosUA::UserSteppingAction(const G4Step* aStep )
{

  G4int index;
  if (theClassifier ) {
    index = theClassifier->GetIndexFromStep(aStep);
  } else {
    index = 0;
  }
  if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier );

  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      (*ite)->Accumulate( aStep, index );
    }
}

//----------------------------------------------------------------
void GmTrackDataHistosUA::PostUserTrackingAction(const G4Track* aTrack )
{
  G4int index;
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theData.begin(); ite != theData.end(); ite++ )
    {
      if( (*ite)->IsAccumulating() ){
	// fill histos for all indices (get index from theDataAccumulated)
	std::map<G4int, G4double> dataAccul = (*ite)->GetDataAccumulated();
	std::map<G4int, G4double>::const_iterator iteacul;
	for(iteacul = dataAccul.begin(); iteacul != dataAccul.end(); iteacul++) {
	  (*ite)->FillHisto( aTrack, (*iteacul).first );
	}
      } else  if( !bUseAtInitial || !(*ite)->IsInitial() ){
	if (theClassifier ) {
	  index = theClassifier->GetIndexFromTrack(aTrack);
	} else {
	  index = 0;
	}
	if( theHistos.find(index) == theHistos.end() ) BookHistos(index, theClassifier );
	(*ite)->FillHisto( aTrack, index );
      }
    }
}


//----------------------------------------------------------------
void GmTrackDataHistosUA::EndOfRunAction(const G4Run* )
{
  EndOfRun();
}
