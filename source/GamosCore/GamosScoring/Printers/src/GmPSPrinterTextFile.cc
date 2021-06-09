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
  theUnit = -1.;
  theUnitName = G4String("");

  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));
}


void GmPSPrinterTextFile::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 2 && params.size() != 0 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterTextFile::SetParameters",
		"There should be two parameters: UNIT UNIT_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ) {
    theUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << this << " GmPSPrinterTextFile::SetParameters " << theUnit << " " << theUnitName << G4endl;
#endif
  }

}


void GmPSPrinterTextFile::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{
  // Variables fo GmTextIOMgr
  theOutputType = theName;
  Init();
  OpenFileOut();

  if( theUnit == -1 ) {
    theUnit = theScorer->GetUnit();
    theUnitName = theScorer->GetUnitName();
  }

  //  G4cout <<" GmPSPrinterTextFile::DumpAll() " << G4endl;
  *theFileOut << G4endl << " MultiFunctionalDet: " << theScorer->GetMultiFunctionalDetector()->GetName() << G4endl;
  *theFileOut << " PrimitiveScorer: " << theScorer->GetName() << G4endl;
  *theFileOut << " Number of entries= " << RunMap->entries() << G4endl;
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
    
    *theFileOut << "  index: " << classifier->GetIndexName(ite->first) << "  = " << aveX/theUnit;
    aveALL += aveX;
    if( theScorer->ScoreErrors() ) {
      G4double error = theScorer->GetErrorRelative( ite->first, sumX, nev );
      //      if( aveX != 0. ) error /= aveX; // relative error
      *theFileOut << " +-(REL) " << error << " " << theUnitName;
      errorALL += (error*aveX)*(error*aveX);
      if( bPrintSumV2 ) {
	*theFileOut << " sumW2= " << theScorer->GetSumV2(ite->first)/(theUnit*theUnit);
      }
      *theFileOut << G4endl;
    } else {
      *theFileOut << " " << theUnitName << G4endl;
    }
    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterTextFile::DumpAll " << theUnit << " " << theUnitName << G4endl;

  *theFileOut << theScorer->GetName() << " SUM ALL: " << aveALL/theUnit << " +/- " << std::sqrt(errorALL)/theUnit << " " << theUnitName;

  *theFileOut << G4endl;

  CloseFileOut();
}
