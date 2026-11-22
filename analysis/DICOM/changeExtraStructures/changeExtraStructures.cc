#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "DCMChangeStructure.hh"

#include <iostream>
#include <map>

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMChangeStructure* dcmCT = new DCMChangeStructure();
  
  dcmCT->Initialise();
  
  dcmCT->ProcessArguments( argc, argv);
  
  dcmCT->CheckArguments();

  dcmCT->ReadFilesAndGetImages();

  dcmCT->BuildStructData();

  dcmCT->ChangeImages();

  dcmCT->WriteNewImage();

}
  
