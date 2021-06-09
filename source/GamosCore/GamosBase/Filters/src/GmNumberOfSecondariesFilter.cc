#include "GmNumberOfSecondariesFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

GmNumberOfSecondariesFilter::GmNumberOfSecondariesFilter(G4String name)
  :GmVFilter(name)
{
}

GmNumberOfSecondariesFilter::~GmNumberOfSecondariesFilter()
{ 
}

G4bool GmNumberOfSecondariesFilter::AcceptStep(const G4Step* )
{
  G4int nseco = GmG4Utils::GetSecondariesOfCurrentStep().size();
  if( nseco >= theNSecoMin && nseco <= theNSecoMax ) {
    return TRUE;
  }
  return FALSE;
}

void GmNumberOfSecondariesFilter::show()
{
  G4cout << "GmNumberOfSecondariesFilter: seco number MIN = " << theNSecoMin << " MAX = " << theNSecoMax << G4endl;
  G4cout << "-------------------------------------------"<<G4endl;
}

void GmNumberOfSecondariesFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 2 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmNumberOfSecondariesFilter::SetParameters","There should be two  parameters: min_seco_number max_seco_number",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }
  
  theNSecoMin = G4int(GmGenUtils::GetValue( params[0] ));
  theNSecoMax = G4int(GmGenUtils::GetValue( params[1] ));
  
}
