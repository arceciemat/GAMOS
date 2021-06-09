#include "GmGenerDistPositionPoint.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4UnitsTable.hh"


//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionPoint::GeneratePosition( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionPoint::Generate pos " << thePoint << G4endl;
#endif
  return thePoint;

}


//---------------------------------------------------------------------
void GmGenerDistPositionPoint::SetParams( const std::vector<G4String>& params )
{

  if( params.size() == 0 ) {
    thePoint = G4ThreeVector();
  } else if( params.size() == 3 ) {
    thePoint = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );
  } else {
    G4Exception(" GmGenerDistPositionPoin::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 3 parameters: POS_X POS_Y POS_Z");
  }

}
