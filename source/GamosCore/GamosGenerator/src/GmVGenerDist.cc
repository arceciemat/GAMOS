#include "GmVGenerDist.hh"


//---------------------------------------------------------------------
void GmVGenerDist::ResetParams()
{
  if( theOrigParams.size() != 0 ) SetParams( theOrigParams ); // if it has 0, there is no need of setting them

}

