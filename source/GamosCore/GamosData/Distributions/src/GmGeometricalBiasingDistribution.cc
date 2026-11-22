#include "GmGeometricalBiasingDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//---------------------------------------------------------------------
GmGeometricalBiasingDistribution::GmGeometricalBiasingDistribution(G4String name)
{
  theName = name;
  BuildData();
}

//---------------------------------------------------------------------
void GmGeometricalBiasingDistribution::BuildData()
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String dataName = parMgr->GetStringValue(theName+":DataPre","");
  theDataPre = Build1StringData( dataName );

  dataName = parMgr->GetStringValue(theName+":DataPost","");
  theDataPost = Build1StringData( dataName );

  theMaxValue = parMgr->GetNumericValue(theName+":MaxValue",10);

  bNoRepeatWeight = G4bool( parMgr->GetNumericValue(theName+":NoRepeatWeight",0));

}


//---------------------------------------------------------------------
G4double GmGeometricalBiasingDistribution::GetValueFromStep(const G4Step* aStep)
{
  if( !theDataPre ) { 
    G4Exception("GmGeometricalBiasingDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":DataPre").c_str());
  } 

  if( !theDataPost ) { 
    G4Exception("GmGeometricalBiasingDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":DataPost").c_str());
  } 


  G4String indexPre = theDataPre->GetStringValueFromStep( aStep );
  G4String indexPost = theDataPost->GetStringValueFromStep( aStep );

  G4double valuePre = GetStringValueFromIndex( indexPre );
  G4double valuePost = GetStringValueFromIndex( indexPost );
  if( bNoRepeatWeight && 1.001/aStep->GetTrack()->GetWeight() >= valuePost )  {
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb) && valuePost > valuePre ) {
      G4cout << " GmGeometricalBiasingDistribution NoRepeatWeight " << 1.001/aStep->GetTrack()->GetWeight() << " " << valuePost << G4endl;
    }
#endif
    return 1;
  }

  if( valuePre == 0. ) {
    return 0;
  } else {
    G4double indexVal = valuePost/valuePre;
    if( indexVal > theMaxValue ) {
      /*t      G4Exception("GmGeometricalBiasingDistribution::GetValueFromStep",
		  "Too big return value",
		  JustWarning,
		  G4String("Value = "+GmGenUtils::ftoa(indexVal)+ " >= " + GmGenUtils::ftoa(theMaxValue)).c_str()); */
      indexVal = theMaxValue;
    }
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb)) {
      G4cout << "GmDistributionRatio::GetValueFromStep " << indexVal
	     << " indexPre " << indexPre 
	     << " indexPost " << indexPost
	     << " valuePre " << valuePre 
	     << " valuePost " << valuePost
	     << G4endl;
    }
#endif
    return indexVal;
  }

}
