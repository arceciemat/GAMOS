//#define VERBOSE_DOSEDEP
#include "GmPSLET_ESpectT.hh"
#include "G4VPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//--------------------------------------------------------------------
GmPSLET_ESpectT::GmPSLET_ESpectT(G4String name) 
  :GmCompoundScorer(name)
{ 
  theUnit = 1;
  theUnitName = "MeV/mm";

  bInitialized = false;
  //  theNEventsType = SNET_ByNFilled;

}


//--------------------------------------------------------------------
G4bool GmPSLET_ESpectT::ProcessHits(G4Step* aStep,G4TouchableHistory* th)
{
  GmCompoundScorer::ProcessHits(aStep,th);  
  return TRUE;
} 

//--------------------------------------------------------------------
void GmPSLET_ESpectT::SetParameters( const std::vector<G4String>& params)
{ 
  if( params.size() != 0 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSLET_ESpectT::SetParameters",
		"There should no parameters",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  std::vector<G4String> paramsNC;
  paramsNC.push_back("GmPSLET_ESpect/GmG4PSNofStep");

  GmCompoundScorer::SetParameters(paramsNC);

}

