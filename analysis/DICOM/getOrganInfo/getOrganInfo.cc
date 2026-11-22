#include "DCMGetOrganInfo.hh"

int main(int argc,char** argv) 
{
  DCMGetOrganInfo* goi = new DCMGetOrganInfo();
  
  goi->ProcessArguments(argc,argv);

  goi->CheckArguments();

  goi->ReadFilesAndGetImages();

  goi->GetInfoFromImage();

}
  
