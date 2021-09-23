#include "G4UImanager.hh"

#include "DCMGetGammaIndex.hh"

//------------------------------------------------------------------------
int main( int argc, char** argv )
{
  DCMGetGammaIndex* gid = new DCMGetGammaIndex();

  gid->ProcessArguments(argc,argv);

  gid->CheckArguments();

  gid->ReadFilesAndGetImages();

  return 0;

}

  
