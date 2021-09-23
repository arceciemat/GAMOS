#include "GmBOptrChangeCrossSectionByDistribution.hh"
#include "G4BiasingProcessInterface.hh"
#include "G4BOptnChangeCrossSection.hh"
#include "GmBiasingVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4VProcess.hh"

#include "Randomize.hh"

#include "G4InteractionLawPhysical.hh"

GmBOptrChangeCrossSectionByDistribution::GmBOptrChangeCrossSectionByDistribution(G4String name)
  : GmVBiasingOperator(name),
    fSetup(true)
{

  G4String distName = GmParameterMgr::GetInstance()->GetStringValue(name+":Distribution","");
  if( distName != "" ) {
    std::vector<G4String> params;
    params.push_back(distName);
    params.push_back(distName);
    theDistribution =  GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);
  } else {
    G4Exception("GmBOptrChangeCrossSectionByDistribution::GmBOptrChangeCrossSectionByDistribution",
		"",
		FatalException,
		("No distribution given, please use /gamos/setParam "+name+":Distribution MY_DISTRIBUTION_NAME").c_str());
  }
  bNonPhysicsBiased = false; 
}


GmBOptrChangeCrossSectionByDistribution::~GmBOptrChangeCrossSectionByDistribution()
{
  for ( std::map< const G4BiasingProcessInterface*, G4BOptnChangeCrossSection* >::iterator 
          it = fChangeCrossSectionOperations.begin() ;
        it != fChangeCrossSectionOperations.end() ;
        it++ ) delete (*it).second;
}


void GmBOptrChangeCrossSectionByDistribution::StartRun()
{
  ConvertParticleProcessToPointers();

  mmpp::const_iterator ite = theParticleProcesses.begin();
  fParticleToBias = (*ite).first;
  std::cout << " GmBOptrChangeCrossSectionByDistribution ::StartRun( " << theParticleProcesses.size() << " fParticleToBias " << fParticleToBias << G4endl; //GDEB

  // --------------
  // -- Setup stage:
  // ---------------
  // -- Start by collecting processes under biasing, create needed biasing
  // -- operations and associate these operations to the processes:
  if ( fSetup )
    {
      const G4ProcessManager* processManager = fParticleToBias->GetProcessManager();
      const G4BiasingProcessSharedData* sharedData =
        G4BiasingProcessInterface::GetSharedData( processManager );
      if ( sharedData ) // -- sharedData tested, as is can happen a user attaches an operator to a
                        // -- volume but without defined BiasingProcessInterface processes.
        {
          for ( size_t i = 0 ; i < (sharedData->GetPhysicsBiasingProcessInterfaces()).size(); i++ )
            {
              const G4BiasingProcessInterface* wrapperProcess =
                (sharedData->GetPhysicsBiasingProcessInterfaces())[i];
              G4String operationName = "XSchange-" + 
                wrapperProcess->GetWrappedProcess()->GetProcessName();
              fChangeCrossSectionOperations[wrapperProcess] = 
                new G4BOptnChangeCrossSection(operationName);
            }
        }
      fSetup = false;
    }
}


G4VBiasingOperation* 
GmBOptrChangeCrossSectionByDistribution::ProposeOccurenceBiasingOperation(const G4Track*            track, 
                                                              const G4BiasingProcessInterface*
                                                                               callingProcess)
{
  if( !CheckParticleProcess( track->GetDefinition(), callingProcess ) ) return 0;
      
  // -----------------------------------------------------
  // -- Check if current particle type is the one to bias:
  // -----------------------------------------------------
  if ( track->GetDefinition() != fParticleToBias ) return 0;
  
  
  // ---------------------------------------------------------------------
  // -- select and setup the biasing operation for current callingProcess:
  // ---------------------------------------------------------------------
  // -- Check if the analog cross-section well defined : for example, the conversion
  // -- process for a gamma below e+e- creation threshold has an DBL_MAX interaction
  // -- length. Nothing is done in this case (ie, let analog process to deal with the case)
  G4double analogInteractionLength =  
    callingProcess->GetWrappedProcess()->GetCurrentInteractionLength();
  if ( analogInteractionLength > DBL_MAX/10. ) return 0;

  // -- Analog cross-section is well-defined:
  G4double analogXS = 1./analogInteractionLength;

  // -- Choose a constant cross-section bias. But at this level, this factor can be made
  // -- direction dependent, like in the exponential transform MCNP case, or it
  // -- can be chosen differently, depending on the process, etc.

  G4double XStransformation = theDistribution->GetValueFromStep(track->GetStep());
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(debugVerb) ) G4cout << "GmBOptrChangeCrossSectionByDistribution::GmBOptrChangeCrossSectionByDistribution XS_FACTOR="  << XStransformation << G4endl;
#endif
  
  // -- fetch the operation associated to this callingProcess:
  G4BOptnChangeCrossSection*   operation = fChangeCrossSectionOperations[callingProcess];
  // -- get the operation that was proposed to the process in the previous step:
  G4VBiasingOperation* previousOperation = callingProcess->GetPreviousOccurenceBiasingOperation();
  
  // -- now setup the operation to be returned to the process: this
  // -- consists in setting the biased cross-section, and in asking
  // -- the operation to sample its exponential interaction law.
  // -- To do this, to first order, the two lines:
  //        operation->SetBiasedCrossSection( XStransformation * analogXS );
  //        operation->Sample();
  // -- are correct and sufficient.
  // -- But, to avoid having to shoot a random number each time, we sample
  // -- only on the first time the operation is proposed, or if the interaction
  // -- occured. If the interaction did not occur for the process in the previous,
  // -- we update the number of interaction length instead of resampling.
  if ( previousOperation == 0 )
    {
      operation->SetBiasedCrossSection( XStransformation * analogXS );
      operation->Sample();
    }
  else
    {
      if (  previousOperation != operation )
        {
          // -- should not happen !
          G4ExceptionDescription ed;
          ed << " Logic problem in operation handling !" << G4endl;
          G4Exception("GmBOptrChangeCrossSectionByDistribution::ProposeOccurenceBiasingOperation(...)",
                      "exGB01.02",
                      JustWarning,
                      ed);
          return 0;
        }
      if ( operation->GetInteractionOccured() )
        {
          operation->SetBiasedCrossSection( XStransformation * analogXS );
          operation->Sample();
        }
      else
        {
          // -- update the 'interaction length' and underneath 'number of interaction lengths'
          // -- for past step  (this takes into accout the previous step cross-section value)
          operation->UpdateForStep( callingProcess->GetPreviousStepSize() );
          // -- update the cross-section value:
          operation->SetBiasedCrossSection( XStransformation * analogXS );
          // -- forces recomputation of the 'interaction length' taking into account above
          // -- new cross-section value [tricky : to be improved]
          operation->UpdateForStep( 0.0 );
        }
    }
  
  return operation;
  
}


void GmBOptrChangeCrossSectionByDistribution::
OperationApplied(const G4BiasingProcessInterface*           callingProcess, 
                 G4BiasingAppliedCase,
                 G4VBiasingOperation*             occurenceOperationApplied,
                 G4double,
                 G4VBiasingOperation*,    
                 const G4VParticleChange*                                  )
{
  G4BOptnChangeCrossSection* operation = fChangeCrossSectionOperations[callingProcess];
  if ( operation ==  occurenceOperationApplied ) operation->SetInteractionOccured();
}

void GmBOptrChangeCrossSectionByDistribution::AddParticleProcess( G4String partName, G4String procName )
{

  if( theParticleProcessNames.size() != 0 &&
      theParticleProcessNames.find( partName ) == theParticleProcessNames.end() ) {
    G4Exception("GmBOptrChangeCrossSectionByDistribution::AddParticleProcess",
		"BOCCS001",
		FatalErrorInArgument,
		("Each instance of this operator can have only one particle type, it cannot be added " + partName).c_str());
  }
  GmVBiasingOperator::AddParticleProcess( partName, procName );
}
