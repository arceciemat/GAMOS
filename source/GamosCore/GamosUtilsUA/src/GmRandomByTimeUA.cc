#include "G4Event.hh"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"

#include "GmRandomByTimeUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmRandomByTimeUA::GmRandomByTimeUA()
{
}

//----------------------------------------------------------------
void GmRandomByTimeUA::BeginOfEventAction( const G4Event*  )
{
  time_t now = time(0);
  tm* timeNow = localtime(&now);

  long nTries = timeNow->tm_hour*36+timeNow->tm_min*6+timeNow->tm_sec;
  // int index = 1111;
  /*
  long seeds[3];
  CLHEP::HepRandom::getTheTableSeeds(seeds,index); 
  //seeds[0] = timeNow->tm_year*2019*365*12*+timeNow->tm_mon*365*12+timeNow->tm_mday*365; 
  seeds[0] = long(fabs(double(seeds[0])));
  seeds[1] = long(fabs(double(seeds[1])));
  seeds[2] = long(fabs(double(seeds[2])));
  CLHEP::HepRandom::setTheSeeds(seeds);
  */
  CLHEP::HepRandom hran;
  while(nTries-- >= 0) {
    hran.flat();
  }

  
}
