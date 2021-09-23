#include "GmVDataAccumulated.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmVDataAccumulated::GmVDataAccumulated()
{
  bAccumulating = TRUE;

}

//----------------------------------------------------------------
void GmVDataAccumulated::Initialise()
{
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmVDataAccumulated::Initialise data " << theName << G4endl; 
#endif
  theDataAccumulated.clear();
}

//----------------------------------------------------------------
GmVDataAccumulated::~GmVDataAccumulated()
{

}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromTrack( const G4Track*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromTrack " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromSecoTrack( const G4Track* , const G4Track* , G4int index)
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromSecoTrack " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromEvent( const G4Event*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromEvent " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}

//----------------------------------------------------------------
G4double GmVDataAccumulated::GetValueFromRun( const G4Run*, G4int index )
{
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromRun " << index << " = " << theDataAccumulated[index] << G4endl;
#endif 
  return theDataAccumulated[index];
}
