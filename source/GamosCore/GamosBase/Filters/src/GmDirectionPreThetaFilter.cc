#include "GmDirectionPreThetaFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//------------------------------------------------------------
GmDirectionPreThetaFilter::GmDirectionPreThetaFilter(G4String name)
  :GmVFilter(name)
{
}

//------------------------------------------------------------
GmDirectionPreThetaFilter::~GmDirectionPreThetaFilter()
{
}

//------------------------------------------------------------
G4bool GmDirectionPreThetaFilter::AcceptStep(const G4Step* aStep)
{
  G4double val = aStep->GetPreStepPoint()->GetMomentumDirection().theta();
  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
G4bool GmDirectionPreThetaFilter::AcceptTrack(const G4Track* aTrack)
{
  G4double val;
  // if begin of track use track variables, if end of track use pre step point variables
  if( aTrack->GetTrackStatus() != fStopAndKill ) {
    val = aTrack->GetVertexMomentumDirection().theta();
  } else {
    val = aTrack->GetStep()->GetPreStepPoint()->GetMomentumDirection().theta();
  }

  if ( val < theLowLimit  ) return FALSE;
  if ( val > theHighLimit ) return FALSE;
  return TRUE;
}

//------------------------------------------------------------
void GmDirectionPreThetaFilter::show() 
{
    G4cout << " GmDirectionPreThetaFilter:: " << GetName()
	 << " LowLimiE  " << G4BestUnit(theLowLimit,"Direction") 
	 << " HighLimit " << G4BestUnit(theHighLimit,"Direction")
	 <<G4endl;
}

//------------------------------------------------------------
void GmDirectionPreThetaFilter::SetParameters( std::vector<G4String>& params)
{
 if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmDirectionPreThetaFilter::SetParameters",
		"There should be two  parameters: low_limit high_limit",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
 }

 theLowLimit  = GmGenUtils::GetValue( params[0] );
 theHighLimit = GmGenUtils::GetValue( params[1] );

}


//-------------------------------------------------------------------------
G4bool GmDirectionPreThetaFilter::AcceptStackedTrack(const G4Track* )
{
  G4Exception(" GmDirectionPreThetaFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}
