//#define VERBOSE_DOSEDEP
#include "GmPSLETByESpectT.hh"
#include "G4VPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//--------------------------------------------------------------------
GmPSLETByESpectT::GmPSLETByESpectT(G4String name) 
  :GmCompoundScorer(name)
{ 
  theUnit = 1;
  theUnitName = "MeV/mm";

  bInitialized = false;
  theNEventsType = SNET_ByNFilled;

}


//--------------------------------------------------------------------
G4bool GmPSLETByESpectT::ProcessHits(G4Step* aStep,G4TouchableHistory* th)
{
  GmCompoundScorer::ProcessHits(aStep,th);  
  return TRUE;
} 

//--------------------------------------------------------------------
void GmPSLETByESpectT::SetParameters( const std::vector<G4String>& params)
{ 
  if( params.size() != 0 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSLETByESpectT::SetParameters",
		"There should no parameters",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  std::vector<G4String> paramsNC;
  paramsNC.push_back("GmPSLETByESpect/GmG4PSNofStep");

  GmCompoundScorer::SetParameters(paramsNC);

}

