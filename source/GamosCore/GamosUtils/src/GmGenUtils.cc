#include "GmGenUtils.hh"

#include "G4tgrFileIn.hh"
#include "G4tgrUtils.hh"
#include "G4UnitsTable.hh"

#include <fstream>
#include <sstream>
G4bool GmGenUtils::bCheckTimeUnits = true;
std::set<G4String> GmGenUtils::theUnits;

//------------------------------------------------------------------------
G4bool GmGenUtils::IsNumber( const G4String& str)
{

  if( str == "false" || str == "FALSE" ) return 1;
  if( str == "true" || str == "TRUE" ) return 1;

  G4bool isnum = TRUE;

  int numE = 0;
  for(unsigned int ii=0; ii<str.length(); ii++){
    if(!isdigit(str[ii]) && str[ii]!='.' && str[ii]!='-' && str[ii]!='+') {
      //--- check for E(xponential)
      if(str[ii] == 'E' || str[ii] == 'e' ) {
        if(numE != 0 || ii == str.length()-1)  {
	  isnum = FALSE;
	  break;
	}
	numE++;
      } else {
	isnum = FALSE; 
	break;
      }
    }
  }
 
  return isnum;
}


//------------------------------------------------------------------------
G4bool GmGenUtils::IsNumberWithUnit( const G4String& str)
{
  //-  G4cout << " GmGenUtils::IsNumberWithUnit( " << str << G4endl;
  if( str == "false" || str == "FALSE" ) return 1;
  if( str == "true" || str == "TRUE" ) return 1;

  //--- Split word in numbers, units and functions
  size_t il;
  std::set<char> symbols; 
  symbols.insert('+');
  symbols.insert('-');
  symbols.insert('*');
  symbols.insert('/');
  symbols.insert('(');
  symbols.insert(')');
  std::set<int> symbolPositions; // position of +,-,*,/,(,)

  symbolPositions.insert( -1 );
  for( il = 0; il < str.length(); il++ ) {
    char chara = str.at(il);
    if( symbols.find(str.at(il)) != symbols.end() ) {
      symbolPositions.insert(il);
      //      G4cout << " symbolPositions " << il << " = " << str[il] << G4endl;
    }
  }
  symbolPositions.insert(il);

  std::set<int>::const_iterator ite1, ite2;
  ite1 = symbolPositions.begin();
  ite2 = ite1; ite2++;
  for( ;; ite1++, ite2++ ){
    int ipos1 = *ite1;
    int ipos2 = *ite2;
    //-    G4cout << " IPOS  " << ipos1 << " " << ipos2 << G4endl;
    if( ipos2-ipos1 != 1 ) { ; //not contiguous 
      G4String word = str.substr(ipos1+1, ipos2-ipos1-1);
      //-      G4cout << " WORD " << word << " " << ipos1 << " " << ipos2 << G4endl;
      if( !IsNumber(word) && !IsUnit(word) && !G4tgrUtils::WordIsFunction(word) ) return 0;
    }
    if( ipos2 == il ) break;
  }

  return 1;

  /*
  //If there is a '*', assume the characters after '*' are the unit
  G4String valstr = str;
  G4int iast =  str.find('*');
  if( iast != -1 ) {
    if( str.length() == 1 ) return false; // * can only refer to names
    valstr = str.substr( 0, iast );
  }
  std::string::size_type iop = valstr.find(')');
  if( iop != std::string::npos ) {
    valstr = valstr.substr( 0, iop ) + valstr.substr( iop+1, valstr.length() );
    return GmGenUtils::IsNumberWithUnit( valstr );
  }
  iop = valstr.find('(');
  if( iop != std::string::npos ) {
    valstr = valstr.substr( 0, iop ) + valstr.substr( iop+1, valstr.length() );
    return GmGenUtils::IsNumberWithUnit( valstr );
  }
  iop = valstr.find('/');
  if( iop != std::string::npos ) {
    valstr = valstr.substr( 0, iop ) + valstr.substr( iop+1, valstr.length() );
    return GmGenUtils::IsNumberWithUnit( valstr );
  }
  iop = valstr.find('+');
  if( iop != std::string::npos ) {
    valstr = valstr.substr( 0, iop ) + valstr.substr( iop+1, valstr.length() );
    return GmGenUtils::IsNumberWithUnit( valstr );
  }
  iop = valstr.find('-');
  if( iop != std::string::npos ) {
    valstr = valstr.substr( 0, iop ) + valstr.substr( iop+1, valstr.length() );
    return GmGenUtils::IsNumberWithUnit( valstr );
  }

  return GmGenUtils::IsNumber( valstr );
*/
}


//------------------------------------------------------------------------
G4int GmGenUtils::GetInteger( const G4String& str )
{
  return G4tgrUtils::GetInt(str);
} 

//------------------------------------------------------------------------
G4int GmGenUtils::GetInt( const G4String& str )
{
  return G4tgrUtils::GetInt(str);
} 


//------------------------------------------------------------------------
G4double GmGenUtils::GetVal( const G4String& str )
{
  return GetValue( str );
}

//------------------------------------------------------------------------
G4double GmGenUtils::GetValue( const G4String& str )
{
  if( str == "false" || str == "FALSE" ) return 0;
  if( str == "true" || str == "TRUE" ) return 1;

  if( GmGenUtils::bCheckTimeUnits ) {
    G4String strnew = str;

    strnew = GmGenUtils::CorrectByTime( strnew, "second", "1.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "sec", "1.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "minute", "60.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "min", "60.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "hour", "3600.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "day", "86400.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "week", "604800.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "month", "2592000.E9" ); 
    strnew = GmGenUtils::CorrectByTime( strnew, "year", "31536000.E9" ); 
    return G4tgrUtils::GetDouble(strnew);
  }

  return G4tgrUtils::GetDouble(str);

}
  
G4String GmGenUtils::CorrectByTime( const G4String& str, const G4String& timeStr, const G4String& nSeconds ) 
{
  G4String strorig = str;
  G4String strnew = str;
  while( strnew.find(timeStr) != std::string::npos ) { 
      //    if( str.find(timeStr) != std::string::npos ) {
      size_t tl = timeStr.length();
      size_t spos = strnew.find(timeStr);
      strnew = strnew.substr(0,spos) + nSeconds;
      if( strorig.length() - spos > tl ) strnew += strorig.substr(spos+tl,strorig.length());
      //   }
      strorig = strnew;
    }; 
  
  return strnew;

}


//------------------------------------------------------------------------
G4bool GmGenUtils::IsFloat( const G4String& str ) 
{

  if(!GmGenUtils::IsNumber(str) ) {
    return FALSE;
  }

  //----- Check that it is not a float, no decimal or E-n
  G4bool isFloat = FALSE;
  G4int ch = str.find('.');
  unsigned int ii = 0;
  if(ch != -1 ) {
    for( ii = ch+1; ii < str.size(); ii++) {
      if( str[ii] != '0' ) isFloat = TRUE;
    }
  }
  
  ch = str.find('E');
  if(ch != -1 ) ch = str.find('e');
  if(ch != -1 ) {
    if(str.substr(ch+1,1) == "-") isFloat = TRUE;
  }
  
  return isFloat;

}

//------------------------------------------------------------------------
G4bool GmGenUtils::IsDouble( const G4String& str ) 
{
  return GmGenUtils::IsFloat(str); 
}

//------------------------------------------------------------------------
G4bool GmGenUtils::IsInt( const G4String& str ) 
{
  //----------- first check that it is an integer
  if(!GmGenUtils::IsNumber(str) ) {
    return FALSE;
  }

  if(GmGenUtils::IsFloat(str) ) {
    return FALSE;
  }

  return TRUE;
}

//------------------------------------------------------------------------
G4bool GmGenUtils::IsInteger( const G4String& str ) 
{
  return IsInt( str );
}

//------------------------------------------------------------------------
G4bool GmGenUtils::GetBool( const G4String& str ) 
{
  if( str == "TRUE" || str == "true" || str == "1" ) {
    return TRUE;
  } else if( str == "FALSE" || str == "false" || str == "0" ){
    return FALSE;
  } else {
    G4Exception("GmGenUtils::GetBool",
		"Wrong internal argument",
		FatalException,
		G4String("trying to get the boolean value from a string that is not 'TRUE', 'true', '1', 'FALSE', 'false', '0' : " + str ).c_str());
  }

  return int( atof( str.c_str() ) );
}

//------------------------------------------------------------------------
G4bool GmGenUtils::GetBoolean( const G4String& str ) 
{ 
  return GetBool( str );
}

//------------------------------------------------------------------------
std::vector<G4String> GmGenUtils::GetWordsInString( const G4String& stemp)
{
  std::vector<G4String> wordlist;

  //---------- Read a line of file:
  //@@@@--- Cannot be read with a istream_iterator, becasuse it uses G4cout, and then doesn't read '\n'
  //----- Clear wordlist
  G4int ii;
  const char* cstr = stemp.c_str();
  int siz = stemp.length();
  int istart = 0;
  int nQuotes = 0;
  bool lastIsBlank = false;
  bool lastIsQuote = false;
  for( ii = 0; ii < siz; ii++ ){
    if(cstr[ii] == '\"' ){
      if( lastIsQuote ){
	G4Exception("GmGenUtils:GetWordsFromString",
		    "Wrong internal argument",
		    FatalException, 
		    ("There cannot be two quotes together " + stemp).c_str() );
      }
      if( nQuotes%2 == 1 ){
	//close word 
	wordlist.push_back( stemp.substr(istart,ii-istart) );
	//	G4cout << "GmGenUtils::GetWordsInString new word0 " << wordlist[wordlist.size()-1] << " istart " << istart << " ii " << ii << G4endl;
      } else {
	istart = ii+1;
      }
      nQuotes++;
      lastIsQuote = true;
      lastIsBlank = false;
    } else if(cstr[ii] == ' ' ){
      //            G4cout << "GmGenUtils::GetWordsInString blank nQuotes " << nQuotes << " lastIsBlank " << lastIsBlank << G4endl;
      if( nQuotes%2 == 0 ){
	if( !lastIsBlank && !lastIsQuote ) {
	  wordlist.push_back( stemp.substr(istart,ii-istart) );
	  //	  G4cout << "GmGenUtils::GetWordsInString new word1 " << wordlist[wordlist.size()-1] << " lastIsBlank " << lastIsBlank << G4endl;
	}

	istart = ii+1;
	lastIsQuote = false;
	lastIsBlank = true;
      }
    } else {
      if( ii == siz-1 ) {
	wordlist.push_back( stemp.substr(istart,ii-istart+1) );
	//		G4cout << "GmGenUtils::GetWordsInString new word2 " << wordlist[wordlist.size()-1] << " istart " << istart << G4endl;
      }
      lastIsQuote = false;
      lastIsBlank = false;
    }
  }
  if( nQuotes%2 == 1 ) {
    G4Exception("GmGenUtils:GetWordsFromString",
		"Wrong internal argument",
		FatalException, 
		("unbalanced quotes in line " + stemp).c_str() );
  }

  return wordlist;
}

//------------------------------------------------------------------------
G4String GmGenUtils::itoa(int val)
{
  const char theDigits[11] = "0123456789";
  G4String result;
  int digit;
  int valAbs = int(fabs(double(val)));
  do
    {
      digit = valAbs-10*(valAbs/10);
      result=theDigits[digit]+result;
      valAbs/=10;
    }
  while(valAbs!=0);
  if( val < 0 ) result = "-" + result;

  return result;
}

//-----------------------------------------------------------------------
G4String GmGenUtils::ftoa(float flo)
{
  std::stringstream df;
  df << flo;
  return df.str();

}

//-----------------------------------------------------------------------
G4String GmGenUtils::FileInPath( const G4String& fileName )
{
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  return GmGenUtils::FileInPath( path, fileName );
}
//-----------------------------------------------------------------------
G4String GmGenUtils::FileInPath( const G4String& filepath, const G4String& fileName )
{
  if( fileName == "" ) return "";

  //----- path is separated by semicolons
  G4int scfound1 = -1;
  G4int scfound2; //- = filepath.find(":",scfound1);
  G4int slen = filepath.length();
  G4String realpath;
  std::ifstream* in;
  G4String file;

  do {
#ifdef WIN32
    scfound2 = filepath.find(";",scfound1+1);
#else
	scfound2 = filepath.find(":",scfound1+1);
#endif
	//	G4cout << filepath << " scfound1 " <<  scfound1 << " scfound2 " << scfound2 << G4endl;
    if( scfound2 == -1 ) {
      realpath = filepath.substr(scfound1+1, slen );
    } else {
      realpath = filepath.substr(scfound1+1, scfound2-scfound1-1);
    }
#ifdef WIN32
    file = realpath + "\\" + fileName;
#else
	file = realpath + "/" + fileName;
#endif
    in = new std::ifstream(file);
    if (in->is_open() ) break;
    delete in;
    scfound1 = scfound2;
  } while(scfound2 != -1 );

  if (!in->is_open() )  { 
    G4Exception("GmFileIn:GmGenUtils::FileInPath",
		"Wrong argument",
		FatalErrorInArgument,
		("file " + fileName + " not found in path " + filepath ).c_str());
  }
  in->close();
//PWIN  delete in;

  return file;

}


//-----------------------------------------------------------------------
G4bool GmGenUtils::CheckNWords( const G4String& line, G4int nWords, const G4String& msg, G4bool error )
{
  if( GmGenUtils::GetWordsInString( line ).size() != (unsigned int)(nWords) ) {
    if( error ) {
      G4Exception("GmGenUtils::CheckNWords",
		  "Wrong argument",
		  FatalErrorInArgument,
		  msg.c_str() ); 
    } else {
      G4cerr << "!!ERROR " << msg << G4endl;
    }
    return 0;
  } else {
    return 1;
  }
}


//-----------------------------------------------------------------------
G4bool GmGenUtils::AreWordsEquivalent( const G4String& word1, const G4String& word2 )
{
//---- Looks if word1 and word2 are equivalent, considering that word1 may have '*', meaning 'any character'
  G4bool bEqual = TRUE;
  std::vector< std::pair< std::string::size_type, std::string::size_type> > stringPairs; //start of substring, number of characters

  //  G4cout << " GmGenUtils::AreWordsEquivalent " << word1 << " =?= " << word2 << G4endl;

  //--- Get string limits between asterisks in word1
  std::string::size_type cStart = 0;
  //  G4int cEnd = word1.length();
  for( ;; ){
    std::string::size_type cAster = word1.find("*",cStart);
    if( cAster != std::string::npos ) {
      if( cAster == cStart ) {
	if( cAster != 0 ){
	  G4Exception("GmGenUtils::AreWordsEquivalent",
		      "Wrong internal argument",
		      FatalException,
		      (G4String("A word has two asterisks together, please correct it") +  "Offending word is: " + word1).c_str() );
	} else {
	  // word1 == *
	  if(word1.size() == 1 ) return TRUE;
	}
      }     
      if( cAster!= cStart ) stringPairs.push_back( std::pair<G4int,G4int>(cStart, cAster-cStart) );
      //      G4cout << " GmGenUtils::AreWordsEquivalent stringPair " << cStart << " to " << cAster << " " <<  G4endl;
      cStart = cAster+1;
    } else {
      if( cStart == 0 ){
	//--- If there is no asterisk check if they are the same
	return word1 == word2;
      }
      
      break;
    }
  }
  
  //---- Add characters after last asterisk as string pair 
  if( cStart <= word1.length() ) {
    if( word1.length() != cStart ) stringPairs.push_back( std::pair<G4int,G4int>(cStart, word1.length()-cStart) );
    //    G4cout << " GmGenUtils::AreWordsEquivalent stringPair " << word1.length() << " " << cStart << " " <<  G4endl;
  }

  //--- If there are not asterisk, simple comparison
  if( stringPairs.size() == 0 ) {
    if( word1 == word2 ) {
      return TRUE;
    } else {
      return FALSE;
    }
  }
  
  /*  for( unsigned int ii = 0; ii < word1.length(); ii++ ){
      if( word1.substr(ii,1) == "*" ) {
      if( ii == cStart+1 ){
	G4Exception("GmGenUtils::AreWordsEquivalent","A word has two asterisks together, please correct it",FatalException,("Offending word is " + word1).c_str() );
      }
      if( ii != 0 ) {
      stringPairs.push_back( std::pair<G4int,G4int>(cStart, ii-1) );
      cStart = ii+1;
      }
      }
      } */


  //--- Find substrings in word2, in same order as in word1
  cStart = 0;
  for( unsigned int ii = 0; ii < stringPairs.size(); ii++ ){
    std::pair< std::string::size_type, std::string::size_type> spair = stringPairs[ii];
    std::string::size_type sFound = word2.find(word1.substr(spair.first, spair.second),cStart);
    //    G4cout << " GmGenUtils::AreWordsEquivalent word  Found:  " << word1.substr(spair.first, spair.second) << " at pos " << sFound << " in " << word2 << G4endl;
    if( sFound  == std::string::npos ) {
      bEqual = FALSE;
      break;
    } else {
      //---- If there is no asterisk before first character, the fisrt string pair found has to start at the first character
      //      G4cout << " GmGenUtils::AreWordsEquivalent chk end " <<  spair.first << " " << spair.second  << " " << spair.first+spair.second << " " << word1.length() << " " << sFound+spair.second << " " << word2.length()<< G4endl;
      if( spair.first == 0 && sFound != 0 ) {
	bEqual = FALSE;
	break;
	//---- If there is no asterisk after last character, the last string pair found has to end at the last character
      } else if( spair.first+spair.second-1 == word1.length() && 
		 sFound+spair.second-1 != word2.length() ) {
	bEqual = FALSE;
	break;
      }
      cStart += spair.second;
    } 
  }

  return bEqual;
}



//-----------------------------------------------------------------------
G4bool GmGenUtils::CaseInsensitiveCompare( const std::string& str1, const std::string& str2 )
{
  std::string::const_iterator it1=str1.begin();
  std::string::const_iterator it2=str2.begin();

  if( str1.length() != str2.length() ) return FALSE;

  //--- stop when either string's end has been reached
  while ( (it1!=str1.end()) && (it2!=str2.end()) ) 
  { 
    if(toupper(*it1) != toupper(*it2)) {//letters differ?
      return FALSE;
    }
    ++it1;
    ++it2;
  }

  return TRUE;
}

//-----------------------------------------------------------------------
G4double GmGenUtils::GetMomentum( G4double mass, G4double kinE )
{
  return sqrt( sqr(kinE)+2.*kinE*mass );
}


//-----------------------------------------------------------------------
G4double GmGenUtils::GetKineticEnergy( G4double mass, G4double mom )
{
  return sqrt( sqr(mom)+sqr(mass) ) - mass;
}

//-----------------------------------------------------------------------
G4bool GmGenUtils::IsLittleEndian()
{
  unsigned char endian[2] = {1, 0};
  short x = *(short *) endian;
  if( x == 1 ) {
    //  G4cout << " IT IS LITTLE ENDIAN " << G4endl;
    return TRUE;
  } else {  //  if( x == 256 ) { 
    //    G4cout << " IT IS BIG ENDIAN " << G4endl;
    return FALSE;
  }

}
  

//-----------------------------------------------------------------------
short GmGenUtils::ShortEndianSwap( short s )
{
  unsigned char b1, b2;
  
  b1 = s & 255;
  b2 = (s >> 8) & 255;

  return (b1 << 8) + b2;
}

//-----------------------------------------------------------------------
int GmGenUtils::LongEndianSwap (int i)
{
  unsigned char b1, b2, b3, b4;

  b1 = i & 255;
  b2 = ( i >> 8 ) & 255;
  b3 = ( i>>16 ) & 255;
  b4 = ( i>>24 ) & 255;

  return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

//-----------------------------------------------------------------------
float GmGenUtils::FloatEndianSwap( float f )
{
  union
  {
    float f;
    unsigned char b[4];
  } dat1, dat2;

  dat1.f = f;
  dat2.b[0] = dat1.b[3];
  dat2.b[1] = dat1.b[2];
  dat2.b[2] = dat1.b[1];
  dat2.b[3] = dat1.b[0];
  return dat2.f;
}

//-----------------------------------------------------------------------
double GmGenUtils::DoubleEndianSwap( double d )
{
  union
  {
    double d;
    unsigned char b[8];
  } dat1, dat2;

  dat1.d = d;
  dat2.b[0] = dat1.b[7];
  dat2.b[1] = dat1.b[6];
  dat2.b[2] = dat1.b[5];
  dat2.b[3] = dat1.b[4];
  dat2.b[4] = dat1.b[3];
  dat2.b[5] = dat1.b[2];
  dat2.b[6] = dat1.b[1];
  dat2.b[7] = dat1.b[0];
  return dat2.d;
}


//------------------------------------------------------------------------
void GmGenUtils::ReadUnits()
{
  G4String path( getenv( "G4INSTALL" ) );
  std::ifstream fin( (path+"/source/externals/clhep/include/CLHEP/Units/SystemOfUnits.h").c_str() );

  // skip the first 50 lines ( problem reading to avoid reading #include "CLHEP/Units/defs.h"
  const G4int NMAXLIN = 10000;
  char ltemp[NMAXLIN]; //there won't be lines longer than NMAXLIN characters
  for( G4int ii = 0; ii < 50; ii++ ){
    fin.getline( ltemp, NMAXLIN ); 
  }

  std::vector<G4String> wl;
  for(;;) {
    for( G4int ii = 0; ii < NMAXLIN; ii++) ltemp[ii] = ' ';
    fin.getline( ltemp, NMAXLIN ); 
    wl =  GmGenUtils::GetWordsInString( G4String(ltemp) );
    if( wl.size() >= 7 ) {
      // wl[0] is blank because line start with blank! 
      if( wl[1] == "static" && wl[2] == "const"){
	//	G4cout << "UNIT inserted " << wl[4] << " " << theUnits.size() << G4endl;
	    theUnits.insert( wl[4] );
      }
    }
    if( fin.eof() ) break;
  }

  theUnits.insert("min");
  theUnits.insert("minute");
  theUnits.insert("hour");
  theUnits.insert("day");
  theUnits.insert("week");
  theUnits.insert("month");
  theUnits.insert("year");

}

//------------------------------------------------------------------------
G4bool GmGenUtils::IsUnit( const G4String str )
{

  if( theUnits.size() == 0 )  {
    GmGenUtils::ReadUnits();
    /*    G4Exception("GmGenUtils::IsUnit",
		"Wrong internal argument",
		FatalException,
		"units file is not read, the code should invoke GmGenUtils::ReadUnits method"); */
  }

  if( theUnits.find(str) != theUnits.end() ) {
    return true;
  } else {
    return false;
  }
}


//------------------------------------------------------------------------
G4int GmGenUtils::sign(double val)
{
  return val > 0. ? 1 : (val < 0. ? -1 : 0);
}

//---------------------------------------------------------------------
G4double GmGenUtils::InterpolateLinLin( G4double indexVal, std::map<G4double,G4double> dataMap ) 
{

  std::map<G4double,G4double>::const_iterator iteDown, iteUp;
  iteUp = dataMap.upper_bound(indexVal);
  if( iteUp == dataMap.end() ) { 
    return (*iteUp).second;
  } else {
    iteDown = iteUp;
    if( iteUp != dataMap.begin() ) { 
      iteDown--;
    }
  }

  G4double indexDown = (*iteDown).first;
  G4double indexUp = (*iteUp).first;
  G4double valDown = (*iteDown).second;
  G4double valUp = (*iteUp).second;

  G4double value = valDown + (indexVal-indexDown)/(indexUp-indexDown)*(valUp-valDown);

  return value;
  
}

//---------------------------------------------------------------------
G4double GmGenUtils::InterpolateLinLog( G4double indexVal, std::map<G4double,G4double> dataMap ) 
{
  std::map<G4double,G4double>::const_iterator iteDown, iteUp;
  iteUp = dataMap.upper_bound(indexVal);
  if( iteUp == dataMap.end() ) { 
    return (*iteUp).second;
  } else {
    iteDown = iteUp;
    if( iteUp != dataMap.begin() ) { 
      iteDown--;
    }
  }
  G4double indexDown = (*iteDown).first;
  G4double indexUp = (*iteUp).first;
  G4double valDown = (*iteDown).second;
  if( indexDown == indexUp ) return valDown;
  G4double valUp = (*iteUp).second;
  
  G4double value = exp( log(valDown) + (indexVal-indexDown)/(indexUp-indexDown)*(log(valUp)-log(valDown)));

  /*  std::cout << "Dose  GmNumericDistributionLinLog::InterpolateValue " << value 
	    << " logValDown= " << log(valDown) << " logValUp= " << log(valUp)
	    << " indexVal= " << indexVal 
	    << " indexDown= " << indexDown << " indexUp= " << indexUp << std::endl
	    << "Dose " << (indexVal-indexDown)/(indexUp-indexDown)*(log(valUp)-log(valDown)) << " " << (indexVal-indexDown) << " / " << (indexUp-indexDown) << " * " << (log(valUp)-log(valDown)) << std::endl;
  */

  return value;
}
  

//---------------------------------------------------------------------
G4double GmGenUtils::InterpolateLogLin( G4double indexVal, std::map<G4double,G4double> dataMap ) 
{

  std::map<G4double,G4double>::const_iterator iteDown, iteUp;
  iteUp = dataMap.upper_bound(indexVal);
  if( iteUp == dataMap.end() ) { 
    return (*iteUp).second;
  } else {
    iteDown = iteUp;
    if( iteUp != dataMap.begin() ) { 
      iteDown--;
    }
  }

  G4double indexDown = (*iteDown).first;
  G4double indexUp = (*iteUp).first;
  G4double valDown = (*iteDown).second;
  G4double valUp = (*iteUp).second;

  G4double logIndexDown = log(indexDown);
  G4double value = valDown + (log(indexVal)-logIndexDown)/(log(indexUp)-logIndexDown)*(valUp-valDown);

  return value;

}

//---------------------------------------------------------------------
G4double GmGenUtils::InterpolateLogLog( G4double indexVal, std::map<G4double,G4double> dataMap ) 
{

  std::map<G4double,G4double>::const_iterator iteDown, iteUp;
  iteUp = dataMap.upper_bound(indexVal);
  if( iteUp == dataMap.end() ) { 
    return (*iteUp).second;
  } else {
    iteDown = iteUp;
    if( iteUp != dataMap.begin() ) { 
      iteDown--;
    }
  }

  G4double indexDown = (*iteDown).first;
  G4double indexUp = (*iteUp).first;
  G4double valDown = (*iteDown).second;
  G4double valUp = (*iteUp).second;

  G4double logIndexDown = log(indexDown);
  G4double value = exp( log(valDown) + (log(indexVal)-logIndexDown)/(log(indexUp)-logIndexDown)*(log(valUp)-log(valDown)));
  
  return value;

}

//---------------------------------------------------------------------
std::vector<G4String> GmGenUtils::StringSplit( const G4String& theString, const  G4String& theDelimiter)
{
  std::vector<G4String> theStringVector;
  //  G4cout << " StringSplit " << theString << " " << theDelimiter<< G4endl;
  size_t istart = 0, iend = 0;
  
  while ( iend != std::string::npos) {
    iend = theString.find( theDelimiter, istart);
    
    // If at end, use length=maxLength.  Else use length=end-start.
    theStringVector.push_back( theString.substr( istart,
						 (iend == std::string::npos) ? std::string::npos : iend - istart));
    
    // If at end, use start=maxSize.  Else use start=end+delimiter.
    istart = (   ( iend > (std::string::npos - theDelimiter.size()) )
		 ?  std::string::npos  :  iend + theDelimiter.size());
    //    G4cout << " StringSplit iend " << iend << "  " << istart << " " << theStringVector[theStringVector.size()-1] << G4endl;
  }

  return theStringVector;
}
