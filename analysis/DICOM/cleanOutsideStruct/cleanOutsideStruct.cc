#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "DCMCleanOutsideStruct.hh"

#include <iostream>
#include <map>

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMCleanOutsideStruct* dcmCT = new DCMCleanOutsideStruct();
  
  dcmCT->Initialise();
  
  dcmCT->ProcessArguments( argc, argv);
  
  dcmCT->CheckArguments();

  dcmCT->ReadFilesAndGetImages();

  dcmCT->BuildStructData();

  dcmCT->ChangeImages();

  dcmCT->WriteNewImage();

}
  
