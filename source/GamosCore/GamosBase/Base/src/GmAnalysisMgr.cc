#include "GmAnalysisMgr.hh"
#include "GmParameterMgr.hh"
#include "GmHistoWriterROOT.hh"
#include "GmHistoWriterCSV.hh"
#include "GmHistoWriterTXT.hh"
#include "GmBaseVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "CLHEP/Units/SystemOfUnits.h"

std::map<G4String,GmAnalysisMgr*> GmAnalysisMgr::theInstances;

std::set<G4String> GmAnalysisMgr::theFileFormats;
std::map<G4String,G4double> GmAnalysisMgr::theHisto1NBins;
std::map<G4String,G4double> GmAnalysisMgr::theHisto1Min;
std::map<G4String,G4double> GmAnalysisMgr::theHisto1Max;
std::map<G4String,G4double> GmAnalysisMgr::theHisto2NBinsX;
std::map<G4String,G4double> GmAnalysisMgr::theHisto2MinX;
std::map<G4String,G4double> GmAnalysisMgr::theHisto2MaxX;
std::map<G4String,G4double> GmAnalysisMgr::theHisto2NBinsY;
std::map<G4String,G4double> GmAnalysisMgr::theHisto2MinY;
std::map<G4String,G4double> GmAnalysisMgr::theHisto2MaxY;
std::map<G4String,G4int> GmAnalysisMgr::theHistoPrint;
std::map<G4String,G4double> GmAnalysisMgr::theHistoNormFactor;

//----------------------------------------------------------------------
GmAnalysisMgr::GmAnalysisMgr(const G4String& fileName)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " AnalysisMgr fileName " << fileName  << G4endl;
#endif
   G4String prefix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  theFileName = prefix + fileName;
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " AnalysisMgr fileName with prefix " << theFileName  << G4endl;
#endif

  G4String suffix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  theFileName = theFileName+suffix;
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(infoVerb) ) G4cout << " AnalysisMgr fileName with suffix " << theFileName  << G4endl;
#endif

  //  G4cout << "GmAnalysisMgr::GmAnalysisMgr creating histo file " << theFileName << G4endl;

  if( theFileFormats.size() == 0 ){
#ifndef GAMOS_NO_ROOT
    theFileFormats.insert("root");
#else
    theFileFormats.insert("csv");
#endif
  }

  bDefaultNormalize = true;
}
 

//------------------------------------------------------------------------------
GmAnalysisMgr* GmAnalysisMgr::GetInstance( const G4String& fileName )
{
  if (theInstances.find(fileName) == theInstances.end() ) theInstances[fileName] = new GmAnalysisMgr( fileName );
  return theInstances[fileName];
}

//----------------------------------------------------------------------
GmAnalysisMgr::~GmAnalysisMgr()
{ 

  bNormalizeToNEvents = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theFileName+":NormalizeToNEvents",bDefaultNormalize));

  Normalize();

  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theFileName+":DeleteEmptyHistos",0)) ) DeleteEmptyHistos();

  std::set<G4String>::const_iterator ite;
  for( ite = GmAnalysisMgr::theFileFormats.begin(); ite != GmAnalysisMgr::theFileFormats.end(); ite++ ){
    Save( theFileName, *ite ); 
  } 

  mih1::const_iterator ite1;
  mih2::const_iterator ite2;
  mihp1::const_iterator itep1;
  mihp2::const_iterator itep2;

  for( ite1 = theHistos1.begin(); ite1 != theHistos1.end(); ite1++ ){ 
    delete (*ite1).second;
  }
  for( ite2 = theHistos2.begin(); ite2 != theHistos2.end(); ite2++ ){
    delete (*ite2).second;
  }
  for( itep1 = theHistosProf1.begin(); itep1 != theHistosProf1.end(); itep1++ ){
    delete (*itep1).second;
  }
  for( itep2 = theHistosProf2.begin(); itep2 != theHistosProf2.end(); itep2++ ){    delete (*itep2).second;
  }

}

//----------------------------------------------------------------------
void GmAnalysisMgr::Normalize()
{

  std::map<G4String,G4double>::const_iterator itenf;
  mih1::const_iterator ite1;
  mih2::const_iterator ite2;
  mihp1::const_iterator itep1;
  mihp2::const_iterator itep2;
    
  for( ite1 = theHistos1.begin(); ite1 != theHistos1.end(); ite1++ ){ 
    for( itenf = theHistoNormFactor.begin(); itenf != theHistoNormFactor.end(); itenf++ ) {
      //      G4cout << " NORM CHECK " <<  (*itenf).first << " " << (*ite1).second->GetName() << " " << GmGenUtils::AreWordsEquivalent( (*itenf).first, (*ite1).second->GetName() ) << G4endl;
      if( GmGenUtils::AreWordsEquivalent( (*itenf).first, (*ite1).second->GetName() ) ) {
	GmHisto1* his = (*ite1).second;
	G4double integral = 0.;
	for( G4int ii = 0; ii < his->GetNbinsX()+1; ii++ ){
	  integral +=  his->GetBinContent(ii);
	}
	for( G4int ii = 0; ii < his->GetNbinsX()+1; ii++ ){
	  his->SetBinContent(ii, his->GetBinContent(ii) * (*itenf).second / integral);
	  his->SetBinError(ii, his->GetBinError(ii) * (*itenf).second / integral);
	}
      }  
    }
  }
  for( ite2 = theHistos2.begin(); ite2 != theHistos2.end(); ite2++ ){
  }
  for( itep1 = theHistosProf1.begin(); itep1 != theHistosProf1.end(); itep1++ ){
  }
  for( itep2 = theHistosProf2.begin(); itep2 != theHistosProf2.end(); itep2++ ){
    
  }
  
}

//----------------------------------------------------------------------
void GmAnalysisMgr::SetFileFormat( G4String& ff ) 
{
  theFileFormats.clear();
  AddFileFormat( ff ); 
}
 
//----------------------------------------------------------------------
void GmAnalysisMgr::AddFileFormat( G4String& ff ) 
{
  for( unsigned int ii = 0; ii < ff.length(); ii++ ){
    ff[ii] = tolower(ff[ii]);
  } 
  theFileFormats.insert(ff); 
}

//----------------------------------------------------------------------
void GmAnalysisMgr::Save( const G4String& name, const G4String& format)
{
  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theFileName+":NoSavingIfEmpty",1)) ) {
    if( theHistos1.size() == 0 && 
	theHistos2.size() == 0 && 
	theHistosProf1.size() == 0 && 
	theHistosProf2.size() == 0 ) {
      return;
    }
  }

  //----- Create histogram writer
  G4String fileName =  name + "." + format;

  GmHistoWriter* hisWriter = 0;
#ifndef GAMOS_NO_ROOT
  if( format == "root" ){
    hisWriter = new GmHistoWriterROOT( fileName );
  }
#endif
 if( format == "csv" ) {
   hisWriter = new GmHistoWriterCSV( fileName );
 } else if ( format == "txt" ) {
   hisWriter = new GmHistoWriterTXT( fileName );
 }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(warningVerb) ) G4cout << "=== saving histograms in file === " << fileName << G4endl;
#endif
  G4int nhistos = theHistos1.size() + theHistos2.size();
  nhistos += theHistosProf1.size() + theHistosProf2.size();

#ifndef GAMOS_NO_VERBOSE
  if( nhistos == 0 )  if( BaseVerb(warningVerb) ) G4cerr << "=== No histograms in file === " << fileName << " ! " << G4endl;
#endif
  //-  TFile fout(fileName.c_str(),"recreate");
  // write out the histos
  mih1::const_iterator ite1;
  mih2::const_iterator ite2;
  mihp1::const_iterator itep1;
  mihp2::const_iterator itep2;
  for( ite1 = theHistos1.begin(); ite1 != theHistos1.end(); ite1++ ){
    GmHisto1* his = (*ite1).second;
    if( bNormalizeToNEvents ) {
      GmHisto1* hisN = new GmHisto1(*his); //make a copy, it case it is saved several times 
      NormalizeToNEvents(hisN); 
      hisWriter->SaveHisto1( hisN );
      delete hisN;
    }else {
      hisWriter->SaveHisto1( his );
    }
  }
  for( ite2 = theHistos2.begin(); ite2 != theHistos2.end(); ite2++ ){
    GmHisto2* his = (*ite2).second;
    if( bNormalizeToNEvents ) {
      GmHisto2* hisN = new GmHisto2(*his);
      NormalizeToNEvents(hisN); 
      hisWriter->SaveHisto2( hisN );
      delete hisN;
    }else {
      hisWriter->SaveHisto2(his);
    }
  }
  for( itep1 = theHistosProf1.begin(); itep1 != theHistosProf1.end(); itep1++ ){
    GmHistoProfile1* his = (*itep1).second;
    if( bNormalizeToNEvents ) {
      GmHistoProfile1* hisN = new GmHistoProfile1(*his);
      NormalizeToNEvents(hisN); 
      hisWriter->SaveHistoProf1( hisN );
      delete hisN;
    }else {
      hisWriter->SaveHistoProf1(his);
    }
  }
  for( itep2 = theHistosProf2.begin(); itep2 != theHistosProf2.end(); itep2++ ){
    GmHistoProfile2* his = (*itep2).second;
    if( bNormalizeToNEvents ) {
      GmHistoProfile2* hisN = new GmHistoProfile2(*his);
      NormalizeToNEvents(hisN); 
      hisWriter->SaveHistoProf2( hisN );
      delete hisN;
    }else {
      hisWriter->SaveHistoProf2(his);
    }
  }

  delete hisWriter;

  PrintInfo();

}
//----------------------------------------------------------------------
void GmAnalysisMgr::PrintInfo()
{
  mih1::const_iterator ite1;
  mih2::const_iterator ite2;
  mihp1::const_iterator itep1;
  mihp2::const_iterator itep2;
  std::map<G4String,G4int>::const_iterator itepr;

  for( ite1 = theHistos1.begin(); ite1 != theHistos1.end(); ite1++ ){
    for( itepr = theHistoPrint.begin(); itepr != theHistoPrint.end(); itepr++ ){
      if( GmGenUtils::AreWordsEquivalent( (*itepr).first, (*ite1).second->GetName() ) ) {
	PrintInfo1( (*ite1).second, (*itepr).second );
      }
    }
  }
  for( ite2 = theHistos2.begin(); ite2 != theHistos2.end(); ite2++ ){
    for( itepr = theHistoPrint.begin(); itepr != theHistoPrint.end(); itepr++ ){
      if( GmGenUtils::AreWordsEquivalent( (*itepr).first, (*ite2).second->GetName() ) ) {
	PrintInfo2( (*ite2).second, (*itepr).second );
      }
    }
  }
  for( itep1 = theHistosProf1.begin(); itep1 != theHistosProf1.end(); itep1++ ){
    for( itepr = theHistoPrint.begin(); itepr != theHistoPrint.end(); itepr++ ){
      if( GmGenUtils::AreWordsEquivalent( (*itepr).first, (*itep1).second->GetName() ) ) {
	GmHisto1* his1 = (GmHisto1*)(*itep1).second;
	PrintInfo1( his1, (*itepr).second );
      }
    }
  }
  for( itep2 = theHistosProf2.begin(); itep2 != theHistosProf2.end(); itep2++ ){
    for( itepr = theHistoPrint.begin(); itepr != theHistoPrint.end(); itepr++ ){
      if( GmGenUtils::AreWordsEquivalent( (*itepr).first, (*itep2).second->GetName() ) ) {
	GmHisto2* his2 = (GmHisto2*)(*itep2).second;
	PrintInfo2( his2, (*itepr).second );
      }
    }
  } 

} 

#ifndef GAMOS_NO_ROOT
#include "TAxis.h"
#endif

//----------------------------------------------------------------------
void GmAnalysisMgr::PrintInfo1( GmHisto1* his, G4int iverbose )
{
  if( iverbose == 0 ) {
    G4cout << "HISINFO: " << his->GetName() 
	   << " NENTRIES= " << his->GetEntries() 
	   << " MEAN= " << his->GetMean() 
	   << " RMS= " << his->GetRMS() 
	   << G4endl;
  } 
  if( iverbose >= 1 ) {
    G4cout << "HISINFO: " << his->GetName() 
	   << " NENTRIES= " << his->GetEntries() 
	   << " MEAN= " << his->GetMean() 
	   << " +- " << his->GetMeanError() 
	   << " RMS= " << his->GetRMS()
	   << " +- " << his->GetRMSError() 
	   << G4endl;
  } 
  if( iverbose >= 2 ) {
    G4int nbins;
#ifndef GAMOS_NO_ROOT 
    TAxis* Xaxis = his->GetXaxis();
    nbins =  Xaxis->GetNbins();
    G4cout << "HISINFO: " << his->GetName() 
	   << " NBINS= " << nbins
	   << " LOWEREDGE= " << Xaxis->GetXmin()
	   << " UPPEREDGE= " << Xaxis->GetXmax()
	   << G4endl;
#else
    nbins = his->GetNbins();
    G4cout << "HISINFO: " << his->GetName() 
	   << " NBINS= " << nbins
	   << " LOWEREDGE= " << his->GetLowerEdge() 
	   << " UPPEREDGE= " <<  his->GetUpperEdge()
	   << G4endl;
#endif
    
    G4cout << "HISINFO: " << his->GetName() 
	   << " UNDERFLOW= " << his->GetBinContent(0)
	   << " OVERFLOW= " << his->GetBinContent(nbins)
	   << G4endl;
  } 
} 


//----------------------------------------------------------------------
void GmAnalysisMgr::PrintInfo2( GmHisto2* his, G4int iverbose )
{
  if( iverbose == 0 ) {
    G4cout << "HISINFO: " << his->GetName() 
	   << " NENTRIES= " << his->GetEntries() 
	   << " MEAN_X= " << his->GetMean(1) 
	   << " RMS_X= " << his->GetRMS(1) 
	   << " MEAN_Y= " << his->GetMean(2) 
	   << " RMS_Y= " << his->GetRMS(2) 
	   << G4endl;
  } 
  if( iverbose >= 1 ) {
    G4cout << "HISINFO: " << his->GetName() 
	   << " NENTRIES= " << his->GetEntries() 
	   << " MEAN_X= " << his->GetMean(1) 
	   << " +- " << his->GetMeanError(1) 
	   << " RMS_X= " << his->GetRMS(1) 
	   << " +- " << his->GetRMSError(1) 
	   << " MEAN_Y= " << his->GetMean(2)
	   << " +- " << his->GetMeanError(2)  
	   << " RMS_Y= " << his->GetRMS(2) 
	   << " +- " << his->GetRMSError(2) 
	   << G4endl;
  } 
  if( iverbose >= 2 ) {
     G4int nbinsx;
    G4int nbinsy;
#ifndef GAMOS_NO_ROOT
    TAxis* Xaxis = his->GetXaxis();
    TAxis* Yaxis = his->GetYaxis();
    nbinsx = Xaxis->GetNbins();
    nbinsy = Yaxis->GetNbins();
    G4cout << "HISINFO: " << his->GetName() 
	   << " NBINS_X= " << nbinsx
	   << " LOWEREDGE_X= " << Xaxis->GetXmin()
	   << " UPPEREDGE_X= " << Xaxis->GetXmax()
	   << " NBINS_Y= " << nbinsy
	   << " LOWEREDGE_Y= " << Yaxis->GetXmin()
	   << " UPPEREDGE_Y= " << Yaxis->GetXmax()
	   << G4endl;
#else 
    nbinsx = his->GetNbinsX();
    nbinsy = his->GetNbinsY();
    G4cout << "HISINFO: " << his->GetName() 
	   << " NBINS_X= " << nbinsx
	   << " LOWEREDGE_X= " << his->GetLowerEdgeX()
	   << " UPPEREDGE_X= " << his->GetUpperEdgeX()
	   << " NBINS_Y= " << nbinsy
	   << " LOWEREDGE_Y= " << his->GetLowerEdgeY()
	   << " UPPEREDGE_Y= " << his->GetUpperEdgeY()
	   << G4endl;
#endif
    G4cout << "HISINFO: " << his->GetName();
    G4double uflow = 0.;
    for( G4int ii = 0; ii < nbinsy; ii++ ) {
      uflow += his->GetBinContent(0,ii);
    } 
    G4cout << " UNDERFLOW_X= " << uflow;

    G4double oflow = 0.;
    for( G4int ii = 0; ii < nbinsx; ii++ ) {
      oflow += his->GetBinContent(nbinsx,ii);
    } 
    G4cout << " OVERFLOW_X= " << oflow;

    uflow = 0.;
    for( G4int ii = 0; ii < nbinsx; ii++ ) {
      uflow += his->GetBinContent(ii,0);
    } 
    G4cout << " UNDERFLOW_Y= " << uflow;

    for( G4int ii = 0; ii < nbinsx; ii++ ) {
      oflow += his->GetBinContent(ii,nbinsy);
    } 
    G4cout << " OVERFLOW_Y= " << oflow
	   << G4endl;
  } 
} 

//----------------------------------------------------------------------
void GmAnalysisMgr::DeleteEmptyHistos()
{
  mih1::const_iterator ite1;
  mih2::const_iterator ite2;
  mihp1::const_iterator itep1;
  mihp2::const_iterator itep2;

  mih1 histos1t;
  for( ite1 = theHistos1.begin(); ite1 != theHistos1.end(); ite1++ ){
    if( ((*ite1).second)->GetEntries() != 0 ) histos1t[(*ite1).first] = (*ite1).second;
  }
  theHistos1 = histos1t;

  mih2 histos2t;
  for( ite2 = theHistos2.begin(); ite2 != theHistos2.end(); ite2++ ){
    if( ((*ite2).second)->GetEntries() != 0 ) histos2t[(*ite2).first] = (*ite2).second;
  }
  theHistos2 = histos2t;

  mihp1 histosp1t;
  for( itep1 = theHistosProf1.begin(); itep1 != theHistosProf1.end(); itep1++ ){
    if( ((*itep1).second)->GetEntries() != 0 ) histosp1t[(*itep1).first] = (*itep1).second;
  }
  theHistosProf1 = histosp1t;

  mihp2 histosp2t;
  for( itep2 = theHistosProf2.begin(); itep2 != theHistosProf2.end(); itep2++ ){
    if( ((*itep2).second)->GetEntries() != 0 ) histosp2t[(*itep2).first] = (*itep2).second;
  }
  theHistosProf2 = histosp2t;

}


//----------------------------------------------------------------------------
bool GmAnalysisMgr::CreateHisto1D(const G4String & pathAndTitle, int nBins, double lowerEdge, double upperEdge, int nHisto)
{
  int ih = 0;
  if( nHisto == -1 ) {
    ih = BuildHistoNumber("1D");
  } else {
    ih = nHisto;
  }

  std::map<G4String,G4double>::const_iterator ite;
  for( ite = theHisto1NBins.begin(); ite != theHisto1NBins.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      nBins = G4int((*ite).second);
    }
  }
  for( ite = theHisto1Min.begin(); ite != theHisto1Min.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      lowerEdge = (*ite).second;
    }
  }
  for( ite = theHisto1Max.begin(); ite != theHisto1Max.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      upperEdge = (*ite).second;
    }
  }

#ifndef GAMOS_NO_ROOT
  theHistos1[ih] = new TH1F( pathAndTitle.c_str(), pathAndTitle.c_str(), nBins, lowerEdge, upperEdge );
  theHistos1[ih]->SetDirectory(0);
#else 
  theHistos1[ih] = new GmHisto1( pathAndTitle, nBins, lowerEdge, upperEdge );
#endif

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(warningVerb) ) G4cout << " CreateHisto1D " << pathAndTitle << " = " << ih << G4endl;
#endif

  return true;
}


//----------------------------------------------------------------------------
bool GmAnalysisMgr::CreateHisto2D(const G4String & pathAndTitle, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nHisto)
{
  int ih = 0;
  if( nHisto == -1 ) {
    ih = BuildHistoNumber("2D");
  } else {
    ih = nHisto;
  }

  std::map<G4String,G4double>::const_iterator ite;
  for( ite = theHisto2NBinsX.begin(); ite != theHisto2NBinsX.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      nBinsX = G4int((*ite).second);
    }
  }
  for( ite = theHisto2MinX.begin(); ite != theHisto2MinX.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      lowerEdgeX = (*ite).second;
    }
  }
  for( ite = theHisto2MaxX.begin(); ite != theHisto2MaxX.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      upperEdgeX = (*ite).second;
    }
  }
  for( ite = theHisto2NBinsY.begin(); ite != theHisto2NBinsY.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      nBinsY = G4int((*ite).second);
    }
  }
  for( ite = theHisto2MinY.begin(); ite != theHisto2MinY.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      lowerEdgeY = (*ite).second;
    }
  }
  for( ite = theHisto2MaxY.begin(); ite != theHisto2MaxY.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      upperEdgeY = (*ite).second;
    }
  }

#ifndef GAMOS_NO_ROOT
  theHistos2[ih] = new TH2F( pathAndTitle.c_str(), pathAndTitle.c_str(), nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY);
  theHistos2[ih]->SetDirectory(0);
#else
  theHistos2[ih] = new GmHisto2( pathAndTitle.c_str(), nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY);
#endif

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(warningVerb) ) G4cout << " CreateHisto2D " << pathAndTitle << " = " << ih << G4endl;
#endif

  return true;
}

//----------------------------------------------------------------------------
bool GmAnalysisMgr::CreateHistoProfile1D(const G4String & pathAndTitle, int nBins, double lowerEdge, double upperEdge, int nHisto)
{ 
  int ih = 0;
  if( nHisto == -1 ) {
    ih = BuildHistoNumber("Prof1D");
  } else {
    ih = nHisto;
  }


  std::map<G4String,G4double>::const_iterator ite;
  for( ite = theHisto1NBins.begin(); ite != theHisto1NBins.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      nBins = G4int((*ite).second);
    }
  }
  for( ite = theHisto1Min.begin(); ite != theHisto1Min.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      lowerEdge = (*ite).second;
    }
  }
  for( ite = theHisto1Max.begin(); ite != theHisto1Max.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      upperEdge = (*ite).second;
    }
  }

#ifndef GAMOS_NO_ROOT
  theHistosProf1[ih] = new TProfile( pathAndTitle.c_str(), pathAndTitle.c_str(), nBins, lowerEdge, upperEdge);
  theHistosProf1[ih]->SetDirectory(0);
#else 
  theHistosProf1[ih] = new GmHistoProfile1( pathAndTitle.c_str(), nBins, lowerEdge, upperEdge);
#endif

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(warningVerb) ) G4cout << " CreateHistoProfile1D " << pathAndTitle << " = " << ih << G4endl;
#endif

  return true;

}

//----------------------------------------------------------------------------
bool GmAnalysisMgr::CreateHistoProfile2D(const G4String & pathAndTitle, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nHisto)
{ 
  int ih = 0;
  if( nHisto == -1 ) {
    ih = BuildHistoNumber("2D");
  } else {
    ih = nHisto;
  }


  std::map<G4String,G4double>::const_iterator ite;
  for( ite = theHisto2NBinsX.begin(); ite != theHisto2NBinsX.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      nBinsX = G4int((*ite).second);
    }
  }
  for( ite = theHisto2MinX.begin(); ite != theHisto2MinX.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      lowerEdgeX = (*ite).second;
    }
  }
  for( ite = theHisto2MaxX.begin(); ite != theHisto2MaxX.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      upperEdgeX = (*ite).second;
    }
  }
  for( ite = theHisto2NBinsY.begin(); ite != theHisto2NBinsY.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      nBinsY = G4int((*ite).second);
    }
  }
  for( ite = theHisto2MinY.begin(); ite != theHisto2MinY.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      lowerEdgeY = (*ite).second;
    }
  }
  for( ite = theHisto2MaxY.begin(); ite != theHisto2MaxY.end(); ite++ ){
    if( GmGenUtils::AreWordsEquivalent( (*ite).first, pathAndTitle ) ) {
      upperEdgeY = (*ite).second;
    }
  }

#ifndef GAMOS_NO_ROOT
  theHistosProf2[ih] = new TProfile2D( pathAndTitle.c_str(), pathAndTitle.c_str(), nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY);
  theHistosProf2[ih]->SetDirectory(0);
#else
  theHistosProf2[ih] = new GmHistoProfile2( pathAndTitle.c_str(), nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY);
#endif

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(warningVerb) ) G4cout << " CreateHistoProfile2D " << pathAndTitle << " = " << ih << G4endl;
#endif

  return true;
}

//----------------------------------------------------------------------------
G4int GmAnalysisMgr::BuildHistoNumber( const G4String& htyp)
{
  G4int ih = -1;
  if( htyp == "1D" ) {
    ih = BuildHistoNumberForType( theHistos1 );
  } else if( htyp == "2D" ) {
    ih = BuildHistoNumberForType( theHistos2 );
  } else if( htyp == "Prof1D" ) {
    ih = BuildHistoNumberForType( theHistosProf1 );
  } else if( htyp == "Prof2D" ) {
    ih = BuildHistoNumberForType( theHistosProf2 );
  } else {
    G4cerr << "!!! FATAL ERROR: GmAnalysisMgr::BuildHistoNumber histo type not supported " << htyp << G4endl;
    G4Exception("GmAnalysisMgr::BuildHistoNumber",
		"histo type not supported ",
		FatalErrorInArgument,
		G4String("Histo type asked for = " + htyp).c_str());
  }
  
  return ih;
}


//-----------------------------------------------------------------------------
template< class TYP >
G4int GmAnalysisMgr::BuildHistoNumberForType( const std::map< int, TYP* >& histos)
{
  typename std::map< int, TYP* >::const_iterator iteh;
  G4int ii = 0;
  for(;;){
    G4bool iiFound = FALSE;
    for( iteh = histos.begin(); iteh != histos.end(); iteh++ ){
      if( (*iteh).first == ii ) {
	iiFound = TRUE;
	break;
      }
    }
    if( !iiFound ) return ii;  // number not found 
  }
}


#ifndef GAMOS_NO_ROOT
//-----------------------------------------------------------------------------
TH1F* GmAnalysisMgr::GetHisto1D( int ih, bool itExists )
{
  TH1F* his = 0;
#else
//-----------------------------------------------------------------------------
GmHisto1* GmAnalysisMgr::GetHisto1D( int ih, bool itExists )
{
  GmHisto1* his = 0;
#endif

  mih1::const_iterator ite = theHistos1.find( ih );
  if( ite != theHistos1.end() ) {
    his = (*ite).second;
  } else {
    HistoDoesNotExists( ih, "1D", itExists );
  }
  return his;

}


//-----------------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
TH1F* GmAnalysisMgr::GetHisto1D( const G4String& hnam, bool itExists )
{
  TH1F* his = 0;
#else
GmHisto1* GmAnalysisMgr::GetHisto1D( const G4String& hnam, bool itExists )
{
  GmHisto1* his = 0;
#endif

  mih1::const_iterator ite;
  for( ite = theHistos1.begin(); ite != theHistos1.end(); ite++ ){ 
    if( ((*ite).second)->GetName() == hnam ) break;
  }

  if( ite != theHistos1.end() ) {
    his = (*ite).second;
  } else {
    HistoDoesNotExists( hnam, "1D", itExists );
  }
  return his;

}


//-----------------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
TH2F* GmAnalysisMgr::GetHisto2D( int ih, bool itExists )
{
  TH2F* his = 0;
#else 
GmHisto2* GmAnalysisMgr::GetHisto2D( int ih, bool itExists )
{
  GmHisto2* his = 0;
#endif

  mih2::const_iterator ite = theHistos2.find( ih );
  if( ite != theHistos2.end() ) {
    his = (*ite).second;
  } else {
    HistoDoesNotExists( ih, "2D", itExists );
  }
  return his;
}

//-----------------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
TH2F* GmAnalysisMgr::GetHisto2D( const G4String& hnam, bool itExists )
{
  TH2F* his = 0;
#else 
GmHisto2* GmAnalysisMgr::GetHisto2D( const G4String& hnam, bool itExists )
{
  GmHisto2* his = 0;
#endif

  mih2::const_iterator ite;
  for( ite = theHistos2.begin(); ite != theHistos2.end(); ite++ ){ 
    if( ((*ite).second)->GetName() == hnam ) break;
  }

  if( ite != theHistos2.end() ) {
    his = (*ite).second;
  } else {
    HistoDoesNotExists( hnam, "2D", itExists );
  }
  return his;

}


//-----------------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
TProfile* GmAnalysisMgr::GetHistoProf1( int ih, bool itExists )
{
  TProfile* his = 0;
#else 
GmHistoProfile1* GmAnalysisMgr::GetHistoProf1( int ih, bool itExists )
{
  GmHistoProfile1* his = 0;
#endif

  mihp1::const_iterator ite = theHistosProf1.find( ih );
  if( ite != theHistosProf1.end() ) {
   his = (*ite).second;
  } else {
    HistoDoesNotExists( ih, "Profile1D", itExists );
  }

  return his;

}

//-----------------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
TProfile* GmAnalysisMgr::GetHistoProf1( const G4String& hnam, bool itExists )
{
  TProfile* his = 0;
#else 
GmHistoProfile1* GmAnalysisMgr::GetHistoProf1( const G4String& hnam, bool itExists )
{
  GmHistoProfile1* his = 0;
#endif

  mihp1::const_iterator ite;
  for( ite = theHistosProf1.begin(); ite != theHistosProf1.end(); ite++ ){ 
    if( ((*ite).second)->GetName() == hnam ) break;
  }

  if( ite != theHistosProf1.end() ) {
    his = (*ite).second;
  } else {
    HistoDoesNotExists( hnam, "Profile1D", itExists );
  }

  return his;

}

//-----------------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
TProfile2D* GmAnalysisMgr::GetHistoProf2( int ih, bool itExists )
{
  TProfile2D* his = 0;
#else 
GmHistoProfile2* GmAnalysisMgr::GetHistoProf2( int ih, bool itExists )
{
  GmHistoProfile2* his = 0;
#endif

  mihp2::const_iterator ite = theHistosProf2.find( ih );
  if( ite != theHistosProf2.end() ) {
   his = (*ite).second;
  } else {
    HistoDoesNotExists( ih, "Profile2D", itExists );
  }

  return his;

}

//-----------------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
TProfile2D* GmAnalysisMgr::GetHistoProf2( const G4String& hnam, bool itExists )
{
  TProfile2D* his = 0;
#else 
GmHistoProfile2* GmAnalysisMgr::GetHistoProf2( const G4String& hnam, bool itExists )
{
  GmHistoProfile2* his = 0;
#endif

  mihp2::const_iterator ite;
  for( ite = theHistosProf2.begin(); ite != theHistosProf2.end(); ite++ ){ 
    if( ((*ite).second)->GetName() == hnam ) break;
  }

  if( ite != theHistosProf2.end() ) {
    his = (*ite).second;
  } else {
    HistoDoesNotExists( hnam, "Profile2D", itExists );
  }

  return his;

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::HistoDoesNotExists( int ih, const G4String& histype, bool itExists )
{
  if( itExists ){
    G4Exception("GmAnalysMgr::GetHisto",
		"Wrong internal argument",
		FatalException,
		G4String("Histogram "+histype+" number "+ GmGenUtils::itoa(ih) + " does not exist ").c_str());
    /* } else {
    G4Exception("GmAnalysMgr::GetHisto",
		"Warning",
		JustWarning,
		G4String("Histogram "+histype+" number "+ GmGenUtils::itoa(ih) + " does not exist ").c_str()); */
  }
}

//----------------------------------------------------------------------------
void GmAnalysisMgr::HistoDoesNotExists( const G4String& hnam, const G4String& histype, bool itExists )
{
  if( itExists ){
    G4Exception("GmAnalysMgr::GetHisto",
		"Wrong internal argument",
		FatalException,
		G4String("Histogram "+histype+" number "+ hnam + " does not exist ").c_str());
    /*  } else {
    G4Exception("GmAnalysMgr::GetHisto",
		"Warning",
		JustWarning,
		G4String("Histogram "+histype+" number "+ hnam + " does not exist ").c_str()); */
  }
}

//----------------------------------------------------------------------------
void GmAnalysisMgr::DeleteInstances()
{
  std::map<G4String,GmAnalysisMgr*>::iterator ite;

  for( ite = theInstances.begin(); ite != theInstances.end(); ite++ ){
    delete (*ite).second;
  }
  theInstances.clear();

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto1NBins( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto1NBins",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo1NBins:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto1NBins",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo1NBins:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }

  theHisto1NBins[ wl[0] ] = GmGenUtils::GetValue(wl[1]);

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto1Min( G4String parstr)
{

  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto1Min",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo1Min:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto1Min",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo1Min:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }

  theHisto1Min[ wl[0] ] = GmGenUtils::GetValue(wl[1]);

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto1Max( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto1Max",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo1Max:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto1Max",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo1Max:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }

  theHisto1Max[ wl[0] ] = GmGenUtils::GetValue(wl[1]);

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto2NBinsX( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto2NBinsX",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2NBinsX:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto2NBinsX",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2NBinsX:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }
  
  theHisto2NBinsX[ wl[0] ] = GmGenUtils::GetValue(wl[1]);

}
//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto2NBinsY( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto2NBinsY",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2NBinsY: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto2NBinsY",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2NBinsY: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }
  
  theHisto2NBinsY[ wl[0] ] = GmGenUtils::GetValue(wl[1]);
  
}
 
//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto2MinX( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MinX",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MinX: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MinX",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MinX: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }
  
  theHisto2MinX[ wl[0] ] = GmGenUtils::GetValue(wl[1]);
  
}
 
//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto2MinY( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MinY",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MinY: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MinY",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MinY: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }

  theHisto2MinY[ wl[0] ] = GmGenUtils::GetValue(wl[1]);

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto2MaxX( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MaxX",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MaxX: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
		} else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MaxX",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MaxX: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }

  theHisto2MaxX[ wl[0] ] = GmGenUtils::GetValue(wl[1]);

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHisto2MaxY( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MaxY",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MaxY: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto2MaxY",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histo2MaxY: Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }

  theHisto2MaxY[ wl[0] ] = GmGenUtils::GetValue(wl[1]);
  
}


//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHistoPrint( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHisto1Print",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histoPrint: Parameter not valid, must have at least two-words: 'HISTO_NAME VERBOSITY_VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHisto1Print",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histoPrint: Parameter not valid, must have at least two-words: 'HISTO_NAME VERBOSITY_VALUE', it has only one: "+wl[0]).c_str());
  }

  theHistoPrint[ wl[0] ] = G4int(GmGenUtils::GetValue(wl[1]));

}


//----------------------------------------------------------------------------
void GmAnalysisMgr::AddHistoNorm( G4String parstr)
{
  std::vector<G4String> wl = GmGenUtils::GetWordsInString( parstr );
  if( wl.size() < 1 ) {
    G4Exception("GmAnalysisMgr::AddHistoNorm",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histoNorm:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has none").c_str());
  } else if( wl.size() < 2 ) {
    G4Exception("GmAnalysisMgr::AddHistoNorm",
		"Wrong internal argument",
		FatalException,
		G4String("Command /gamos/analysis/histoNorm:Parameter not valid, must have at least two-words: 'HISTO_NAME VALUE', it has only one: "+wl[0]).c_str());
  }

  theHistoNormFactor[ wl[0] ] = GmGenUtils::GetValue(wl[1]);

}
 
 
//----------------------------------------------------------------------------
void GmAnalysisMgr::NormalizeToNEvents( GmHisto1* his )
{
  G4int NEvents = GmNumberOfEvent::GetNumberOfEvent();
  G4double nent = his->GetEntries();
  for( G4int ii = 0; ii < his->GetNbinsX()+1; ii++ ){
    double error = his->GetBinError(ii);
    his->SetBinContent(ii, his->GetBinContent(ii) / NEvents );
    his->SetBinError(ii, error / NEvents );
  }
  his->SetEntries(nent);

}


//----------------------------------------------------------------------------
void GmAnalysisMgr::NormalizeToNEvents( GmHistoProfile1* his )
{
  G4int NEvents = GmNumberOfEvent::GetNumberOfEvent();
  G4double nent = his->GetEntries();
  for( G4int ii = 0; ii < his->GetNbinsX()+1; ii++ ){
    his->SetBinContent(ii, his->GetBinContent(ii) / NEvents );
    his->SetBinError(ii, his->GetBinError(ii) / NEvents );
  }
  his->SetEntries(nent);
  
}

//----------------------------------------------------------------------------
void GmAnalysisMgr::NormalizeToNEvents( GmHisto2* his )
{
  G4int NEvents = GmNumberOfEvent::GetNumberOfEvent();
  G4double nent = his->GetEntries();
  for( G4int ix = 0; ix < his->GetNbinsX()+1; ix++ ){
    for( G4int iy = 0; iy < his->GetNbinsY()+1; iy++ ){
      his->SetBinContent(ix, iy, his->GetBinContent(ix,iy) / NEvents );
      his->SetBinError(ix, iy, his->GetBinError(ix,iy) / NEvents );
    }
  }
  his->SetEntries(nent);

}

//----------------------------------------------------------------------------
void GmAnalysisMgr::NormalizeToNEvents( GmHistoProfile2* his )
{
  G4int NEvents = GmNumberOfEvent::GetNumberOfEvent();
  G4double nent = his->GetEntries();
  for( G4int ix = 0; ix < his->GetNbinsX()+1; ix++ ){
    for( G4int iy = 0; iy < his->GetNbinsY()+1; iy++ ){
      his->SetBinContent(ix, iy, his->GetBinContent(ix,iy) / NEvents );
      his->SetBinError(ix, iy, his->GetBinError(ix,iy) / NEvents );
    }
  }
  his->SetEntries(nent);

}
