/// \file GmBOptrDirBremsSplitting.cc
/// \brief Implementation of the GmBOptrDirBremsSplitting class

#include "GmBOptrDirBremsSplitting.hh"
#include "GmBOptnDirBremsSplitting.hh"

#include "G4BiasingProcessInterface.hh"
#include "G4GenericMessenger.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmBOptrDirBremsSplitting::GmBOptrDirBremsSplitting( G4String name )
: GmVBiasingOperator(name)
{
  fBremsSplittingOperation = new GmBOptnDirBremsSplitting(name);

  GmParameterMgr* theParamMgr = GmParameterMgr::GetInstance();
  fNSplit = theParamMgr->GetIntegerValue(GetName()+":NSplit",2);
  fNKill = theParamMgr->GetIntegerValue(GetName()+":NKill",fNSplit);
  fBiasPrimaryOnly = theParamMgr->GetBooleanValue(GetName()+":BiasPrimaryOnly",true);
  fBiasOnlyOnce = theParamMgr->GetBooleanValue(GetName()+":BiasOnlyOnce",true);
  bNonPhysicsBiased = false;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmBOptrDirBremsSplitting::StartRun()
{
  ConvertParticleProcessToPointers();
  
  fBremsSplittingOperation->SetNSplit ( fNSplit );
  fBremsSplittingOperation->SetNKill ( fNKill );
  G4cout << GetName() << " : starting run with brem. splitting factor = " 
         << fNSplit;
  if ( fBiasPrimaryOnly ) G4cout << ", biasing only primaries ";
  else                    G4cout << ", biasing primary and secondary tracks ";
  if ( fBiasOnlyOnce )    G4cout << ", biasing only once per track ";
  else                    G4cout << ", biasing several times per track ";
  G4cout << " . " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmBOptrDirBremsSplitting::StartTracking( const G4Track* /* track */ )
{
  // -- reset the number of times the brem. splitting was applied:
  nInteractions = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation*
GmBOptrDirBremsSplitting::
ProposeFinalStateBiasingOperation(const G4Track* track,
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
  return fBremsSplittingOperation;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
