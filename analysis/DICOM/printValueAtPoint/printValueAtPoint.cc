#include "DCMPrintValueAtPoint.hh"

int main(int argc,char** argv) 
{
  DCMPrintValueAtPoint* goi = new DCMPrintValueAtPoint();
  
  goi->ProcessArguments(argc,argv);

  goi->CheckArguments();

  goi->ReadFilesAndGetImages();

  goi->PrintInfoFromImage();

}
  
