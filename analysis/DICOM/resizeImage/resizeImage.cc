#include "DCMResizeImage.hh"

#include <iostream>
#include <map>

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMResizeImage* dcmCT = new DCMResizeImage();
  
  dcmCT->ProcessArguments( argc, argv);
  
  dcmCT->CheckArguments();

  dcmCT->ReadFilesAndGetImages();

}
  
