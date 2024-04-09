#include "GmGenerDistEnergyFromFile.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//-----------------------------------------------------------------------
GmGenerDistEnergyFromFile::GmGenerDistEnergyFromFile()
{

}

//-----------------------------------------------------------------------
void GmGenerDistEnergyFromFile::ReadEnergyDist()
{
  // Read energy - probability  pairs
  theFileName = GmGenUtils::FileInPath( theFileName );

  GmFileIn fin = GmFileIn::GetInstance(theFileName);

  std::vector<G4String> wl;
  G4int ii = 1;
  //  G4cout << " GmGenerDistEnergyFromFile::ReadEnergyDist() START " << theFileName << G4endl; //GDEB
  for( ;; ){    
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2) {
      G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + theFileName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: ENERGY PROBABILITY").c_str());
    }
    theEnerProb[ GmGenUtils::GetValue( wl[0] ) * theUnit ] = GmGenUtils::GetValue( wl[1] );
    //    G4cout << ii << " ENERPROB  READ " <<  GmGenUtils::GetValue( wl[0] ) * theUnit << " =" <<  GmGenUtils::GetValue( wl[1] ) << G4endl; //GDEB

    ii++;
  }

  if( theEnerProb.size() == 0 ) {
      G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		  "Error in file",
		  FatalErrorInArgument,
		  G4String("No line found in file: " + theFileName).c_str());
  }

  /*  //--- For interpolate: add one bin at the end 
  if( theCalculationType == EFFCT_Interpolate || theCalculationType == EFFCT_InterpolateLog ){
    std::map<G4double,G4double>::const_iterator ite1 = theEnerProb.end();
    ite1--;
    std::map<G4double,G4double>::const_iterator ite2 = theEnerProb.end();
    ite2--; ite2--;
    theEnerProb[(*ite1).first+ (*ite1).first-(*ite2).first] = 0.;//1.e-20;  // if =0., it find the last instead of the second to last
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistEnergyFromFile::ReadEnergyDist add one bin for interpolate " << (*ite1).first+ (*ite1).first-(*ite2).first << " = 0. " << G4endl;
#endif
    }*/

  //----- Sanity checks
  std::map<G4double,G4double>::iterator ite, ite2;
  //--- For all: Check that energies are positive
  for(ite = theEnerProb.begin(); ite != theEnerProb.end(); ite++){
    if( (*ite).first < 0 ) {
	G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		    "Energies should be positive",
		    FatalErrorInArgument,
		    G4String("Energy = " + GmGenUtils::ftoa((*ite).first)).c_str());
    }
  }      
  //--- For all: Check that energy increases
  //  G4cout << " ENERPROB " << theEnerProb.size() << G4endl;
  ite2 = theEnerProb.begin(); ite2++;
  for(ite = theEnerProb.begin(); ite2 != theEnerProb.end(); ite++, ite2++){
    //    G4cout << " ENERPROB " << (*ite).first << " >= " << (*ite2).first << G4endl;
    if( (*ite).first >= (*ite2).first ) {
	G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		    "Energies should be in increasing order",
		    FatalErrorInArgument,
		    G4String("Previous energy = " + GmGenUtils::ftoa((*ite).first) + " new energy = " + GmGenUtils::ftoa((*ite2).first)).c_str());
    }
  } 
  //--- For histogram: Check that the binning is fixed
  ite2 = theEnerProb.begin(); ite2++;
  if( theCalculationType == EFFCT_Histogram ) {
    for(ite = theEnerProb.begin(); ite2 != theEnerProb.end(); ite++, ite2++){
      ite2 = ite; ite2++;
      if( ite == theEnerProb.begin() ) {
	theHBin = (*ite2).first - (*ite).first;
      }
      G4double hbinn;
      if( theCalculationType == EFFCT_InterpolateLog ){
	hbinn = log((*ite2).first) - log((*ite).first);
      } else {
	hbinn = (*ite2).first - (*ite).first;
      }
      if( fabs(hbinn - theHBin) > 1.E-6*theHBin ) {
	G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		    "Asking for histogram type, and energy bin is not fixed",
		    FatalErrorInArgument,
		    G4String("Previous bin = " + GmGenUtils::ftoa(theHBin) + " new bin = " + GmGenUtils::ftoa(hbinn) + " difference= " + GmGenUtils::ftoa(hbinn-theHBin) ).c_str());
      }
    }
  }
  //--- For log interpolation: Check that energy = 0
  if( theCalculationType == EFFCT_InterpolateLog ) {
    if( (*(theEnerProb.begin())).first == 0. ) {
      G4Exception("GmGenerDistEnergyFromFile::ReadEnergyDist",
		  "Energy cannot be 0 for logarithmic interpolation",
		  FatalErrorInArgument,
		  G4String("Energy = " + GmGenUtils::ftoa((*ite).first)).c_str());
    } 
  }

  //--- Get the inverse of probabilities - energies distribution
  G4double tp = 0.;
  std::multimap<G4double,G4double> probaccumEner0; // multimap <added up probability, energy> not normalized

  for(ite = theEnerProb.begin(); ite != theEnerProb.end(); ite++){
    G4double prob = 0.;
    //-    if( prob == 0. && std::distance( theEnerProb.begin(), ite ) == G4int(theEnerProb.size())-1 ) {
    //-      prob = 1.E-10; // for last bin limit the probability is set to 0. by convention
    //-    }
    if( theCalculationType == EFFCT_Fixed || 
	theCalculationType == EFFCT_Histogram ){
      prob = (*ite).second; 
      tp += prob;
      probaccumEner0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
    } else if( theCalculationType == EFFCT_Interpolate ) {
      if( ite != theEnerProb.begin() ) {
	ite2 = ite; ite2--;
	// get area between the two points
	prob = ((*ite).second+(*ite2).second)/2.*((*ite).first-(*ite2).first);
	tp += prob;
	probaccumEner0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile  PROB_SUM " << tp << " " << prob << " " << (*ite).first << " " << (*ite).second << " " << (*ite2).second << " " << (*ite).first << " " << (*ite2).first << G4endl;
#endif
      }
    } else if( theCalculationType == EFFCT_InterpolateLog ) {
      if( ite != theEnerProb.begin() ) {
	ite2 = ite; ite2--;
	prob = ((*ite).second+(*ite2).second)/2.*(log((*ite).first)-log((*ite2).first));
	tp += prob;
	probaccumEner0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile  PROB " << prob << " PROB_SUM " << tp << " " << (*ite).first << " " << (*ite).second << " " << (*ite2).second << " " << log((*ite).first) << " " << log((*ite2).first) << G4endl;
#endif
      }
    }
   
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistEnergyFromFile  Reading data: Energy =  " << (*ite).first 
				     << " Integrated probability= " << tp << G4endl;
#endif
  }

#ifndef GAMOS_NO_VERBOSE
  if( tp != 1. && GenerVerb(warningVerb) ) G4cerr << "GmGenerDistEnergyFromFile::ReadEnergyDist probabilities do not sum up to 1., but to " << tp << " They will be normalized to 1. " << G4endl;
#endif
  std::multimap<G4double,G4double>::iterator itea;
  for( itea = probaccumEner0.begin(); itea != probaccumEner0.end(); itea++ ) {
    theProbaccumEner.insert( std::multimap<G4double,G4double>::value_type((*itea).first / tp, (*itea).second));
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistEnergyFromFile  Reading data: Energy =  " << (*itea).second 
				     << " Integrated normalized probability= " << (*itea).first/tp << G4endl;
#endif
  }

}

//-----------------------------------------------------------------------
G4double GmGenerDistEnergyFromFile::GenerateEnergy( const GmParticleSource* )
{
  G4double energy;
  G4double pv = CLHEP::RandFlat::shoot();
  std::multimap<G4double,G4double>::iterator ite = theProbaccumEner.upper_bound( pv );
  if( ite == theProbaccumEner.end() ) ite--; // precision problems if prob = 1.0000
  energy = (*ite).second;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile energy bin found =  " << energy << " prob = " << pv << " <prob " << (*ite).first << G4endl;
#endif
  /* Keep it in case other calculation types are requested
  if( theCalculationType != EFFCT_Fixed ){
    if( ite != theProbaccumEner.end() ){
      if( ite != theProbaccumEner.begin() ){
	ite--;
	ener1 = (*ite).second;
      } else {
	ener1 = 0.;
      }
      
    } else {
      //precision problem, it should not happen
      G4Exception("GmGenerDistEnergyFromFile::GenerateEnergy",
		  "Probability too small, this should not happen",
		  FatalErrorInArgument,
		  G4String("probability is = " + GmGenUtils::ftoa(pv)).c_str());
    }
    }*/
  
  if( theCalculationType == EFFCT_Histogram ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile histogram prev energy =  " << energy << G4endl;
#endif
    energy += (-0.5 + CLHEP::RandFlat::shoot() ) * theHBin;
  } else if( theCalculationType == EFFCT_Interpolate ){
    //--- Get the slope of the interpolating line (energy corresponds to upper limit)
    std::map<G4double,G4double>::const_iterator iteU = theEnerProb.find(energy);
    std::map<G4double,G4double>::const_iterator iteD = iteU; iteD--;
    G4double diffP = (*iteU).second-(*iteD).second;
    G4double diffE = (*iteU).first-(*iteD).first;
    G4double slope = diffP/diffE;
    G4double normCDF = slope*sqr(diffE)/2. + (*iteD).second*diffE ; // normalize Cumulative Distribution Function
    //    G4double randomProb = ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot()/ normCDF;
    G4double randomProb = CLHEP::RandFlat::shoot();
    if(slope == 0) {
      energy = (*iteD).first + randomProb* ((*iteU).first-(*iteD).first);
    } else {
      energy = (( -(*iteD).second + sqrt( sqr((*iteD).second) + 2*slope*randomProb*normCDF) ) / slope ) + (*iteD).first;
    }
    //    energy = ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot() / slope + (*iteD).first;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile energy =  " << energy 
				      << " randomProb " << randomProb
				      << " ener-prob D " << (*iteD).first << " " << (*iteD).second
				      << " ener-prob U " << (*iteU).first << " " << (*iteU).second
				      << " slope " << slope
				      << " normCDF " << normCDF << G4endl;
#endif
  } else if( theCalculationType == EFFCT_InterpolateLog ){
    //--- Get the slope of the interpolating line (energy corresponds to upper limit)
    std::map<G4double,G4double>::const_iterator iteU = theEnerProb.find(energy);
    std::map<G4double,G4double>::const_iterator iteD = iteU; iteD--;
    G4double diffP = (*iteU).second-(*iteD).second;
    G4double diffE = log((*iteU).first)-log((*iteD).first);
    G4double slope = diffP/diffE;
    G4double normCDF = slope*sqr(diffE)/2. + (*iteD).second*diffE ; // normalize Cumulative Distribution Function
    G4double randomProb = CLHEP::RandFlat::shoot();
    if(slope == 0) {
      energy = exp( log((*iteD).first) + randomProb* (log((*iteU).first)-log((*iteD).first)) );
    } else {
     energy = exp( (( -(*iteD).second + sqrt( sqr((*iteD).second) + 2*slope*randomProb*normCDF) ) / slope ) + log((*iteD).first) );
    }
     //    energy = exp( ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot() / slope + log((*iteD).first) );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyFromFile energy =  " << energy 
				      << " randomProb " << randomProb
				      << " ener-prob D " << (*iteD).first << " " << (*iteD).second
				      << " ener-prob U " << (*iteU).first << " " << (*iteU).second
							     << " slope " << slope << G4endl;
  }

  return energy;

}

//---------------------------------------------------------------------
void GmGenerDistEnergyFromFile::SetParams( const std::vector<G4String>& params )
{

  G4int nParams = params.size();
  theUnit = 1.;
  G4String calcType = "histogram";
  //  G4cout << "File PARAMS " << params[0] << " " << params[1] << G4endl; //GDEB
    
  switch (nParams) {
  case 3:
    theUnit = GmGenUtils::GetValue(params[2]);
#ifndef WIN32
    [[fallthrough]];
#endif
  case 2:
    calcType = params[1];
#ifndef WIN32
    [[fallthrough]];
#endif
  case 1:
    theFileName = params[0];
    break;
  default:
    G4Exception("GmGenerDistEnergyFromFile::SetParams",
		"Wrong number of parameters",
		FatalErrorInArgument,
		G4String("There should be 1, 2 or 3 parameters: FILE_NAME CALCULATION_TYPE UNIT, there are " + GmGenUtils::itoa(nParams)).c_str());
  }

  //  G4cout << this << " calcType " << calcType << "==" << theCalculationTypeOld << " filename " <<theFileName << "==" << params[0] << G4endl; //GDEB
  
  if( calcType == theCalculationTypeOld && theFileName == params[0] ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistEnergyFromFile::SetParams: same parameters as last time, do not read file again " << G4endl;
#endif
    return;
  }
    
  if( calcType == "fixed" ) {
    theCalculationType = EFFCT_Fixed;
  } else if( calcType == "histogram" ) {
    theCalculationType = EFFCT_Histogram;
  } else if( calcType == "interpolate" ) {
    theCalculationType = EFFCT_Interpolate;
  } else if( calcType == "interpolate_log" ) {
    theCalculationType = EFFCT_InterpolateLog;
  } else {
    G4Exception("GmGenerDistEnergyFromFile::SetParams",
		"Error in type of calculation", 
		  FatalErrorInArgument,
		  G4String("only options supported: fixed , histogram, interpolate, interpolate_log,  you have selected " + calcType).c_str());
  }
  theCalculationTypeOld = calcType;
  
  ReadEnergyDist();

}

#endif
