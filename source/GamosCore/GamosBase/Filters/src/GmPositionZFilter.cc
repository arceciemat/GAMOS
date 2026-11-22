#include "GmPositionZFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//------------------------------------------------------------
GmPositionZFilter::GmPositionZFilter(G4String name)
  :GmVFilter(name)
{
}

//------------------------------------------------------------
GmPositionZFilter::~GmPositionZFilter()
{
}

//------------------------------------------------------------
G4bool GmPositionZFilter::AcceptStep(const G4Step* aStep)
{
  G4double val = aStep->GetPostStepPoint()->GetPosition().z();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
G4bool GmPositionZFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double val = aTrack->GetPosition().z();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
void GmPositionZFilter::show() 
{
    G4cout << " GmPositionZFilter:: " << GetName()
	 << " LowLimiE  " << G4BestUnit(theLowLimit,"Position") 
	 << " HighLimit " << G4BestUnit(theHighLimit,"Position")
	 <<G4endl;
}

//------------------------------------------------------------
void GmPositionZFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPositionZFilter::SetParameters",
		"There should be two  parameters: low_limit high_limit",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 theLowLimit  = GmGenUtils::GetValue( params[0] );
 theHighLimit = GmGenUtils::GetValue( params[1] );

}
