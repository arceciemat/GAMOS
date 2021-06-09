#include "GamosCore/GamosSD/include/GmSDType.hh"

SDType GmSDType::GetType( const G4String& type )
{
  int isiz = type.length();
  if( isiz == 0 ) {
    G4Exception("GmSDType::GetType",
		"Wrong argument",
		FatalErrorInArgument,
		"Type is blank ");
  }

  //  if( type.c_str()[isiz-1] == '/' ) type.substr( 0, isiz-1 );
  //  if( type.c_str()[0] == '/' ) type.substr( 1, isiz );

  if( type.substr(isiz-1,1) == "/" ) type.substr( 0, isiz-1 );
  if( type.substr(0,1) == "/" ) type.substr( 1, isiz );

  if( type == "/Tracker/" ) {
    return SDTracker;
  } else if( type == "/Calor/" ) {
    return SDCalor;
  } else {
    G4Exception("GmSDType::GetType",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Type is not 'Tracker' or 'Calor', but " + type).c_str());
  }

  return SDTracker;
}


G4String GmSDType::Print( SDType sdtyp )
{
  if( sdtyp == SDTracker ){
    return "Tracker";
  } else if( sdtyp == SDCalor ){
    return "Calor";
  }

  return "";
}
