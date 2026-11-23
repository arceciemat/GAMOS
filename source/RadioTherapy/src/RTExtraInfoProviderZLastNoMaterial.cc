#include "RTExtraInfoProviderZLastNoMaterial.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "RTVerbosity.hh"
#include "G4Region.hh"
#include "G4Track.hh"


//---------------------------------------------------------------------
RTExtraInfoProviderZLastNoMaterial::RTExtraInfoProviderZLastNoMaterial()
{
  std::vector<G4String> materials;
  materials.push_back("G4_AIR");

  materials = GmParameterMgr::GetInstance()->GetVStringValue("RTExtraInfoProviderZLastNoMaterial:Materials",materials);

  for( size_t ii = 0; ii < materials.size(); ii++ ){
    theExcludedMaterials.insert(materials[ii]);
  }

  theWord = 0;
}


//---------------------------------------------------------------------
void RTExtraInfoProviderZLastNoMaterial::UserSteppingAction( const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();

  if( theWord ) theWordPrev = *theWord;
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType != fNotDefined
      && procType !=  fTransportation ) { 
    G4Material* mate = aStep->GetTrack()->GetMaterial();
    if( theExcludedMaterials.find(mate->GetName()) == theExcludedMaterials.end() ){
      *theWord = aTrack->GetPosition().z();
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTExtraInfoProviderZLastNoMaterial ( aTrack ) " << *theWord << G4endl;
#endif
  
}


//---------------------------------------------------------------------
void RTExtraInfoProviderZLastNoMaterial::CheckZStop( G4double zstop )
{
  if( *theWord > zstop ) *theWord = theWordPrev;
}
