#ifndef readElementZ_C
#define readElementZ_C

#include <fstream>
#include <string>
#include <map>
std::map<std::string,std::string> theElementZ;

void readElementZ()
{
  std::ifstream finEle("/home/arce/gamos/GAMOS.3.0.0/data/elementZ.lis");
  std::string symbol, Z;
  for(;;) {
    if( finEle.eof() ) break;
    finEle >> symbol >> Z;
    theElementZ[symbol] = Z;
    //    if( !GetWordsInLine(finEle,wl) ) break;
    // theElementZ[wl[0]] = wl[1];
  }
}    

#endif
