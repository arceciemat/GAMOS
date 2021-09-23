#include "DCMPrintValueAlongLine.hh"

int main(int argc,char** argv) 
{
  DCMPrintValueAlongLine* pval = new DCMPrintValueAlongLine();
  
  pval->ProcessArguments(argc,argv);

  pval->CheckArguments();

  pval->ReadFilesAndGetImages();

  pval->GetInfoFromImages();

}
  
