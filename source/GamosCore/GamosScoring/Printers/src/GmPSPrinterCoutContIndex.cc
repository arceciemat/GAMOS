#include "GmPSPrinterCoutContIndex.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4PhantomParameterisation.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"


GmPSPrinterCoutContIndex::GmPSPrinterCoutContIndex(G4String name) : GmVPSPrinter( name )
{
  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));
  theFirstIndex = G4int( GmParameterMgr::GetInstance()->GetNumericValue(theName+":FirstIndex",0));
  theLastIndex = G4int( GmParameterMgr::GetInstance()->GetNumericValue(theName+":LastIndex",100));

  
}


void GmPSPrinterCoutContIndex::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);
  
  //  G4cout <<" GmPSPrinterCoutContIndex::DumpAll() " << G4endl;
  G4cout << G4endl << " MultiFunctionalDet: " << scorer->GetMultiFunctionalDetector()->GetName() << G4endl;
  G4cout << " PrimitiveScorer: " << scorer->GetName() << G4endl;
  G4cout << " Number of entries= " << RunMap->entries() << G4endl;
  G4double aveALL = 0.;
  G4double errorALL = 0.;
  std::map<G4int,G4double*>* scorerMap = RunMap->GetMap();
  for( G4int ii = theFirstIndex; ii <= theLastIndex; ii++ ){
    std::map<G4int,G4double*>::iterator ite = scorerMap->find( ii );
    if( ite != scorerMap->end() ){
      G4double sumX = (*(ite->second));
      G4double aveX;
      //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")
      aveX = sumX *theUnitRatio;
      G4cout << "  index: " << ite->first << "  = " << aveX;
      aveALL += aveX;
      if( scorer->ScoreErrors() ) {
        G4double error = scorer->GetErrorRelative( ite->first, sumX );
        //   if( aveX != 0. ) error /= (aveX/theUnit); // relative error
        G4cout << " +-(REL) " << error << " " << theUnitName;
        errorALL += (error*aveX)*(error*aveX);
        if( bPrintSumV2 ) {
	  G4cout << " sumV2= " << scorer->GetSumV2(ite->first)*theUnitRatio2;
	}
	G4cout << G4endl;
      } else {
	G4cout << " " << theUnitName << G4endl;
      }
    } else {
      G4cout << "  index: " << ii  << "  = " << "0.";
      if( scorer->ScoreErrors() ) {
        //   if( aveX != 0. ) error /= (aveX/theUnit); // relative error
        G4cout << " +-(REL) " << "0." << " " << theUnitName;
        if( bPrintSumV2 ) {
	  G4cout << " sumV2= " << "0.";
	}
	G4cout << G4endl;
      } else {
	G4cout << " " << theUnitName << G4endl;
      }
    }

    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterCoutContIndex::DumpAll " << theUnit << " " << theUnitName << G4endl;

  errorALL = std::sqrt(errorALL);
  if( aveALL != 0. ) errorALL /= aveALL;
  G4cout << scorer->GetName() << " SUM_ALL: " << aveALL << " +-(REL) " << errorALL << " " << theUnitName;

  G4cout << G4endl;
}
