#include "DCMGamosToDICOM.hh"

int main ( int argc, char** argv )
{
  DCMGamosToDICOM* str = new DCMGamosToDICOM();

  str->ProcessArguments(argc,argv);

  str->CheckArguments();

  str->ConvertImages();

  return 1;
}
 
  
