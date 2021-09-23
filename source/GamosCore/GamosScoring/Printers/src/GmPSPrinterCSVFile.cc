#include "GmPSPrinterCSVFile.hh"
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


GmPSPrinterCSVFile::GmPSPrinterCSVFile(G4String name) : GmVPSPrinter( name )
{
  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));
  bPrintHeader = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintHeader",0));

  G4cout << theName << G4endl;
}


void GmPSPrinterCSVFile::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  // Variables fo GmTextIOMgr
  theOutputType = theName + ".csv";
  Init();
  OpenFileOut();

  SetUnit(scorer);

  //  G4cout <<" GmPSPrinterCSVFile::DumpAll() " << G4endl;
   if( bPrintHeader ) {
     *theFileOut << "MultiFunctionalDet: " << scorer->GetMultiFunctionalDetector()->GetName() << G4endl;
     *theFileOut << "PrimitiveScorer: " << scorer->GetName() << G4endl;
     *theFileOut << "Number of entries= " << RunMap->entries() << G4endl << G4endl;
     *theFileOut << "Index [#]" << ", ";
     *theFileOut << scorer->GetName() << " [" << theUnitName << "], ";
     if( scorer->ScoreErrors() ) {
       *theFileOut << "+/- (REL ERROR)" << " [" << theUnitName <<"], "; 
     }
     if( bPrintSumV2 ) {
       *theFileOut << "sumV2" << " [" << theUnitName <<"^2]";
     }
  }
  *theFileOut << G4endl;
  std::map<G4int,G4double*>::iterator ite;

  G4double aveALL = 0.;
  G4double errorALL = 0.;
  GmVClassifier* classifier = scorer->GetClassifier();
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){

    G4double sumX = (*(ite->second));
    G4double aveX;
    //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")

    aveX = sumX *theUnitRatio;
    
    *theFileOut << classifier->GetIndexName(ite->first) << ", " << aveX;
    aveALL += aveX;
    
    if( scorer->ScoreErrors() ) {
      G4double error = scorer->GetErrorRelative( ite->first, sumX );
      //      if( aveX != 0. ) error /= aveX; // relative error
      *theFileOut << ", "<< error;
      errorALL += (error*aveX)*(error*aveX);
      if( bPrintSumV2 ) {
	*theFileOut << ", " << scorer->GetSumV2(ite->first)*theUnitRatio2;
      }
      *theFileOut << G4endl;
    } else {
      *theFileOut << G4endl;
    }

  }
  //  G4cout << this << " GmPSPrinterCSVFile::DumpAll " << theUnit << " " << theUnitName << G4endl;  
  if( bPrintHeader) {
    errorALL = std::sqrt(errorALL);
    if( aveALL != 0. ) errorALL /= aveALL;
    *theFileOut << G4endl << "SUM_ALL: " << aveALL << " +/- " << errorALL << " " << theUnitName;  
  }
  *theFileOut << G4endl;

  CloseFileOut();
}
