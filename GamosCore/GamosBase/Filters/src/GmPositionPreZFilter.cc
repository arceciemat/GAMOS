#include "GmPositionPreZFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//------------------------------------------------------------
GmPositionPreZFilter::GmPositionPreZFilter(G4String name)
  :GmVFilter(name)
{
}

//------------------------------------------------------------
GmPositionPreZFilter::~GmPositionPreZFilter()
{
}

//------------------------------------------------------------
G4bool GmPositionPreZFilter::AcceptStep(const G4Step* aStep)
{
  G4double val = aStep->GetPreStepPoint()->GetPosition().z();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
G4bool GmPositionPreZFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double val;
  // if begin of track use track variables, if end of track use pre step point variables
  if( aTrack->GetTrackStatus() != fStopAndKill ) {
    val = aTrack->GetPosition().z();
  } else {
    val = aTrack->GetStep()->GetPreStepPoint()->GetPosition().z();
  }

  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
void GmPositionPreZFilter::show() 
{
    G4cout << " GmPositionPreZFilter:: " << GetName()
	 << " LowLimiE  " << G4BestUnit(theLowLimit,"Position") 
	 << " HighLimit " << G4BestUnit(theHighLimit,"Position")
	 <<G4endl;
}

//------------------------------------------------------------
void GmPositionPreZFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPositionPreZFilter::SetParameters",
		"There should be two  parameters: low_limit high_limit",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 theLowLimit  = GmGenUtils::GetValue( params[0] );
 theHighLimit = GmGenUtils::GetValue( params[1] );

}
