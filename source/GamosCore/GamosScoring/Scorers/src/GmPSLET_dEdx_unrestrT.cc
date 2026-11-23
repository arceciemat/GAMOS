//#define VERBOSE_DOSEDEP
#include "GmPSLET_dEdx_unrestrT.hh"
#include "G4VPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4EmParameters.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmPSLET_dEdx_unrestrT::GmPSLET_dEdx_unrestrT(G4String name) 
  :GmCompoundScorer(name)
{ 
  theUnit = 1;
  theUnitName = "MeV/mm";

  bInitialized = false;
  //  theNEventsType = SNET_ByNFilled;
  
  G4EmParameters::Instance()->SetBuildCSDARange(true); // to build G4VEnergyLossProcess::theDEDXunRestrictedTable
}


//--------------------------------------------------------------------
G4bool GmPSLET_dEdx_unrestrT::ProcessHits(G4Step* aStep,G4TouchableHistory* th)
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
void GmPSLET_dEdx_unrestrT::SetParameters( const std::vector<G4String>& params)
{ 
  if( params.size() != 0 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSLET_dEdx_unrestrT::SetParameters",
		"There should no parameters",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }

  std::vector<G4String> paramsNC;
  paramsNC.push_back("GmPSdx_LET_dEdx_unrestr/GmG4PSTrackLength");

  GmCompoundScorer::SetParameters(paramsNC);

}

