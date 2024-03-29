#include "GmDirectionPrePhiFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//------------------------------------------------------------
GmDirectionPrePhiFilter::GmDirectionPrePhiFilter(G4String name)
  :GmVFilter(name)
{
}

//------------------------------------------------------------
GmDirectionPrePhiFilter::~GmDirectionPrePhiFilter()
{
}

//------------------------------------------------------------
G4bool GmDirectionPrePhiFilter::AcceptStep(const G4Step* aStep)
{
  G4double val = aStep->GetPreStepPoint()->GetMomentumDirection().phi();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
G4bool GmDirectionPrePhiFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double val;
  // if begin of track use track variables, if end of track use pre step point variables
  if( aTrack->GetTrackStatus() != fStopAndKill ) {
    val = aTrack->GetVertexMomentumDirection().phi();
  } else {
    val = aTrack->GetStep()->GetPreStepPoint()->GetMomentumDirection().phi();
  }

  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
void GmDirectionPrePhiFilter::show() 
{
    G4cout << " GmDirectionPrePhiFilter:: " << GetName()
	 << " LowLimiE  " << G4BestUnit(theLowLimit,"Direction") 
	 << " HighLimit " << G4BestUnit(theHighLimit,"Direction")
	 <<G4endl;
}

//------------------------------------------------------------
void GmDirectionPrePhiFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmDirectionPrePhiFilter::SetParameters",
		"There should be two  parameters: low_limit high_limit",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 theLowLimit  = GmGenUtils::GetValue( params[0] );
 theHighLimit = GmGenUtils::GetValue( params[1] );

}


//-------------------------------------------------------------------------
G4bool GmDirectionPrePhiFilter::AcceptStackedTrack(const G4Track* )
{
  G4Exception(" GmDirectionPrePhiFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}
