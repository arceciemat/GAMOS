#include "DCMSqToRT.hh"

int main ( int argc, char** argv )
{
  DCMSqToRT* str = new DCMSqToRT();

  str->ProcessArguments(argc,argv);

  str->CheckArguments();

  str->ConvertImages();

  return 1;
}
 
  
