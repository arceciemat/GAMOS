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
void GmVDataInt::WriteText( const G4Step* aStep, G4bool bIsFirst )
{
  long unsigned int data = (long unsigned int) (GetValueFromStep( aStep, 0 ));
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Track* aTrack, G4bool bIsFirst )
{
  long unsigned int data = (long unsigned int) (GetValueFromTrack( aTrack, 0 ));
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Track " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst )
{
  G4double data = GetValueFromSecoTrack( aTrack1, aTrack2, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText SecoTrack " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Event* anEvent, G4bool bIsFirst )
{
  G4double data = GetValueFromEvent( anEvent, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Event " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteText( const G4Run* aRun, G4bool bIsFirst )
{
  G4double data = GetValueFromRun( aRun, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteText Run " << theName << " = " << data << G4endl;
#endif

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

//----------------------------------------------------------------
void GmVDataInt::WriteCout( const G4Step* aStep, G4bool bIsFirst )
{
  long unsigned int data = (long unsigned int) (GetValueFromStep( aStep, 0 ));
  if( !bIsFirst ) G4cout << ",";
  G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteCout Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteCout( const G4Track* aTrack, G4bool bIsFirst )
{
  long unsigned int data = (long unsigned int) (GetValueFromTrack( aTrack, 0 ));
  if( !bIsFirst ) G4cout << ",";
  G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteCout Track " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVDataInt::WriteCout( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst )
{
  G4double data = GetValueFromSecoTrack( aTrack1, aTrack2, 0 );
  if( !bIsFirst ) G4cout << ",";
  G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteCout SecoTrack " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteCout( const G4Event* anEvent, G4bool bIsFirst )
{
  G4double data = GetValueFromEvent( anEvent, 0 );
  if( !bIsFirst ) G4cout << ",";
  G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteCout Event " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVDataInt::WriteCout( const G4Run* aRun, G4bool bIsFirst )
{
  G4double data = GetValueFromRun( aRun, 0 );
  if( !bIsFirst ) G4cout << ",";
  G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataInt::WriteCout Run " << theName << " = " << data << G4endl;
#endif

}
