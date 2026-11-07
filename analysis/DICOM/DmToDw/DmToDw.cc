#include "DCMDmToDw.hh"

#include <iostream>
#include <map>

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMDmToDw* dcmCT = new DCMDmToDw();
  
  dcmCT->ProcessArguments( argc, argv);
  
  dcmCT->CheckArguments();

  dcmCT->ReadFilesAndGetImages();

}
  
