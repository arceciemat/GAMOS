#include "GmPSPrinterCout.hh"
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


GmPSPrinterCout::GmPSPrinterCout(G4String name) : GmVPSPrinter( name )
{
  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));

}


void GmPSPrinterCout::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  SetUnit(scorer);
  
  //  G4cout <<" GmPSPrinterCout::DumpAll() " << G4endl;
  G4cout << G4endl << " MultiFunctionalDet: " << scorer->GetMultiFunctionalDetector()->GetName() << G4endl;
  G4cout << " PrimitiveScorer: " << scorer->GetName() << G4endl;
  G4cout << " Number of entries= " << RunMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator ite;

  G4double aveALL = 0.;
  G4double errorALL = 0.;
  GmVClassifier* classifier = scorer->GetClassifier();
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){

    G4double sumX = (*(ite->second));
    G4double aveX;
    //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")
    aveX = sumX *theUnitRatio;
    
    G4cout << "  index: " << classifier->GetIndexName(ite->first) << "  = " << aveX;
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

    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterCout::DumpAll " << theUnit << " " << theUnitName << G4endl;

  errorALL = std::sqrt(errorALL);
  if( aveALL != 0. ) errorALL /= aveALL;
  G4cout << scorer->GetName() << " SUM_ALL: " << aveALL << " +-(REL) " << errorALL << " " << theUnitName;

  G4cout << G4endl;
}
