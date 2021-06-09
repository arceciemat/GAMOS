#include "GmVData.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

G4bool GmVData::bComposeHLimits = true;

//----------------------------------------------------------------
GmVData::GmVData()
{
  theHNBins = 100;
  theHMin = 0.;
  theHMax = 100.;

  //  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTRun);

  bAccumulating = false;
  bInitial = true;  

  theNBytes = 4; // float
  theCType = "float";
  //  G4cout << " SIZEOF FLOAT " << sizeof(float) << " D " <<  sizeof(double)<< " C " << sizeof(char) << " I " <<  sizeof(int) << " LI " << sizeof(long int) << G4endl;

  GmVData::bComposeHLimits = G4bool( GmParameterMgr::GetInstance()->GetNumericValue("GmVData:ComposeHistoLimits",1));

}

//----------------------------------------------------------------
GmVData::~GmVData()
{
}


//----------------------------------------------------------------
G4bool GmVData::CheckAvailability( GmDType dtyp )
{
  //  G4cout << " CheckAvail " << *(theExcludedTypes.begin()) << " look for " << dtyp << G4endl;
  if( theExcludedTypes.find( dtyp ) != theExcludedTypes.end() ) 
    {
      return FALSE;
    } 
  else
    {
      return TRUE;
    }

}

//----------------------------------------------------------------
void GmVData::BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr )
{

  G4String hName = hNamePrefix+":"+theName;

  anaMgr->CreateHisto1D(hName, theHNBins, theHMin, theHMax, hNumber);
  theHistos1[index] = anaMgr->GetHisto1D(hNumber);

}


//----------------------------------------------------------------
void GmVData::FillHisto( const G4Step* aStep, G4int index )
{
  G4double data = GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::FillHisto Step data " << theName << " index " << index << " = " << data << " W: " << aStep->GetTrack()->GetWeight() << G4endl; 
#endif
  theHistos1[index]->Fill( data, aStep->GetTrack()->GetWeight());

}

//----------------------------------------------------------------
void GmVData::FillHisto( const G4Track* aTrack, G4int index )
{
  G4double weight = 1.;
  if( aTrack ) weight = aTrack->GetWeight(); // Event or Run DataUser do not pass G4Track pointer

  G4double data = GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::FillHisto Track data " << theName  << " index " << index << " = " << data << " weight " << weight << G4endl; 
#endif
  theHistos1[index]->Fill( data, weight);

}

//----------------------------------------------------------------
void GmVData::FillHisto( const G4Track* aTrack1, const G4Track* aTrack2, G4int index )
{ 
  G4double data = GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::FillHisto SecoTrack data " << theName  << " -> " << theHistos1[index]->GetName() << " index " << index << " = " << data << G4endl; 
#endif
  theHistos1[index]->Fill( data, aTrack2->GetWeight());

}

//----------------------------------------------------------------
void GmVData::FillHisto( const G4Event* anEvent, G4int index )
{
  G4double data = GetValueFromEvent( anEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::FillHisto Event data " << theName  << " index " << index << " = " << data << G4endl; 
#endif
  theHistos1[index]->Fill( data, 1. );

}


//----------------------------------------------------------------
void GmVData::FillHisto( const G4Run* aRun, G4int index )
{
  G4double data = GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::FillHisto Run data " << theName  << " index " << index << " = " << data << G4endl; 
#endif
  theHistos1[index]->Fill( data, 1. );

}


//----------------------------------------------------------------
void GmVData::WriteText( const G4Step* aStep, G4bool bIsFirst )
{
  G4double data = GetValueFromStep( aStep, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteText Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteText( const G4Track* aTrack, G4bool bIsFirst )
{
  G4double data = GetValueFromTrack( aTrack, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteText Track " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteText( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst )
{
  G4double data = GetValueFromSecoTrack( aTrack1, aTrack2, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteText SecoTrack " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVData::WriteText( const G4Event* anEvent, G4bool bIsFirst )
{
  G4double data = GetValueFromEvent( anEvent, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteText Event " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVData::WriteText( const G4Run* aRun, G4bool bIsFirst )
{
  G4double data = GetValueFromRun( aRun, 0 );
  if( !bIsFirst ) *theFileOutText << ",";
  *theFileOutText << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteText Run " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVData::WriteBin( const G4Step* aStep )
{
  float data = GetValueFromStep( aStep, 0 );
  theFileOutBin->write((char*)&data, sizeof(float));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteBin Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteBin( const G4Track* aTrack )
{
  float data = GetValueFromTrack( aTrack, 0 );
  theFileOutBin->write((char*)&data, sizeof(float));

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteBin Track " << theName << " = " << data << G4endl;
#endif

}
//----------------------------------------------------------------
void GmVData::WriteBin( const G4Track* aTrack1, const G4Track* aTrack2 )
{
  float data = GetValueFromSecoTrack( aTrack1, aTrack2, 0 );
  theFileOutBin->write((char*)&data, sizeof(float));

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteBin SecoTrack " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVData::WriteBin( const G4Event* anEvent )
{
  float data = GetValueFromEvent( anEvent, 0 );
  theFileOutBin->write((char*)&data, sizeof(float));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteBin Event " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteBin( const G4Run* aRun )
{
  float data = GetValueFromRun( aRun, 0 );
  theFileOutBin->write((char*)&data, sizeof(float));
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteBin Run " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteCout( const G4Step* aStep, G4bool bIsFirst )
{
  G4double data = GetValueFromStep( aStep, 0 );
  if( !bIsFirst )G4cout << ",";
   G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteCout Step " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteCout( const G4Track* aTrack, G4bool bIsFirst )
{
  G4double data = GetValueFromTrack( aTrack, 0 );
  if( !bIsFirst )G4cout << ",";
 G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteCout Track " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteCout( const G4Track* aTrack1, const G4Track* aTrack2, G4bool bIsFirst )
{
  G4double data = GetValueFromSecoTrack( aTrack1, aTrack2, 0 );
  if( !bIsFirst )G4cout << ",";
 G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteCout SecoTrack " << theName << " = " << data << G4endl;
#endif

}


//----------------------------------------------------------------
void GmVData::WriteCout( const G4Event* anEvent, G4bool bIsFirst )
{
  G4double data = GetValueFromEvent( anEvent, 0 );
  if( !bIsFirst )G4cout << ",";
 G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteCout Event " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
void GmVData::WriteCout( const G4Run* aRun, G4bool bIsFirst )
{
  G4double data = GetValueFromRun( aRun, 0 );
  if( !bIsFirst )G4cout << ",";
 G4cout << data;

#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::WriteCout Run " << theName << " = " << data << G4endl;
#endif

}

//----------------------------------------------------------------
G4double GmVData::GetValueFromStep( const G4Step* aStep, G4int index )
{
  if( theSubExpressions.size() == 0 ) {
    G4Exception("GmVData::GetValueFromStep(aStep)",
		"This method should not be called, check implementation of data class",
		FatalErrorInArgument,
		G4String("DATA= "+theName).c_str()); 
    return 0.;
  } else {
    G4String expression;
    unsigned int siz = theSubExpressions.size();
    unsigned int sizData = theSubData.size();
    for( unsigned int ii = 0; ii < siz; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizData ) expression += GmGenUtils::ftoa( theSubData[ii]->GetValueFromStep(aStep, index));
    }
    return GmGenUtils::GetValue( expression ); //GmGenUtils
  }
}

//----------------------------------------------------------------
G4double GmVData::GetValueFromTrack( const G4Track* aTrack, G4int index )
{
  if( theSubData.size() == 0 ) {
    G4Exception("GmVData::GetValueFromTrack(aTrack)",
		"This method should not be called, check implementation of data class",
		FatalErrorInArgument,
		G4String("DATA= "+theName).c_str()); 
    return 0.;
  } else {
    G4String expression;
    unsigned int siz = theSubExpressions.size();
    unsigned int sizData = theSubData.size();
    for( unsigned int ii = 0; ii < siz; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizData ) expression += GmGenUtils::ftoa( theSubData[ii]->GetValueFromTrack(aTrack, index));
    }
    return GmGenUtils::GetValue( expression ); //GmGenUtils
  }
}
  
//----------------------------------------------------------------
G4double GmVData::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int index)
{
  if( theSubData.size() == 0 ) {
    G4Exception("GmVData::GetValueFromSecoTrack(aTrack1,aTrack2)",
		"This method should not be called, check implementation of data class",
		FatalErrorInArgument,
		G4String("DATA= "+theName).c_str()); 
    return 0.;
  } else {
    G4String expression;
    unsigned int siz = theSubExpressions.size();
   unsigned int sizData = theSubData.size();
    for( unsigned int ii = 0; ii < siz; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizData ) expression += GmGenUtils::ftoa( theSubData[ii]->GetValueFromSecoTrack(aTrack1, aTrack2, index));
    }
    return GmGenUtils::GetValue( expression ); //GmGenUtils
  }
}


//----------------------------------------------------------------
G4double GmVData::GetValueFromEvent( const G4Event* anEvent, G4int index )
{
  if( theSubData.size() == 0 ) {
    G4Exception("GmVData::GetValueFromEvent(anEvent)",
		"This method should not be called, check implementation of data class",
		FatalErrorInArgument,
		G4String("DATA= "+theName).c_str()); 
    return 0.;
  } else {
    G4String expression;
    unsigned int siz = theSubExpressions.size();
    unsigned int sizData = theSubData.size();
    for( unsigned int ii = 0; ii < siz; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizData ) expression += GmGenUtils::ftoa( theSubData[ii]->GetValueFromEvent(anEvent, index));
    }
    return GmGenUtils::GetValue( expression ); //GmGenUtils
  }
}

//----------------------------------------------------------------
G4double GmVData::GetValueFromRun( const G4Run* aRun, G4int index )
{
  if( theSubData.size() == 0 ) {
    G4Exception("GmVData::GetValueFromRun(aRun)",
		"This method should not be called, check implementation of data class",
		FatalErrorInArgument,
		G4String("DATA= "+theName).c_str()); 
    return 0.;
  } else {
    G4String expression;
    unsigned int siz = theSubExpressions.size();
    unsigned int sizData = theSubData.size();
    for( unsigned int ii = 0; ii < siz; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizData ) expression += GmGenUtils::ftoa( theSubData[ii]->GetValueFromRun(aRun, index));
    }
    return GmGenUtils::GetValue( expression ); //GmGenUtils
  }
}

//----------------------------------------------------------------
G4String GmVData::GetStringValueFromStep( const G4Step* )
{
  G4Exception("GmVData::GetStringValueFromStep",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}

//----------------------------------------------------------------
G4String GmVData::GetStringValueFromTrack( const G4Track* )
{
  G4Exception("GmVData::GetStringValueFromTrack",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}

//----------------------------------------------------------------
G4String GmVData::GetStringValueFromSecoTrack( const G4Track* , const G4Track* )
{
  G4Exception("GmVData::GetStringValueFromSecoTrack",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}

//----------------------------------------------------------------
G4String GmVData::GetStringValueFromEvent( const G4Event* )
{
  G4Exception("GmVData::GetStringValueFromEvent",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}


//----------------------------------------------------------------
G4String GmVData::GetStringValueFromRun( const G4Run* )
{
  G4Exception("GmVData::GetStringValueFromRun",
	      "This method should not be called, check implementation of data class",
	      FatalErrorInArgument,
	      G4String("DATA= "+theName).c_str()); 
  return "";
}


//----------------------------------------------------------------
void GmVData::AddSubData(GmVData* data )
{
  theSubData.push_back( data );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData::AddSubData data " << theSubData.size()-1 << " " << data->GetName() << " " <<  theSubData[theSubData.size()-1]->GetName() << G4endl; 
#endif

  //set properties as that of subdata, but check for conflicts with other previous subdata
  std::vector<GmVData*>::const_iterator ite;
  for( ite = theSubData.begin(); ite != theSubData.end(); ite++ ){
    CheckSubData( theHNBins, data->GetHNBins(), "HNBins" );
    CheckSubData( theHMin, data->GetHMin(), "HMin" );
    CheckSubData( theHMax, data->GetHMax(), "HMax" );

    CheckSubData( theFileOutText, data->GetFileOutText(), "FileOutText" );
    CheckSubData( theFileOutBin, data->GetFileOutBin(), "FileOutBin" );

    CheckSubData( bAccumulating, data->IsAccumulating(), "Accumulating" );

    CheckSubData( theNBytes, data->GetNBytes(), "NBytes" );

    CheckSubData( theCType, data->GetCType(), "CType" );

    CheckSubData( bInitial, data->IsInitial(), "Initial" );

  }
}

//----------------------------------------------------------------
//template<class TYPE> void GmVData::CheckSubData( TYPE& dataVal, TYPE subdataVal, const G4String name ) 
template<class TYPE> void GmVData::CheckSubData( TYPE& dataVal, TYPE subdataVal, const G4String ) 
{
  /*AccumulatedEnergyDeposited/InitialDensity/keV*cm/1.  if( dataVal != subdataVal ) {
    G4Exception("GmVData::CheckSubData",
		"Error in argument",
		JustWarning,
		G4String("subdata has different " + name + " than data").c_str());
		} */
  dataVal = subdataVal;

}

//----------------------------------------------------------------
void GmVData::AddSubExpression( G4String expr )
{
  theSubExpressions.push_back( expr );
}

//----------------------------------------------------------------
void GmVData::Initialise()
{
  unsigned int sizData = theSubData.size();
  for( unsigned int ii = 0; ii < sizData; ii++ ){
    theSubData[ii]->Initialise();
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmVData::Initialise data " << theName << " subData " << ii << " " <<  theSubData[ii]->GetName() << G4endl; 
#endif
  }

}

//----------------------------------------------------------------
void GmVData::Accumulate( const G4Step* aStep, G4int index )
{
  unsigned int sizData = theSubData.size();
  for( unsigned int ii = 0; ii < sizData; ii++ ){
    theSubData[ii]->Accumulate( aStep, index );
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmVData::Accumulate data " << ii << " " << theSubData[ii] << " " <<  theSubData[ii]->GetName()  << " index " << index << G4endl; 
#endif
  }

}

//----------------------------------------------------------------
G4double GmVData::GetHMin() const 
{
  if( theSubExpressions.size() == 0 || !GmVData::bComposeHLimits ) {
    return theHMin;
  } else {
    G4String expression;
    unsigned int siz = theSubExpressions.size();
    unsigned int sizData = theSubData.size();
    for( unsigned int ii = 0; ii < siz; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizData ) expression += GmGenUtils::ftoa( theSubData[ii]->GetHMin());
    }
    if( expression.find("log") != std::string::npos && expression.find("(0") != std::string::npos ) {
      G4Exception("GmVData::GetHMin",
		  "",
		  JustWarning,
		  "Minimum of histogram with logarithmc X axis set to -99");
      expression = "-99";
    }
    return GmGenUtils::GetValue( expression ); //GmGenUtils
  }
}

//----------------------------------------------------------------
G4double GmVData::GetHMax() const 
{
  if( theSubExpressions.size() == 0 || !GmVData::bComposeHLimits ) {
    return theHMax;
  } else {
    G4String expression;
    unsigned int siz = theSubExpressions.size();
    unsigned int sizData = theSubData.size();
    for( unsigned int ii = 0; ii < siz; ii++ ){
      expression += theSubExpressions[ii];
      if( ii < sizData ) expression += GmGenUtils::ftoa( theSubData[ii]->GetHMax());
    }
    return GmGenUtils::GetValue( expression ); //GmGenUtils
  } 

}
