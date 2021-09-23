#include "GmPSPrinterHistos.hh"
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
GmPSPrinterHistos::GmPSPrinterHistos(G4String name) : GmVPSPrinter( name )
{

}

//--------------------------------------------------------------------
void GmPSPrinterHistos::SetParameters( const std::vector<G4String>& params )
{
  if( params.size() != 4 && params.size() != 8 && params.size() != 9 ){ 
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmPSPrinterHistos::SetParameters",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("There should be 4 parameters for 1D histos and 8 for 2D histos (plus optionally 1 for HistoEachRow): NAME NBINS MIN MAX. There are: "+parastr).c_str());
  }

  theNameX = params[0];
  theNBinsX = G4int(GmGenUtils::GetValue( params[1] ));
  theMinX = GmGenUtils::GetValue( params[2] );
  theMaxX = GmGenUtils::GetValue( params[3] );
  b2D = FALSE;
#ifndef GAMOS_NO_VERBOSE 
  if( ScoringVerb(debugVerb) ) G4cout << this << " GmPSPrinterHistos::SetParameters for histo X axis " << theNameX 
				      << " NBINS= " << theNBinsX 
				      << " MIN= " << theMinX
				      << " MAX= " << theMaxX 
				      << " OFFSET= " << theOffsetX << G4endl;
#endif

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  theOffsetX = paramMgr->GetNumericValue(theName+sepa+"OffsetX",0);

  if( params.size() >= 8 ) {
    b2D = TRUE;
    theNameY = params[4];
    theNBinsY = G4int(GmGenUtils::GetValue( params[5] ));
    theMinY = GmGenUtils::GetValue( params[6] );
    theMaxY = GmGenUtils::GetValue( params[7] );
    theOffsetY = paramMgr->GetNumericValue(theName+sepa+"OffsetY",0);
    bHistoEachRow = 0;
    if( params.size() == 9 ) {
      bHistoEachRow = G4bool(GmGenUtils::GetValue( params[8] ));
    }
      
#ifndef GAMOS_NO_VERBOSE 
    if( ScoringVerb(debugVerb) ) G4cout << this << " GmPSPrinterHistos::SetParameters for histo Y axis " << theNameY 
					<< " NBINS= " << theNBinsY 
					<< " MIN= " << theMinY
					<< " MAX= " << theMaxY 
					<< " OFFSET= " << theOffsetY 
					<< " Histo each row " << bHistoEachRow << G4endl;
#endif
  }
  

}


//--------------------------------------------------------------------
void GmPSPrinterHistos::DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* scorer )
{
  if( b2D ) {
    if( !scorer->GetClassifier()->IsCompound() ) {
      G4Exception("GmPSPrinterHistos::DumpAll",
		  G4String("Error in SCORER= " + scorer->GetName() + "  CLASSIFIER= " +  scorer->GetClassifier()->GetName()).c_str(),
		  JustWarning,
		  G4String("Making a 2D histogram is not possible if you do not use a GmCompoundClassifier. SCORER= " + scorer->GetName() + "  CLASSIFIER= " +  scorer->GetClassifier()->GetName()).c_str());
      theNShift = theNBinsX;
    } else {
      GmCompoundClassifier* classif = (GmCompoundClassifier*)(scorer->GetClassifier());
      theNShift = classif->GetNShift();
      /* if( theNShift != theNBinsY ) {
	G4Exception("GmPSPrinterHistos::DumpAll",
		    G4String("Error in SCORER= " + scorer->GetName() + "  CLASSIFIER= " +  scorer->GetClassifier()->GetName()).c_str(),
		    JustWarning,
		    G4String("Number of values in classifier = " + GmGenUtils::itoa(theNShift) + " is different than in histogram = " + GmGenUtils::itoa(theNBinsY)).c_str());
		    }*/
    }
  }

  // Variables fo GmTextIOMgr
  std::vector<GmVFilter*> filters;
  SetHistoNameAndNumber(theName,filters,(GmVClassifier*)0);
  theAnaMgr->SetDefaultNormalize(false);

  theAnaMgr->CreateHisto1D( theNameX,theNBinsX,theMinX,theMaxX,theHistoNumber+1);
  GmHisto1* hisX = theAnaMgr->GetHisto1(theHistoNumber+1);
#ifndef GAMOS_NO_ROOT
  for( int ii = 0; ii < hisX->GetXaxis()->GetNbins()+1; ii++ ){
#else
  for( int ii = 0; ii < hisX->GetNbins()+1; ii++ ){
#endif
    hisX->SetBinContent( ii, 0. );
  }
  GmHisto1* hisY;
  GmHisto2* hisXY;
  if( b2D ) {
    theAnaMgr->CreateHisto1D( theNameY,theNBinsY,theMinY,theMaxY,theHistoNumber+2);
    theAnaMgr->CreateHisto2D( theNameX+".vs."+theNameY,theNBinsX,theMinX,theMaxX,theNBinsY,theMinY,theMaxY,theHistoNumber+3);
    hisY = theAnaMgr->GetHisto1(theHistoNumber+2);
#ifndef GAMOS_NO_ROOT
    for( int iy = 0; iy < hisY->GetXaxis()->GetNbins()+1; iy++ ){
#else
    for( int iy = 0; iy < hisY->GetNbins()+1; iy++ ){
#endif
      hisY->SetBinContent( iy, 0. );
    }
    hisXY = theAnaMgr->GetHisto2(theHistoNumber+3);
#ifndef GAMOS_NO_ROOT
    for( int ix = 0; ix < hisXY->GetXaxis()->GetNbins()+1; ix++ ){
      for( int iy = 0; iy < hisXY->GetYaxis()->GetNbins()+1; iy++ ){
#else
    for( int ix = 0; ix < hisXY->GetNbinsX()+1; ix++ ){
      for( int iy = 0; iy < hisXY->GetNbinsY()+1; iy++ ){
#endif 
	hisXY->SetBinContent( ix, iy, 0. );
      }
    }
    if( bHistoEachRow ) {
      G4double stepX = (theMaxX-theMinX)/theNBinsX;
      G4double stepY = (theMaxY-theMinY)/theNBinsY;
      for( int ix = 0; ix < theNBinsX; ix++ ){
	theAnaMgr->CreateHisto1D( theNameX+"_"+GmGenUtils::ftoa(theMinX+ix*stepX)+"-"+GmGenUtils::ftoa(theMinX+(ix+1)*stepX),theNBinsY,theMinY,theMaxY,theHistoNumber+10000+ix);
      }
      for( int iy = 0; iy < theNBinsY; iy++ ){
	theAnaMgr->CreateHisto1D( theNameY+"_"+GmGenUtils::ftoa(theMinY+iy*stepY)+"-"+GmGenUtils::ftoa(theMinY+(iy+1)*stepY),theNBinsX,theMinX,theMaxX,theHistoNumber+20000+iy);
      }

    }
  }

  std::map<G4int,G4double*>::iterator ite;

  G4int startWith0 = 0;
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){
    if(  ite->first == 0 ) {
      startWith0 = 1;
    }
  }

  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){

    G4double sumVal = (*(ite->second));
    G4double aveVal;
    aveVal = sumVal;
 
    G4int index;
    if( !b2D ) {
      index = ite->first + startWith0;
    } else {
      index = (ite->first)%theNShift + startWith0; 
    }
    if( index < 0 ) index = 0;
    if( index > theNBinsX ) index = theNBinsX;


#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(warningVerb) ) {
      if( index < theOffsetX || index > theOffsetX+theNBinsX ) {
	G4Exception("GmPSPrinterHisto",
		    "Classifier index value is outside limits",
		    JustWarning,
		    G4String("CLASSIFIER= " + theName + " INDEX= " + GmGenUtils::itoa(index) + " MIN_INDEX= " + GmGenUtils::ftoa(theOffsetX) + " MAX_INDEX= " + GmGenUtils::ftoa(theOffsetX+theNBinsX)).c_str());
      }
    }
#endif

#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) {
      G4cout << "GmPSPrinterHistos " << theName << " X Add BinContent " << ite->first << ": " << index << " = " << aveVal << G4endl;
    }
#endif
    G4double errorPast = hisX->GetBinError( index );
    hisX->AddBinContent( index, aveVal );
    // MEAN AND RMS?
    
    if( scorer->ScoreErrors() ) {
      G4double error = scorer->GetErrorRelative( ite->first, sumVal)*aveVal;
      hisX->SetBinError( index, sqrt(sqr(error)+sqr(errorPast)));
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) {
	G4cout << "GmPSPrinterHistos " << theName << " X Add BinError " << ite->first << ": " << index << " = " << error << " errorPast " << errorPast << " TOTAL " <<  hisX->GetBinError( index ) << G4endl;
      }
#endif
    }
    
    if( b2D ) {
      G4int indexX = (ite->first)%theNShift + startWith0;
      G4int indexY = (ite->first)/theNShift + startWith0;
      //      G4cout << " indexX " << indexX << " indexY " << indexY << " index " << (ite->first) << " nshift " << theNShift << " startWith0 " << startWith0 << G4endl; //GDEB
      if( indexX < 0 ) indexX = 0;
      if( indexX > theNBinsX ) indexX = theNBinsX;
      if( indexY < 0 ) indexY = 0;
      if( indexY > theNBinsY ) indexY = theNBinsY;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(warningVerb) ) {
	if( indexX < theOffsetX || indexX > theOffsetX+theNBinsX ) {
	  G4Exception("GmPSPrinterHisto",
		      "Classifier indexX value is outside limits",
		      JustWarning,
		      G4String("CLASSIFIER= " + theName + " INDEX= " + GmGenUtils::itoa(indexX) + " MIN_INDEX= " + GmGenUtils::ftoa(theOffsetX) + " MAX_INDEX= " + GmGenUtils::ftoa(theOffsetX+theNBinsX)).c_str());
	}
	if( indexY < theOffsetY || indexY > theOffsetY+theNBinsY ) {
	  G4Exception("GmPSPrinterHisto",
		      "Classifier indexY value is outside limits",
		      JustWarning,
		      G4String("CLASSIFIER= " + theName + " INDEX= " + GmGenUtils::itoa(indexY) + " MIN_INDEX= " + GmGenUtils::ftoa(theOffsetY) + " MAX_INDEX= " + GmGenUtils::ftoa(theOffsetY+theNBinsY)).c_str());
	}
      }
#endif
      
      G4double errorPastY = hisY->GetBinError( indexY );
      hisY->AddBinContent( indexY, aveVal );
      G4double errorPastXY = hisXY->GetBinError( indexX, indexY );
      hisXY->SetBinContent( indexX, indexY, aveVal + hisXY->GetBinContent(indexX,indexY) );
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) {
	G4cout << "GmPSPrinterHistos " << theName << " Y Add BinContent " << ite->first << ": " << indexY << " = " << aveVal << " TOTAL= " << hisY->GetBinContent( indexY ) << G4endl;
	G4cout << "GmPSPrinterHistos " << theName << " XY Set BinContent " << ite->first << ": " << indexX << " , " << indexY << " = " << aveVal << G4endl;
      }
#endif
      GmHisto1* hisY1 = 0;
      GmHisto1* hisX1 = 0;
      if( bHistoEachRow ) {
	if( indexX < 0 || indexX >= theNBinsX ) {
          hisY1 = 0;
	  G4Exception("GmPSPrinterHistos::DumpAll",
		      "Out of limits",
		      JustWarning,
		      G4String("Index X is out of limits, histo per row will not be filled. Index= " + GmGenUtils::itoa(indexX) + " Value skipped="+GmGenUtils::ftoa(aveVal)).c_str());
	} else {
	  hisY1 = theAnaMgr->GetHisto1(theHistoNumber+10000+indexX);
	  hisY1->AddBinContent( indexY, aveVal );
#ifndef GAMOS_NO_VERBOSE
          if( ScoringVerb(debugVerb) ) {
            G4cout << "GmPSPrinterHistos " << theName << " Y1 Add BinContent " << ite->first << ": " << indexY << " = " << aveVal << " TOTAL= " << hisY1->GetBinContent( indexY ) << G4endl;
          }
#endif
        }

	if( indexY < 0 || indexY >= theNBinsY ) {
          hisX1 = 0;
	  G4Exception("GmPSPrinterHistos::DumpAll",
		      "Out of limits",
		      JustWarning,
		      G4String("Index Y is out of limits, histo per row will not be filled. Index= " + GmGenUtils::itoa(index) + " Value skipped="+GmGenUtils::ftoa(aveVal)).c_str());
	} else {
   	  hisX1 = theAnaMgr->GetHisto1(theHistoNumber+20000+indexY);
	  hisX1->AddBinContent( indexX, aveVal ); 
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) {
	    G4cout << "GmPSPrinterHistos " << theName << " X1 Add BinContent " << ite->first << ": " << indexX << " = " << aveVal << " TOTAL= " << hisX1->GetBinContent( indexX ) << G4endl;
	  }
#endif
        }
      }


      if( scorer->ScoreErrors() ) {
	G4double error = scorer->GetErrorRelative( ite->first, sumVal )*aveVal;
	hisY->SetBinError( indexY, sqrt(sqr(error)+sqr(errorPastY)));
	hisXY->SetBinError( indexX, indexY, sqrt(sqr(error)+sqr(errorPastXY)));
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) {
	  G4cout << "GmPSPrinterHistos " << theName << " Y Add BinError " << ite->first << ": " << indexY << " = " << error << " errorPast " << errorPastY << " TOTAL " << hisY->GetBinError( indexY ) << G4endl;
	  G4cout << "GmPSPrinterHistos " << theName << " XY Set BinError " << ite->first << ": " << indexX << " , " << indexY << " = " << error  << " errorPast " << errorPastXY << " TOTAL " <<  hisXY->GetBinError( indexX, indexY ) << G4endl;
	}
#endif
	if( bHistoEachRow ) {
          if( hisY1 ) { 
 	    G4double errorPastY1 = hisY1->GetBinError( indexY );
	    hisY1->SetBinError( indexY, sqrt(sqr(error)+sqr(errorPastY1)));
#ifndef GAMOS_NO_VERBOSE
            if( ScoringVerb(debugVerb) ) {
              if( hisY1 ) G4cout << "GmPSPrinterHistos " << theName << " Y1 Add BinError " << ite->first << ": " << indexY << " = " << error << " errorPast " << errorPastY1 << " TOTAL " << hisY1->GetBinError( indexY ) << G4endl;
            }
#endif
	  }
          if( hisX1 ) {
  	    G4double errorPastX1 = hisX1->GetBinError( indexX );
	    hisX1->SetBinError( indexX, sqrt(sqr(error)+sqr(errorPastX1)));
#ifndef GAMOS_NO_VERBOSE
	    if( ScoringVerb(debugVerb) ) {
	      if( hisX1 ) G4cout << "GmPSPrinterHistos " << theName << " X1 Add BinError " << ite->first << ": " << indexX << " = " << error << " errorPast " << errorPastX1 << " TOTAL " << hisX1->GetBinError( indexX ) << G4endl;
	    }
#endif
	  }
        }
      }
    }
  }
  
}
