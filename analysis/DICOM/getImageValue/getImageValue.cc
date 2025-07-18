#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "DCMGetImageValue.hh"

#include <iostream>
#include <map>

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMGetImageValue* dcmCT = new DCMGetImageValue();
  
  dcmCT->Initialise();
  
  dcmCT->ProcessArguments( argc, argv);
  
  dcmCT->CheckArguments();

  dcmCT->ReadFilesAndGetImages();

  dcmCT->GetClosestValue();

}
  
