#include "GmEnergyChangeFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

GmEnergyChangeFilter::GmEnergyChangeFilter(G4String name)
  :GmVFilter(name)
{
  theTrackElost = 0.;
  theLastTrackID = 0;
  theLastEventID = 0;
}

GmEnergyChangeFilter::~GmEnergyChangeFilter()
{;}

G4bool GmEnergyChangeFilter::AcceptStep(const G4Step* aStep)
{
  G4double elost = aStep->GetPostStepPoint()->GetKineticEnergy() - aStep->GetPreStepPoint()->GetKineticEnergy();
  G4int trackID = aStep->GetTrack()->GetTrackID();
  G4int eventID = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID();
  if( trackID != theLastTrackID || eventID != theLastEventID ) theTrackElost = 0.;
  theTrackElost += elost;
  theLastTrackID = trackID; 
  theLastEventID = eventID;
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "  GmEnergyChangeFilter::AcceptStep " << elost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( elost < fLowEnergy  ) return FALSE;
  if ( elost > fHighEnergy ) return FALSE;

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "  GmEnergyChangeFilter::AcceptStep accepted " << elost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  return TRUE;
}

G4bool GmEnergyChangeFilter::AcceptTrack(const G4Track* aTrack)
{
  if( aTrack->GetTrackStatus() != fStopAndKill ) { // start of track does not have energy lost
    return FALSE;
  }
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) )  G4cout << "  GmEnergyChangeFilter::AcceptTrack " << theTrackElost << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
#endif
  if ( theTrackElost < fLowEnergy  ) return FALSE;
  if ( theTrackElost > fHighEnergy ) return FALSE;
  return TRUE;
}

void GmEnergyChangeFilter::show() 
{
    G4cout << " GmEnergyChangeFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowEnergy,"Energy") 
	 << " HighE " << G4BestUnit(fHighEnergy,"Energy")
	 <<G4endl;
}


void GmEnergyChangeFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmEnergyChangeFilter::SetParameters","There should be two  parameters: min_energy max_energy",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 fLowEnergy  = GmGenUtils::GetValue( params[0] );
 fHighEnergy = GmGenUtils::GetValue( params[1] );

}
