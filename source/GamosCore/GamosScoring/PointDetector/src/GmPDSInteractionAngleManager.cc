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

  G4int minimumNumberOfEntries = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:InteractionAngleManager:MinimumNumberOfEntries",1000));

  if( !hFile.IsOpen() ){
    G4Exception("GmPDSInteractionAngleManager::GmPDSInteractionAngleManager",
		"File not found",
		FatalErrorInArgument,
		G4String("FILE = " + filename ).c_str());
  }

  bEnergyLog = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:InteractionAngleManager:EnergyLog",1));

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
      //      G4String procname = hisname.substr(ic1+2,ic2-ic1-3);
      G4String procname = hisname.substr(0,ic2-1);
      int ic3 = hisname.find(":",ic2+1);
      G4String matname = hisname.substr(ic2+2,ic3-ic2-3);
      int ic4 = hisname.find(":",ic3+1);
      G4String enername = hisname.substr(ic3+2,ic4-ic3-3);
      G4double ener = atof(enername);
      //      G4cout << " GmPDSInteractionAngleManager " << hisname << " " << ic1 << " " << ic2 << " proc= " << procname << " mat= " << matname << " ener= " << ener << G4endl;
      //      G4cout << " enername" << enername << "ZZ" << G4endl;

      //----- histo variables
      G4double nent = histo->Integral();
      G4double xmin = histo->GetXaxis()->GetXmin();
      G4double xmax = histo->GetXaxis()->GetXmax();
      G4int nbins = histo->GetXaxis()->GetNbins();
      G4double binstep = (xmax-xmin)/nbins;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << histo->GetName() << " xmin " << xmin << " xmax " << xmax << " binstep " << theBinStep << " nbins" << nbins << " integral " << nent << G4endl;
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
      std::map<G4double,G4String>* energies;
      msmds::const_iterator itepe = theEnergies.find(procname);
      if( itepe == theEnergies.end() ) {
	theEnergies[procname] = new std::map<G4double,G4String>;
	if( ScoringVerb(infoVerb) ) G4cout << " probE filling procname " << procname << G4endl;
	itepe = theEnergies.find(procname);
      } 
      energies = (*itepe).second;
      if( bEnergyLog ) {
	(*energies)[log10(ener)] = enername;
      } else {
	(*energies)[ener] = enername;
      }

      theHistoEntries[hisname.substr(0,ic4-1)] = hent;
#ifndef GAMOS_NO_VERBOSE
       if( ScoringVerb(infoVerb) )G4cout << " GmPDSInteractionAngleManager histo entries =  " << hisname.substr(0,ic3-1) << G4endl;
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
  msmds::const_iterator itepe = theEnergies.find(procname);
  if( itepe == theEnergies.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) {
      G4cerr << " GmPDSInteractionAngleManager::GetHistoValue:  no histogram found with enough entries, RETURNING 0.5 " << procname << " " << theEnergies.size() << G4endl;
      for( itepe = theEnergies.begin() ; itepe != theEnergies.end(); itepe++ ) {
	G4cerr << " ENERGIES " << (*itepe).first << " = " << procname << G4endl;
      }
    }
#endif
    return 0.5;
  }
  
  std::map<G4double,G4String>* energies = (*itepe).second;
  if( energies->size() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cerr << " GmPDSInteractionAngleManager::GetHistoValue:  no histogram found with enough entries, RETURNING 0.5 " << energies->size() << G4endl;
#endif
    return 0.5;
  }

  std::map<G4double,G4String>::iterator ite1 = GetEnergyRange( energies, ener );
  std::map<G4double,G4String>::iterator ite2 = ite1; ite2++;

  G4String hisname = procname + " : " + matname + " : " + (*ite1).second;
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

  G4double binCEN2 = -9999.;
  //t should get interpolation   G4double binPOS = theHistoEntries[hisname][nbin];
  if( ite2 != energies->end() ){
    hisname = procname + " : " + matname + " : " + (*ite2).second;
    itee = theHistoEntries.find( hisname );
    if( itee != theHistoEntries.end() ){
      //      G4cout << " GmPDSInteractionAngleManager::GetHistoValue(  hisname2 " << hisname << G4endl;
      binCEN2 = theHistoEntries[hisname][nbin];
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInteractionAngleManager::GetHistoValue:  hisname2 " << hisname << " nbin " << nbin << " = " << binCEN2 << G4endl;
#endif
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInteractionAngleManager::GetHistoValue:  hisname1 " << hisname << " nbin " << nbin << " = " << binCEN1 << " , " << binCEN2 << "  ENERS " << ener << " " << (*ite1).first << " " << (*ite2).first << G4endl;
#endif


  if( binCEN2 != -9999. ) {
    G4double diffE = (*ite2).first - (*ite1).first;
    //linear interpolation 
    G4double val;
    if( bEnergyLog ) { 
      val = binCEN1 +  ( log10(ener) - (*ite1).first )/diffE *(binCEN2-binCEN1);
      //      G4cout << "log_VAL_NOTNORM " << val << " " << binCEN1 << " + " << (log10(ener) - (*ite1).first) << " / " << diffE << " * " << (binCEN2-binCEN1) << G4endl;  //GDEB
    } else {
      val = binCEN1 +  ( ener - (*ite1).first )/diffE *(binCEN2-binCEN1);
      //      G4cout << "lin_VAL_NOTNORM " << val << " " << binCEN1 << " + " << (ener - (*ite1).first) << " / " << diffE << " * " << (binCEN2-binCEN1) << G4endl; //GDEB
    }
    val /= theBinStep; 
    //-    val = ( binCEN1 * ( ener - (*ite1).first ) + binCEN2 * ( (*ite2).first - ener ) ) 
    if( val < 0 ) {
      G4Exception("GmPDSInteractionAngleManager::GetHistoValue",
		  "",
		  JustWarning,
		  "NEGATIVE ANGLE PROBABILITY, SHOULD NOT HAPPEN, PROBABLY ANGLE HISTOGRAMS DO NOT HAVE ENOUGH ENTRIES.Try changing parameter /gamos/setParam GmPDS:InteractionAngleManager:MinimumNumberOfEntries VAL (default is 1000)");
    }
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInteractionAngleManager::GetHistoValue " << val << " proc " << procname << " mat " << matname << " ener " << ener << " cosang " << cosang << " binstep " << theBinStep << " ENERS " <<  ener <<  " " << (*ite1).first << " " << (*ite2).first <<  G4endl;
#endif
    return val;
//    return ( binCEN1 * fabs( ener - (*ite1).first ) +  binCEN2 * fabs( ener - (*ite2).first ) ) / theBinStep;
  } else {
    return  binCEN1 / theBinStep; 
  }

}


//------------------------------------------------------------------
std::map<G4double,G4String>::iterator GmPDSInteractionAngleManager::GetEnergyRange( mds* energies, G4double ener )
{
  std::map<G4double,G4String>::iterator ite;
  if( bEnergyLog ) ener = log10(ener);
  for( ite = energies->begin(); ite != energies->end(); ite++ ){
    if( ener < (*ite).first ) { 
      ite--;
      break;
    }
  }

  if( ite == energies->end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) {
      G4cerr << "LOOKING FOR ENERGY: " << pow(10.,ener) << " log " << ener << G4endl;
      for( ite = energies->begin(); ite != energies->end(); ite++ ){
	G4cerr << " ENER: " << pow(10.,(*ite).first) << " log " << (*ite).first << G4endl;
      }
      G4Exception("GmPDSInteractionAngleManager::GetEnergyRange",
		  "",
		  JustWarning,
		  ("Energy is bigger than maximum energy" + GmGenUtils::ftoa( (*(--ite)).first) ).c_str());
#endif
    }
    ite--;
  } 
  
  return ite;

}
