#include "GmGenerDistPositionLineSteps.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4UnitsTable.hh"


//---------------------------------------------------------------------
GmGenerDistPositionLineSteps::GmGenerDistPositionLineSteps()
{
  nSteps = 0;
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionLineSteps::GeneratePosition( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionLineSteps::Generate pos " << thePoint + theDirection * theStep * nSteps << G4endl;
#endif
  return thePoint + theDirection * theStep * nSteps++;

}


//---------------------------------------------------------------------
void GmGenerDistPositionLineSteps::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 7 ) {
    G4Exception(" GmGenerDistPositionLineSteps::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 4 parameters: POS_X POS_Y POS_Z DIR_X DIR_Y DIR_Z STEP");
  }

  thePoint = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );
  theDirection = G4ThreeVector(GmGenUtils::GetValue( params[3] ), GmGenUtils::GetValue( params[4] ), GmGenUtils::GetValue( params[5] ) );
  theStep = GmGenUtils::GetValue( params[6] );
}
