#include "GmDepositedEnergyFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

GmDepositedEnergyFilter::GmDepositedEnergyFilter(G4String name)
  :GmVFilter(name)
{
  theTrackEdepo = 0.;
  theLastTrackID = 0;
  theLastEventID = 0;
}

GmDepositedEnergyFilter::~GmDepositedEnergyFilter()
{;}

G4bool GmDepositedEnergyFilter::AcceptStep(const G4Step* aStep)
{
  G4double edepo = aStep->GetTotalEnergyDeposit();
  G4int trackID = aStep->GetTrack()->GetTrackID();
  G4int eventID = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID();
  if( trackID != theLastTrackID || eventID != theLastEventID ) theTrackEdepo = 0.;
  theTrackEdepo += edepo;
  theLastTrackID = trackID; 
  theLastEventID = eventID;
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "  GmDepositedEnergyFilter::AcceptStep " << edepo << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( edepo < fLowEnergy  ) return FALSE;
  if ( edepo > fHighEnergy ) return FALSE;
  return TRUE;
}

G4bool GmDepositedEnergyFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetTrackStatus() != fStopAndKill ) { // start of track does not have energy lost
    return FALSE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) )  G4cout << "  GmDepositedEnergyFilter::AcceptTrack " << theTrackEdepo << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( theTrackEdepo < fLowEnergy  ) return FALSE;
  if ( theTrackEdepo > fHighEnergy ) return FALSE;
  return TRUE;
}

void GmDepositedEnergyFilter::show() 
{
    G4cout << " GmDepositedEnergyFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowEnergy,"Energy") 
	 << " HighE " << G4BestUnit(fHighEnergy,"Energy")
	 <<G4endl;
}


void GmDepositedEnergyFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmDepositedEnergyFilter::SetParameters","There should be two  parameters: min_energy max_energy",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 fLowEnergy  = GmGenUtils::GetValue( params[0] );
 fHighEnergy = GmGenUtils::GetValue( params[1] );

}
