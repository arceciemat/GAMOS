#include "GmGenerDistDirectionThetaFromFile.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Units/SystemOfUnits.h"

//-----------------------------------------------------------------------
GmGenerDistDirectionThetaFromFile::GmGenerDistDirectionThetaFromFile()
{

}

//-----------------------------------------------------------------------
void GmGenerDistDirectionThetaFromFile::ReadThetaDist()
{
  // Read theta - probability  pairs
  theFileName = GmGenUtils::FileInPath( theFileName );

  GmFileIn fin = GmFileIn::GetInstance(theFileName);

  std::vector<G4String> wl;
  G4int ii = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2) {
      G4Exception("GmGenerDistDirectionThetaFromFile::ReadThetaDist",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + theFileName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: THETA PROBABILITY").c_str());
    }
    theThetaProb[ GmGenUtils::GetValue( wl[0] ) * theUnit ] = GmGenUtils::GetValue( wl[1] );
    //    G4cout << " THETAPROB  READ " <<  GmGenUtils::GetValue( wl[0] ) * theUnit << " =" <<  GmGenUtils::GetValue( wl[1] ) << G4endl; //GDEB

    ii++;
  }

  if( theThetaProb.size() == 0 ) {
      G4Exception("GmGenerDistDirectionThetaFromFile::ReadThetaDist",
		  "Error in file",
		  FatalErrorInArgument,
		  G4String("No line found in file: " + theFileName).c_str());
  }

  //----- Sanity checks
  std::map<G4double,G4double>::iterator ite, ite2;
  //--- For all: Check that thetas are positive
  for(ite = theThetaProb.begin(); ite != theThetaProb.end(); ite++){
    if( (*ite).first < 0 ) {
	G4Exception("GmGenerDistDirectionThetaFromFile::ReadThetaDist",
		    "Thetas should be positive",
		    FatalErrorInArgument,
		    G4String("Theta = " + GmGenUtils::ftoa((*ite).first)).c_str());
    }
  }      
  //--- For all: Check that theta increases
  //  G4cout << " THETAPROB " << theThetaProb.size() << G4endl;
  ite2 = theThetaProb.begin(); ite2++;
  for(ite = theThetaProb.begin(); ite2 != theThetaProb.end(); ite++, ite2++){
    //    G4cout << " THETAPROB " << (*ite).first << " >= " << (*ite2).first << G4endl;
    if( (*ite).first >= (*ite2).first ) {
	G4Exception("GmGenerDistDirectionThetaFromFile::ReadThetaDist",
		    "Thetas should be in increasing order",
		    FatalErrorInArgument,
		    G4String("Previous theta = " + GmGenUtils::ftoa((*ite).first) + " new theta = " + GmGenUtils::ftoa((*ite2).first)).c_str());
    }
  } 
  //--- For histogram: Check that the binning is fixed
  ite2 = theThetaProb.begin(); ite2++;
  if( theCalculationType == EFFCT_Histogram ) {
    for(ite = theThetaProb.begin(); ite2 != theThetaProb.end(); ite++, ite2++){
      ite2 = ite; ite2++;
      if( ite == theThetaProb.begin() ) {
	theHBin = (*ite2).first - (*ite).first;
      }
      G4double hbinn;
      if( theCalculationType == EFFCT_InterpolateLog ){
	hbinn = log((*ite2).first) - log((*ite).first);
      } else {
	hbinn = (*ite2).first - (*ite).first;
      }
      if( fabs(hbinn - theHBin) > 1.E-6*theHBin ) {
	G4Exception("GmGenerDistDirectionThetaFromFile::ReadThetaDist",
		    "Asking for histogram type, and theta bin is not fixed",
		    FatalErrorInArgument,
		    G4String("Previous bin = " + GmGenUtils::ftoa(theHBin) + " new bin = " + GmGenUtils::ftoa(hbinn) + " difference= " + GmGenUtils::ftoa(hbinn-theHBin) ).c_str());
      }
    }
  }
  //--- For log interpolation: Check that theta = 0
  if( theCalculationType == EFFCT_InterpolateLog ) {
    if( (*(theThetaProb.begin())).first == 0. ) {
      G4Exception("GmGenerDistDirectionThetaFromFile::ReadThetaDist",
		  "Theta cannot be 0 for logarithmic interpolation",
		  FatalErrorInArgument,
		  G4String("Theta = " + GmGenUtils::ftoa((*ite).first)).c_str());
    } 
  }

  //--- Get the inverse of probabilities - thetas distribution
  G4double tp = 0.;
  std::multimap<G4double,G4double> probaccumTheta0; // multimap <added up probability, theta> not normalized

  for(ite = theThetaProb.begin(); ite != theThetaProb.end(); ite++){
    G4double prob = 0.;
    //-    if( prob == 0. && std::distance( theThetaProb.begin(), ite ) == G4int(theThetaProb.size())-1 ) {
    //-      prob = 1.E-10; // for last bin limit the probability is set to 0. by convention
    //-    }
    if( theCalculationType == EFFCT_Fixed || 
	theCalculationType == EFFCT_Histogram ){
      prob = (*ite).second; 
      tp += prob;
      probaccumTheta0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
    } else if( theCalculationType == EFFCT_Interpolate ) {
      if( ite != theThetaProb.begin() ) {
	ite2 = ite; ite2--;
	// get area between the two points
	prob = ((*ite).second+(*ite2).second)/2.*((*ite).first-(*ite2).first);
	tp += prob;
	probaccumTheta0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile  PROB " << prob << " " << (*ite).first << " " << (*ite).second << " " << (*ite2).second << " " << (*ite).first << " " << (*ite2).first << G4endl;
#endif
      }
    } else if( theCalculationType == EFFCT_InterpolateLog ) {
      if( ite != theThetaProb.begin() ) {
	ite2 = ite; ite2--;
	prob = ((*ite).second+(*ite2).second)/2.*(log((*ite).first)-log((*ite2).first));
	tp += prob;
	probaccumTheta0.insert( std::multimap<G4double,G4double>::value_type(tp, (*ite).first));
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile  PROB " << prob << " " << (*ite).first << " " << (*ite).second << " " << (*ite2).second << " " << log((*ite).first) << " " << log((*ite2).first) << G4endl;
#endif
      }
    }
   
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile  Reading data: Theta =  " << (*ite).first 
				     << " Integrated probability= " << tp << G4endl;
#endif
  }

#ifndef GAMOS_NO_VERBOSE
  if( tp != 1. && GenerVerb(warningVerb) ) G4cerr << "GmGenerDistDirectionThetaFromFile::ReadThetaDist probabilities do not sum up to 1., but to " << tp << " They will be normalized to 1. " << G4endl;
#endif
  std::multimap<G4double,G4double>::iterator itea;
  for( itea = probaccumTheta0.begin(); itea != probaccumTheta0.end(); itea++ ) {
    theProbaccumTheta.insert( std::multimap<G4double,G4double>::value_type((*itea).first / tp, (*itea).second));
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(infoVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile  Normalized Theta =  " << (*itea).second 
				     << " Integrated normalized probability= " << (*itea).first/tp << G4endl;
#endif
  }

}

//-----------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionThetaFromFile::GenerateDirection( GmParticleSource* )
{
  G4double theta;
  G4double pv = CLHEP::RandFlat::shoot();
  std::multimap<G4double,G4double>::iterator ite = theProbaccumTheta.upper_bound( pv );
  if( ite == theProbaccumTheta.end() ) ite--; // precision problems if prob = 1.0000
  theta = (*ite).second;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile theta bin found =  " << theta << " prob = " << pv << " <prob " << (*ite).first << G4endl;
#endif
  if( theCalculationType == EFFCT_Histogram ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile histogram prev theta =  " << theta << G4endl;
#endif
    theta += (-0.5 + CLHEP::RandFlat::shoot() ) * theHBin;
  } else if( theCalculationType == EFFCT_Interpolate ){
    //--- Get the slope of the interpolating line (theta corresponds to upper limit)
    std::map<G4double,G4double>::const_iterator iteU = theThetaProb.find(theta);
    std::map<G4double,G4double>::const_iterator iteD = iteU; iteD--;
    G4double diffP = (*iteU).second-(*iteD).second;
    G4double diffE = (*iteU).first-(*iteD).first;
    G4double slope = diffP/diffE;
    G4double normCDF = slope*sqr(diffE)/2. + (*iteD).second*diffE ; // normalize Cumulative Distribution Function
    //    G4double randomProb = ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot()/ normCDF;
    G4double randomProb = CLHEP::RandFlat::shoot();
    if(slope == 0) {
      theta = (*iteD).first + randomProb* ((*iteU).first-(*iteD).first);
    } else {
      theta = (( -(*iteD).second + sqrt( sqr((*iteD).second) + 2*slope*randomProb*normCDF) ) / slope ) + (*iteD).first;
    }
    //    theta = ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot() / slope + (*iteD).first;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile theta =  " << theta 
				      << " randomProb " << randomProb
				      << " theta-prob D " << (*iteD).first << " " << (*iteD).second
				      << " theta-prob U " << (*iteU).first << " " << (*iteU).second
				      << " slope " << slope
				      << " normCDF " << normCDF << G4endl;
#endif
  } else if( theCalculationType == EFFCT_InterpolateLog ){
    //--- Get the slope of the interpolating line (theta corresponds to upper limit)
    std::map<G4double,G4double>::const_iterator iteU = theThetaProb.find(theta);
    std::map<G4double,G4double>::const_iterator iteD = iteU; iteD--;
    G4double diffP = (*iteU).second-(*iteD).second;
    G4double diffE = log((*iteU).first)-log((*iteD).first);
    G4double slope = diffP/diffE;
    G4double normCDF = slope*sqr(diffE)/2. + (*iteD).second*diffE ; // normalize Cumulative Distribution Function
    G4double randomProb = CLHEP::RandFlat::shoot();
    if(slope == 0) {
      theta = exp( log((*iteD).first) + randomProb* (log((*iteU).first)-log((*iteD).first)) );
    } else {
     theta = exp( (( -(*iteD).second + sqrt( sqr((*iteD).second) + 2*slope*randomProb*normCDF) ) / slope ) + log((*iteD).first) );
    }
     //    theta = exp( ((*iteU).second-(*iteD).second) * CLHEP::RandFlat::shoot() / slope + log((*iteD).first) );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistDirectionThetaFromFile theta =  " << theta 
				      << " randomProb " << randomProb
				      << " theta-prob D " << (*iteD).first << " " << (*iteD).second
				      << " theta-prob U " << (*iteU).first << " " << (*iteU).second
							     << " slope " << slope << G4endl;
  }

  //--- Get direction 
  G4ThreeVector newDir = theInitialDir; 
  newDir.rotate( theta, thePerpDir );
#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionThetaFromFile::Generate:  dir after theta rotation " << newDir << " theta " << theta << G4endl;
#endif

  //---- Get phi angle around newDir
  G4double phi = CLHEP::RandFlat::shoot()*2.*CLHEP::pi;
  newDir.rotate( phi , theInitialDir );

#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionThetaFromFile::Generate:  final dir " << newDir << G4endl;
#endif

  return newDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionThetaFromFile::SetParams( const std::vector<G4String>& params )
{

  G4int nParams = params.size();
  theUnit = 1.;
  G4String calcType = "histogram";

  switch (nParams) {
  case 6:
     theUnit = GmGenUtils::GetValue( params[5] );
#ifndef WIN32
     [[fallthrough]];
#endif
  case 5:
    theInitialDir = G4ThreeVector(GmGenUtils::GetValue( params[2] ), GmGenUtils::GetValue( params[3] ), GmGenUtils::GetValue( params[4] ) );
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
    G4Exception("GmGenerDistDirectionThetaFromFile::SetParams",
		"Wrong number of parameters", 
		FatalErrorInArgument,
		G4String("There should be 1, 2, 5 or 6 parameters: FILE_NAME CALCULATION_TYPE AXIS_DIR_X AXIS_DIR_Y AXIS_DIR_Z UNIT, there are " + GmGenUtils::itoa(nParams)).c_str());
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
    G4Exception("GmGenerDistDirectionThetaFromFile::SetParams",
		"Error in type of calculation", 
		  FatalErrorInArgument,
		  G4String("only options supported: fixed , histogram, interpolate, interpolate_log,  you have selected " + calcType).c_str());
  }
 
  //----- Get one perpendicular direction
  G4ThreeVector dir(1.,0.,0.);

  if( fabs(fabs(theInitialDir*dir) - 1.) < 1.E-9 ){
    dir = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theInitialDir.cross(dir);

  ReadThetaDist();

}

#endif
