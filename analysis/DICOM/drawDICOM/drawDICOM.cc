#include "DCMDrawDICOM.hh"

int main(int argc,char** argv) 
{
  DCMDrawDICOM* ddd = new DCMDrawDICOM();
  
  ddd->ProcessArguments(argc,argv);

  ddd->CheckArguments();

  ddd->ReadFilesAndGetImages();

  ddd->DrawImages();

}
  
