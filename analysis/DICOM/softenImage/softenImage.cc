#include "DCMSoftenImage.hh"

#include <iostream>
#include <map>

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMSoftenImage* dcmCT = new DCMSoftenImage();
  
  dcmCT->ProcessArguments( argc, argv);
  
  dcmCT->CheckArguments();

  dcmCT->ReadFilesAndGetImages();

}
  
