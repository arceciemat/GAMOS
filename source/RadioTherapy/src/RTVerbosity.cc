#include "RTVerbosity.hh"

GmVerbosity RTVerb;
#include <iostream>
/** Constructor   
 */
RTVerbosity::RTVerbosity()
{
  RTVerb.SetFilterLevel(2);
  //  std::cout << RTVerb(3) << " RTVerbosity " << RTVerb.GetFilterLevel() << std::endl;
}


void RTVerbosity::SetFilterLevel( int fl )
{
  RTVerb.SetFilterLevel( fl );
}

int RTVerbosity::GetFilterLevel() const
{
 return RTVerb.GetFilterLevel();
}
