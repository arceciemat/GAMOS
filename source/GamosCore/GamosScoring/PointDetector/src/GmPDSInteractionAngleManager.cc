#include "GmPDSInteractionAngleManager.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosBase/Base/include/GmHistoReaderCSV.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include <vector>
#ifdef GAMOS_ROOT
#include "TFile.h"
#include "TKey.h"
#endif

//------------------------------------------------------------------
// Reads the "cos Deviation Angle" histograms created by GmPDSCreateAngleTablesUA:
//  - from a ROOT file, if the file name ends with ".root" (only with ROOT)
//  - from a CSV file written by GmAnalysisMgr otherwise
//    (default format without ROOT, or with /gamos/analysis/addFileFormat CSV)
GmPDSInteractionAngleManager::GmPDSInteractionAngleManager( const G4String& filename )
{
  if( filename == "" ) return;

  bFirstAngleHisto = true;
  theXmin = 0.;
  theXmax = 0.;
  theBinStep = 0.;
  G4double minimumNumberOfEntries = GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:InteractionAngleManager:MinimumNumberOfEntries",1000);
  bEnergyLog = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:InteractionAngleManager:EnergyLog",1));

  if( filename.size() > 5 && filename.substr(filename.size()-5) == ".root" ) {
#ifdef GAMOS_ROOT
    ReadROOTFile( filename, minimumNumberOfEntries );
#else
    G4Exception("GmPDSInteractionAngleManager::GmPDSInteractionAngleManager",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("File " + filename + " is a ROOT file, but GAMOS is built without ROOT (GAMOS_ROOT not defined). Create the angle tables in CSV format (default without ROOT) and use the '.csv' file").c_str());
#endif
  } else {
    ReadCSVFile( filename, minimumNumberOfEntries );
  }
}

#ifdef GAMOS_ROOT
//------------------------------------------------------------------
void GmPDSInteractionAngleManager::ReadROOTFile( const G4String& filename, G4double minimumNumberOfEntries )
{
  TFile hFile( filename );
  if( !hFile.IsOpen() ){
    G4Exception("GmPDSInteractionAngleManager::GmPDSInteractionAngleManager",
		"File not found",
		FatalErrorInArgument,
		G4String("FILE = " + filename ).c_str());
  }

  TIter ListOfHistograms( hFile.GetListOfKeys() );
  TKey *HistoKey;
  while ( (HistoKey = (TKey*)ListOfHistograms()) ) {    
    TH1F* histo = (TH1F*) hFile.Get( HistoKey->GetName() );
    if( histo->GetEntries() < minimumNumberOfEntries ) continue;
    std::string hisname = histo->GetName();
    if( hisname.find("cos Deviation Angle") == std::string::npos ) continue;
    G4int nbins = histo->GetXaxis()->GetNbins();
    std::vector<G4double> binContents;
    for( G4int ii = 1; ii <= nbins; ii++ ) binContents.push_back( histo->GetBinContent(ii) );
    AddAngleHisto( hisname, histo->GetXaxis()->GetXmin(), histo->GetXaxis()->GetXmax(), binContents );
  }
}
#else
//------------------------------------------------------------------
void GmPDSInteractionAngleManager::ReadROOTFile( const G4String&, G4double )
{
}
#endif

//------------------------------------------------------------------
void GmPDSInteractionAngleManager::ReadCSVFile( const G4String& filename, G4double minimumNumberOfEntries )
{
  GmHistoReaderCSV reader( filename );
  std::map<G4String,GmHisto1*> histos = reader.GetHistos1();
  if( histos.size() == 0 ) {
    G4Exception("GmPDSInteractionAngleManager::GmPDSInteractionAngleManager",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("No 1D histograms found in file " + filename + " (it must be a CSV histogram file written by GAMOS)").c_str());
  }
  std::map<G4String,GmHisto1*>::const_iterator iteh;
  for( iteh = histos.begin(); iteh != histos.end(); iteh++ ) {
    GmHisto1* histo = (*iteh).second;
    if( histo->GetEntries() < minimumNumberOfEntries ) continue;
    std::string hisname = (*iteh).first;
    if( hisname.find("cos Deviation Angle") == std::string::npos ) continue;
    G4int nbins = histo->GetNbinsX();
    std::vector<G4double> binContents;
    for( G4int ii = 1; ii <= nbins; ii++ ) binContents.push_back( histo->GetBinContent(ii) );
#ifdef GAMOS_ROOT
    G4double xmin = histo->GetXaxis()->GetXmin(); // GmHisto1 is TH1F
    G4double xmax = histo->GetXaxis()->GetXmax();
#else
    G4double xmin = histo->GetLowerEdge();
    G4double xmax = histo->GetUpperEdge();
#endif
    AddAngleHisto( hisname, xmin, xmax, binContents );
  }
}

//------------------------------------------------------------------
void GmPDSInteractionAngleManager::AddAngleHisto( const std::string& hisname, G4double xmin, G4double xmax,
						 const std::vector<G4double>& binContents )
{
  //----- name: "PRIMARY - SECONDARY : PROCESS : MATERIAL : ENERGY : cos Deviation Angle"
  int ic1 = hisname.find(":");
  int ic2 = hisname.find(":",ic1+1);
  G4String procname = hisname.substr(0,ic2-1);
  int ic3 = hisname.find(":",ic2+1);
  G4String matname = hisname.substr(ic2+2,ic3-ic2-3);
  int ic4 = hisname.find(":",ic3+1);
  G4String enername = hisname.substr(ic3+2,ic4-ic3-3);
  G4double ener = atof(enername);

  //----- histo variables
  G4double nent = 0.; // integral of bins (without underflow/overflow)
  for( size_t ii = 0; ii < binContents.size(); ii++ ) nent += binContents[ii];
  G4int nbins = binContents.size();
  G4double binstep = (xmax-xmin)/nbins;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << hisname << " xmin " << xmin << " xmax " << xmax << " binstep " << binstep << " nbins" << nbins << " integral " << nent << G4endl;
#endif
#endif
  if( !bFirstAngleHisto ) {
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
    bFirstAngleHisto = false;
  }
  std::vector<G4double> hent;
  for( int ii = 0; ii < nbins; ii++ ){
    if( nent != 0. ){
      hent.push_back( binContents[ii] / nent );
    } else {
      hent.push_back( 0. );
    }
  }
  msmds::const_iterator itepe = theEnergies.find(procname);
  if( itepe == theEnergies.end() ) {
    theEnergies[procname] = new std::map<G4double,G4String>;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << " probE filling procname " << procname << G4endl;
#endif
#endif
    itepe = theEnergies.find(procname);
  } 
  std::map<G4double,G4String>* energies = (*itepe).second;
  if( bEnergyLog ) {
    (*energies)[log10(ener)] = enername;
  } else {
    (*energies)[ener] = enername;
  }

  theHistoEntries[hisname.substr(0,ic4-1)] = hent;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) )G4cout << " GmPDSInteractionAngleManager histo entries =  " << hisname.substr(0,ic4-1) << G4endl;
#endif
#endif
}

//------------------------------------------------------------------
GmPDSInteractionAngleManager::~GmPDSInteractionAngleManager()
{
  theEnergies.clear();
  theHistoEntries.clear();
}
//------------------------------------------------------------------
G4double GmPDSInteractionAngleManager::GetHistoValue(const G4String& procname, const G4String& matname, G4double ener, const G4double cosang)
{
    msmds::const_iterator itepe = theEnergies.find(procname);
    if (itepe == theEnergies.end()) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
        if (ScoringVerb(infoVerb)) {
            G4cerr << " GmPDSInteractionAngleManager::GetHistoValue:  no histogram found with enough entries, RETURNING 0.5 " << procname << " " << theEnergies.size() << G4endl;
            for (itepe = theEnergies.begin(); itepe != theEnergies.end(); itepe++) {
                G4cerr << " ENERGIES " << (*itepe).first << " =? " << procname << G4endl;
            }
        }
#endif
#endif
        return 0.5;
    }

    std::map<G4double, G4String>* energies = (*itepe).second;
    if (energies->size() == 0) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
        if (ScoringVerb(debugVerb)) G4cerr << " GmPDSInteractionAngleManager::GetHistoValue:  no histogram found with enough entries, RETURNING 0.5 " << energies->size() << G4endl;
#endif
#endif
        return 0.5;
    }

    std::map<G4double, G4String>::iterator ite1 = GetEnergyRange(energies, ener);
    std::map<G4double, G4String>::iterator ite2 = ite1; ite2++;
    G4double enerCmp = bEnergyLog ? log10(ener) : ener;
    if( enerCmp < (*ite1).first ) ite2 = energies->end(); // energy below first histogram: use it, do not extrapolate

    G4String hisname = procname + " : " + matname + " : " + (*ite1).second;
    G4int nbin = int((cosang - theXmin) / theBinStep);
    if( nbin < 0 ) nbin = 0;
    //  G4cout << "NBIN " << nbin << " " << (cosang-theXmin) << " " << cosang << " - " << theXmin << " / " << theBinStep << G4endl;

    std::map<G4String, std::vector<G4double> >::iterator itee = theHistoEntries.find(hisname);
    if (itee == theHistoEntries.end()) {
        /*    for( itee = theHistoEntries.begin(); itee != theHistoEntries.end(); itee++) {
          G4cerr << " HISTO: " << (*itee).first << "ZZ" << G4endl;
          }*/
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
        if (ScoringVerb(debugVerb)) G4cerr << " GmPDSInteractionAngleManager::GetHistoValue:   histo not found: " << hisname << " RETURNING 0.5 " << G4endl;
#endif
#endif
        return 0.5;
        //    G4Exception( "GmPDSInteractionAngleManager::GetHistoValue histo nof found: " + hisname );
    }

    if( nbin >= G4int(((*itee).second).size()) ) nbin = G4int(((*itee).second).size())-1; // cosang == theXmax
    G4double binCEN1 = ((*itee).second)[nbin];

    G4double binCEN2 = -9999.;
    //t should get interpolation   G4double binPOS = theHistoEntries[hisname][nbin];
    if (ite2 != energies->end()) {
        hisname = procname + " : " + matname + " : " + (*ite2).second;
        itee = theHistoEntries.find(hisname);
        if (itee != theHistoEntries.end()) {
            //      G4cout << " GmPDSInteractionAngleManager::GetHistoValue(  hisname2 " << hisname << G4endl;
            binCEN2 = theHistoEntries[hisname][nbin];
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
            if (ScoringVerb(debugVerb)) G4cout << " GmPDSInteractionAngleManager::GetHistoValue:  hisname2 " << hisname << " nbin " << nbin << " = " << binCEN2 << G4endl;
#endif
#endif
        }
    }

#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if (ScoringVerb(debugVerb)) G4cout << " GmPDSInteractionAngleManager::GetHistoValue:  hisname1 " << hisname << " nbin " << nbin << " = " << binCEN1 << " , " << binCEN2 << "  ENERS " << ener << " " << (*ite1).first << " " << (ite2 != energies->end() ? (*ite2).first : -9999.) << G4endl;
#endif
#endif

    if (binCEN2 != -9999.) {
        G4double diffE = (*ite2).first - (*ite1).first;
        //linear interpolation 
        G4double val;
        if (bEnergyLog) {
            val = binCEN1 + (log10(ener) - (*ite1).first) / diffE * (binCEN2 - binCEN1);
            //      G4cout << "log_VAL_NOTNORM " << val << " " << binCEN1 << " + " << (log10(ener) - (*ite1).first) << " / " << diffE << " * " << (binCEN2-binCEN1) << G4endl;  //GDEB
        }
        else {
            val = binCEN1 + (ener - (*ite1).first) / diffE * (binCEN2 - binCEN1);
            //      G4cout << "lin_VAL_NOTNORM " << val << " " << binCEN1 << " + " << (ener - (*ite1).first) << " / " << diffE << " * " << (binCEN2-binCEN1) << G4endl; //GDEB
        }
        val /= theBinStep;
        //-    val = ( binCEN1 * ( ener - (*ite1).first ) + binCEN2 * ( (*ite2).first - ener ) ) 
        if (val < 0) {
            G4Exception("GmPDSInteractionAngleManager::GetHistoValue",
                "",
                JustWarning,
                "NEGATIVE ANGLE PROBABILITY, SHOULD NOT HAPPEN, PROBABLY ANGLE HISTOGRAMS DO NOT HAVE ENOUGH ENTRIES.Try changing parameter /gamos/setParam GmPDS:InteractionAngleManager:MinimumNumberOfEntries VAL (default is 1000)");
        }
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
        if (ScoringVerb(debugVerb)) G4cout << " GmPDSInteractionAngleManager::GetHistoValue " << val << " proc " << procname << " mat " << matname << " ener " << ener << " cosang " << cosang << " binstep " << theBinStep << " ENERS " << ener << " " << (*ite1).first << " " << (*ite2).first << G4endl;
#endif
#endif
        return val;
        //    return ( binCEN1 * fabs( ener - (*ite1).first ) +  binCEN2 * fabs( ener - (*ite2).first ) ) / theBinStep;
    }
    else {
        return  binCEN1 / theBinStep;
    }
}

//------------------------------------------------------------------
std::map<G4double,G4String>::iterator GmPDSInteractionAngleManager::GetEnergyRange( mds* energies, G4double ener )
{
  std::map<G4double,G4String>::iterator ite;
  if( bEnergyLog ) ener = log10(ener);
  ite = energies->upper_bound(ener); // first energy > ener
  if( ite == energies->begin() ) {
    // energy below first histogram energy: return first one (before, begin() was decremented)
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cerr << " GmPDSInteractionAngleManager::GetEnergyRange energy " << (bEnergyLog ? pow(10.,ener) : ener) << " is smaller than minimum energy, using first histogram " << (*ite).second << G4endl;
#endif
    return ite;
  }

  if( ite != energies->end() ) {
    ite--;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) {
      G4cerr << "LOOKING FOR ENERGY: " << pow(10.,ener) << " log " << ener << G4endl;
      for( ite = energies->begin(); ite != energies->end(); ite++ ){
	G4cerr << " ENER: " << pow(10.,(*ite).first) << " log " << (*ite).first << G4endl;
      }
      G4Exception("GmPDSInteractionAngleManager::GetEnergyRange",
		  "",
		  JustWarning,
		  ("Energy is bigger than maximum energy" + GmGenUtils::ftoa( (*(energies->rbegin())).first) ).c_str()); // do not move ite here, it is decremented below
    }
#endif
    ite--;
  } 
  
  return ite;
  
}
