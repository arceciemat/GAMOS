#include "GmSplitWrapperProcess.hh"

GmSplitWrapperProcess::GmSplitWrapperProcess(const G4String& aName,
                                         G4ProcessType   aType)
  : G4WrapperProcess(aName,aType), theNSplit(0)
{
  pRegProcess = (G4VProcess*)(0);
}

GmSplitWrapperProcess::~GmSplitWrapperProcess()
{
  if (pRegProcess!=0) delete pRegProcess;
}

G4VParticleChange* GmSplitWrapperProcess::PostStepDoIt( const G4Track& track,
                                                   const G4Step&  stepData )
{
  std::vector<G4Track*> theSecos;
  G4VParticleChange* pchan;
  for( G4int ii = 0; ii < theNSplit; ii++ ) {
    pchan = pRegProcess->PostStepDoIt( track, stepData );        
    if( ii != theNSplit -1 ) { // the last one has already the list of secondaries, do not add it
      G4int nseco = pchan->GetNumberOfSecondaries(); 
      for( G4int in = 0; in < nseco; in++ ) {
	theSecos.push_back( pchan->GetSecondary(in) );
      }
    }
  }

  G4int nseco = theSecos.size(); 
  pchan->SetNumberOfSecondaries( nseco + pchan->GetNumberOfSecondaries() );
  for( G4int in = 0; in < nseco; in++ ) {
    pchan->AddSecondary( theSecos[in] );
  }

  return pchan;

}
