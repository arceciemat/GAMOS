#include "GmVHistoBuilder.hh"
#include "GmAnalysisVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------------------
std::set<G4int> GmVHistoBuilder::theHistoNumbers;

//----------------------------------------------------------------------------
GmVHistoBuilder::GmVHistoBuilder()
{
}

//----------------------------------------------------------------------------
void GmVHistoBuilder::SetHistoNameAndNumber( const G4String& fname, std::vector<GmVFilter*>& filters, GmVClassifier* classifier )
{
  BuildHistoName( fname, filters, classifier );
  BuildHistoFileName( fname, filters, classifier );

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4String userHName = paramMgr->GetStringValue(theHistoFileName+":FileName","");
#ifndef GAMOS_NO_VERBOSE
  if( AnaVerb(infoVerb) ) G4cout << " userHName " << userHName << " original hname " << theHistoFileName  << G4endl;
#endif

  if( userHName != "" )  {
    theHistoFileName = userHName;
    //-    BuildHistoFileName( userHName, filters, classifier );
  }

  BuildHistoNumber();
  theAnaMgr = GmAnalysisMgr::GetInstance(theHistoFileName);

#ifndef GAMOS_NO_VERBOSE
  if( AnaVerb(infoVerb) ) G4cout << " GmVHistoBuilder: FileName= " << theHistoFileName << " HistoFileName= " << theHistoFileName << " HistoNumber= " << theHistoNumber << G4endl;
#endif

  //t  bPrintHistoStats = GmParameterMgr::GetInstance()->GetNumericValue(theHistoFileName+":PrintHistoStats",0);

}

//----------------------------------------------------------------------------
void GmVHistoBuilder::BuildHistoName(const G4String& name, std::vector<GmVFilter*>& filters, GmVClassifier* classifier )
{
  theHistoName = name;
  for( unsigned int ii = 0; ii < filters.size(); ii++ ){
    theHistoName += "_" + filters[ii]->GetName();
  }
  if( classifier ) theHistoName += "_" + classifier->GetName();
}

//----------------------------------------------------------------------------
void GmVHistoBuilder::BuildHistoFileName(const G4String& name, std::vector<GmVFilter*>& filters, GmVClassifier* classifier )
{
  theHistoFileName = name;
  for( size_t ii = 0; ii < filters.size(); ii++ ){
    theHistoFileName += "_" + filters[ii]->GetName();
  }
  if( classifier ) theHistoFileName += "_" + classifier->GetName();
}

//----------------------------------------------------------------------------
void GmVHistoBuilder::BuildHistoNumber()
{
  G4int hn;
  do{
    hn = G4int(CLHEP::RandFlat::shoot()*1000)*1000000;
  } while( theHistoNumbers.find(hn) != theHistoNumbers.end() );
  
  theHistoNumbers.insert(hn);
  theHistoNumber = hn;
}

//----------------------------------------------------------------------------
void GmVHistoBuilder::PrintHisto( G4int hn )
{
#ifndef GAMOS_NO_ROOT
  TH1F* his = theAnaMgr->GetHisto1(hn);
#else
  GmHisto1* his = theAnaMgr->GetHisto1(hn);
#endif
  
  G4cout << theHistoFileName << " " << his->GetName() << " : " <<  his->GetEntries() << " MEAN " << his->GetMean() << " +- " << his->GetRMS() << " UNDER " << his->GetBinContent(0) << " OVER " << his->GetBinContent(his->GetNbinsX()) << G4endl;
}
