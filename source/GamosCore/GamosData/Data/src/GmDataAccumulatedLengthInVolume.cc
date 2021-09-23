#include "GmDataAccumulatedLengthInVolume.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedLengthInVolume::GmDataAccumulatedLengthInVolume()
{
  bInitial = false;
  theHMax = 10.;

  theExcludedTypes.clear(); // delete DTRun
  theVolumeNames = GmParameterMgr::GetInstance()->GetVStringValue("GmDataAccumulatedLengthInVolume:Volumes",theVolumeNames);
  if( theVolumeNames.size() == 0 ) {
    G4Exception("GmDataAccumulatedLengthInVolume::GmDataAccumulatedLengthInVolume",
		"",
		FatalException,
		"No volume name given, please use '/gamos/setParam GmDataAccumulatedLengthInVolume:Volumes VOL_NAME_1 VOL_NAME_2 ...'");
  }
}

//----------------------------------------------------------------
GmDataAccumulatedLengthInVolume::~GmDataAccumulatedLengthInVolume()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedLengthInVolume::Accumulate( const G4Step* aStep, G4int index )
{
  G4String volName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  for( size_t ii = 0; ii < theVolumeNames.size(); ii++ ) {
    if( GmGenUtils::AreWordsEquivalent(volName,theVolumeNames[ii]) ) {
      theDataAccumulated[index] += aStep->GetStepLength();
    }
    //    G4cout << " GmDataAccumulatedLengthInVolume::Accumulate " << volName <<" : " <<  theDataAccumulated[index] << G4endl; //GDEB
  }
}

//----------------------------------------------------------------
G4double GmDataAccumulatedLengthInVolume::GetValueFromStep( const G4Step* aStep, G4int )
{
  Accumulate( aStep );
  return GetValueFromTrack( aStep->GetTrack() );
}

//----------------------------------------------------------------
G4double GmDataAccumulatedLengthInVolume::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* , G4int )
{ 
  return GetValueFromTrack( aTrack1 );
}
