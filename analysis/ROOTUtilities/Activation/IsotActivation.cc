#include "IsotActivation.hh"

std::set<double> IsotActivation::theTimes;
double IsotActivation::theEndProductionTime;
int IsotActivation::theNeventsPerSecond;
int IsotActivation::verbose = 1;

//-----------------------------------------------------------------
void IsotActivation::AddData(std::vector<std::string> wl )
{
  //%%% SHActivationUA_inCameraF: END_PRODUCTION: 3.1536e+08 sec  TIME: 2.592e+06 sec : Nb93[30.8] : ACTIVITY= 490.959 decays/sec   LIFETIME= 7.34332e+08 sec   NPART_IN_JOB= 88   NIsotopes= 5.20131e+11 
  // %%% SHActivationUA  TIME= 420 sec Al26[0.0]  ACTIVITY= 0.63173602 decays/sec  HALFLIFE= 2.3350006e+13 sec  NATOMS_IN_JOB= 2270 NATOMS= 2.1281252e+13

  // %%% SHActivationUA  TIME= 1 sec Al24  ACTIVITY= 598.88076 decays/sec  HALFLIFE= 2.0530001 sec  NATOMS_IN_JOB= 1 NATOMS= 1773.7968
  if( verbose >= 3 ) std::cout << " IsotActivation::AddData " << wl.size() << std::endl; //GDEB
  //-  theEndProductionTime = atof( wl[3].c_str() );  
  double time = atof( wl[3].c_str());
  theTimes.insert( time );
  double activ = atof( wl[7].c_str() );
  theActivities[time] = activ;
  theIsotName = wl[5];
  theLifeTime = atof( wl[10].c_str() ) * log(2.);
  theNPartInJob = int(atof( wl[13].c_str()));
  theNIsotInTime[time] = atof( wl[15].c_str());  

  if( verbose >= 2 ) Dump(std::cout);
}


void IsotActivation::Dump( std::ostream& out ) 
{
  out << " ISOTOPE: " << theIsotName
	 << " LifeTime= " << theLifeTime 
	 << " NPartInJob= " << theNPartInJob << std::endl;
  std::set<double>::const_iterator itet;
  for( itet = theTimes.begin(); itet != theTimes.end(); itet++ ) {
    double time = *itet;
    out << " time: " << time
	      << " activity= " << (*(theActivities.find(time))).second
	      << " NIsotInTime= " << (*(theNIsotInTime.find(time))).second 
	      << std::endl;
  }

}

double IsotActivation::GetMinimumActivity() 
{
  double vmin = DBL_MAX;
  std::map<double,double>::const_iterator ite;
  for( ite = theActivities.begin(); ite != theActivities.end(); ite++ ) {
    vmin = std::min(vmin, (*ite).second );
  }

  return vmin;
}

double IsotActivation::GetMaximumActivity() 
{
  double vmax = 0;
  std::map<double,double>::const_iterator ite;
  for( ite = theActivities.begin(); ite != theActivities.end(); ite++ ) {
    vmax = std::max(vmax, (*ite).second );
  }

  return vmax;
}

double IsotActivation::GetActivity( double time) 
{
  double vmax = 0;
  std::map<double,double>::const_iterator ite = theActivities.upper_bound(time);
  if( ite == theActivities.begin() ) {
    return (*ite).second;
  } else {
    if( ite == theActivities.end() ) {
      ite--;
    }
    std::map<double,double>::const_iterator itePrev = ite;
    itePrev--;
    double activity =  (*ite).second * exp((log(time)-log((*itePrev).first))/(log((*ite).first)-log((*itePrev).first))*(log((*ite).second)-log((*itePrev).second)));
    std::cout << theIsotName << " GETACTIVITY " << time << " = " << activity << " " << (*ite).first << " " << (*itePrev).first << " " << (*ite).second << " " << (*itePrev).second << std::endl;
    std::cout << theIsotName << " GACT " << (log(time)-log((*itePrev).first)) << " / " << (log((*ite).first)-log((*itePrev).first)) << " * " << (log((*ite).second)-log((*itePrev).second)) << std::endl;
    return activity;
  }

}
