#include "GmPSPrinterBinFile.hh"
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


GmPSPrinterBinFile::GmPSPrinterBinFile(G4String name) : GmVPSPrinter( name )
{
  theUnit = -1.;
  theUnitName = G4String("");

  bPrintSumV2 = G4bool( GmParameterMgr::GetInstance()->GetNumericValue(theName+":PrintSumV2",1));

}

void GmPSPrinterBinFile::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 2 && params.size() != 0 ){ //if 0 parameters, scorer unit/unitname is taken
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterBinFile::SetParameters",
		"There should be two parameters: UNIT UNIT_NAME",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }

  if( params.size() == 2 ) {
    theUnit = GmGenUtils::GetValue( params[0] );
    theUnitName = params[1];
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << this << " GmPSPrinterBinFile::SetParameters " << theUnit << " " << theUnitName << G4endl;
#endif    
  }

}


void GmPSPrinterBinFile::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer )
{
  // Variables fo GmBinIOMgr
  theOutputType = theName;
  Init();
  OpenFileOut();

  if( theUnit == -1 ) {
    theUnit = theScorer->GetUnit();
    theUnitName = theScorer->GetUnitName();
  }

  const char* dataSTR30= new char[30];
  float dataF;
  const char* dataSTR20 = new char[20];
  const char* dataSTR10 = new char[10];

  dataSTR30 = theScorer->GetMultiFunctionalDetector()->GetName();
  fwrite (dataSTR30, sizeof(char),30,theFileOut);

  dataSTR30 = theScorer->GetName();
  fwrite (dataSTR30, sizeof(char),30,theFileOut);

  dataSTR10 = theUnitName;
  fwrite (dataSTR10, sizeof(char),10,theFileOut);

  fwrite (&theUnit, sizeof(float),1,theFileOut);

  unsigned int nEntries = RunMap->entries();
  fwrite (&nEntries, sizeof(unsigned int),1,theFileOut);

  std::map<G4int,G4double*>::iterator ite;

  G4double nev;
  if( bScoreByEvent ) {
    nev = GmNumberOfEvent::GetNumberOfEvent();
  } else {
    nev = 1;
  }
  //  G4double aveALL = 0.;
  //  G4double errorALL = 0.;
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
    
    dataSTR20 = classifier->GetIndexName(ite->first);
    dataF = aveX/theUnit;
    fwrite (dataSTR20, sizeof(char),20,theFileOut);
    fwrite (&dataF, sizeof(float),1,theFileOut);

    //    aveALL += aveX;
    if( theScorer->ScoreErrors() && bPrintSumV2 ) {
      if( ite != RunMap->GetMap()->end() ){
	dataF = theScorer->GetSumV2(ite->first)/(theUnit*theUnit);
      } else {
	dataF = 0.;
      }    
      fwrite (&dataF, sizeof(float),1,theFileOut);
      //      errorALL += (error*aveX)*(error*aveX);
     
    }
    //    G4cout << "  copy no.:EV " << bScoreByEvent << " = " << sumX << " nev " << nev << " unit " << theUnit << G4endl;
  }
  //  G4cout << this << " GmPSPrinterBinFile::DumpAll " << theUnit << " " << theUnitName << G4endl;

  /*  dataF = aveALL/theUnit;
  fwrite (&dataF, sizeof(float),1,theFileOut);
  dataF =  std::sqrt(errorALL)/theUnit;
  fwrite (&dataF, sizeof(float),1,theFileOut); */

  CloseFileOut();
}
