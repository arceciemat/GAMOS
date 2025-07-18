#include "DCMExtendImageAir.hh"

#include <iostream>
#include <map>

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMExtendImageAir* dcmCT = new DCMExtendImageAir();
  
  dcmCT->ProcessArguments( argc, argv);
  
  dcmCT->CheckArguments();

  dcmCT->ReadFilesAndGetImages();

}
  
