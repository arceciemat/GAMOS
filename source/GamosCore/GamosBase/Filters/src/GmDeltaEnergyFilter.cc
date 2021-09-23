#include "GmDeltaEnergyFilter.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

GmDeltaEnergyFilter::GmDeltaEnergyFilter(G4String name)
  :GmVFilter(name)
{
  G4Exception("GmDeltaEnergyFilter::GmDeltaEnergyFilter",
	      "",
	      JustWarning,
	      "GmDeltaEnergyFilter is deprecated, please use GmEnergyChangeFilter instead");
  theTrackElost = 0.;
  theLastTrackID = 0;
  theLastEventID = 0;
}

GmDeltaEnergyFilter::~GmDeltaEnergyFilter()
{;}

G4bool GmDeltaEnergyFilter::AcceptStep(const G4Step* aStep)
{
  G4double elost = aStep->GetPostStepPoint()->GetKineticEnergy() - aStep->GetPreStepPoint()->GetKineticEnergy();
  G4int trackID = aStep->GetTrack()->GetTrackID();
  G4int eventID = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID();
  if( trackID != theLastTrackID || eventID != theLastEventID ) theTrackElost = 0.;
  theTrackElost += elost;
  theLastTrackID = trackID; 
  theLastEventID = eventID;
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "  GmDeltaEnergyFilter::AcceptStep " << elost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( elost < fLowEnergy  ) return FALSE;
  if ( elost > fHighEnergy ) return FALSE;

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "  GmDeltaEnergyFilter::AcceptStep accepted " << elost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  return TRUE;
}

G4bool GmDeltaEnergyFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetTrackStatus() != fStopAndKill ) { // start of track does not have energy lost
    return FALSE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) )  G4cout << "  GmDeltaEnergyFilter::AcceptTrack " << theTrackElost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( theTrackElost < fLowEnergy  ) return FALSE;
  if ( theTrackElost > fHighEnergy ) return FALSE;
  return TRUE;
}

G4bool GmDeltaEnergyFilter::AcceptStackedTrack(const G4Track* )
{
  G4Exception(" GmDeltaEnergyFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}


void GmDeltaEnergyFilter::show() 
{
    G4cout << " GmDeltaEnergyFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowEnergy,"Energy") 
	 << " HighE " << G4BestUnit(fHighEnergy,"Energy")
	 <<G4endl;
}


void GmDeltaEnergyFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmDeltaEnergyFilter::SetParameters","There should be two  parameters: min_energy max_energy",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 fLowEnergy  = GmGenUtils::GetValue( params[0] );
 fHighEnergy = GmGenUtils::GetValue( params[1] );

}
