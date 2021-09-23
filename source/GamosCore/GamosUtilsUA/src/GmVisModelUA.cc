#include "GmVisModelUA.hh"
#include "GmVisModelByEnergy.hh"
#include "G4VisManager.hh"

//------------------------------------------------------------------
GmVisModelUA::GmVisModelUA()
{
}

//------------------------------------------------------------------
GmVisModelUA::~GmVisModelUA()
{
}

//------------------------------------------------------------------
void GmVisModelUA::BeginOfRunAction( const G4Run* )
{
// Create custom model
  GmVisModelByEnergy* myModel = new GmVisModelByEnergy("ModelByEnergy");
  // Configure it if necessary and then register with G4VisManager ...
  G4VisManager::GetInstance()->RegisterModel(myModel);

}
