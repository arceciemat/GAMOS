#include "DicomVObjectWithParams.hh"
#include "DicomVerbosity.hh"
#include "DicomMgr.hh"

#include "dcmtk/dcmrt/seq/drtcps.h"    // for ControlPointSequence
#include "globals.hh"

//-----------------------------------------------------------------------------
DicomVObjectWithParams::DicomVObjectWithParams()
{
}

//-----------------------------------------------------------------------------
Float64 DicomVObjectWithParams::CorrectByDirection( Float64 angle, OFString dir )
{
 
  if( dir == "CC" ) { // counter-clockwise
    return -angle;
  } else if( dir == "CW" || dir == "NONE" || dir == "") { // clockwise
    return angle;
  } else {
    G4Exception("DicomVObjectWithParams::CorrectByDirection",
		"",
		FatalException,
		("Not supported Direction " + dir + " Please contact GAMOS authors").c_str());
    return 0.;
  }
}

//-----------------------------------------------------------------------------
bool DicomVObjectWithParams::ParamExists( G4String str )
{
  std::map<G4String,Float64>::const_iterator ite = theParams.find(str);
  if( ite != theParams.end() ) {
    return TRUE;
  } else {
    return FALSE;
  }
}

//-----------------------------------------------------------------------------
void DicomVObjectWithParams::SetParam( G4String str, Float64 flt )
{
  theParams[str] = flt;
  if( DicomVerb(debugVerb) ) {
    G4cout << "DicomVObjectWithParams::SetParam " << str << " = " << flt << G4endl;
  }
}

//-----------------------------------------------------------------------------
void DicomVObjectWithParams::SetParamStr( G4String str,  OFString fst )
{
  theParamStrs[str] = fst;
  if( DicomVerb(debugVerb) ) {
    G4cout << "DicomVObjectWithParams::SetParamStr " << str << " = " << fst << G4endl;
  }
}

//-----------------------------------------------------------------------------
Float64 DicomVObjectWithParams::GetParam( G4String str, bool bMustExist )
{
  std::map<G4String,Float64>::const_iterator ite = theParams.find(str);
  if( ite == theParams.end() ) {
    if( bMustExist ) {
      G4Exception("DicomVObjectWithParams::GetParam",
		  "",
		  FatalException,
		  ("Parameter not found " + str + " Please contact GAMOS authors").c_str());
    }
  }

  return (*ite).second;
}

//-----------------------------------------------------------------------------
void DicomVObjectWithParams::Print( std::ostream& out, G4String name )
{
  std::map<G4String,Float64>::const_iterator ite;
  for( ite = theParams.begin(); ite != theParams.end(); ite++ ) {
    out << ":P " << name << (*ite).first << " " << (*ite).second << G4endl;
  }

  std::map<G4String,OFString>::const_iterator ites;
  for( ites = theParamStrs.begin(); ites != theParamStrs.end(); ites++ ) {
    if( (*ites).second != "" ) out << ":PS " << name << (*ites).first << " \"" << (*ites).second << "\"" << G4endl;
  }

}

//-----------------------------------------------------------------------------
void DicomVObjectWithParams::DicomObjectException(std::string className, std::string paramName)
{

  G4Exception("DicomObjectException parameter not found in file ",
	      "",
	      JustWarning,
	      (className + " does not have mandatory parameter " + paramName).c_str());
}
