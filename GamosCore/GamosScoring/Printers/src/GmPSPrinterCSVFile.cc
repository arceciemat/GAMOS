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
  theUnit = -1.;
  theUnitName = G4String("");

  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));
  bPrintHeader = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintHeader",0));

G4cout << theName << G4endl;
}


void GmPSPrinterCSVFile::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 2 && params.size() != 0 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterCSVFile::SetParameters",
		"There should be two parameters: UNIT UNIT_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ) {
    theUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << this << " GmPSPrinterCSVFile::SetParameters " << theUnit << " " << theUnitName << G4endl;
#endif
  }

}


void GmPSPrinterCSVFile::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{
  // Variables fo GmTextIOMgr
  theOutputType = theName + ".csv";
  Init();
  OpenFileOut();

  if( theUnit == -1 ) {
    theUnit = theScorer->GetUnit();
    theUnitName = theScorer->GetUnitName();
  }

  //  G4cout <<" GmPSPrinterCSVFile::DumpAll() " << G4endl;
   if( bPrintHeader ) {
  	  *theFileOut << "MultiFunctionalDet: " << theScorer->GetMultiFunctionalDetector()->GetName() << G4endl;
	  *theFileOut << "PrimitiveScorer: " << theScorer->GetName() << G4endl;
	  *theFileOut << "Number of entries= " << RunMap->entries() << G4endl << G4endl;
	  *theFileOut << "Index [#]" << ", ";
	  *theFileOut << theScorer->GetName() << " [" << theUnitName << "], ";
	  if( theScorer->ScoreErrors() ) {
	    *theFileOut << "+/- (REL ERROR)" << " [" << theUnitName <<"], "; 
	  }
	  if( bPrintSumV2 ) {
		*theFileOut << "sumV2" << " [" << theUnitName <<"^2]";
	  }
  }
  *theFileOut << G4endl;
  std::map<G4int,G4double*>::iterator ite;
  G4double nev;
  //??  G4bool bScoreByEvent = theScorer->ScoreByEvent();

  if( bScoreByEvent ) {
    nev = GmNumberOfEvent::GetNumberOfEvent();
  } else {
    nev = 1;
  }

  G4double aveALL = 0.;
  G4double errorALL = 0.;
  GmVClassifier* classifier = theScorer->GetClassifier();
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){

    G4double sumX = (*(ite->second));
    G4double aveX;
    //	   << "  dose deposit= " << G4BestUnit(*(ite->second),"Dose")

    if( bScoreByEvent ){
      aveX = sumX/nev;
    } else {
      aveX = sumX;
    } 
    
    *theFileOut << classifier->GetIndexName(ite->first) << ", " << aveX/theUnit;
    aveALL += aveX;
	
	if( theScorer->ScoreErrors() ) {
      G4double error = theScorer->GetErrorRelative( ite->first, sumX, nev );
      //      if( aveX != 0. ) error /= aveX; // relative error
      *theFileOut << ", "<< error;
      errorALL += (error*aveX)*(error*aveX);
      if( bPrintSumV2 ) {
	*theFileOut << ", " << theScorer->GetSumV2(ite->first)/(theUnit*theUnit);
      }
      *theFileOut << G4endl;
    } else {
      *theFileOut << G4endl;
    }

    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterCSVFile::DumpAll " << theUnit << " " << theUnitName << G4endl;  
  if( bPrintHeader) {
  *theFileOut << G4endl << "SUM ALL: " << aveALL/theUnit << " +/- " << std::sqrt(errorALL)/theUnit << " " << theUnitName;  
  }
  *theFileOut << G4endl;

  CloseFileOut();
}
