#ifndef GmGenUtils_HH
#define GmGenUtils_HH
//
#include "globals.hh"
#include <set>
#include <vector>
#include <map>
#include <iostream>

class GmGenUtils 
{
public:
  static G4bool bCheckTimeUnits;

#ifdef WIN32  //WINDEB                                                                                           
#if defined GmUtils_ALLOC_EXPORT
	G4DLLEXPORT  void SetCheckTimeUnits( G4bool bctu );
#else
	G4DLLIMPORT  void SetCheckTimeUnits( G4bool bctu );
#endif
#else
	static void SetCheckTimeUnits( G4bool bctu );
#endif
  
  static G4bool IsNumber( char ch );
  static G4bool IsNumber( const G4String& str );
  static G4bool IsNumberWithUnit( const G4String& str );
  static G4double GetVal( const G4String& str );
  static G4double GetValue( const G4String& str );
  static G4String CorrectByTime( const G4String& str, const G4String& timeStr, const G4String& nSeconds );
  static G4bool IsFloat( const G4String& str );
  static G4bool IsDouble( const G4String& str ); 
  static G4bool IsInt( const G4String& str );
  static G4bool IsInteger( const G4String& str );
  static G4bool IsInteger( char ch );
  static G4bool GetBool( const G4String& str ); 
  static G4bool GetBoolean( const G4String& str ); 
  static G4int GetInt( const G4String& str );
  static G4int GetInteger( const G4String& str ); 
  //G4double GetFloat( const G4String& str );  //from G4tgrUtils
  //G4double GetDouble( const G4String& str ); //from G4tgrUtils
  static std::vector<G4String> GetWordsInString( const G4String& stemp);
  static G4String itoa(int current, int nChars = -1);
  static G4String ftoa(float flo);
  static G4String FileInPath( G4String filename );
  static G4String FileInPath( G4String& filepath, const G4String filename );
  static G4bool CheckNWords( const G4String& line, G4int nWords, const G4String& msg, G4bool error = 1 );
  
  //---- Looks if word1 and word2 are equivalent, considering that word1 may have '*', meaning 'any character'
  static G4bool AreWordsEquivalent( const G4String& word1, const G4String& word2 );
  
  static G4bool CaseInsensitiveCompare( const std::string& str1, const std::string& str2 );
  static G4double GetMomentum( G4double mass, G4double kinE );
  static G4double GetKineticEnergy( G4double mass, G4double mom );
  
  static G4bool IsLittleEndian();
  template <typename TYP> static TYP reverseEndian(TYP val);
  template <typename TYP> static TYP checkEndian(TYP val);
  static size_t freadLittleEndian4( void * ptr, size_t size, size_t count, FILE * stream );
  static size_t freadLittleEndian8( void * ptr, size_t size, size_t count, FILE * stream );
  static size_t fwriteLittleEndian4( const void * ptr, size_t size, size_t count, FILE * stream ); // int / float  
  static size_t fwriteLittleEndian8( const void * ptr, size_t size, size_t count, FILE * stream ); // size_t 
  static void ReadUnits();
  static G4bool IsUnit( const G4String str );
  static std::set<G4String> theUnits;
  
  static G4int sign(G4double val );
  
  static G4double InterpolateLinLin( G4double indexVal, std::map<G4double,G4double> dataMap );
  static G4double InterpolateLinLog( G4double indexVal, std::map<G4double,G4double> dataMap );
  static G4double InterpolateLogLin( G4double indexVal, std::map<G4double,G4double> dataMap );
  static G4double InterpolateLogLog( G4double indexVal, std::map<G4double,G4double> dataMap );

  static std::vector<G4String> StringSplit( const G4String& theString, const  G4String& theDelimiter);

  static G4bool IsSeparator( const G4String word );
  static std::string::size_type GetNextSeparator( G4int iSeparator, G4String dataName );

  static void WriteStringToBinaryFile(std::ofstream& fout, G4String dat, size_t nChars );

  static void DateAndTimeNow( G4String& date, G4String& time );

#ifndef WIN32
  static G4int GetAboveInt( G4double val, G4double precision = thePrecision );
  static G4int GetBelowInt( G4double val, G4double precision = thePrecision );
  static G4double thePrecision;
#else 
  static G4int GetAboveInt(G4double val, G4double precision = 1.e-6);
  static G4int GetBelowInt(G4double val, G4double precision = 1.e-6);
#endif
  static G4String GetEnv(const G4String& var, bool bExists = true);
  
  static std::string rtrim(const std::string &s);
  
};

//-----------------------------------------------------------------------
template <typename TYP>
TYP GmGenUtils::checkEndian(TYP val)
{
  if( IsLittleEndian() ) {
    return val;
  } else {
    return reverseEndian(val);
  }
  return val;

}

//-----------------------------------------------------------------------
template <typename TYP>
TYP GmGenUtils::reverseEndian(TYP val)
{
    TYP retVal;
    char *pVal = (char*) &val;
    char *pRetVal = (char*)&retVal;
    int size = sizeof(TYP);
    for(int i=0; i<size; i++) {
      pRetVal[size-1-i] = pVal[i];
    }

    return retVal;
}

#endif
