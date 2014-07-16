#include "GmPDSInteractionAngleManager.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include <vector>
#ifndef GAMOS_NO_ROOT
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#endif

//------------------------------------------------------------------
GmPDSInteractionAngleManager::GmPDSInteractionAngleManager( const G4String& filename )
{
  if( filename == "" ) return;

#ifndef GAMOS_NO_ROOT
  //  TFile* hFile = new TFile( filename );
  TFile hFile( filename );

  G4int minimumNumberOfEntries = GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:InteractionAngleManager::MinimumNumberOfEntries",1000);

  if( !hFile.IsOpen() ){
    G4Exception("GmPDSInteractionAngleManager::GmPDSInteractionAngleManager",
		"File not found",
		FatalErrorInArgument,
		G4String("FILE = " + filename ).c_str());
  }

  TIter ListOfHistograms( hFile.GetListOfKeys() );
  TKey *HistoKey;
  TH1F* histo;

  std::map<G4double, std::vector<G4double> > enerEnt;
  G4int nh = 0;
  while ( (HistoKey = (TKey*)ListOfHistograms()) ) {    
    //    if( nh == 10 ) break;
    histo = (TH1F*) hFile.Get( HistoKey->GetName() );
    std::string hisname = histo->GetName();
    if( histo->GetEntries() < minimumNumberOfEntries ) continue;
    nh++;
    if( int(hisname.find("cos Deviation Angle")) != -1 ) {
      int ic1 = hisname.find(":");
      int ic2 = hisname.find(":",ic1+1);
      G4String procname = hisname.substr(ic1+2,ic2-ic1-3);
      int ic3 = hisname.find(":",ic2+1);
      G4String matname = hisname.substr(ic2+2,ic3-ic2-3);
      int ic4 = hisname.find(":",ic3+1);
      G4String enername = hisname.substr(ic3+2,ic4-ic3-3);
      G4double ener = atof(enername);
      //      G4cout << " GmPDSInteractionAngleManager " << hisname << " " << ic1 << " " << ic2 << " proc= " << procname << " mat= " << matname << " ener= " << ener << G4endl;
      //   G4cout << " enername" << enername << "ZZ" << G4endl;

      //----- histo variables
      G4double nent = histo->GetEntries();
      G4double xmin = histo->GetXaxis()->GetXmin();
      G4double xmax = histo->GetXaxis()->GetXmax();
      G4int nbins = histo->GetXaxis()->GetNbins();
      G4double binstep = (xmax-xmin)/nbins;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << histo->GetName() << " xmin " << xmin << " xmax " << xmax << " binstep " << theBinStep << " nbins" << nbins << G4endl;
#endif
      if( nh != 1 ) {
	if( xmin != theXmin || xmax != theXmax || binstep != theBinStep ){
	  G4Exception( "GmPDSInteractionAngleManager::GmPDSInteractionAngleManager",
                       "Wrong argument",
                       FatalErrorInArgument,
                       "Different histogram dimensions ");
 	}
      } else {
	theXmin = xmin; 
	theXmax = xmax;
	theBinStep = binstep;
      }
      std::vector<G4double> hent;
      for( int ii = 1; ii <= nbins; ii++ ){
	if( nent != 0. ){
	  hent.push_back( histo->GetBinContent(ii) / nent );
	} else {
	  hent.push_back( 0. );
	}
      }
      theEnergies[log10(ener)] = enername;
      theHistoEntries[hisname.substr(0,ic4-1)] = hent;
#ifndef GAMOS_NO_VERBOSE
      //      G4cout << ScoringVerb(3) << " GmPDSInteractionAngleManager histo entries =  " << hisname.substr(0,ic3-1) << G4endl;
#endif
    }

    //    delete hFile;
  }
#else
  G4Exception("GmPDSInteractionAngleManager::GmPDSInteractionAngleManager",
              "Wrong argument",
              FatalErrorInArgument,
              "Only works currently for ROOT histograms, please undefine GAMOS_NO_ROOT variable");
#endif  
}

//------------------------------------------------------------------
G4double GmPDSInteractionAngleManager::GetHistoValue(const G4String& procname, const G4String& matname, G4double ener, const G4double cosang )
{
  if( theEnergies.size() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cerr << " GmPDSInteractionAngleManager::GetHistoValue:  no histogram found with enough entries, RETURNING 0.5 " << G4endl;
#endif
    return 0.5;
  }

  ener = log10( ener );
  std::map<G4double,G4String>::iterator ite1 = GetEnergyRange( ener );
  std::map<G4double,G4String>::iterator ite2 = ite1; ite2++;

  G4String hisname = procname + " : " + matname + " : " + (*ite1).second;
  //  G4cout << " histo " << hisname << " ZZ " << procname << " ZZ " << matname << " ZZ " << (*ite1).second << G4endl; 
  G4int nbin = int((cosang-theXmin)/theBinStep);
  //  G4cout << "NBIN " << nbin << " " << (cosang-theXmin) << " " << cosang << " - " << theXmin << " / " << theBinStep << G4endl;

  std::map<G4String, std::vector<G4double> >::iterator itee = theHistoEntries.find( hisname );
  if( itee == theHistoEntries.end() ){
    /*    for( itee = theHistoEntries.begin(); itee != theHistoEntries.end(); itee++) {
      G4cerr << " HISTO: " << (*itee).first << "ZZ" << G4endl;
      }*/
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cerr << " GmPDSInteractionAngleManager::GetHistoValue:   histo not found: " << hisname << " RETURNING 0.5 " << G4endl;
#endif
    return 0.5;
    //    G4Exception( "GmPDSInteractionAngleManager::GetHistoValue histo nof found: " + hisname );
  }

  G4double binCEN1 = ((*itee).second)[nbin];

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInteractionAngleManager::GetHistoValue:  hisname1 " << hisname << " nbin " << nbin << " = " << binCEN1 << "  ENERS " << ener << " " << (*ite1).first << " " << (*ite2).first << G4endl;
#endif

  G4double binCEN2 = -9999.;
  //t should get interpolation   G4double binPOS = theHistoEntries[hisname][nbin];
  if( ite2 != theEnergies.end() ){
    hisname = procname + " : " + matname + " : " + (*ite2).second;
    itee = theHistoEntries.find( hisname );
    if( itee != theHistoEntries.end() ){
      //  G4cout << " GmPDSInteractionAngleManager::GetHistoValue(  hisname2 " << hisname << G4endl;
      binCEN2 = theHistoEntries[hisname][nbin];
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInteractionAngleManager::GetHistoValue:  hisname2 " << hisname << " nbin " << nbin << " = " << binCEN2 << G4endl;
#endif
    }
  }

  if( binCEN2 != -9999. ) {
    G4double diffE = (*ite2).first - (*ite1).first;
    G4double val = ( binCEN1 * ( ener - (*ite1).first ) + binCEN2 * ( (*ite2).first - ener ) ) / (diffE*theBinStep);
    //    G4cout << " VAL " << val << " " <<  ( binCEN1 * fabs( ener - (*ite1).first ) + binCEN2 * fabs( ener - (*ite2).first ) ) << " " << binCEN1 << " * " << fabs( ener - (*ite1).first ) << " + " << binCEN2 << " * " << fabs( ener - (*ite2).first ) << " / " << theBinStep << G4endl;
    if( val < 0 ) G4Exception("GmPDSInteractionAngleManager::GetHistoValue",
			      "Error",
			      FatalException,
			      "NEGATIVE ANGLE PROBABILITY, SHOULD NOT HAPPEN");
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInteractionAngleManager::GetHistoValue " << val << " proc " << procname << " mat " << matname << " ener " << ener << " cosang " << cosang << " binstep " << theBinStep << " ENERS " <<  ener <<  " " << (*ite1).first << " " << (*ite2).first <<  G4endl;
#endif
    return val;
//    return ( binCEN1 * fabs( ener - (*ite1).first ) +  binCEN2 * fabs( ener - (*ite2).first ) ) / theBinStep;
  } else {
    return  binCEN1;
  }

}


//------------------------------------------------------------------
std::map<G4double,G4String>::iterator GmPDSInteractionAngleManager::GetEnergyRange( G4double ener )
{
  std::map<G4double,G4String>::iterator ite;
  for( ite = theEnergies.begin(); ite != theEnergies.end(); ite++ ){
    //    G4cout << "GetEnergyRange ener " << ener << " < " << (*ite).first << G4endl;
    if( ener < (*ite).first ) { 
      //  G4cout << "GetEnergyRange " << (*ite).first << " = " << (*ite).second << " from ener " << ener << G4endl;
      ite--;
      break;
    }
  }

  if( ite == theEnergies.end() ) {
    //    G4Exception("GmPDSInteractionAngleManager::GetEnergyRange Energy is bigger than maximum energy" + GmGenUtils::ftoa( (*(--ite)).first ) );
    ite--;
  } 
  
  return ite;

}
