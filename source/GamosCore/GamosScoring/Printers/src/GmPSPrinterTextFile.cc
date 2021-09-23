#include "GmPSPrinterTextFile.hh"
#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#include "G4PhantomParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"


GmPSPrinterTextFile::GmPSPrinterTextFile(G4String name) : GmVPSPrinter( name )
{
  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));
}


void GmPSPrinterTextFile::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  // Variables fo GmTextIOMgr
  theOutputType = theName;
  Init();
  OpenFileOut();

  SetUnit(scorer);

  //  G4cout <<" GmPSPrinterTextFile::DumpAll() " << G4endl;
  *theFileOut << G4endl << " MultiFunctionalDet: " << scorer->GetMultiFunctionalDetector()->GetName() << G4endl;
  *theFileOut << " PrimitiveScorer: " << scorer->GetName() << G4endl;
  *theFileOut << " Number of entries= " << RunMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator ite;

  G4double aveALL = 0.;
  G4double errorALL = 0.;
  GmVClassifier* classifier = scorer->GetClassifier();
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){

    G4double sumX = (*(ite->second));
    G4double aveX;

    aveX = sumX *theUnitRatio;
    
    *theFileOut << "  index: " << classifier->GetIndexName(ite->first) << "  = " << aveX;
    aveALL += aveX;
    if( scorer->ScoreErrors() ) {
      G4double error = scorer->GetErrorRelative( ite->first, sumX);
      //      if( aveX != 0. ) error /= aveX; // relative error
      *theFileOut << " +-(REL) " << error << " " << theUnitName;
      errorALL += (error*aveX)*(error*aveX);
      if( bPrintSumV2 ) {
	*theFileOut << " sumW2= " << scorer->GetSumV2(ite->first)*theUnitRatio2;
      }
      *theFileOut << G4endl;
    } else {
      *theFileOut << " " << theUnitName << G4endl;
    }
  }
  //  G4cout << this << " GmPSPrinterTextFile::DumpAll " << theUnit << " " << theUnitName << G4endl;
  errorALL = std::sqrt(errorALL);
  if( aveALL != 0. ) errorALL /= aveALL;
  
  *theFileOut << scorer->GetName() << " SUM_ALL: " << aveALL << " +/- " << errorALL << " " << theUnitName;

  *theFileOut << G4endl;

  CloseFileOut();
}
