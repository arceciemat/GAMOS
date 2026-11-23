/// \file GmBOptrEWBS.cc
/// \brief Implementation of the GmBOptrEWBS class

#include "GmBOptrEWBS.hh"
#include "GmBOptnEWBS.hh"

#include "G4BiasingProcessInterface.hh"
#include "G4GenericMessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptrEWBS::GmBOptrEWBS( G4String name )
: GmVBiasingOperator(name)
{
  fSplittingOperation = new GmBOptnEWBS(name);

  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  fNSplit = theParamMgr->GetIntegerValue(GetName()+":NSplit",2);
  fBiasPrimaryOnly = theParamMgr->GetBooleanValue(GetName()+":BiasPrimaryOnly",false);
  fBiasOnlyOnce = theParamMgr->GetBooleanValue(GetName()+":BiasOnlyOnce",false);
  bNonPhysicsBiased = false;  
}

//-----------------------------------------------------------------
void GmBOptrEWBS::StartRun()
{
  ConvertParticleProcessToPointers();
  
  fSplittingOperation->SetNSplit ( fNSplit );
  G4cout << GetName() << " : starting run with brem. splitting factor = " 
         << fNSplit;
  if ( fBiasPrimaryOnly ) G4cout << ", biasing only primaries ";
  else                    G4cout << ", biasing primary and secondary tracks ";
  if ( fBiasOnlyOnce )    G4cout << ", biasing only once per track ";
  else                    G4cout << ", biasing several times per track ";
  G4cout << " . " << G4endl;
}


//-----------------------------------------------------------------
void GmBOptrEWBS::StartTracking( const G4Track* /* track */ )
{
  // -- reset the number of times the splitting was applied:
  nInteractions = 0;
}

//-----------------------------------------------------------------
G4VBiasingOperation* GmBOptrEWBS::ProposeFinalStateBiasingOperation(const G4Track* track,
                                  const G4BiasingProcessInterface* callingProcess )
{
  if( !CheckParticleProcess( track->GetDefinition(), callingProcess ) ) return 0;

      // -- Check if biasing of primary particle only is requested. If so, and
  // -- if particle is not a primary one, don't ask for biasing:
  if ( fBiasPrimaryOnly && ( track->GetParentID() !=0 ) ) return 0;
  // -- Check if brem. splitting should be applied only once to the track,
  // -- and if so, and if brem. splitting already occured, don't ask for biasing:
  if ( fBiasOnlyOnce    && ( nInteractions > 0 )        ) return 0;
  
  // -- Count the number of times the brem. splitting is applied:
  nInteractions++;
  // -- Return the brem. splitting operation:
  return fSplittingOperation;
}



//-----------------------------------------------------------------
void GmBOptrEWBS::AddParticleProcess( G4String partName, G4String procName )
{
  /*
  if( partName != "e-" && partName != "e+" ) {
    G4Exception("GmBOptrEWBS::AddParticleProcess",
		"BOptrEWA1",
		FatalException,
		("Operator EWBremsSplitting can only be defined for e- or e+, it is defined for " + partName).c_str());
  }
  if( procName.find("Brem") == std::string::npos ) {
    G4Exception("GmBOptrEWBS::AddParticleProcess",
		"BOptrEWA2",
		FatalException,
		("Operator EWBremsSplitting can only be defined for annihilation proceses, it is defined for " + procName).c_str());
  }
  */
  
  theParticleProcessNames.insert(mmss::value_type(partName,procName));
  
}
