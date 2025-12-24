#include "GmVPSRBEPhenom.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ProcessManager.hh"
#include <cmath>

//--------------------------------------------------------------------
GmVPSRBEPhenom::GmVPSRBEPhenom(G4String name)
  :GmVPrimitiveScorer(name)
{
  theAlphaBetaX = GmParameterMgr::GetInstance()->GetNumericValue(name+":AlphaBetaX",-DBL_MAX);
  G4String theAlphaBetaXFN = GmParameterMgr::GetInstance()->GetStringValue(name+":AlphaBetaXFileName","");
  if( theAlphaBetaXFN != "" && theAlphaBetaX != -DBL_MAX ) {
    G4Exception("GmVPSRBEPhenom::GmVPSRBEPhenom",
		"",
		FatalException,
		("Cannot use parameters AlphaBetaX and AlphaBetaXFileName for the same scorer "+name).c_str());    
  }
  if ( theAlphaBetaXFN != "" ) {
    BuildAlphaBetaStructMap(theAlphaBetaXFN);
  } else {
    if ( theAlphaBetaX == -DBL_MAX ) {
      if ( bIsOnlyAlpha ) {
	theAlphaBetaX = GmParameterMgr::GetInstance()->GetNumericValue(name+":AlphaX",-DBL_MAX);
	if ( theAlphaBetaX == -DBL_MAX ) {
	  G4Exception("GmVPSRBEPhenom::GmVPSRBEPhenom",
		      "",
		      FatalException,
		      ("It is mandatory to set the parameter "+name+":AlphaX").c_str());    
	}
      } else {
	G4Exception("GmVPSRBEPhenom::GmVPSRBEPhenom",
		    "",
		    FatalException,
		    ("It is mandatory to set the parameter "+name+":AlphaBetaX").c_str());    
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmVPSRBEPhenom::GmVPSRBEPhenom AlphaBetaX / AlphaX " << theAlphaBetaX << G4endl;
#endif
  }
  
  bOnlyEndOfEvent = false;
  theDefaultAlphaBetaX = 3.;
}

//--------------------------------------------------------------------
void GmVPSRBEPhenom::BuildAlphaBetaStructMap(G4String alphaBetaXFN)
{
  theReadPhantomStMgr = GmReadPhantomStMgr::GetInstance();
  std::map<G4int,G4String> theStructs = theReadPhantomStMgr->GetStructs();
  if( theStructs.size() == 0 ) {
    G4Exception("GmVPSRBEPhenom::BuildAlphaBetaStructMap",
		"",
		FatalErrorInArgument,
		" No structure found in DICOM CT file");
  }
  
  std::map<G4String,G4double> alphaBetaStructNameMap;
  GmFileIn fin = GmFileIn::GetInstance(alphaBetaXFN,true);
  std::vector<G4String> wl;
  for(;;) {
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2 ) {
      for( size_t ii = 0; ii < wl.size(); ii++ ) {
	G4cerr << " WL " << wl[ii] << G4endl;
      }
      G4Exception("GmVPSRBEPhenom::BuildAlphaBetaStructMap",
		  "",
		  FatalException,
		  ("LINE IN "+alphaBetaXFN+" DOES NOT HAVE TWO WORDS").c_str());
    }
    
    G4String stName = "*"+wl[0]+"*";
    G4double ab = GmGenUtils::GetValue(wl[1]);
    // find a structure in CT file that corresponds to this name (*name*)
    for( auto ite = theStructs.begin(); ite != theStructs.end(); ite++ ) {
      G4int stid = ite->first;
      G4String stn = ite->second;
      G4cout << stid << " BuildAlphaBetaStructMap CHECKING stName " << stName << " stn " << stn << G4endl; //GDEB
      if( GmGenUtils::AreWordsEquivalent( stName, stn ) ) {
	theAlphaBetaStructIDMap[stid] = ab;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmVPSRBEPhenom::BuildAlphaBetaStructMap BuildAlphaBetaStructMap  ADD " << stid << " = " << ab << "  " << stName << " " << stn << G4endl; 
#endif	
      } else {
	G4String stNameU = stName;
	G4String stnU = stn;
	std::transform(stNameU.begin(), stNameU.end(), stNameU.begin(), [](unsigned char c){ return std::toupper(c); });
	std::transform(stnU.begin(), stnU.end(), stnU.begin(), [](unsigned char c){ return std::toupper(c); });
	if( GmGenUtils::AreWordsEquivalent( stNameU, stnU ) ) {
	  theAlphaBetaStructIDMap[stid] = ab;
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << " GmVPSRBEPhenom::BuildAlphaBetaStructMap BuildAlphaBetaStructMap  ADD " << stid << " = " << ab << "  " << stNameU << " " << stnU << G4endl; 
#endif	
	}
      }
    }

  }
}

//--------------------------------------------------------------------
GmVPSRBEPhenom::~GmVPSRBEPhenom()
{;}

//--------------------------------------------------------------------
G4double GmVPSRBEPhenom::GetRBE(G4Step* aStep )
{
  G4double dedx = GetElectronicDEDX(aStep, false);
  //  G4double dedx = aStep->GetTotalEnergyDeposit()/aStep->GetStepLength();
  const G4VTouchable* touch = aStep->GetPostStepPoint()->GetTouchable();
  G4int idx = touch->GetReplicaNumber();
  G4double alphaBetaX = GetAlphaBetaX(idx);

  G4double dose  = GetElectronicDose(aStep, true ); 
  G4double rbeMin = GetRBEMin(dedx,alphaBetaX);
  G4double rbeMax = GetRBEMax(dedx,alphaBetaX);

  G4double rbe = 1./dose*(sqrt(sqr(alphaBetaX)+4*dose*alphaBetaX*rbeMax+4*sqr(dose*rbeMin))-alphaBetaX);
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
  rbe *= weight;
  
  if( ScoringVerb(debugVerb) ) G4cout << " GmVPSRBEPhenom::GetRBE posZ " << aStep->GetPostStepPoint()->GetPosition().z() << " energy " << 
				   (aStep->GetPreStepPoint()->GetKineticEnergy()+aStep->GetPostStepPoint()->GetKineticEnergy())/2.
				       << " dedx " << dedx << " dose " << dose
				      << " rbeMin " << rbeMin << " rbeMax " << rbeMax << " weight " << weight << " alphaBetaX " << alphaBetaX << " rbe= " << rbe << G4endl;

  return rbe;
  
}

//--------------------------------------------------------------------
G4double GmVPSRBEPhenom::GetRBE(G4double dose, G4double LET, G4int idx ) // For EndOfEvent
{
  G4double alphaBetaX = GetAlphaBetaX(idx);
  G4double rbeMin = GetRBEMin(LET,alphaBetaX);
  G4double rbeMax = GetRBEMax(LET,alphaBetaX);

  G4double rbe = 1./(2*dose)*(sqrt(sqr(alphaBetaX)+4*dose*alphaBetaX*rbeMax+4*sqr(dose*rbeMin))-alphaBetaX);
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) {
    G4cout << " RBE= " <<  1./(2*dose)*(sqrt(sqr(alphaBetaX)+4*dose*alphaBetaX*rbeMax+4*sqr(dose*rbeMin))-alphaBetaX) << " " << " 1./"<<dose<<"*(sqrt(" << sqr(alphaBetaX) << "+4*"<<2*dose<<"*"<<alphaBetaX<<"*"<<rbeMax<<"+4*"<< sqr(dose*rbeMin)<<")-"<<alphaBetaX<<")" << G4endl; //GDEB
    G4cout << " RBE2= " << " 1./"<<2*dose<<"* "<<(sqrt(sqr(alphaBetaX)+4*dose*alphaBetaX*rbeMax+4*sqr(dose*rbeMin))-alphaBetaX) << "    sqrt.."<<sqrt(sqr(alphaBetaX)+4*dose*alphaBetaX*rbeMax+4*sqr(dose*rbeMin))<< "*(sqrt("<<sqr(alphaBetaX)<<" +"<<4*dose*alphaBetaX*rbeMax<<"+"<<4*sqr(dose*rbeMin)<<")-alphaBetaX"<< G4endl; //GDEB
  }
  if( ScoringVerb(debugVerb) ) G4cout << " GmVPSRBEPhenom::GetRBE(dose,LET) "
				      << " dose " << dose << " LET " << LET 
				      << " rbeMin " << rbeMin << " rbeMax " << rbeMax << " rbe= " << rbe << G4endl;
#endif
  
  return rbe;
  
}

//--------------------------------------------------------------------
G4double GmVPSRBEPhenom::GetAlphaBetaX( G4int copyNo )
{  
 if( theAlphaBetaStructIDMap.size() == 0 ) {
    return theAlphaBetaX;
  } else {
    std::set<size_t> StIDList = theReadPhantomStMgr->GetStIDList(copyNo);
    G4double ab = 0.;
    G4int nAB = 0;
    for( std::set<size_t>::const_iterator ite = StIDList.begin(); ite != StIDList.end(); ite++ ) {
      auto iteab = theAlphaBetaStructIDMap.find(*ite);
      if( iteab  != theAlphaBetaStructIDMap.end() ) {
	ab = iteab->second;
	nAB += 1;
	G4cout << " GmVPSRBEPhenom::GetAlphaBetaX ab= " << ab << " nAB " << nAB << G4endl; //GDEB
      }
    }
    if( ab == 0. ) {
      ab = theDefaultAlphaBetaX;
      //      for( std::set<size_t>::const_iterator ite = StIDList.begin(); ite != StIDList.end(); ite++ ) {
      //	G4cout << *ite << " GmVPSRBEPhenom::GetAlphaBetaX theDefaultAlphaBetaX ab= " << ab << G4endl; //GDEB
      // }
    } else {
      ab /= nAB;
    }
    return ab;
  }
}
  
