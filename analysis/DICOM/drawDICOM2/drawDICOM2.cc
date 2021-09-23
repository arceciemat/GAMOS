#include "DCMDrawDICOM2.hh"

int main(int argc,char** argv) 
{
  DCMDrawDICOM2* ddd = new DCMDrawDICOM2();
  
  ddd->ProcessArguments(argc,argv);

  ddd->CheckArguments();

  ddd->ReadFilesAndGetImages();

  ddd->DrawImages();

}
  
