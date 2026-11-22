#include "RTVClassifierByExtraInfo.hh"
#include "RTExtraInfoMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//---------------------------------------------------------------
RTVClassifierByExtraInfo::RTVClassifierByExtraInfo(G4String name) : GmVClassifier( name )
{
  theExtraInfoMgr = RTExtraInfoMgr::GetInstance();
}

//---------------------------------------------------------------
G4String RTVClassifierByExtraInfo::GetIndexName(int64_t index)
{
  return GmGenUtils::itoa( int64_t(index) );
}
/*	if( theNExtraInfoLong == 2 ) {
	  G4Exception(" RTPhaseSpaceUA::BeginOfRunAction"
		      ,"Two many extra info providers for variables of type long",
		      JustWarning,
		      G4String("This provider info will not be used: "
			       + eiProviders[ii]->GetName()
			       + " , position "+ GmGenUtils::itoa(jj)).c_str());
	} else {
*/
