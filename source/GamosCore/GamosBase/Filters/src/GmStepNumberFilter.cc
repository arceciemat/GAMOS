#include "GmStepNumberFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

GmStepNumberFilter::GmStepNumberFilter(G4String name)
  :GmVFilter(name)
{
}

GmStepNumberFilter::~GmStepNumberFilter()
{ 
}

G4bool GmStepNumberFilter::AcceptStep(const G4Step* aStep)
{
  G4int nstep = aStep->GetTrack()->GetCurrentStepNumber();
  if( nstep >= theNStepMin && nstep <= theNStepMax ) {
    return TRUE;
  }
  return FALSE;
}

void GmStepNumberFilter::show()
{
  G4cout << "GmStepNumberFilter: step number MIN = " << theNStepMin << " MAX = " << theNStepMax << G4endl;
  G4cout << "-------------------------------------------"<<G4endl;
}

void GmStepNumberFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmStepNumberFilter::SetParameters","There should be two  parameters: min_step_number max_step_number",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }
  
  theNStepMin = G4int(GmGenUtils::GetValue( params[0] ));
  theNStepMax = G4int(GmGenUtils::GetValue( params[1] ));
  
}
