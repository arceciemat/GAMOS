#include "GmPostKineticEnergyFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

GmPostKineticEnergyFilter::GmPostKineticEnergyFilter(G4String name)
  :GmVFilter(name)
{
}

GmPostKineticEnergyFilter::~GmPostKineticEnergyFilter()
{;}

G4bool GmPostKineticEnergyFilter::AcceptStep(const G4Step* aStep)
{
  G4double kinetic = aStep->GetPostStepPoint()->GetKineticEnergy();
  //  G4cout << "  GmPostKineticEnergyFilter::AcceptTrack " << kinetic << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
  if ( kinetic < fLowEnergy  ) return FALSE;
  if ( kinetic > fHighEnergy ) return FALSE;
  return TRUE;
}

G4bool GmPostKineticEnergyFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double kinetic = aTrack->GetKineticEnergy();
  //  G4cout << "  GmPostKineticEnergyFilter::AcceptTrack " << kinetic << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
  if ( kinetic < fLowEnergy  ) return FALSE;
  if ( kinetic >= fHighEnergy ) return FALSE;
  return TRUE;
}

void GmPostKineticEnergyFilter::show() 
{
    G4cout << " GmPostKineticEnergyFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowEnergy,"Energy") 
	 << " HighE " << G4BestUnit(fHighEnergy,"Energy")
	 <<G4endl;
}


void GmPostKineticEnergyFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPostKineticEnergyFilter::SetParameters","There should be two  parameters: min_energy max_energy",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 fLowEnergy  = GmGenUtils::GetValue( params[0] );
 fHighEnergy = GmGenUtils::GetValue( params[1] );

}
