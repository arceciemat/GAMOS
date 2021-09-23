#include "../include/GmAODataSearch.hh"
#include "../include/GmAOData.hh"
#include "../include/GmAOParam.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//#include "CLHEP/GenericFunctions/Gamma.hh"

//-----------------------------------------------------------------------
GmAODataSearch::GmAODataSearch( const std::vector<G4String>& wl )
{
  theNEvents = -1;

  AOProcessing aop;
  for( unsigned int ii = 0; ii < wl.size(); ii++ ){
    if( wl[ii].substr(0,1) == ":" ) {
      aop = BuildProcessing( wl[ii] );
    } else {
      if( aop == AOPPrefix ){
	thePrefixes[GmGenUtils::GetInt(wl[ii])-1] = (wl[ii+1]);
	ii++;
      } else if( aop == AOPSuffix ){
	theSuffixes[GmGenUtils::GetInt(wl[ii])-1] = (wl[ii+1]);
	ii++;
      } else if( aop == AOPNumberOfLines ){
	theNumberOfLines.insert( GmGenUtils::GetInt(wl[ii]) );
      } else if( aop == AOPWordNumber ){
	theWordNumbers.push_back( GmGenUtils::GetInt(wl[ii])-1 );
      } else if( aop == AOPTreatment ){
	theTreatment = BuildTreatment(wl[ii]);
      } else if( aop == AOPMessage ){
	theMessage = (wl[ii]);
      } else if( aop == AOPNEvents ){
	theNEvents = GmGenUtils::GetInt(wl[ii] );
      }

    }
  } 

}


//---------------------------------------------------------------------
AOProcessing GmAODataSearch::BuildProcessing(const G4String& str ) const
{
  AOProcessing aop;
  if( str == ":PRE" || str == ":PREFIX" ){
    aop = AOPPrefix;
  }else if( str == ":SUF" || str == ":SUFFIX" ){
    aop = AOPSuffix;
  }else if( str == ":W" || str == ":WORD" ){
    aop = AOPWordNumber;
  }else if( str == ":L" || str == ":LINE" ){
    aop = AOPNumberOfLines;
  }else if( str == ":T" || str == ":TREATMENT" ){
    aop = AOPTreatment;
  }else if( str == ":M" || str == ":MESSAGE" ){
    aop = AOPMessage;
  }else if( str == ":NEV" || str == ":NEVENTS" ){
    aop = AOPNEvents;
  } else {
    G4Exception("GmAODataSearch::BuildProcessing",
		"Error in argument",
		FatalErrorInArgument,
		G4String("word must be processing type : PRE / :PREFIX / :SUF / :SUFFIX / :W / :WORD / :L / :LINE / :T / :TREATMENT, it is " + str ).c_str());
  }
  return aop;
}

//---------------------------------------------------------------------
AOTreatment GmAODataSearch::BuildTreatment(const G4String& str )
{
  AOTreatment aot;
  if( str == "P" || str == "PRINT" ) {
    aot = AOTPrint1;
  } else if( str == "PT" || str == "PRINT_TOGETHER" ){
    aot = AOTPrintTogether;
  } else if( str == "S" || str == "SUM" ){
    aot = AOTSum;
  } else if( str == "M" || str == "MEAN" ){
    aot = AOTMean;
  } else if( str == "TTEST" ){
    aot = AOTStudenttTest;
    theTTestType = AOttestFractionNEvents;
  } else if( str == "TTEST_FRAC" ){
    aot = AOTStudenttTest;
    theTTestType = AOttestFraction;
  } else if( str == "TTEST_VALUE" ){
    aot = AOTStudenttTest;
    theTTestType = AOttestValue;
  } else {
    G4Exception("GmAODataSearch::GmAODataSearch",
		"Error in argument",
		FatalErrorInArgument,
		G4String("Last word must be treatment type : PRINT / SUM / MEAN , it is " + str ).c_str());
  }

  return aot;
}


//-----------------------------------------------------------------------
G4bool GmAODataSearch::CheckLine( std::vector<G4String>& wl, G4String fileName )
{
  std::map<G4int,G4String>::const_iterator ite;
  //---- Check prefixes and suffixes
  for( ite = thePrefixes.begin(); ite != thePrefixes.end(); ite++ ){
    if( G4int(wl.size()) <= (*ite).first ) return false;
    if( wl[(*ite).first] != (*ite).second ) {
      //  if( wl[(*ite).first].find((*ite).second ) != 0 ) { //it must be first characters
      if( verbose >= 2 ) G4cout << "GmAODataSearch::CheckLine  NOT EQUAL prefix " << wl[(*ite).first] << " <> " << (*ite).second << G4endl;
      return false;
    } else {
      if( verbose >= 2 ) G4cout << "GmAODataSearch::CheckLine  EQUAL prefix " << wl[(*ite).first] << " <> " << (*ite).second << G4endl;
    }
      
  }

  for( ite = theSuffixes.begin(); ite != theSuffixes.end(); ite++ ){
    if( G4int(wl.size()) <= (*ite).first ) return false;
    if( wl[(*ite).first].find((*ite).second ) 
	!= wl[(*ite).first].length() - (*ite).second.length() ) { //it must be last characters
      if( verbose >= 2 ) G4cout << "GmAODataSearch::CheckLine  NOT EQUAL suffix " << wl[(*ite).first] << " <> " << (*ite).second << G4endl;
      return false;
    } else {
      if( verbose >= 2 ) G4cout << "GmAODataSearch::CheckLine  EQUAL suffix " << wl[(*ite).first] << " <> " << (*ite).second << G4endl;
    }
  }

  theLinesInFile[fileName]++;
  //---- check line number
  if( theNumberOfLines.size() != 0 && theNumberOfLines.find(theLinesInFile[fileName]) == theNumberOfLines.end() ) { 
    //theLinesInFile[fileName] != theNumberOfLines[0] ) { 
    if( verbose >= 2 ) G4cout << " NOT PROPER LINE " <<  theLinesInFile[fileName] << G4endl;
    return false;
  }
 
  if( verbose >= 1 ) G4cout << " !! LINE FOUND " << fileName << G4endl;
  return true;
}


//-----------------------------------------------------------------------
void GmAODataSearch::ProcessData()
{
  if( theData.size() == 0 ) {
    G4cout << " DataSearch " << GetMessage() << G4endl;
    Print(G4cout);
    G4cout << G4endl;
    G4Exception("GmAODataSearch.:ProcessData",
		"Error",
		FatalException,
		"NO DATA FOUND ");
  }
  if( GetTreatment() == AOTPrint1 ) {
    ProcessDataPrint1();
  } else if( GetTreatment() == AOTPrintTogether ) {
    ProcessDataPrintTogether();
  } else if( GetTreatment() == AOTSum 
	     || GetTreatment() == AOTMean ) {
    ProcessDataSumOrMean();
  } else if( GetTreatment() == AOTStudenttTest ){
    ProcessDataStudenttTest();
  }
}

 
//-----------------------------------------------------------------------
void GmAODataSearch::ProcessDataPrint1()
{
  for( unsigned int ii = 0; ii < theData.size(); ii++ ){
    //----- print data
    G4cout << theData[0]->GetMessage();
    theData[ii]->PrintData();
    G4cout << G4endl;
  }
}

//-----------------------------------------------------------------------
void GmAODataSearch::ProcessDataPrintTogether()
{
  G4cout << theData[0]->GetMessage();
  for( unsigned int ii = 0; ii < theData.size(); ii++ ){
    //----- print data
    theData[ii]->PrintData();
  }
  G4cout << G4endl;

}

//-----------------------------------------------------------------------
void GmAODataSearch::ProcessDataSumOrMean()
{
  std::map<G4int,G4double> theValues;
  std::map<G4int,G4String> theStrings;
  std::vector<G4bool> isValue;
  std::vector<G4String> dLine;
  std::vector<G4int> wordn = GetWordNumbers();
  
  for( unsigned int ii = 0; ii < theData.size(); ii++ ){
    dLine = theData[ii]->GetLine();
    for( unsigned int jj = 0; jj < wordn.size(); jj++ ){
      G4String word = dLine[wordn[jj]];
      if( ii == 0 ) {
	if( GmGenUtils::IsNumber( word ) ) {
	  theValues[jj] = GmGenUtils::GetValue( word );
	  isValue.push_back(TRUE);
	  if( verbose >= 2 ) G4cout << ii << " " << wordn[jj] << " SUM VALUES " << theValues[jj] << G4endl; 
	} else {
	  theStrings[jj] = word;
	      isValue.push_back(FALSE);
	      if( verbose >= 2 ) G4cout << jj << " " << wordn[jj] << " SUM STRINGS " << theStrings[jj] << G4endl; 
	      
	}
	
      } else {
	if( isValue[jj] ) {
	  theValues[jj] += GmGenUtils::GetValue( word );
	}  
	if( verbose >= 2 ) G4cout << ii << " " << wordn[jj] << " SUM VALUES " << theValues[jj] << G4endl; 
      }
      
    }
  }
  
  //----- print data
  G4cout << theData[0]->GetMessage() << " ";
  for( unsigned int jj = 0; jj < wordn.size(); jj++ ){
    if( isValue[jj] ) {
      if( GetTreatment() == AOTSum ) {
	G4cout << theValues[jj] << " ";
      } else if ( GetTreatment() == AOTMean ) {
	G4cout << theValues[jj]/theData.size() << " ";
      }
    } else {
      G4cout << theStrings[jj] << " ";
    }
  }
  G4cout << G4endl;
}

//-----------------------------------------------------------------------
void GmAODataSearch::ProcessDataStudenttTest()
{
  /*t
  if(theData.size() != 2 && theData.size() != 1 ) {
    Print();
    G4Exception("GmAODataSearch::ProcessDataStudenttTest",
		"Error",
		FatalException,
		G4String("2 and only 2 data sets must be used for the Student's t-test, number of data found is " + itoa(theData.size())).c_str());
    return;
  }

  Genfun::Gamma funGamma;
  std::vector<G4int> wordn = GetWordNumbers();  
  // assuming unequal samples sizes, equal variances
  for( unsigned int jj = 0; jj < wordn.size(); jj++ ){
    G4double val1 = GetValue(theData[0]->GetWord(wordn[jj]));
    G4int nEvents1 = theData[0]->GetNEvents();
    G4double frac1 = val1/nEvents1;
    G4double sigma1sqr = (frac1-sqr(frac1))/(nEvents1-1);
    G4double val2;
    G4int nEvents2;
    G4double frac2;
    G4double sigma2sqr;
    if( theData.size() == 2 ) {
      val2 = GetValue(theData[1]->GetWord(wordn[jj]));
      nEvents2 = theData[1]->GetNEvents();
    } else {
      val2 = 0.;
      nEvents2 = theData[0]->GetNEvents();
    }
    frac2 = val2/nEvents2;
    sigma2sqr = (frac2-sqr(frac2))/(nEvents2-1);

    //    if( iopt == AOttestFractionNEvents
    if( theTTestType == AOttestFraction ) { // value is already fraction (divided by nEvents)
      frac1 *= nEvents1;
      frac2 *= nEvents2;
    } else if( theTTestType == AOttestValue ) { // value is not per nEvents
      sigma1sqr = 1./(nEvents1-1);
      sigma2sqr = 1./(nEvents2-1);
    }

    G4int ndof = nEvents1+nEvents2-2;
    G4double sigma12 = sqrt(((nEvents1-1)*sigma1sqr+(nEvents2-1)*sigma2sqr)/ndof);
    G4double tval = (frac1-frac2)/(sigma12*sqrt(1./nEvents1+1./nEvents2));
    
    G4double pValue = funGamma((ndof+1.)/2.)/sqrt(ndof*M_PI)/funGamma(ndof/2.)*pow(1+tval/ndof,-(ndof+1.)/2.);
    //    G4cout << " TTEST vals " << val1 << " " << val2 << G4endl
	   << " nevents " << nEvents1 << " " << nEvents2 << G4endl
	   << " fracs " << frac1 << " " << frac2 << G4endl
	   << " sigmasqrs " << sigma1sqr << " " << sigma2sqr << G4endl
	   << " ndof " << ndof << " sigma12 " << sigma12 
	   << " tval " << tval << G4endl;
    G4cout << " FUNGAMMS " <<  funGamma((ndof+1.)/2.) << " / " 
	   << sqrt(ndof*M_PI) << " / " 
	   << funGamma(ndof/2.) << " * " 
	   << tval/ndof << " " 
	   << pow(1+tval/ndof,-(ndof+1.)/2.) << G4endl;
//
    if( frac1 * frac2 == 1 ) continue;
    if( sigma12 == 0 ) continue;
    G4cout << " TTEST vals " << val1 << " " << val2 << G4endl
	   << " fracs " << frac1 << " " << frac2 << G4endl
	   << " sigmasqrs " << sigma1sqr << " " << sigma2sqr << G4endl
	   << " sigma12 " << sigma12 << G4endl;

    G4double diff = fabs(frac1-frac2)/(sigma12);
    Print( G4cout, wordn[jj] );
    //t    G4cout << " PVALUE= " << pValue << G4endl;
    G4cout << " DIFF_REL= " << diff << G4endl;
  }
*/
}

void GmAODataSearch::Print( std::ostream& out, G4int wordn ) 
{
  out << " GmAODataSearch " << GetType() << ":PRE ";  

  std::map<G4int,G4String>::const_iterator ite;
  for( ite = thePrefixes.begin(); ite !=  thePrefixes.end(); ite++ ){
    out << (*ite).first+1 << " " << (*ite).second << " ";
  }
  out << " :POST ";
  for( ite = theSuffixes.begin(); ite !=  theSuffixes.end(); ite++ ){
    out << (*ite).first << " " << (*ite).second << " ";
  }
  out << " :L ";
  std::set<G4int>::const_iterator ites;
  for( ites = theNumberOfLines.begin(); ites != theNumberOfLines.end(); ites++ ){
    out << *ites << " ";
  }
  out << " :W ";
  std::vector<G4int>::const_iterator itev;
  for( itev = theWordNumbers.begin(); itev != theWordNumbers.end(); itev++ ){
    if( wordn == -1 || wordn == *itev ) {
      out << *itev << " ";
    }
  }
  out << " :M " << theMessage;

  if( theNEvents != -1 ) {
    out << " :NEV " << theNEvents;
  }

  out << " NDATA= " << theData.size();

}

G4String GmAODataSearch::GetType()
{
  if( GetTreatment() == AOTPrint1 ) {
    return "AOTPrint1";
  } else if( GetTreatment() == AOTPrintTogether ) {
    return "AOTPrintTogether";
  } else if( GetTreatment() == AOTSum ) {
    return "AOTSum";
  } else if( GetTreatment() == AOTMean ) {
    return "AOTMean";
  } else if( GetTreatment() == AOTStudenttTest ){
    return "AOTStudenttTest";
  }
  return "AOT_NONE";

}
