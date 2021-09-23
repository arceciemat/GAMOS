#include "GmGenerDistPositionDiscFromFile.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4GeometryTolerance.hh"

//-----------------------------------------------------------------------
GmGenerDistPositionDiscFromFile::GmGenerDistPositionDiscFromFile()
{

}

//-----------------------------------------------------------------------
void GmGenerDistPositionDiscFromFile::ReadPositionDist()
{
  // Read energy - probability  pairs
  theFileName = GmGenUtils::FileInPath( theFileName );

  GmFileIn fin = GmFileIn::GetInstance(theFileName);

  std::vector<G4String> wl;
  G4int ii = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2) {
      G4Exception("GmGenerDistPositionDiscFromFile::ReadPositionDist",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + theFileName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: ENERGY PROBABILITY").c_str());
    }
    theRadiusProb[ GmGenUtils::GetValue( wl[0] ) * theUnit ] = GmGenUtils::GetValue( wl[1] );
    //    G4cout << " ENERPROB  READ " <<  GmGenUtils::GetValue( wl[0] ) * theUnit << " =" <<  GmGenUtils::GetValue( wl[1] ) << G4endl;

    ii++;
  }

  if( theRadiusProb.size() == 0 ) {
      G4Exception("GmGenerDistPositionDiscFromFile::ReadPositionDist",
		  "Error in file",
		  FatalErrorInArgument,
		  G4String("No line found in file: " + theFileName).c_str());
  }

  /*  //--- For interpolate: add one bin at the end 
  if( theCalculationType == EFFCT_Interpolate || theCalculationType == EFFCT_InterpolateLog ){
    std::map<G4double,G4double>::const_iterator ite1 = theRadiusProb.end();
    ite1--;
    std::map<G4double,G4double>::const_iterator ite2 = theRadiusProb.end();
    ite2--; ite2--;
    theRadiusProb[(*ite1).first+ (*ite1).first-(*ite2).first] = 0.;//1.e-20;  // if =0., it find the last instead of the second to last
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDiscFromFile::ReadPositionDist add one bin for interpolate " << (*ite1).first+ (*ite1).first-(*ite2).first << " = 0. " << G4endl;
#endif
    }*/

  //----- Sanity checks
  std::map<G4double,G4double>::iterator ite, ite2;
  //--- For all: Check that energies are positive
  for(ite = theRadiusProb.begin(); ite != theRadiusProb.end(); ite++){
    if( (*ite).first < 0 ) {
	G4Exception("GmGenerDistPositionDiscFromFile::ReadPositionDist",
		    "Radiusgies should be positive",
		    FatalErrorInArgument,
		    G4String("Position = " + GmGenUtils::ftoa((*ite).first)).c_str());
    }
  }      
  //--- For all: Check that energy increases
  //  G4cout << " ENERPROB " << theRadiusProb.size() << G4endl;
  ite2 = theRadiusProb.begin(); ite2++;
  for(ite = theRadiusProb.begin(); ite2 != theRadiusProb.end(); ite++, ite2++){
    //    G4cout << " ENERPROB " << (*ite).first << " >= " << (*ite2).first << G4endl;
    if( (*ite).first >= (*ite2).first ) {
	G4Exception("GmGenerDistPositionDiscFromFile::ReadPositionDist",
		    "Radiusgies should be in increasing order",
		    FatalErrorInArgument,
		    G4String("Previous energy = " + GmGenUtils::ftoa((*ite).first) + " new energy = " + GmGenUtils::ftoa((*ite2).first)).c_str());
    }
  } 
  //--- For histogram: Check that the binning is fixed
  ite2 = theRadiusProb.begin(); ite2++;
  if( theCalculationType == EFFCT_Histogram ) {
    for(ite = theRadiusProb.begin(); ite2 != theRadiusProb.end(); ite++, ite2++){
      ite2 = ite; ite2++;
      if( ite == theRadiusProb.begin() ) {
	theHBin = (*ite2).first - (*ite).first;
      }
      G4double hbinn;
      if( theCalculationType == EFFCT_InterpolateLog ){
	hbinn = log((*ite2).first) - log((*ite).first);
      } else {
	hbinn = (*ite2).first - (*ite).first;
      }
      if( fabs(hbinn - theHBin) > 1.E-6*theHBin ) {
	G4Exception("GmGenerDistPositionDiscFromFile::ReadPositionDist",
		    "Asking for histogram type, and energy bin is not fixed",
		    FatalErrorInArgument,
		    G4String("Previous bin = " + GmGenUtils::ftoa(theHBin) + " new bin = " + GmGenUtils::ftoa(hbinn) + " difference= " + GmGenUtils::ftoa(hbinn-theHBin) ).c_str());
      }
    }
  }
  //--- For log interpolation: Check that energy = 0
  if( theCalculationType == EFFCT_InterpolateLog ) {
    if( (*(theRadiusProb.begin())).first == 0. ) {
      G4Exception("GmGenerDistPositionDiscFromFile::ReadPositionDist",
		  "Position cannot be 0 for logarithmic interpolation",
		  FatalErrorInArgument,
		  G4String("Position = " + GmGenUtils::ftoa((*ite).first)).c_str());
    } 
  }

  //--- Get the inverse of probabilities - energies distribution
  G4double tp = 0.;
  std::multimap<G4double,G4double> probaccumRadius0; // multimap <added up probability, energy> not normalized

  for(ite = theRadiusProb.begin(); ite != theRadiusProb.end(); ite++){
    G4double prob = 0.;
    //-    if( prob == 0. && std::distance( theRadiusProb.begin(), ite ) == G4int(theRadiusProb.size())-1 ) {
    //-      prob = 1.E-10; // for last bin limit the probability is set to 0. by convention
    //-    }
    if( theCalculationType == EFFCT_Fixed || 
	theCalculationType == EFFCT_Histogram ){
      prob = (*ite).second; 
      tp += prob;
      probaccumRadius0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
    } else if( theCalculationType == EFFCT_Interpolate ) {
      if( ite != theRadiusProb.begin() ) {
	ite2 = ite; ite2--;
	// get area between the two points
	prob = ((*ite).second+(*ite2).second)/2.*((*ite).first-(*ite2).first);
	tp += prob;
	probaccumRadius0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistPositionDiscFromFile  PROB " << prob << " " << (*ite).first << " " << (*ite).second << " " << (*ite2).second << " " << (*ite).first << " " << (*ite2).first << G4endl;
#endif
      }
    } else if( theCalculationType == EFFCT_InterpolateLog ) {
      if( ite != theRadiusProb.begin() ) {
	ite2 = ite; ite2--;
	prob = ((*ite).second+(*ite2).second)/2.*(log((*ite).first)-log((*ite2).first));
	tp += prob;
	probaccumRadius0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistPositionDiscFromFile  PROB " << prob << " " << (*ite).first << " " << (*ite).second << " " << (*ite2).second << " " << log((*ite).first) << " " << log((*ite2).first) << G4endl;
#endif
      }
    }
   
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionDiscFromFile  Reading data: Position =  " << (*ite).first 
				     << " Integrated probability= " << tp << G4endl;
#endif
  }

#ifndef GAMOS_NO_VERBOSE
  if( tp != 1. && GenerVerb(warningVerb) ) G4cerr << "GmGenerDistPositionDiscFromFile::ReadPositionDist probabilities do not sum up to 1., but to " << tp << " They will be normalized to 1. " << G4endl;
#endif
  std::multimap<G4double,G4double>::iterator itea;
  for( itea = probaccumRadius0.begin(); itea != probaccumRadius0.end(); itea++ ) {
    theProbaccumRadius.insert( std::multimap<G4double,G4double>::value_type((*itea).first / tp, (*itea).second));
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistPositionDiscFromFile  Reading data: Position =  " << (*itea).second 
				     << " Integrated normalized probability= " << (*itea).first/tp << G4endl;
#endif
  }

}

//-----------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDiscFromFile::GeneratePosition( const GmParticleSource* )
{
  G4double radius;
  G4double pv = CLHEP::RandFlat::shoot();
  std::multimap<G4double,G4double>::iterator ite = theProbaccumRadius.upper_bound( pv );
  if( ite == theProbaccumRadius.end() ) ite--; // precision problems if prob = 1.0000
  radius = (*ite).second;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistPositionDiscFromFile radius bin found =  " << radius << " prob = " << pv << " <prob " << (*ite).first << G4endl;
#endif
  /* Keep it in case other calculation types are requested
  if( theCalculationType != EFFCT_Fixed ){
    if( ite != theProbaccumRadius.end() ){
      if( ite != theProbaccumRadius.begin() ){
	ite--;
	ener1 = (*ite).second;
      } else {
	ener1 = 0.;
      }
      
    } else {
      //precision problem, it should not happen
      G4Exception("GmGenerDistPositionDiscFromFile::GeneratePosition",
		  "Probability too small, this should not happen",
		  FatalErrorInArgument,
		  G4String("probability is = " + GmGenUtils::ftoa(pv)).c_str());
    }
    }*/
  
  if( theCalculationType == EFFCT_Histogram ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistPositionDiscFromFile histogram prev radius =  " << radius << G4endl;
#endif
    radius += (-0.5 + CLHEP::RandFlat::shoot() ) * theHBin;
  } else if( theCalculationType == EFFCT_Interpolate ){
    //--- Get the slope of the interpolating line (radius corresponds to upper limit)
    std::map<G4double,G4double>::const_iterator iteU = theRadiusProb.find(radius);
    std::map<G4double,G4double>::const_iterator iteD = iteU; iteD--;
    G4double diffP = (*iteU).second-(*iteD).second;
    G4double diffE = (*iteU).first-(*iteD).first;
    G4double slope = diffP/diffE;
    G4double normCDF = slope*sqr(diffE)/2. + (*iteD).second*diffE ; // normalize Cumulative Distribution Function
    //    G4double randomProb = ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot()/ normCDF;
    G4double randomProb = CLHEP::RandFlat::shoot();
    radius = (( -(*iteD).second + sqrt( sqr((*iteD).second) + 2*slope*randomProb*normCDF) ) / slope ) + (*iteD).first;
    //    radius = ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot() / slope + (*iteD).first;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistPositionDiscFromFile radius =  " << radius 
				      << " randomProb " << randomProb
				      << " ener-prob D " << (*iteD).first << " " << (*iteD).second
				      << " ener-prob U " << (*iteU).first << " " << (*iteU).second
				      << " slope " << slope
				      << " normCDF " << normCDF << G4endl;
#endif
  } else if( theCalculationType == EFFCT_InterpolateLog ){
    //--- Get the slope of the interpolating line (radius corresponds to upper limit)
    std::map<G4double,G4double>::const_iterator iteU = theRadiusProb.find(radius);
    std::map<G4double,G4double>::const_iterator iteD = iteU; iteD--;
    G4double diffP = (*iteU).second-(*iteD).second;
    G4double diffE = log((*iteU).first)-log((*iteD).first);
    G4double slope = diffP/diffE;
    G4double normCDF = slope*sqr(diffE)/2. + (*iteD).second*diffE ; // normalize Cumulative Distribution Function
    G4double randomProb = CLHEP::RandFlat::shoot();
    radius = exp( (( -(*iteD).second + sqrt( sqr((*iteD).second) + 2*slope*randomProb*normCDF) ) / slope ) + log((*iteD).first) );
    //    radius = exp( ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot() / slope + log((*iteD).first) );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistPositionDiscFromFile radius =  " << radius 
				      << " randomProb " << randomProb
				      << " ener-prob D " << (*iteD).first << " " << (*iteD).second
				      << " ener-prob U " << (*iteU).first << " " << (*iteU).second
							     << " slope " << slope << G4endl;
  }

  if( radius < 0 ) {
    G4Exception("GmGenerDistPositionDiscFromFile::GeneratePosition",
		"Wrong distribution", 
		FatalErrorInArgument,
		G4String("Radius is negative : " + GmGenUtils::ftoa(radius) + " Please check your file distribution").c_str());
  }
  radius = fabs(radius);
  double phi = CLHEP::RandFlat::shoot() * 360.*CLHEP::deg;
  double x = radius * cos(phi);
  double y = radius * sin(phi);
  double z = 0.;
  G4ThreeVector pos( x, y, z);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDiscFromFile::Generate pos before rotation " << pos << G4endl;
#endif
  pos = theRotation * pos;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDiscFromFile::Generate pos before translation " << pos << G4endl;
#endif
  pos += theCentre;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionDiscFromFile::Generate pos " << pos << G4endl;
#endif
  return pos;

}

//---------------------------------------------------------------------
void GmGenerDistPositionDiscFromFile::SetParams( const std::vector<G4String>& params )
{
  
  if( params.size() != 1  && params.size() != 2 && params.size() != 5 && params.size() != 6 ) {
    G4Exception(" GmGenerDistPositionDisc::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 1, 2, 5 or 8 parameters: FILE_NAME (CALCULATION_TYPE) (POS_X POS_Y POS_Z) (DIR_X DIR_Y DIR_Z");
  }
  G4int nParams = params.size();
  theUnit = 1.;
  G4String calcType = "histogram";

  theFileName = params[0];
  if( nParams >= 2 ) {
    calcType = params[1];
  }
  if( nParams >= 5 ) { 
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[2] ), GmGenUtils::GetValue( params[3] ), GmGenUtils::GetValue( params[4] ) );
 }
  if( nParams >= 8 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[5] ), GmGenUtils::GetValue( params[6] ), GmGenUtils::GetValue( params[7] ) );
    SetRotation( dir );
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
    G4Exception("GmGenerDistPositionDiscFromFile::SetParams",
		"Error in type of calculation", 
		  FatalErrorInArgument,
		  G4String("only options supported: fixed , histogram, interpolate, interpolate_log,  you have selected " + calcType).c_str());
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDisc::SetParams filename " << theFileName << " calc type " << calcType << " translation " << theCentre << " rotation " << theRotation << G4endl;
#endif

  ReadPositionDist();

}

#endif
