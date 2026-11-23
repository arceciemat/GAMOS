#include "GmDirectionPhiFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//------------------------------------------------------------
GmDirectionPhiFilter::GmDirectionPhiFilter(G4String name)
  :GmVFilter(name)
{
}

//------------------------------------------------------------
GmDirectionPhiFilter::~GmDirectionPhiFilter()
{
}

//------------------------------------------------------------
G4bool GmDirectionPhiFilter::AcceptStep(const G4Step* aStep)
{
  G4double val = aStep->GetPostStepPoint()->GetMomentumDirection().phi();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
G4bool GmDirectionPhiFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double val = aTrack->GetMomentumDirection().phi();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
void GmDirectionPhiFilter::show() 
{
    G4cout << " GmDirectionPhiFilter:: " << GetName()
	 << " LowLimiE  " << G4BestUnit(theLowLimit,"Direction") 
	 << " HighLimit " << G4BestUnit(theHighLimit,"Direction")
	 <<G4endl;
}

//------------------------------------------------------------
void GmDirectionPhiFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmDirectionPhiFilter::SetParameters",
		"There should be two  parameters: low_limit high_limit",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 theLowLimit  = GmGenUtils::GetValue( params[0] );
 theHighLimit = GmGenUtils::GetValue( params[1] );

}

