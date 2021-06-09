#ifndef GmGenUtils_HH
#define GmGenUtils_HH
//
#include "globals.hh"
#include <set>
#include <vector>
#include <map>

class GmGenUtils 
{
public:
  static G4bool bCheckTimeUnits;
  static void SetCheckTimeUnits( G4bool bctu ) {
    bCheckTimeUnits = bctu;
  }
  
  static G4bool IsNumber( const G4String& str);
  static G4bool IsNumberWithUnit( const G4String& str);
  static G4double GetVal( const G4String& str );
  static G4double GetValue( const G4String& str );
  static G4String CorrectByTime( const G4String& str, const G4String& timeStr, const G4String& nSeconds );
  static G4bool IsFloat( const G4String& str );
  static G4bool IsDouble( const G4String& str ); 
  static G4bool IsInt( const G4String& str );
  static G4bool IsInteger( const G4String& str );
  static G4bool GetBool( const G4String& str ); 
  static G4bool GetBoolean( const G4String& str ); 
  static G4int GetInt( const G4String& str );
  static G4int GetInteger( const G4String& str ); 
  //G4double GetFloat( const G4String& str );  //from G4tgrUtils
  //G4double GetDouble( const G4String& str ); //from G4tgrUtils
  static std::vector<G4String> GetWordsInString( const G4String& stemp);
  static G4String itoa(int current);
  static G4String ftoa(float flo);
  static G4String FileInPath( const G4String& filename );
  static G4String FileInPath( const G4String& filepath, const G4String& filename );
  static G4bool CheckNWords( const G4String& line, G4int nWords, const G4String& msg, G4bool error = 1 );
  
  //---- Looks if word1 and word2 are equivalent, considering that word1 may have '*', meaning 'any character'
  static G4bool AreWordsEquivalent( const G4String& word1, const G4String& word2 );
  
  static G4bool CaseInsensitiveCompare( const std::string& str1, const std::string& str2 );
  static G4double GetMomentum( G4double mass, G4double kinE );
  static G4double GetKineticEnergy( G4double mass, G4double mom );
  
  static G4bool IsLittleEndian();
  static short ShortEndianSwap( short s );
  static int LongEndianSwap (int i);
  static float FloatEndianSwap( float f );
  static double DoubleEndianSwap( double d );
  
  static void ReadUnits();
  static G4bool IsUnit( const G4String str );
  static std::set<G4String> theUnits;
  
  static G4int sign(G4double val );
  
  static G4double InterpolateLinLin( G4double indexVal, std::map<G4double,G4double> dataMap );
  static G4double InterpolateLinLog( G4double indexVal, std::map<G4double,G4double> dataMap );
  static G4double InterpolateLogLin( G4double indexVal, std::map<G4double,G4double> dataMap );
  static G4double InterpolateLogLog( G4double indexVal, std::map<G4double,G4double> dataMap );

  static std::vector<G4String> StringSplit( const G4String& theString, const  G4String& theDelimiter);

};

#endif
