#include "GmPSPrinterHistosSpectrum.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmCompoundClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "G4PhantomParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"


//--------------------------------------------------------------------
GmPSPrinterHistosSpectrum::GmPSPrinterHistosSpectrum(G4String name) : GmVPSPrinterSpectrum( name )
{

}

//--------------------------------------------------------------------
G4bool GmPSPrinterHistosSpectrum::IsSpectrum()
{
  return true;
}

//--------------------------------------------------------------------
void GmPSPrinterHistosSpectrum::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() > 4 ){ 
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterHistosSpectrum::SetParameters",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("There should be not more than 4 parameters: NAME NBINS MIN MAX. There are: "+parastr).c_str());
  }

  if( params.size() >= 1 ) {
    theName = params[0];
  } else {
    theName = "";
  }
  if( params.size() >= 2 ) {
    theNBins = G4int(GmGenUtils::GetValue( params[1] ));
  } else {
    theNBins = INT_MAX;
  }
  if( params.size() >= 3 ) {
    theMin = GmGenUtils::GetValue( params[2] );
  } else {
    theMin = DBL_MAX;
  }
  if( params.size() >= 4 ) {
    theMax = GmGenUtils::GetValue( params[3] );
  } else {
    theMax = DBL_MAX;
  }
#ifndef GAMOS_NO_VERBOSE 
  if( ScoringVerb(debugVerb) ) G4cout << this << " GmPSPrinterHistosSpectrum::SetParameters for axis " << theName 
				      << " NBINS= " << theNBins 
				      << " MIN= " << theMin
				      << " MAX= " << theMax << G4endl;
#endif

}


//--------------------------------------------------------------------
void GmPSPrinterHistosSpectrum::DumpAll( G4THitsMap<G4double>* , GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);
  if( theName == "" ) {
    theName = "hist"+scorer->GetName() + "_" + GetName();
  }
  if( theNBins == INT_MAX ) {
    theNBins = scorer->GetSpectrumNBins();
  }
  if( theMin == DBL_MAX ) {
    theMin = scorer->GetSpectrumMinimum();
  }
  if( theMax == DBL_MAX ) {
    theMax = scorer->GetSpectrumMaximum();
  }
  
  // Variables fo GmTextIOMgr
  std::vector<GmVFilter*> filters;
  SetHistoNameAndNumber(theName,filters,(GmVClassifier*)0);
  theAnaMgr->SetDefaultNormalize(false);

  mimid scorerVal = scorer->GetSpectrumSumV();
  G4bool bBinLog10 = scorer->GetSpectrumBinLog10();
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) {
    G4cout <<" GmPSPrinterHistosSpectrum::DumpAll() " << G4endl;
    G4cout << G4endl << " MultiFunctionalDet: " << scorer->GetMultiFunctionalDetector()->GetName() << G4endl; //GDEB
    G4cout << " PrimitiveScorer: " << scorer->GetName() << G4endl;
    G4cout << " Number of entries= " << scorerVal.size() << G4endl;
  }
#endif

  mimid::iterator itemm;
  for(itemm = scorerVal.begin(); itemm != scorerVal.end(); itemm++){
    G4int index = itemm->first;
    G4double nev = scorer->GetNEvents( index );
    mid* sumVIdx = itemm->second;
    if( !bBinLog10 ) {
      theAnaMgr->CreateHisto1D( theName+GmGenUtils::itoa(index),theNBins,theMin,theMax,theHistoNumber + index);
    } else {
      theAnaMgr->CreateHisto1D( theName+GmGenUtils::itoa(index),theNBins,log10(theMin),log10(theMax),theHistoNumber + index);
    }
    GmHisto1* his = theAnaMgr->GetHisto1D( theHistoNumber + index);
    for( mid::const_iterator item = sumVIdx->begin(); item != sumVIdx->end(); item++ ) {
      G4int ibin = item->first;
      G4double sumX = item->second;
      G4double aveX;
      //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")
      aveX = sumX *theUnitRatio;
      his->SetBinContent( ibin, aveX*nev ); //scorer is normalized to nev and histogram will be normalized again
      if( scorer->ScoreErrors() ) {
	G4double error = scorer->GetErrorSpectrum( index, ibin );
	//   if( aveX != 0. ) error /= (aveX/theUnit); // relative error
	G4cout << theUnitRatio << " VAL " << sumX << " " << aveX << " +- " << error << G4endl; //GDEB
	his->SetBinError( ibin, error*nev );
      }
    }

    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterCoutSpectrum::DumpAll " << theUnit << " " << theUnitName << G4endl;

}
