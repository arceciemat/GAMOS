//#define VERBOSE_DOSEDEP
#include "GmPSLET_dEdxT.hh"
#include "G4VPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmPSLET_dEdxT::GmPSLET_dEdxT(G4String name) 
  :GmCompoundScorer(name)
{ 
  theUnit = 1;
  theUnitName = "MeV/mm";

  bInitialized = false;
  //  theNEventsType = SNET_ByNFilled;

}


//--------------------------------------------------------------------
G4bool GmPSLET_dEdxT::ProcessHits(G4Step* aStep,G4TouchableHistory* th)
{
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType == fTransportation || procType == fElectromagnetic ) {
    GmCompoundScorer::ProcessHits(aStep,th);
   return true;
  } else {
    return false;
  }
} 

//--------------------------------------------------------------------
void GmPSLET_dEdxT::SetParameters( const std::vector<G4String>& params)
{ 
  if( params.size() != 0 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSLET_dEdxT::SetParameters",
		"There should no parameters",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  std::vector<G4String> paramsNC;
  paramsNC.push_back("GmPSdx_LET_dEdx/GmG4PSTrackLength");

  GmCompoundScorer::SetParameters(paramsNC);

}

