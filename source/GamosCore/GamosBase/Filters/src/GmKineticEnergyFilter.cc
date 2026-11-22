#include "GmKineticEnergyFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

GmKineticEnergyFilter::GmKineticEnergyFilter(G4String name)
  :GmVFilter(name)
{
}

GmKineticEnergyFilter::~GmKineticEnergyFilter()
{
}

G4bool GmKineticEnergyFilter::AcceptStep(const G4Step* aStep)
{
  G4double kinetic = aStep->GetPreStepPoint()->GetKineticEnergy();
  //  G4cout << "  GmKineticEnergyFilter::AcceptTrack " << kinetic << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
  if ( kinetic < fLowEnergy  ) return FALSE;
  if ( kinetic > fHighEnergy ) return FALSE;
  return TRUE;
}

G4bool GmKineticEnergyFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double kinetic = aTrack->GetKineticEnergy();
  //  G4cout << "  GmKineticEnergyFilter::AcceptTrack " << kinetic << " lowE " << fLowEnergy << " high " << fHighEnergy << G4endl;
  if ( kinetic < fLowEnergy  ) return FALSE;
  if ( kinetic > fHighEnergy ) return FALSE;
  return TRUE;
}

void GmKineticEnergyFilter::show() 
{
    G4cout << " GmKineticEnergyFilter:: " << GetName()
	 << " LowE  " << G4BestUnit(fLowEnergy,"Energy") 
	 << " HighE " << G4BestUnit(fHighEnergy,"Energy")
	 <<G4endl;
}

void GmKineticEnergyFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmKineticEnergyFilter::SetParameters","There should be two  parameters: min_energy max_energy",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
 }

 fLowEnergy  = GmGenUtils::GetValue( params[0] );
 fHighEnergy = GmGenUtils::GetValue( params[1] );

}
