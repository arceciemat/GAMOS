#ifndef GenUtils_C
#define GenUtils_C
#include "TColor.h"
#include <iostream>

#include <sstream>

//-----------------------------------------------------------------------
std::string itoa(int val)
{
  const char theDigits[11] = "0123456789";
  std::string result;
  int digit;
  int valAbs = int(fabs(val));
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
std::string ftoa(float flo, int precision = -1)
{
  if( precision <= 0 ) {
    std::stringstream df;
    df << flo;
    return df.str();
  } else {
    int power = int(log10(flo));
    if( power < 0 ) power -= 1;
    std::string powerStr = itoa(power);
    if( power > 0 ) powerStr = "+" + powerStr;
    int mantissa = int( flo/pow(10.,power) );
    int signiDigits = int( flo/pow(10,power-precision) ); // significative digits
    //    std::cout << " FTOA " << " signiDigits " << signiDigits << std::endl;
    signiDigits -= int( mantissa*pow(10,precision) );
    //   std::cout << " FTOA " << " signiDigits2 " <<  mantissa*pow(10,precision) << std::endl;
    std::string signiDigitsStr = itoa(signiDigits);
    for( int ii = 0; ii < precision; ii++ ) {
      if( signiDigits < pow(10,ii) ) {
	signiDigitsStr = "0"+ signiDigitsStr;
      }
    }
    std::string df = itoa(mantissa) + "." + signiDigitsStr + "E" + powerStr;
    //   std::cout << " FTOA " << flo << " -> " << df << " power " << powerStr << " mantissa " <<  mantissa << " signiDigits " << signiDigitsStr << std::endl;
    return df;
  }
}

//-----------------------------------------------------------------------
float atof( std::string str )
{
  float vl;
  std::istringstream is(str);
  is >> vl;
  return vl;
}

//-----------------------------------------------------------------------
void CheckValue( std::string word, double value, std::string wordName ) 
{
  if( atof(word.c_str()) != value ) {
    std::cerr << "!!! ERROR: CheckValue " << word << " <> " << value << " for " << wordName << std::endl;
    exit(1);
  } else {
    std::cout << "CheckValue " << wordName << " = " << value << std::endl;
  }
}
 
//-----------------------------------------------------------------------
//---- Count number of words in line, without the last four words
void CheckNWords( int nWords, std::vector<std::string>& wl ) 
{ 
  if( int(wl.size()) != nWords ) {
    std::cerr << "!!! ERROR: CheckNWords " << wl.size() << " <> " << nWords << std::endl;
    for( size_t ii = 0; ii < wl.size(); ii++ ) {
      std::cerr << wl[ii] << " ";
    }
    std::cerr << std::endl;
    exit(1);
  }
}

//------------------------------------------------------------------------
bool IsInteger( const std::string& str ) 
{
  bool bInt = true;
  std::string numbers("0123456789");
  for( size_t ii = 0; ii < str.length(); ii++ ){
    if( numbers.find( str[ii] ) == std::string::npos ) {
      bInt = false;
      break;
    }
  }

  return bInt;
}

#endif
