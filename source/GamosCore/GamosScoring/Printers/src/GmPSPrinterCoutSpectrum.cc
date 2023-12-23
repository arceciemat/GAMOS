#include "GmPSPrinterCoutSpectrum.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "G4UnitsTable.hh"


//--------------------------------------------------------------------
GmPSPrinterCoutSpectrum::GmPSPrinterCoutSpectrum(G4String name) : GmVPSPrinterSpectrum( name )
{
  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));

}

//--------------------------------------------------------------------
G4bool GmPSPrinterCoutSpectrum::IsSpectrum() {
  return true;
}

//--------------------------------------------------------------------
void GmPSPrinterCoutSpectrum::DumpAll( G4THitsMap<G4double>* , GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);
  mimid scorerVal = scorer->GetSpectrumSumV();
    //  if( theSumV.find( index ) == theSumV.end() ) return 0.;
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) {
    G4cout <<" GmPSPrinterCoutSpectrum::DumpAll() " << G4endl;
    G4cout << G4endl << " MultiFunctionalDet: " << scorer->GetMultiFunctionalDetector()->GetName() << G4endl; //GDEB
    G4cout << " PrimitiveScorer: " << scorer->GetName() << G4endl;
    G4cout << " Number of entries= " << scorerVal.size() << G4endl;
  }
#endif

  G4double aveALL = 0.;
  G4double aveIdxALL = 0.;
  G4double errorALL = 0.;
  GmVClassifier* classifier = scorer->GetClassifier();
  mimid::iterator itemm;
  G4double binWidth = scorer->GetSpectrumBinWidth();
  G4double minimum = scorer->GetSpectrumMinimum();
  G4bool bBinLog10 = scorer->GetSpectrumBinLog10();
  for(itemm = scorerVal.begin(); itemm != scorerVal.end(); itemm++){
    G4int index = itemm->first;
    mid* sumVIdx = itemm->second;
    aveIdxALL = 0.;
    mid* sumV2Idx = scorer->GetSpectrumSumV2(index);
    for( mid::const_iterator item = sumVIdx->begin(); item != sumVIdx->end(); item++ ) {
      G4int ibin = item->first;
      G4double sumX = item->second;
      G4double aveX;
      //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")
      aveX = sumX *theUnitRatio;
      G4cout << " IBIN " << ibin << " " << G4endl; //GDEB
      if( ! bBinLog10 ) {
	G4cout << "  index: " << classifier->GetIndexName(index) << " " << minimum+ibin*binWidth << " - " << minimum+(ibin+1)*binWidth << ":  = " << aveX;
      } else {
	G4cout << "  index: " << classifier->GetIndexName(index) << std::setprecision(6) << " " << minimum*pow(10,ibin*binWidth) << " - " << minimum*pow(10,(ibin+1)*binWidth) << ":  = " << aveX;
      }
      aveIdxALL += aveX;
      aveALL += aveX;
      if( scorer->ScoreErrors() ) {
	G4double error = scorer->GetErrorRelativeSpectrum( index, ibin, sumX);
	//   if( aveX != 0. ) error /= (aveX/theUnit); // relative error
	G4cout << " +-(REL) " << error << " " << theUnitName;
	errorALL += (error*aveX)*(error*aveX);
	if( bPrintSumV2 ) {
	  G4double sumV2 = sumV2Idx->at(ibin);
	  G4cout << " sumV2= " << sumV2*theUnitRatio2;
	}
	G4cout << G4endl;
      } else {
	G4cout << " " << theUnitName << G4endl;
      }
    }

    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterCoutSpectrum::DumpAll " << theUnit << " " << theUnitName << G4endl;

  errorALL = std::sqrt(errorALL);
  if( aveALL != 0. ) errorALL /= aveALL;
  G4cout << scorer->GetName() << " SUM_ALL: " << aveALL << " +-(REL) " << errorALL << " " << theUnitName;

  G4cout << G4endl;
}
