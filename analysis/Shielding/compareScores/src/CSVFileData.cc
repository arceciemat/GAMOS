#include "CSVFileData.hh"
#include "CSVScorerData.hh"
#include "ScoreData.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoReaderCSV.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

//----------------------------------------------------------------------
CSVFileData::CSVFileData(G4String fileName, G4bool bInvX)
{
  theFileName = fileName;
  theNEvents = 0.;
  bInvertX = bInvX;

  GmHistoReaderCSV reader(fileName);
  AddHistos( reader.GetHistos1() );
  AddHistos( reader.GetHistosProfile1() );
  // the histograms are not deleted: GmHistoReaderCSV does not delete them either
}

//----------------------------------------------------------------------
template<class H> void CSVFileData::AddHistos( const std::map<G4String,H*>& histos )
{
  for( typename std::map<G4String,H*>::const_iterator ite = histos.begin(); ite != histos.end(); ite++ ) {
    const H* his = ite->second;
    G4int nBins = his->GetNbinsX();
    std::vector<G4double> contents, errors;
    for( G4int ib = 0; ib < nBins+2; ib++ ) { // underflow, bins, overflow
      contents.push_back( his->GetBinContent(ib) );
      errors.push_back( his->GetBinError(ib) );
    }
    if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData from .csv file " << ite->first << G4endl;
    CSVScorerData* scorerData = new CSVScorerData( ite->first, nBins, his->GetBinLowEdge(1),
						   his->GetBinLowEdge(nBins)+his->GetBinWidth(nBins), contents, errors );
    if( bInvertX ) scorerData->InvertX();
    AddScorerData( scorerData );
  }
}

//----------------------------------------------------------------------
void CSVFileData::Print( std::ofstream& fout )
{
  for( size_t ii = 0; ii < theScorerData.size(); ii++ ) {
    theScorerData[ii]->Print(fout);
  }
}
