#ifndef GetValue_HH
#define GetValue_HH

#include <math.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>

//-----------------------------------------------------------------------
double GetVal( std::string str ) 
{
  return atof( str.c_str() );
}

//-----------------------------------------------------------------------
int GetValInt( std::string str ) 
{
  return atoi( str.c_str() );
}

//-----------------------------------------------------------------------
double GetValue( const std::string valstr )
{
  return atof(valstr.c_str());
}

//-----------------------------------------------------------------------
double GetValuePrecision( double val, int prec )
{
  //t  return val;
  if( val == 0 ) return 0.;

  double valAbs = fabs(val);
  double sign = valAbs/val;
  int loge = int(log10(valAbs));
  int ival = int(valAbs*pow(10,-loge+prec));
  //  std::cout << " VAL " << val << " -> " <<  ival / pow(10,-loge+prec) << " loge " << loge << " ival " << ival << std::endl;
  valAbs = ival / pow(10,-loge+prec);

  return valAbs * sign;
}

//-----------------------------------------------------------------------
double GetValuePrecision( const std::string valstr, int prec )
{
  return GetValuePrecision( GetValue(valstr), prec );
}

//-----------------------------------------------------------------------
int SmallerInt( double dval )
{
  if( dval > pow(2,31.)-1 ) {
    std::cerr << "!! ERROR CANNOT USE SmallerInt for a number bigger than pow(2,32) " << std::endl;
    exit(1);
  }
  if( dval < 0 ) {
    return int(dval-1);
  } else {
    return int(dval);
  }
}

//-----------------------------------------------------------------------
int BiggerInt( double dval )
{

  if( dval > pow(2,31.)-1 ) {
    std::cerr << "!! ERROR CANNOT USE BiggerInt for a number bigger than pow(2,32) " << dval << " <> " << int(dval) << std::endl;
    exit(1);
  }  if( dval < 0 ) {
    return dval;
  } else {
    return int(dval+1);
  }
}

#endif
