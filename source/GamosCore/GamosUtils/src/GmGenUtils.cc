#include "GmGenUtils.hh"

#include "G4tgrFileIn.hh"
#include "G4tgrUtils.hh"
#include "G4UnitsTable.hh"

#include <fstream>
#include <sstream>
std::set<G4String> GmGenUtils::theUnits;
#ifdef WIN32
 #if defined GmUtils_ALLOC_EXPORT 
  G4bool GmGenUtils::bCheckTimeUnits = true;
 #endif
#else
 G4bool GmGenUtils::bCheckTimeUnits = true;
  G4double GmGenUtils::thePrecision = 1.e-6;
#endif

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
  //  G4cout << " GmGenUtils::IsNumberWithUnit( " << str << G4endl; //GDEB
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
    //    char chara = str.at(il);
    if( symbols.find(str.at(il)) != symbols.end() ) {
      if( il != 0 && (str.at(il) == '-' || str.at(il) == '+' ) && (str.at(il-1) == 'e' || str.at(il-1) == 'E' ) ) continue;
      symbolPositions.insert(il);
      //           G4cout << " symbolPositions " << il << " = " << str[il] << G4endl; //GDEB
    }
  }
  symbolPositions.insert(il);

  std::set<int>::const_iterator ite1, ite2;
  ite1 = symbolPositions.begin();
  ite2 = ite1; ite2++;
  for( ;; ite1++, ite2++ ){
    int ipos1 = *ite1;
    int ipos2 = *ite2;
    //    G4cout << " IPOS  " << ipos1 << " " << ipos2 << G4endl; //GDEB
    if( ipos2-ipos1 != 1 ) { ; //not contiguous 
      G4String word = str.substr(ipos1+1, ipos2-ipos1-1);
      //      G4cout << " WORD " << word << " " << ipos1 << " " << ipos2 << G4endl; //GDEB
      if( !IsNumber(word) && !IsUnit(word) && !G4tgrUtils::WordIsFunction(word) ) {
	//	G4cout << " return 0 " << word << " IsNumber(word) " << IsNumber(word) << " IsUnit(word) " << IsUnit(word) << " G4tgrUtils::WordIsFunction(word) " << G4tgrUtils::WordIsFunction(word) << G4endl; //GDEB
	return 0;
      }
    }
    if( ipos2 == int(il) ) break;
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
G4int GmGenUtils::GetInt( const G4String& str )
{
  return GetInteger(str);
} 


//------------------------------------------------------------------------
G4int GmGenUtils::GetInteger( const G4String& str )
{
  G4String strnew = str;
#ifndef WIN32
  if( strnew[strnew.length()-1] == '\r' ) {
    strnew = strnew.substr(0,strnew.length()-1);
  }
#endif
  return G4tgrUtils::GetInt(strnew);
} 


//------------------------------------------------------------------------
G4double GmGenUtils::GetVal( const G4String& str )
{
  return GetValue( str );
}

//------------------------------------------------------------------------
G4double GmGenUtils::GetValue( const G4String& str )
{
  G4String strnew = str;

#ifndef WIN32
  if( strnew[strnew.length()-1] == '\r' ) {
    strnew = strnew.substr(0,strnew.length()-1);
  }
#endif
  
  if( str == "false" || str == "FALSE" ) return 0;
  if( str == "true" || str == "TRUE" ) return 1;

  if( GmGenUtils::bCheckTimeUnits ) {
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
  
//------------------------------------------------------------------------
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
  if(ch != -1 ) {
    isFloat = TRUE;
    /*-   unsigned int ii = 0;
      for( ii = ch+1; ii < str.size(); ii++) {
      if( str[ii] != '0' ) isFloat = TRUE;
      } */
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

  G4String str2 = "100";
  if(GmGenUtils::IsFloat(str2) ) {
  G4Exception("","",FatalException,"");
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
G4String GmGenUtils::itoa(int val, int nChars )
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

  if( nChars != -1 && G4int(result.length()) < nChars ) {
    for( size_t ii = 0; ii < nChars-result.length(); ii++ ) {
      result = "0" + result;
    }
  }
  
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
G4String GmGenUtils::FileInPath( G4String fileName )
{
  std::ifstream* in = new std::ifstream(fileName);
  if (in->is_open()) return fileName;
  
  char* pathc = getenv( "GAMOS_SEARCH_PATH" );
  if( !pathc ) G4Exception("GmGenUtils::FileInPath",
			   "",
			   FatalErrorInArgument,
			   "No GAMOS_SEARCH_PATH variable defined, please define it as in config/confgamos.sh or config/confgamos.csh");
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );

  return GmGenUtils::FileInPath( path, fileName );
}
//-----------------------------------------------------------------------
G4String GmGenUtils::FileInPath( G4String& filepath, const G4String fileName )
{
  if( fileName == "" ) return "";

  //----- path is separated by semicolons
  G4int scfound1 = -1;
  G4int scfound2; //- = filepath.find(":",scfound1);
  G4int slen = filepath.length();
  G4String realpath;
  std::ifstream* in;
  G4String file;
#ifdef WIN32
  filepath += ";/"; // include absolute path
#else
  filepath += ":/"; // include absolute path
#endif

  do {
#ifdef WIN32
    scfound2 = filepath.find(";",scfound1+1);
#else
    scfound2 = filepath.find(":",scfound1+1);
#endif
    //    G4cout << filepath << " scfound1 " <<  scfound1 << " scfound2 " << scfound2 << G4endl; //GDEB
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
//file="g:\gamoswin\GAMOS.6.1.0\data: / \deprecatedCommands.lis
    in = new std::ifstream(file);
    // G4cout << file << " open? " << in->is_open() << G4endl; //GDEB
    if (in->is_open() ) break;
    delete in;
    scfound1 = scfound2;
  } while(scfound2 != -1 );

  //  G4cout << file << " openFINAL " << in->is_open() << G4endl; //GDEB
  if (!in->is_open() )  {
    G4Exception("GmGenUtils::FileInPath",
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
  //  G4cout << "START GmGenUtils::AreWordsEquivalent " << word1 << " =?= " << word2 << G4endl; //GDEB

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
      //      G4cout << " GmGenUtils::AreWordsEquivalent string limits between asterisks stringPair " << cStart << " to " << cAster << " " <<  G4endl; //GDEB
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
    //    G4cout << " GmGenUtils::AreWordsEquivalent characters after last asterisk stringPair " << cStart << " " << word1.length()-cStart << " " << G4endl; //GDEB
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
    //    G4cout << ii << " GmGenUtils::AreWordsEquivalent word  Found:  " << spair.first << ":" <<spair.second << ":" << cStart << " " << word1.substr(spair.first, spair.second) << " at pos " << sFound << " in " << word2 << G4endl; //GDEB
    if( sFound == std::string::npos ) {
      bEqual = FALSE;
      break;
    } else {
      bEqual = TRUE;
      //---- If there is no asterisk before first character, the first string pair found has to start at the first character
      //      G4cout << " GmGenUtils::AreWordsEquivalent chk end " << spair.first << " " << spair.second  << " " << spair.first+spair.second << " " << word1.length() << " " << sFound+spair.second << " " << word2.length()<< G4endl; //GDEB
      if( spair.first == 0 && sFound != 0 ) {
	bEqual = FALSE;
	break;
	//---- If there is no asterisk after last character, the last string pair found has to end at the last character
      } else if( spair.first+spair.second == word1.length() && 
		 sFound+spair.second != word2.length() ) {
	//	G4cout << " GmGenUtils::AreWordsEquivalent word1 end, word2 not end " << spair.second  << " " << sFound+spair.second << " " << word2.length()<< G4endl; //GDEB	
	bEqual = FALSE;
	//---- Check next occurence
	ii--;
	cStart += sFound-spair.second+1; // look after bFound
	//	break;
      }
      cStart += spair.second;
    } 
  }

  //  G4cout << " GmGenUtils::AreWordsEquivalent RETURN " << bEqual << G4endl; //GDEB

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
        G4cout << " IT IS BIG ENDIAN " << G4endl;
    return FALSE;
  }

}

//-----------------------------------------------------------------------
size_t GmGenUtils::freadLittleEndian4 ( void * ptr, size_t size, size_t count, FILE * stream )
{
  fread(ptr, size, count, stream );
  if( !IsLittleEndian() ) {
    float* newVal = static_cast<float*>(ptr);
    float newValE = reverseEndian(*newVal);
    *newVal = newValE;
  }
  return count;
}

//-----------------------------------------------------------------------
size_t GmGenUtils::freadLittleEndian8( void * ptr, size_t size, size_t count, FILE * stream )
{
  fread(ptr, size, count, stream );
  if( !IsLittleEndian() ) {
    size_t* newVal = static_cast<size_t*>(ptr);
    size_t newValE = reverseEndian(*newVal);
    *newVal = newValE;
  }
  return count;
}

//-----------------------------------------------------------------------
size_t GmGenUtils::fwriteLittleEndian4( const void * ptr, size_t size, size_t count, FILE * stream )
{
  if( IsLittleEndian() ) {
    fwrite(ptr,size,count,stream);
  } else {
    G4Exception("GmGenUtils::fwriteLittleEndian4",
		"",
		FatalException,
		"BIG ENDIAN IS NOT SUPPORTED, please contact GAMOS authors");
		/* THIS IS WRONG 
    const float* newVal = static_cast<const float*>(ptr);
    const float newValE = checkEndian(*newVal);
    fwrite(&newValE,size,count,stream);
		*/
  }
  return count;

  /*
  if( sizeof(ptr) == 1 ) {
    //void *q = const_cast<void *>(p);
    //char *r = static_cast<char *>(q);
    const char* newVal = static_cast<const char*>(ptr);    
    const char newValE = GmGenUtils::checkEndian(*newVal);
    //    const char newVal2E = GmGenUtils::reverseEndian(*newVal);
    fwrite(&newValE,size,count,stream);
  } else if( sizeof(ptr) == 2 ) {
    const short int* newVal = static_cast<const short int*>(ptr);
    const short int newValE = GmGenUtils::checkEndian(*newVal);
    fwrite(&newValE,size,count,stream);
  } else if( sizeof(ptr) == 4 ) {
    const int* newVal = static_cast<const int*>(ptr);
    const int newValE = GmGenUtils::checkEndian(*newVal);
    fwrite(&newValE,size,count,stream);
    G4cout << " fwriteLE int/float " << newValE << G4endl; //GDEB
  } else if( sizeof(ptr) == 8 ) {
    const long long* newVal = static_cast<const long long*>(ptr);
    const long long newValE = GmGenUtils::checkEndian(*newVal);
    fwrite(&newValE,size,count,stream); 
  } else {
    return 0;
  }
  return count; 
*/
}

//-----------------------------------------------------------------------
size_t GmGenUtils::fwriteLittleEndian8( const void * ptr, size_t size, size_t count, FILE * stream )
{
  if( IsLittleEndian() ) {
    fwrite(ptr,size,count,stream);
  } else {
    G4Exception("GmGenUtils::fwriteLittleEndian8",
		"",
		FatalException,
		"BIG ENDIAN IS NOT SUPPORTED, please contact GAMOS authors");
		/* THIS IS WRONG   const size_t* newVal = static_cast<const size_t*>(ptr);
    const size_t newValE = GmGenUtils::checkEndian(*newVal);
    fwrite(&newValE,size,count,stream); 
    //    G4cout << " BIG ENDIAN fwriteLittleEndian8 " << ptr << " = " << newVal << " " << &newValE << " .:: " << newValE << " = " << *newVal << G4endl; //GDEB */
  }
  
  return count;
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
      if( wl[1] == "static" && wl[2] == "constexpr"){
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
    /*    for( std::set<G4String>::const_iterator ite = theUnits.begin(); ite != theUnits.end(); ite++ ) {
      G4cout << " UNIT " << *ite << G4endl; //GDEB
      } */
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

//----------------------------------------------------------------
std::string::size_type GmGenUtils::GetNextSeparator( G4int iSeparator, G4String dataName )
{
  if( iSeparator >= G4int(dataName.length()) ) return std::string::npos;

  const char* separators("+-*/()");
  std::string::size_type isepF = std::string::npos;
  for( G4int ii = 0; ii < 6; ii++ ){
    std::string::size_type isepFt = dataName.find(separators[ii],iSeparator);
    //-    G4cout  << " separator " << separators[ii] << " " << isepFt << G4endl;
    // check for case of exponential
    if( ii < 2 && G4int(isepFt) >= 2 ) {
      if( ( dataName[isepFt-1] == 'e' || dataName[isepFt-1] == 'E' )
	  && ( GmGenUtils::IsNumber( dataName[isepFt-2] ) || dataName[isepFt-2] == '.' )
	  && ( GmGenUtils::IsNumber( dataName[isepFt+1] ) ) ){
	 isepFt = dataName.find(isepFt+1,iSeparator);
      }
    }

    if( isepFt != std::string::npos ) {
      isepF = std::min( isepF, isepFt );
    }
  }

  //  G4cout << " GmVDataUser::GetNextSeparator( " <<  G4int(isepF) << " " << dataName << " " << iSeparator << " LEN " << dataName.length() << G4endl;
  return isepF;
}


//----------------------------------------------------------------
G4bool GmGenUtils::IsSeparator( const G4String word )
{
  
  if( G4tgrUtils::WordIsFunction( word ) || GmGenUtils::IsNumber( word ) || GmGenUtils::IsUnit(word) ){
    return true;
  } else {	
    return false;
  }
}

//----------------------------------------------------------------
void GmGenUtils::WriteStringToBinaryFile(std::ofstream& fout, G4String dat, size_t nChars )
{
  if( dat.length() > nChars ) {
    G4Exception("GmGenUtils::WriteStringToBinaryFile",
		"",
		JustWarning,
		("String length is " + itoa(dat.length()) + " only " + itoa(nChars) + " will be written : " + dat).c_str());
  }

  fout.write((char*)&dat,sizeof(char)*nChars );
}

#include <ctime>
#include <chrono>

//----------------------------------------------------------------
void GmGenUtils::DateAndTimeNow( G4String& date, G4String& time )
{
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  time_t tt = std::chrono::system_clock::to_time_t(now);
  //  tm utc_tm = *gmtime(&tt);
  tm local_tm = *localtime(&tt);
  date = itoa(local_tm.tm_year+1900,2) + itoa(local_tm.tm_mon,2) + itoa(local_tm.tm_mday,2);
  time = itoa(local_tm.tm_hour,2) + itoa(local_tm.tm_min,2) + itoa(local_tm.tm_sec,2);
}

//------------------------------------------------------------------------
G4int GmGenUtils::GetAboveInt( G4double val, G4double precision )
{
  //  G4cout << " GetAboveInt " << val << " - " << precision << " " << val - precision << " " << G4int( val - 10*precision )+1*(val>0) << G4endl; //GDEB
  return G4int( val - precision )+1*(val-precision>0); // return integer above (if val is not already an integer)
} 

//------------------------------------------------------------------------
G4int GmGenUtils::GetBelowInt( G4double val, G4double precision )
{
  // G4cout << " GetBelowInt " << G4int( val + precision )-1*(val+precision<0) << " : " << val + precision << " - " << 1*(val<0) << " : " << val << " + " << precision << " " << (val<0) << G4endl; //GDEB
  return G4int( val + precision )-1*(val+precision<0); // return integer below (if val is not already an integer)
} 

//------------------------------------------------------------------------
G4String GmGenUtils::GetEnv(const G4String& var, bool bExists )
{
  G4String varValue = "";
  char* varc = getenv(var);
  if (!varc) {
	  if (bExists) {
		  G4Exception("GmGenUtils::GetEnv",
			  "",
			  FatalErrorInArgument,
			  ("No VARIABLE " + var + " defined, please define it with 'set " + var + "= XXX'").c_str());
	  }
  }
  else {
	  varValue = getenv(var);
  }

  return varValue;
}

//------------------------------------------------------------------------
void GmGenUtils::SetCheckTimeUnits(G4bool bctu) {
	bCheckTimeUnits = bctu;
}

//------------------------------------------------------------------------
std::string GmGenUtils::rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
