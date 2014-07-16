#include "GmTrackInfo.hh"

// --------------------------------------------------------------
GmTrackInfo::  GmTrackInfo( const G4String& type ) 
  : G4VUserTrackInformation(type)
{
}

// --------------------------------------------------------------
GmTrackInfo::~GmTrackInfo() 
{
}

// --------------------------------------------------------------
G4bool GmTrackInfo::BoolValueExists( const G4String& key )
{
  if( theBoolValues.find(key) == theBoolValues.end() ) {
    return false;
  } else {
    return true;
  }
}

// --------------------------------------------------------------
G4bool GmTrackInfo::IntValueExists( const G4String& key )
{
  if( theIntValues.find(key) == theIntValues.end() ) {
    return false;
  } else {
    return true;
  }
}

// --------------------------------------------------------------
G4bool GmTrackInfo::DoubleValueExists( const G4String& key )
{
  if( theDoubleValues.find(key) == theDoubleValues.end() ) {
    return false;
  } else {
    return true;
  }
}

// --------------------------------------------------------------
G4bool GmTrackInfo::StringValueExists( const G4String& key )
{
  if( theStringValues.find(key) == theStringValues.end() ) {
    return false;
  } else {
    return true;
  }
}
// --------------------------------------------------------------
G4bool GmTrackInfo::ThreeVectorValueExists( const G4String& key )
{
  if( theThreeVectorValues.find(key) == theThreeVectorValues.end() ) {
    return false;
  } else {
    return true;
  }
}

//--------------------------------------------------------------
G4bool GmTrackInfo::GetBoolValue( const G4String& key )
{
  std::map<G4String,G4bool>::const_iterator ite = theBoolValues.find( key );
  if( ite == theBoolValues.end() ) {
    G4Exception("GmTrackInfo::GetBoolValue",
		"String not found",
		FatalErrorInArgument,
		G4String("Looking for: " + key).c_str());
  }
  return (*ite).second;
  
}

//--------------------------------------------------------------
G4int GmTrackInfo::GetIntValue( const G4String& key )
{
  //  G4cout << this << " GmTrackInfo::GetInterValue" << key << " " << theIntValues.size() << G4endl;
  std::map<G4String,G4int>::const_iterator ite = theIntValues.find( key );
  if( ite == theIntValues.end() ) {
    return -INT_MAX;
    /*G4Exception("GmTrackInfo::GetInterValue",
		"String not found",
		FatalErrorInArgument,
		G4String("Looking for: " + key).c_str());*/
  }
  return (*ite).second;
  
}

//--------------------------------------------------------------
G4double GmTrackInfo::GetDoubleValue( const G4String& key )
{
  std::map<G4String,G4double>::const_iterator ite = theDoubleValues.find( key );
  if( ite == theDoubleValues.end() ) {
    G4Exception("GmTrackInfo::GetDoubleValue",
		"String not found",
		FatalErrorInArgument,
		G4String("Looking for: " + key).c_str());
  }
  return (*ite).second;
  
}

//--------------------------------------------------------------
G4String GmTrackInfo::GetStringValue( const G4String& key )
{
  std::map<G4String,G4String>::const_iterator ite = theStringValues.find( key );
  if( ite == theStringValues.end() ) {
    G4Exception("GmTrackInfo::GetStringValue",
		"String not found",
		FatalErrorInArgument,
		G4String("Looking for: " + key).c_str());
  }
  return (*ite).second;
  
}

//--------------------------------------------------------------
G4ThreeVector GmTrackInfo::GetThreeVectorValue( const G4String& key )
{
  std::map<G4String,G4ThreeVector>::const_iterator ite = theThreeVectorValues.find( key );
  if( ite == theThreeVectorValues.end() ) {
    G4Exception("GmTrackInfo::GetThreeVectorValue",
		"ThreeVector not found",
		FatalErrorInArgument,
		G4String("Looking for: " + key).c_str());
  }
  return (*ite).second;
  
}


//--------------------------------------------------------------
void GmTrackInfo::SetBoolValue(  const G4String& key, G4bool value )
{
  theBoolValues[key] = value;
}


//--------------------------------------------------------------
void GmTrackInfo::SetIntValue(  const G4String& key, G4int value )
{
  //  G4cout << this << "GmTrackInfo::SetIntValue " << key << " = " << value << G4endl;
  theIntValues[key] = value;
}


//--------------------------------------------------------------
void GmTrackInfo::SetDoubleValue(  const G4String& key, G4double value )
{
  theDoubleValues[key] = value;
}


//--------------------------------------------------------------
void GmTrackInfo::SetStringValue(  const G4String& key, G4String value )
{
  theStringValues[key] = value;
}


//--------------------------------------------------------------
void GmTrackInfo::SetThreeVectorValue( const G4String& key, G4ThreeVector value )
{
  theThreeVectorValues[key] = value;
}
