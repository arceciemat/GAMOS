#include "GmGenerDistDirectionConst.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//---------------------------------------------------------------------
GmGenerDistDirectionConst::GmGenerDistDirectionConst()
{
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionConst::GenerateDirection( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionConst::Generate  dir " << theDir << G4endl;
#endif
  return theDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionConst::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 3 ) {
    G4Exception(" GmGenerDistDirectionConst::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 3 parameters: DIR_X DIR_Y DIR_Z");
  }

  theDir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

}
