#include "GmVDataInt.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmVDataInt::GmVDataInt()
{
  theNBytes = 4; // sizeof(int)
  theCType = "int";
}

//----------------------------------------------------------------
GmVDataInt::~GmVDataInt()
{
}

//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Step* aStep )
{
  int data = int(GetValueFromStep( aStep ));
  theFileOutBin->write((char*)&data, sizeof(int));

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Track* aTrack )
{
  int data = int(GetValueFromTrack( aTrack ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Track " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 )
{
  int data = int(GetValueFromSecoTrack( aTrack1, aTrack2 ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin SecondaryTrack " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Event* aEvent )
{
  int data = int(GetValueFromEvent( aEvent ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Event " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteBin( const G4Run* aRun )
{
  int data = int(GetValueFromRun( aRun ));
  theFileOutBin->write((char*)&data, sizeof(int));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteBin Run " << theName << " = " << data << G4endl;
#endif

}
