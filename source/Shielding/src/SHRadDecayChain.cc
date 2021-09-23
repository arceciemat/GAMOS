#include "SHRadDecayChain.hh"
#include "SHRadDecayChainSet.hh"
#include "ShieldingVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <iomanip>

//-----------------------------------------------------------------
SHRadDecayChain::SHRadDecayChain(G4String name, SHRadDecayChainSet* set)
  : theSet(set)
{
  SetName( name );
  theSetName = theSet->GetName();
  set->AddChain( this );

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << this << " SHRadDecayChain::SHRadDecayChain " << theName << G4endl;
#endif

  /*  // split name in ions
  size_t nS = 0;
  size_t nSold = 0;
  for( ;; ) {
    nS = theName.find("->",nS+1);
    if( nS == G4String::npos ) {
      theIonNames.push_back(theName.substr(nSold,theName.length()));
      break;
    }
    theIonNames.push_back(theName.substr(nSold,nS-nSold));
    nSold = nS+2;
    }*/
  theIonNames = GmGenUtils::StringSplit(theName,"->");
  theBR = 1.;
  for( size_t ii = 0; ii < theIonNames.size(); ii++ ) {
    size_t ip1 = theIonNames[ii].find("(");
    G4double BR = 1.;
    if( ip1 != std::string::npos ) {
      size_t ip2 = theIonNames[ii].find(")");
      BR = GmGenUtils::GetValue(theIonNames[ii].substr(ip1+1,ip2-ip1-1));
      theIonNames[ii] = theIonNames[ii].substr(ip2+1,999);
    } 
    theBR *= BR;
    theBRs.push_back( BR );
    //    G4cout << " ION NAMES " << theIonNames[ii] << " BR " << BR << G4endl;//DEBUG
  }
    
  theNAtomsInJob.clear();

}

//-----------------------------------------------------------------
SHRadDecayChain::SHRadDecayChain( G4Ions* ion, SHRadDecayChainSet* set)
  : theSet(set)
{
  theIons.push_back( ion );
  theIonNames.push_back( ion->GetParticleName() );
  theInvLifeTimes.push_back( 1. / ion->GetPDGLifeTime() );

  theName = ion->GetParticleName();
  theBR = 1.;
  theBRs.push_back( 1. );
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) )  G4cout << " SHRadDecayChain::SHRadDecayChain  ion " << theName << " ION " << ion->GetParticleName() << " LIFETIME add " << ion->GetPDGLifeTime()/ CLHEP::second << " sec " << G4endl;
#endif

}

//-----------------------------------------------------------------
SHRadDecayChain::SHRadDecayChain(SHRadDecayChain& chain, SHRadDecayChainSet* set)
  : theSet(set)
{ 
  
  theIons = chain.GetIons();
  theInvLifeTimes = chain.GetInvLifeTimes();
  theIonNames = chain.GetIonNames();
  theBR = chain.GetBR();
  theBRs = chain.GetBRs();
  theName = chain.GetName();
  theNAtomsInJob = chain.GetNAtomsInJob_all();

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) )  G4cout << " SHRadDecayChain::SHRadDecayChain  chain " << theName << " BR " << theBR << G4endl;
#endif
}

//-----------------------------------------------------------------
SHRadDecayChain::SHRadDecayChain(SHRadDecayChain& chain, G4int nIsot, SHRadDecayChainSet* set)
  : theSet(set)
{ 
  if( chain.GetNIons() < nIsot ){
    G4Exception("SHRadDecayChain::SHRadDecayChain",
		"Error",
		FatalException,
		("Building a chain with " + GmGenUtils::itoa(nIsot) + " ions, while there are only " + GmGenUtils::itoa(chain.GetNIons()) + " in " + chain.GetName()).c_str());
  }

  theIons = chain.GetIons();
  theInvLifeTimes = chain.GetInvLifeTimes();
  theIonNames = chain.GetIonNames();
  theBRs = chain.GetBRs();
  theBR = chain.GetBR();
  for( G4int ii = chain.GetNIons(); ii > nIsot; ii-- ){
    theIons.pop_back();
    theInvLifeTimes.pop_back();
    theIonNames.pop_back();
    theBR /= theBRs[ii-1];
    theBRs.pop_back();
  }
  theName = chain.GetSubName(nIsot);
  theNAtomsInJob = chain.GetNAtomsInJob_all();

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) )  G4cout << " SHRadDecayChain::SHRadDecayChain  chain " << theName << " BR " << theBR << G4endl;
#endif
}

//-----------------------------------------------------------------
bool SHRadDecayChain::operator==( const SHRadDecayChain& chain ) const
{
  if( chain.GetName() == theName ) {
    return true;
  } else {
    return false;
  }
}


//-----------------------------------------------------------------
void SHRadDecayChain::AddIon( G4Ions* ion, G4double BR )
{
  theIons.push_back( ion );
  theIonNames.push_back( ion->GetParticleName() );
  theInvLifeTimes.push_back( 1. / ion->GetPDGLifeTime() );

  theBR *= BR;
  theBRs.push_back( BR );
  theName += "->(" +GmGenUtils::ftoa(theBR)+")" + ion->GetParticleName();
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) )  G4cout << this << " SHRadDecayChain::AddIon  ion " << theName << " ION " << ion->GetParticleName() << " LIFETIME add " << ion->GetPDGLifeTime()/CLHEP::second << " sec " << " BR " << theBR << " newBR " << BR << G4endl;
#endif
  
}

/*//-----------------------------------------------------------------
void SHRadDecayChain::SumBR( G4double BR )
{
  // sum it with the last one
  G4double br = theBRs[theBRs.size()-1];
  theBR *= (br+BR) / br;
  br += BR;
  theBRs[theBRs.size()-1] = br;

  }*/

//-----------------------------------------------------------------
G4Ions* SHRadDecayChain::GetIon( int ii ) const
{
  if( ii >= G4int(theIons.size()) ) {
    G4Exception("SHRadDecayChain::GetIon",
		"Error",
		FatalException,
		("Asking for ion number " + GmGenUtils::itoa(ii) + ", while there are only "+ GmGenUtils::itoa(theIons.size())).c_str());
  }
  return theIons[ii];

}

//-----------------------------------------------------------------
void SHRadDecayChain::SetName( G4String name )
{
  theName = name;
  /*  theNIons = 1;
  size_t sep = 0;
  size_t sepold = 0;
  std::vector<G4String> isots;
  for( ;; ) {
    sep = name.find("->",sepold);
    if( sep == G4String::npos ) {
      theIonNames.insert( theIonNames.begin(), name.substr(sepold, name.length()) );
      break;
    } else {
      theIonNames.insert( theIonNames.begin(), name.substr(sepold, sepold-sep) );
    }
    theNIons++;
    sepold = sep+2;
  }*/
}

//-----------------------------------------------------------------
void SHRadDecayChain::AddData(std::vector<G4String> wl )
{
  //  G4cout << theName << " SHRadDecayChain::AddData  theInvLifeTimes0 " << theInvLifeTimes.size() << G4endl; //DEBUG

  //%%% SHActivationUA_inCameraF TIME= 2.592e+06 sec Cr50[0.0]  ACTIVITY= 2.97071e-12 decays/sec  LIFETIME= 8.19451e+24 sec  NPART_IN_JOB= 6 NATOMS= 3.51203e+13
  // branching ratios and ion names are set from the name passed in the constructor

  /*- al  G4cout << " IONNAME " << wl[5] << " " << theIonNames.size() << G4endl;
  if( theIonNames.size() == 0 ) {
    //--- Extract names
    //  G4String ionNames = wl[5];
    std::vector<G4String> ionNames = GmGenUtils::StringSplit(wl[5],"->");
    for( size_t ii = 0; ii < ionNames.size(); ii++ ) {
      G4cout << ii << " IONNAME " << ionNames[ii] << G4endl;
      theIonNames.push_back(ionNames[ii]);
    }
  } */

  // This is the only needed data, the rest will be recalculated
  //  G4double halfLife = GmGenUtils::GetValue( wl[10] );
  //  G4double halfLifeUnit = GmGenUtils::GetValue( wl[11] );
  G4double lifeTime = GmGenUtils::GetValue( wl[10] )/log(2.);
  G4double lifeTimeUnit = GmGenUtils::GetValue( wl[11] );

  /*  for( size_t ii = 0; ii < theIonNames.size()-1; ii++ ) { // there is only life time of last ion
    theInvLifeTimes.push_back( 0. );
    } */

  if(theInvLifeTimes.size() == 0 ) theInvLifeTimes.push_back( 1./(lifeTime*lifeTimeUnit) ); // only the first line
  //-  G4cout << theName << " SHRadDecayChain::AddData  theInvLifeTimes " <<  theInvLifeTimes[theInvLifeTimes.size()-1] << G4endl; //DEBUG
  double time = atof( wl[3].c_str());
  G4double timeUnit = GmGenUtils::GetValue( wl[4] );
  time *= timeUnit;
  theNAtomsInJob[time] += atof( wl[13].c_str());
  //-  G4cout << theName << " " << this << " SHRadDecayChain::AddData  theInvLifeTimes0 " << theInvLifeTimes.size() << G4endl; //DEBUG

  //  G4cout << " theNAtomsInJob " << theNAtomsInJob[time] << " = " << wl[13] << G4endl;
  double activ = atof( wl[7].c_str() );
  theActivities[time] = activ;
  //-  theNAtomsInTime[time] = atof( wl[14].c_str());  

  //  Dump(G4cout);
}

//-----------------------------------------------------------------
void SHRadDecayChain::AddActivity(G4double time, G4double activity )
{
  theActivities[time] = activity;
  theSet->AddTime( time );
}

//-----------------------------------------------------------------
void SHRadDecayChain::Dump( std::ostream& out ) 
{
  out << this << " ION: " << theName;
  for( size_t ii = 0; ii < theInvLifeTimes.size(); ii++ ){
    out << " HalfLife= " << log(2.)/theInvLifeTimes[ii]/CLHEP::second << " sec ";
  }
  out << " HalfLife= " << log(2.)/theInvLifeTimes[0]/CLHEP::second << " sec "
      << " NAtomsInJob= " << GetNAtomsInJob() << std::endl;
  std::set<double>::const_iterator itet;
  std::set<double> times = theSet->GetTimes();
  for( itet = times.begin(); itet != times.end(); itet++ ) {
    double time = *itet;
    out << " time: " << time
	<< " activity= " << (*(theActivities.find(time))).second
      //-	<< " NAtomsInTime= " << (*(theNAtomsInTime.find(time))).second 
	<< std::endl;
  }

}

//-----------------------------------------------------------------
double SHRadDecayChain::GetMinimumActivity() const
{
  double vmin = DBL_MAX;
  std::map<double,double>::const_iterator ite;
  for( ite = theActivities.begin(); ite != theActivities.end(); ite++ ) {
    vmin = std::min(vmin, (*ite).second );
  }

  return vmin;
}

//-----------------------------------------------------------------
double SHRadDecayChain::GetMaximumActivity() const
{
  double vmax = 0;
  std::map<double,double>::const_iterator ite;
  for( ite = theActivities.begin(); ite != theActivities.end(); ite++ ) {
    vmax = std::max(vmax, (*ite).second );
  }

  return vmax;
}

//-----------------------------------------------------------------
double SHRadDecayChain::GetLifeTime( int ii ) const
{
  G4cout << " SHRadDecayChain::GetLifeTime " << ii << " N = " << theInvLifeTimes.size() << G4endl;
  if( ii >= G4int(theInvLifeTimes.size()) ) {
    G4Exception(" SHRadDecayChain::GetLifeTime ",
		"BAD ARGUMENT",
		FatalErrorInArgument,
		("Asking fo rlifetime " + GmGenUtils::itoa(ii) + ", while thre are only " + GmGenUtils::itoa(theInvLifeTimes.size())).c_str());
  }
  return 1./theInvLifeTimes[ii];
}

//-----------------------------------------------------------------
void SHRadDecayChain::PrintData( G4double endTime, std::ostream& out )
{
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChain::PrintData " << GetName() << " time " << endTime << G4endl;
#endif

  G4double nAtomsPrev = GetNAtomsPrev( endTime );
  //--- Reset data per time
  long double lambda = theInvLifeTimes[0];
  long double activity = nAtomsPrev * lambda *CLHEP::second; // activity is per second
  theActivities[endTime] = activity;
  //-  theNAtomsInTime[endTime] = nAtomsPrev;
  
  //      G4cout << " activity " << activity << " NAtomsInTime " << NAtomsInTime << "  lambda " << lambda << " timeProduction " << timeProduction << " exp(-lambda*timeProduction) " << exp(-lambda*timeProduction) << G4endl;
  out << "%%% " << theSet->GetName()
      << std::setprecision(8) << "  TIME= " << endTime/CLHEP::second << " sec "
      << theName << " "
      << " ACTIVITY= " << activity << " decays/sec "
      << " HALFLIFE= " << log(2.)/theInvLifeTimes[theInvLifeTimes.size()-1]/CLHEP::second << " sec "
      << " NATOMS_IN_JOB= " << GetNAtomsInJob() 
      << " NATOMS= " << nAtomsPrev
      << G4endl;

}

//-----------------------------------------------------------------
G4double SHRadDecayChain::GetNAtomsPrev( G4double endTime )
{
  //-- Divide time in periods
  std::map<G4double,G4bool>::const_iterator ite;
  G4double nAtomsPrev = 0.;
  G4double timePrev = 0.;
  std::map<G4double,G4bool> periods = theSet->GetPeriods();
  for( ite = periods.begin(); ite != periods.end(); ite++ ){
    G4double tim = (*ite).first;
    G4double countTime = endTime;
    if( tim < endTime ) {
      countTime = tim;
    }

#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChain::GetNAtomsPrev period time " << (*ite).first << " countTime " << countTime << G4endl;
#endif
    if( (*ite).second ) {
      if( theIonNames.size() == 1 ) {
	if( getenv("ACTIOLD") ) {
	  nAtomsPrev = GetNAtomsProduction1( nAtomsPrev, countTime - timePrev);
	}
      } 
      if( !getenv("ACTIOLD") ) {
	nAtomsPrev = GetNAtomsProduction( nAtomsPrev, countTime - timePrev, true);
      }
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(debugVerb) ) G4cout << " SHActivationUA::PrintActivity invoked GetNAtomsProduction1 " << nAtomsPrev << G4endl;
#endif

    } else {
      if( theIonNames.size() == 1 ) {
	if( getenv("ACTIOLD") ) {
	  nAtomsPrev = GetNAtomsNoProduction1( nAtomsPrev, countTime - timePrev );
	}
      } 
      if( !getenv("ACTIOLD") ) {
	nAtomsPrev = GetNAtomsProduction( nAtomsPrev, countTime - timePrev, false);
      }
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(debugVerb) ) G4cout << " SHActivationUA::PrintActivity invoked GetNAtomsNoProduction " << nAtomsPrev << G4endl;
#endif

    }
    timePrev = countTime;
    if( tim >= endTime ) break;

  }

  return nAtomsPrev;  
}


//-----------------------------------------------------------------
G4double SHRadDecayChain::GetNAtomsProduction1( G4double , G4double time )
{

  //  G4double lifetime = theLifeTimes[0];
  long double lambda = theInvLifeTimes[0];

  long double NAtomsPerNanoSecond = GetNAtomsInJob()/theSet->GetNEventsInJob()*theSet->GetNEventsPerNanoSecond();
  long double factor = (1.-exp(-lambda*time));  
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(testVerb) )	  G4cout << " FACTOR " << factor << " -1 " << factor-1. << " l*t " << lambda<< " *time " << lambda*time << G4endl;
#endif
  // check for precision problems when lambda is very small
  if( factor == 0. ) {
    factor = lambda*time - std::pow(float(lambda*time),float(2.))/2; 
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(testVerb) )    G4cout << this << " " << theName << " SHRadDecayChain::GetNAtomsProduction1 recalculate factor " << factor << G4endl;
#endif
  }
  long double NAtomsInTime = NAtomsPerNanoSecond/lambda*factor;

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << "GetNAtomsProduction1 " << theName << " NAtomsInTime " << NAtomsInTime << " NAtomsPerNanoSecond " << NAtomsPerNanoSecond << " N_PART_IN_JOB " << GetNAtomsInJob() << " /theNEvents " << theSet->GetNEventsInJob() << " *theNEventsPerNanoSecond " << theSet->GetNEventsPerNanoSecond() << " factor " << factor << " lambda " << lambda << " time " << time << G4endl;
#endif

  return NAtomsInTime;

}


//-----------------------------------------------------------------
G4double SHRadDecayChain::GetNAtomsNoProduction1( G4double nAtomsPrev, G4double time )
{
  //  G4double lifetime = theLifeTimes[0];
  long double lambda = theInvLifeTimes[0];

  long double NAtomsInTime = nAtomsPrev * exp(-lambda*time);

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << "GetNAtomsNoProduction1 " << " NAtomsInTime " << NAtomsInTime << " nAtomsPrev " << nAtomsPrev << " lambda " << lambda << " time " << time << G4endl;
#endif

  return NAtomsInTime;

}

//-----------------------------------------------------------------
G4double SHRadDecayChain::GetNAtomsProduction( G4double nAtomsPrev, G4double time, G4bool bProduction )
{
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(testVerb) )  G4cout << this << " " << theName << " SHRadDecayChain::GetNAtomsProduction START for time " << time << " from nAtoms " << nAtomsPrev << " bProd " << bProduction << G4endl;
#endif
  
  // use general equation, with Initial N <> 0 and production rate <> 0 for all ions in chain
  G4int nIsot = theIonNames.size();
  std::vector<long double> N0s(nIsot,0.);
  N0s[nIsot-1] = nAtomsPrev;
  std::vector<long double> NAtomsPerNanoSecond(nIsot,0.);
  if( bProduction ) NAtomsPerNanoSecond[0] = GetNAtomsInJob()/theSet->GetNEventsInJob()*theSet->GetNEventsPerNanoSecond();
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(testVerb) ) G4cout << " NAtomsPerNanoSecond[0] " << NAtomsPerNanoSecond[0] << " = " << GetNAtomsInJob() << " / " << theSet->GetNEventsInJob() << " * " << theSet->GetNEventsPerNanoSecond() << G4endl;
#endif

  // Get coefficients of N
  // Ni = C_i_0 + C_i_1 exp(-l_1*t) + C_i_2 exp(-l_2*t) + ... C_i_i exp(-l_i*t) (l_i is invLifeTimes ion i
  std::map<G4int,long double> coef;
  for( G4int in = 1; in <= nIsot; in++ ) {
    if( in == 1 ) {
      coef[in*100+0] = NAtomsPerNanoSecond[in-1] / theInvLifeTimes[in-1];
    } else  {
      coef[in*100+0] = theBRs[in-1] * coef[(in-1)*100+0] * theInvLifeTimes[in-2] / theInvLifeTimes[in-1];
    }
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) ) G4cout << " SHRadDecayChain::GetNAtomsProduction COEF[" << in*100+0 << "] = " << coef[in*100+0] << " NAtomsPerNanoSecond " <<  NAtomsPerNanoSecond[in-1] << " / InvLifeTimes " << theInvLifeTimes[in-1] << G4endl;
#endif
    for( G4int in2 = 1; in2 < in; in2++ ) {
      coef[in*100+in2] = theInvLifeTimes[in2-1] * coef[(in-1)*100+in2] * theBRs[in-1] / ( theInvLifeTimes[in-1]  -theInvLifeTimes[in2-1] ); 
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(testVerb) ) G4cout << " SHRadDecayChain::GetNAtomsProduction COEF[" << in*100+in2 << "] = " << coef[in*100+in2] << " InvLF " <<theInvLifeTimes[in2-1] << " C-1 " << coef[(in-1)*100+in2] << " BR " << theBRs[in-1] << " / ILF " << theInvLifeTimes[in-1] << " - " << theInvLifeTimes[in2-1] << G4endl;
#endif
    }
    // Cii = N0s - Ci0 - Ci1 - Ci2 - ...
    coef[in*100+in] = N0s[in-1]; 
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) ) G4cout << " SHRadDecayChain::GetNAtomsProduction COEF[" << in*100+in << "] = " << coef[in*100+in] << " N0s[" << in-1 << "] = " << N0s[in-1] << G4endl;
#endif
    for( G4int in2 = 0; in2 < in; in2++ ) {
      coef[in*100+in] -= coef[in*100+in2];
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(testVerb) ) G4cout << " SHRadDecayChain::GetNAtomsProduction COEF[" << in*100+in << "] = " << coef[in*100+in] << " COEF[" << in*100+in2 << "] = " << coef[in*100+in2] << G4endl;
#endif
    }

  }

  long double NAtomsDecayingPerNanoSec = coef[nIsot*100+0];
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(testVerb) ) G4cout << " SHRadDecayChain::GetNAtomsProduction NAtomsDecayingPerNanoSec_0 " << NAtomsDecayingPerNanoSec << G4endl;
#endif
  for( G4int in2 = 1; in2 <= nIsot; in2++ ) {
    long double newNAT;
    if( theInvLifeTimes[in2-1]*time < 1.e-4 ) {
      // add coef * (1+factor)
      NAtomsDecayingPerNanoSec += coef[nIsot*100+in2];
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(testVerb) ) G4cout << " NAtomsDecayingPerNanoSec_1 " << NAtomsDecayingPerNanoSec << " added " << coef[nIsot*100+in2] << " coef[ " <<nIsot*100+in2 << "] " << coef[nIsot*100+in2] << G4endl; 
#endif
      long double factor = - theInvLifeTimes[in2-1]*time + sqr(theInvLifeTimes[in2-1]*time) / 2.;
      NAtomsDecayingPerNanoSec += coef[nIsot*100+in2] * factor;
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(testVerb) )  {
	G4cout  << this << " " << theName << in2 << " SHRadDecayChain::GetNAtomsProduction recalculate FACTOR " << factor << " ILF " << -theInvLifeTimes[in2-1] << " *time " << -theInvLifeTimes[in2-1]*time << G4endl; 
	G4cout << " NAtomsDecayingPerNanoSec " << NAtomsDecayingPerNanoSec << " added " << coef[nIsot*100+in2] * factor << " coef[ " <<nIsot*100+in2 << "] " << coef[nIsot*100+in2] << " *factor " << G4endl; 
      } 
#endif
      
    } else {
      long double factor = exp(-theInvLifeTimes[in2-1]*time);
      newNAT = coef[nIsot*100+in2] * factor;
      NAtomsDecayingPerNanoSec += newNAT;
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(testVerb) ) G4cout  << this << " " << theName << in2 << " SHRadDecayChain::GetNAtomsProduction NO recalculate factor " << factor << " exp " << -theInvLifeTimes[in2-1]*time << " ILF " << -theInvLifeTimes[in2-1] << " time " << time << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
      if( SHVerb(testVerb) ) G4cout << " NAtomsDecayingPerNanoSec " << NAtomsDecayingPerNanoSec << " NEWNAT " << newNAT << " coef[" << nIsot*100+in2 << "] " << coef[nIsot*100+in2] << G4endl;
#endif
    }
//   NAtomsDecayingPerNanoSec += coef[(nIsot-1)*100+in2] * factor;
  }

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) 
    G4cout << "GetNAtomsProduction " << theName 
	   << " NAtomsDecayingPerNanoSec " << NAtomsDecayingPerNanoSec 
	   << " NAtomsProducedPerNanoSecond " << NAtomsPerNanoSecond[0] 
	   << " N_PART_IN_JOB " << GetNAtomsInJob() 
	   << " /theNEvents " << theSet->GetNEventsInJob() 
	   << " *theNEventsPerNanoSecond " << theSet->GetNEventsPerNanoSecond() 
	   << " time " << time << G4endl;
#endif

  return NAtomsDecayingPerNanoSec;

}

//-----------------------------------------------------------------
G4double SHRadDecayChain::GetActivity( double time) const
{
  std::map<double,double>::const_iterator ite = theActivities.find(time);
  if( ite == theActivities.end() ) {
    G4Exception("SHRadDecayChain::GetActivity",
		"GAMOS bug",
		FatalException,
		G4String("Time not found in activity list " + GmGenUtils::ftoa( time )).c_str());
  }

  return (*ite).second;
}

//-----------------------------------------------------------------
G4double SHRadDecayChain::GetNAtomsInJob() const
{
  return (*(theNAtomsInJob.begin())).second;
}

//-----------------------------------------------------------------
void SHRadDecayChain::DeleteShortLived( G4double minLifeTime )
{
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(testVerb) ) G4cout << this << " SHRadDecayChain::DeleteShortLived START " << GetName() << " Nions " << theIonNames.size() << G4endl;
#endif
  std::vector<G4Ions*> ions;
  std::vector<G4String> ionNames;
  std::vector<G4double> invLifeTimes;
  std::vector<G4double> BRs;
  //  G4double BR = 1.;
  theName = "";
  unsigned int iilast = theIons.size();
  for( unsigned int ii = 0; ii < theIons.size(); ii++ ){
    G4Ions* ion = theIons[ii];
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) ) G4cout << " SHRadDecayChain::DeleteShortLived " << ion->GetParticleName() << " LT " << ion->GetPDGLifeTime()  << " >=? " << minLifeTime << " " << theBRs[ii] << G4endl;
#endif
    if( ion->GetPDGLifeTime() >= minLifeTime ) {
      if( theName != "" ) theName += "->";
      theName += ion->GetParticleName();
      ions.push_back( ion );
      ionNames.push_back( theIonNames[ii] );
      invLifeTimes.push_back( theInvLifeTimes[ii] );
      G4double BRnew = 1.;
      for( unsigned int iibr = ii; iibr > iilast; iibr-- ){
	BRnew *= theBRs[iibr];
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(testVerb) ) G4cout << BRs.size() << " SHRadDecayChain::DeleteShortLived " << iibr << " BRmult " << BRnew << G4endl;
#endif
      }
      BRs.push_back( BRnew );
      iilast = ii;
      //      BR *= theBRs[ii];
    }
  }

  // add ions ont considered to last isotpe
  for( unsigned int iibr = theBRs.size()-1; iibr > iilast; iibr-- ){
    BRs[BRs.size()-1] *= theBRs[iibr];
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(testVerb) ) G4cout << iibr << " SHRadDecayChain::DeleteShortLived adding BR to " << iilast << " " << BRs[BRs.size()-1] << " new " << theBRs[iibr] << G4endl;
#endif
  }

  theIons = ions;
  theIonNames = ionNames;
  theInvLifeTimes = invLifeTimes;
  theBRs = BRs;
  //  theBR = BR;  
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(testVerb) )  G4cout << this << " SHRadDecayChain::DeleteShortLived END " << GetName() << " Nions " << theIons.size() << " BR " << theBR << G4endl;
#endif
}

//-----------------------------------------------------------------
void SHRadDecayChain::ResetBR()
{
  theBR = 1.;
  for( unsigned int ii = 0; ii < theBRs.size(); ii++ ){
      theBR *= theBRs[ii];
  }
}

//-----------------------------------------------------------------
void SHRadDecayChain::CheckLifeTimes()
{
  G4Exception("SHRadDecayChain::CheckLifeTimes",
	      "",
	      FatalException,
	      "NOT IMPLEMENTED YET");
	     
  /*
#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) {
    std::vector<G4Ions*>::const_iterator itet = theIons.begin();
    for( itet = theIons.begin(); itet != theIons.end(); itet++ ) {
      G4cout << "SHRadDecayChainSet::CheckLifeTimes " << (*itet)->GetParticleName() << G4endl; 
    }
  }
#endif

  std::vector<G4Ions*>::const_iterator ite = theIons.begin();
  ite++;
  std::map<G4String,SHRadDecayChain*>::const_iterator ites;
  for( ; ite != theIonNames.end(); ite++ ) {
    // look for another ion chain with name equal to this ion
    std::map<G4String,SHRadDecayChain*> chains = theSet->GetChains();
    for( ites = chains.begin(); ites != chains.end(); ites++ ){
      if( (*ites).second->GetName() == *ite ) {
	theLifeTimes.push_back( (*ites).second->GetLifeTime(0) );
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(debugVerb) )  G4cout << " SHRadDecayChainSet::CheckLifeTimes() set LifeTime " << (*ites).second->GetLifeTime(0) << " N " << theLifeTimes.size() << G4endl; 
#endif
	break;
      }
    }
  }

  */
}

//-----------------------------------------------------------------
void SHRadDecayChain::SetNAtomsInJob( double npart )
{ 
  theNAtomsInJob.clear();
  std::set<double> times = theSet->GetTimes();
  if( times.size() != 0 ) {
    theNAtomsInJob[ (*(times.begin())) ] = npart;
  } else {
    theNAtomsInJob[ 0. ] = npart;
  }
}

//-----------------------------------------------------------------
void SHRadDecayChain::SetNAtomsInJob( double time, double npart )
{ 
  theNAtomsInJob[ time ] = npart;
		  
}
 
//-----------------------------------------------------------------
void SHRadDecayChain::AddBR( G4double newBR )
{
  theBR += newBR;
  theBRs[theBRs.size()-1] += newBR;
#ifndef GAMOS_NO_VERBOSE
	if( SHVerb(testVerb) )  G4cout << " SHRadDecayChain::AddBR " << theName << " " << theBR << " theBRs[" << theBRs.size()-1 << "] " << theBRs[theBRs.size()-1] << G4endl;
#endif

}

//-----------------------------------------------------------------
G4String SHRadDecayChain::GetSubName( G4int nIsot ) const
{
  size_t in = std::string::npos;
  for( G4int ii = 0; ii < nIsot; ii++ ){
    in = theName.find("->",in+1);
    if( in == std::string::npos ) {
      if( ii != nIsot-1 ) {
	G4Exception("SHRadDecayChain::GetSubName",
		    "Error",
		    FatalException,
		    ("Asking of subname with " + GmGenUtils::itoa(nIsot) + " in name " + theName).c_str());
      } else {
	in = theName.length();	
      }
    }
  }

  return theName.substr(0,in);

}

//-----------------------------------------------------------------
mdpdd* SHRadDecayChain::GetPlotData( G4double timeUnit )
{
  // time in nanoseconds and activity in decays/second
  mdpdd* plotData = new mdpdd;
  std::set<G4double>::const_iterator itet;
  std::set<double> times = theSet->GetTimes();
  for( itet = times.begin(); itet != times.end(); itet++ ) {
    G4double nAtomsPrev = GetNAtomsPrev( *itet );
    long double lambda = theInvLifeTimes[0];
    long double activity = nAtomsPrev * lambda *CLHEP::second; // activity is per second
    if( activity == 0. ) activity = 1.E-99;
    pdd pd(activity,0.);
    (*plotData)[*itet/timeUnit] = pd;
#ifndef GAMOS_NO_VERBOSE
    if( SHVerb(testVerb) ) G4cout << " SHRadDecayChain::GetPlotData add data " << *itet << " : " << pd.first << G4endl;
#endif

  }

#ifndef GAMOS_NO_VERBOSE
  if( SHVerb(debugVerb) ) G4cout << " SHRadDecayChain::GetPlotData " << plotData->size() << G4endl;
#endif

  return plotData;

}
