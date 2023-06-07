//#define VERBOSE_DOSEDEP
#include "GmCompoundScorer.hh"
#include "G4VPrimitiveScorer.hh"
#include "GmScoringMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GamosCore/GamosScoring/Management/include/GmPrimitiveScorerFactory.hh"
#endif

//--------------------------------------------------------------------
GmCompoundScorer::GmCompoundScorer(G4String name) 
   :GmVPrimitiveScorer(name)
{ 
  theUnit = 1;
  theUnitName = "";

  bInitialized = false;

  theScoringMgr = GmScoringMgr::GetInstance();

  theNameSuffix = "";
}


//--------------------------------------------------------------------
G4bool GmCompoundScorer::ProcessHits(G4Step* ,G4TouchableHistory*)
{
  if( !bInitialized ) {
    //--- set unit
    if( theSubExpressions.size() == 0 ) {
      theUnit = theSubScorers[0]->GetUnit();
      theUnitName = theSubScorers[0]->GetUnitName();
    } else {
      G4String expression;
      unsigned int siz = theSubExpressions.size();
      unsigned int sizScorers = theSubScorers.size();
      for( unsigned int ii = 0; ii < siz; ii++ ){
	expression += theSubExpressions[ii];
      if( ii < sizScorers )  expression += GmGenUtils::ftoa( theSubScorers[ii]->GetUnit());
      //      G4cout << ii << " EXPRESSION " << expression << G4endl;//GDEB
      }
      theUnitName = expression;
      theUnit = GmGenUtils::GetValue( expression );
    }
    bInitialized = true;
  }

  // subscorers already called by MFD 
  /*  std::vector<GmVPrimitiveScorer*>::const_iterator ite;
  for( ite = theSubScorers.begin(); ite != theSubScorers.end(); ite++ ){
    (*ite)->ProcessHits(aStep, (G4TouchableHistory*)0 );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb)) G4cout  << "GmCompoundScorer::ProcessHits for scorer " << (*ite)->GetName() << G4endl;
#endif
  }
  */

  //-  FillScorer( aStep, -1, 0., 0., true ); // bOnlyNewEvent=true
  
  return TRUE;
} 

//--------------------------------------------------------------------
void GmCompoundScorer::EndOfEvent(G4HCofThisEvent*)
{
}

//--------------------------------------------------------------------
void GmCompoundScorer::DrawAll()
{;}

//--------------------------------------------------------------------
void GmCompoundScorer::AddSubScorer(GmVPrimitiveScorer* scorer )
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmCompoundScorer::AddSubScorer " << scorer->GetName() << " N= " << theSubScorers.size() << G4endl; 
#endif

  theSubScorers.push_back( scorer );
  scorer->SetSubScorer(true);
}


//--------------------------------------------------------------------
void GmCompoundScorer::AddSubExpression( G4String expr )
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmCompoundScorer::AddSubExpression " << theSubExpressions.size() << " expr " << expr << G4endl; 
#endif

  theSubExpressions.push_back( expr );
}

//--------------------------------------------------------------------
void GmCompoundScorer::BuildCompoundScores()
//-- Fill SumV_tmp and EvtMap from subScorers
{
  unsigned int siz = theSubScorers.size();
  for( unsigned int ii = 0; ii < siz; ii++ ){
    if( theSubScorers[0]->GetSumV_tmp().size() == 0 ) return;
  }

  std::vector<std::map<G4int,G4double>> SumV_tmps;
  std::vector<std::set<size_t>> NFilled_tmps;
  for( unsigned int ii = 0; ii < siz; ii++ ){
    SumV_tmps.push_back( theSubScorers[ii]->GetSumV_tmp() );
    NFilled_tmps.push_back( theSubScorers[ii]->GetNFilled_tmp() );
  }
  //  G4cout << " GmCompoundScorer::BuildCompoundScores SumV_size " << SumV_tmps.size() << G4endl; //GDEB
  std::map<G4int,G4double>::iterator itee;

  std::map<G4int,G4double> sumV_tmp0 = SumV_tmps[0];
  
 #ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmCompoundScorer::BuildCompoundScores " << GetName() << " subNindices sumV " << sumV_tmp0.size() << " subNindices NFilled " << NFilled_tmps[0].size() << G4endl;
#endif
  unsigned int sizScorers = theSubScorers.size();
  for( itee = sumV_tmp0.begin(); itee != sumV_tmp0.end(); itee++ ){
    //-      G4cout << " EvtMap " << (*itee).first << " = " << (*itee).second << G4endl;
    G4int index = (*itee).first;
    //    G4cout << "GmCompoundScorer::BuildCompoundScores() CS " << index << G4endl; //GDEB
    G4String expression;
    unsigned int sizExpr = theSubExpressions.size();
    for( unsigned int ii = 0; ii < sizExpr; ii++ ){
      expression += theSubExpressions[ii];
      G4String exprValS = GmGenUtils::ftoa((SumV_tmps[ii])[index]);
      if( ii < sizScorers ) expression += exprValS;
      /*      G4bool bBadExpr = false;
      size_t icbe = expression.find("/0");
      if( icbe != std::string::npos ) {
	if( icbe == expression.lengt()-2 || expresssionn[icbe+2]!='.' ) {
	  bBadExpr = true;
	}
	}*/
      if( theSubExpressions[ii] == "/" && exprValS == "0" ) {
	expression = "0";
	break;
      }
      //      G4cout << ii << " CS expression " << expression << " : " << theSubExpressions[ii] << " " << (SumV_tmps[ii])[index] << " = " << GmGenUtils::ftoa((SumV_tmps[ii])[index]) << G4endl; //GDEB
    }
    G4double newVal = GmGenUtils::GetValue( expression );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmCompoundScorer::BuildCompoundScores " << GetName() << " index "<< index << " expression " << expression << " " << newVal << G4endl; 
#endif
    theSumV_tmp[index] = newVal;
    //  G4cout << this << " " << GetName() << " BUILD " << bFillEvtMap << " " << EvtMap << G4endl; //GDEB
    EvtMap->add(index,newVal);
    theNFilled_tmp.insert(index);
  }

  std::vector<std::set<size_t>>::const_iterator itemnf;
  std::set<size_t> NFilled_tmpAll;
  std::set<size_t>::const_iterator itenf;
  for( itemnf = NFilled_tmps.begin(); itemnf != NFilled_tmps.end(); itemnf++ ) {
    for( itenf = (*itemnf).begin(); itenf != (*itemnf).end(); itenf++ ) {
      NFilled_tmpAll.insert(*itenf); // all indices from all scores
    }
  }
  G4bool bFound = true;
  for( itenf = NFilled_tmpAll.begin(); itenf != NFilled_tmpAll.end(); itenf++ ) {
    for( itemnf = NFilled_tmps.begin(); itemnf != NFilled_tmps.end(); itemnf++ ) {
      //      G4cout << " NFilled index is in all scorers " << *itenf << " " << G4endl; //GDEB
      if( (*itemnf).find(*itenf) == (*itemnf).end() ) { // only if index is in all scorers
	bFound = false;
	//      G4cout << " NFilled index is in all scorers " << *itenf << " NOT FOUND " << G4endl; //GDEB
	break;
      } else {
	if( (*itemnf).find(*itenf) == (*itemnf).end() ) { // only if index is in all scorers
	  bFound = false;
	  break; 
	}
      }
    }
    if( bFound ) {
      //      theNFilled[*itenf]++; 
      //      G4cout << GetName() << " " << *itenf << " GmCompoundScorer::BuildCompoundScores NFilled++ " <<  theNFilled[*itenf] << G4endl; //GDEB
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmCompoundScorer::BuildCompoundScores NFilled_tmps0 " << NFilled_tmps[0].size() << " NFilled_tmpAll " << NFilled_tmpAll.size() << " theNFilled " << theNFilled.size() << G4endl;
#endif
  
  for( unsigned int ii = 0; ii < sizScorers; ii++ ){
    theSubScorers[ii]->ClearSumV_tmp();
    theSubScorers[ii]->ClearNFilled_tmp();
  } 
}

//--------------------------------------------------------------------
void GmCompoundScorer::SetParameters( const std::vector<G4String>& params )
{
  std::map<G4String,GmVPrimitiveScorer*> theScorers = theScoringMgr->GetScorers();

  if( params.size() == 0 ) {
    G4Exception("GmCompoundScorer::SetParameters",
		"",
		FatalException,
		"Cannot create a GmCompoundScorer without parameters (for example passing 3 scorers to /gamos/scoring/addScorerToMFD");
  }
  
  if( ScoringVerb(infoVerb) ) {
    for( size_t ii = 0; ii < params.size(); ii++ ) {
      G4cout << ii << " GmCompoundScorer:SetParameters PARAM " << params[ii] << G4endl;
    }
  }

  if( params.size() != 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmCompoundScorer::SetParameters",
		"There should be one parameters: ARITHMETIC_EXPRESSION_OF_SCORERS",FatalErrorInArgument,G4String("They are: "+parastr).c_str());
  }
  
  G4String scorerClass = params[0];
  G4int iSeparator = GmGenUtils::GetNextSeparator( 0, scorerClass );
  //   G4cout << " CS: iSeparator " << iSeparator << " " << scorerClass << G4endl; //GDEB
  if( iSeparator == G4int(std::string::npos) ) {
    //    G4cout << " !!! CALL  GmCompoundScorer::FindOrCreateScorer " << scorerClass << " " << scorerClass<<"+"<<theNameSuffix << G4endl; //GDEB
    G4String scorerName = scorerClass+theNameSuffix;
    GmVPrimitiveScorer* subScorer = FindOrCreateScorer(scorerClass,scorerName);
    theScoringMgr->AddScorer( scorerName, subScorer );
    subScorer->SetDefaultPrinter(false);
    AddSubScorer( subScorer );
    theSubScorerClasses.push_back(scorerClass);
    AddSubExpression( "" );
    subScorer->SetNEventsType( GetNEventsType() );
    
  } else {
    //    G4cout << " decompose " << scorerClass << G4endl; //GDEB
    //-----  decompose the regular expression
    unsigned int nLen = scorerClass.length();
    std::string expressions;
    std::vector<G4String> words;
    std::vector<G4bool> isSeparator;
    
    G4String word = scorerClass.substr(0,iSeparator);
    if( iSeparator != 0 ) {
      // first word has to be a separator
      words.push_back("");
      isSeparator.push_back(1);
      words.push_back( word );
      isSeparator.push_back( GmGenUtils::IsSeparator(word) );
      
      word = scorerClass.substr(iSeparator,1);
      words.push_back( word );
      isSeparator.push_back(1);
      //      G4cout << " FIRST SEPARATOR " << word << " N= " << isSeparator.size() << G4endl; //GDEB
    } else {
      word = scorerClass.substr(0,1);
      words.push_back( word );
      isSeparator.push_back(1);
    }
    //    G4cout << " FIRST WORDS " << words.size() << G4endl; //GDEB
    //      G4cout << " FIRST WORDS " << words[0] << " " << words[1] << " " << words[2] << G4endl;  //GDEB
    
    G4int iSeparatorNew;
    for( ;; ){
      iSeparatorNew = GmGenUtils::GetNextSeparator( iSeparator+1, scorerClass );
      //      G4cout << " iSeparatorNew " << iSeparatorNew << G4endl; //GDEB
      if( iSeparatorNew != G4int(std::string::npos) ) {
	if( iSeparatorNew-iSeparator-1 != 0 )  {
	  word = scorerClass.substr(iSeparator+1, iSeparatorNew-iSeparator-1);
	  words.push_back( word );
	  isSeparator.push_back( GmGenUtils::IsSeparator(word) );
	}
	word = scorerClass.substr(iSeparatorNew, 1);
	words.push_back( word );
	isSeparator.push_back(1);
	
	iSeparator = iSeparatorNew;
      } else {
	break;
      } 
    }
    if( iSeparator != G4int(nLen) - 1 ) {
      word = scorerClass.substr(iSeparator+1, nLen);
      words.push_back( word );
      //      G4cout << " LAST WORD " << word << G4endl; //GDEB
    }
    isSeparator.push_back( GmGenUtils::IsSeparator(word) );
    
    std::vector<G4String> subExpressions;
    for( unsigned int ii = 0; ii < words.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmCompoundScorer::CreateScorer WORD " << words[ii] << " " << isSeparator[ii] << G4endl;
#endif
      if( isSeparator[ii] ) {
	if( ii && isSeparator[ii-1] ) {
	  // modify last subExpressions
	  subExpressions[subExpressions.size()-1] += words[ii];
	} else {
	  subExpressions.push_back(words[ii]);
	}
      } else {
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << "GmCompoundScorer::CreateScorer  AddSubScorer " << words[ii] << " ii " << ii << G4endl;
#endif
	//----- Check that Scorer exists
	//	G4String subScorerClass = words[ii].split;std::getline(ss, item, delimeter)
	G4String subScorerClass = words[ii];
	//	G4cout << GetName() << " !!! CALL2  GmCompoundScorer::FindOrCreateScorer " << subScorerClass << " " << subScorerClass<<"+"<<theNameSuffix << " FROM word " << words[0] << G4endl; //GDEB
	G4String scorerName = subScorerClass+theNameSuffix;
	GmVPrimitiveScorer* subScorer = FindOrCreateScorer(subScorerClass,scorerName);
	std::vector<G4String> wl;
	//	G4cout << " COMP theScoringMgr->AddScored " << scorerName << G4endl; //GDEB
	theScoringMgr->AddScorer( scorerName, subScorer );
	subScorer->SetDefaultPrinter(false);
	AddSubScorer( subScorer );
	theSubScorerClasses.push_back(words[ii]);
	subScorer->SetNEventsType( GetNEventsType() );
      }
    }
      
    for( unsigned int ii2 = 0; ii2 < subExpressions.size(); ii2++ ){
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(-infoVerb) ) G4cout << " GmCompoundScorer:SetParameters  AddSubExpression " << subExpressions[ii2] << " ii " << ii2 << G4endl;
#endif
      AddSubExpression( subExpressions[ii2] );
    }
  }

  //  G4cout << GetName() << " !! GetMFD " << GetMFD() << G4endl; //GDEB
  if( GetMFD() ) PropagateMFDToSubScorers(); //-- If called by createScorer, it does not exists; if called by LET scorers SetParameters, it does
  
  //--- Call subScorer SetParameters, in case it is compound itself 
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    std::vector<G4String> subParams;
    //    if( theSubScorerClasses.size() != 0 ) subParams.push_back( theSubScorerClasses[ii] );
    if( theExtraParams.size() > ii ) {
      std::vector<G4String> extraParams1 = theExtraParams[ii];
      for( size_t jj = 0; jj < extraParams1.size(); jj++ ){
	subParams.push_back(extraParams1[jj]);
	//	G4cout << ii << " : " << jj << " GmCompoundScorer:SetParameters  Add extraParams  " << extraParams1[jj] << " N " << extraParams1.size() <<  G4endl; //GDEB
      }
    }
	  
    theSubScorers[ii]->SetParameters(subParams);
  }
}

//--------------------------------------------------------------------
GmVPrimitiveScorer* GmCompoundScorer::FindOrCreateScorer(G4String scorerClass,G4String& scorerName )
{
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) )  G4cout << " !! GmCompoundScorer::FindOrCreateScorer " << scorerClass << " " << scorerName << G4endl; 
#endif
  GmVPrimitiveScorer* scorer = 0;

  //----- Check that Scorer exists
  std::map<G4String,GmVPrimitiveScorer*> theScorers = theScoringMgr->GetScorers();
  std::map<G4String,GmVPrimitiveScorer*>::iterator itesco = theScorers.find(scorerName);
  G4int iFound = 0;
  for( itesco == theScorers.begin(); itesco != theScorers.end(); itesco++ ) { // scorer already created with same name, change name
    G4String scorerName_short = itesco->first;
    size_t ius = scorerName_short.find("_");
    if( ius != std::string::npos && GmGenUtils::IsInteger(scorerName_short.substr(ius+1,scorerName_short.length())) ) {
      //      G4cout << scorerName_short << " IUS " << ius << " " << GmGenUtils::IsInteger(scorerName_short.substr(ius+1,scorerName.length())) << G4endl; //GDEB
      scorerName_short = scorerName_short.substr(0,ius);
    }
    if(scorerName_short == scorerName ) iFound++;
      
  }
  //--- Change scorer name to avoid name repetition
  if( iFound != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )  G4cout << " !! GmCompoundScorer::FindOrCreateScorer SCORER NAME FOUND " << scorerName << " CHANGE IT TO " << scorerName+"_"+GmGenUtils::itoa( iFound+1 ) << G4endl;
    scorerName += "_"+GmGenUtils::itoa( iFound+1 );
  }
#endif
#ifdef ROOT5
    scorer = Reflex::PluginService::Create<GmVPrimitiveScorer*>(scorerClass,scorerName);
#else
    scorer = GmPrimitiveScorerFactory::get()->create(scorerClass,scorerName);
#endif
      
    if( !scorer ) {
      G4Exception(" GmCompoundScorer::FindOrCreateScorer",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("Scorer class not found " + scorerClass).c_str());
    }
    //  }
  
  return scorer;
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateMFDToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    G4MultiFunctionalDetector* mfd = GetMFD();
    GmVPrimitiveScorer* subScorer = theSubScorers[ii];
    mfd->RegisterPrimitive(subScorer);   
    subScorer->RegisterMFD( mfd );
    if( ScoringVerb(debugVerb) ) G4cout << ii << " GmCompoundScorer::PropagateMFDToSubScorers " << mfd->GetName() << " FROM " << GetName() << " TO " << subScorer->GetName() << G4endl; 
  }

  //--- Call subScorer SetParameters, in case it is compound itself 
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->PropagateMFDToSubScorers();
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateFilterToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetGmFilter( GetGmFilter() );
  }
  //--- Call subScorer SetParameters, in case it is compound itself 
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->PropagateFilterToSubScorers();
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateClassifierToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetClassifier( GetClassifier() );
  }
  //--- Call subScorer SetParameters, in case it is compound itself 
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->PropagateClassifierToSubScorers();
  }
}
  

//--------------------------------------------------------------------
void GmCompoundScorer::PropagatePrinterToSubScorers(GmVPSPrinter* prt)
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->AddPrinter( prt );
  }
  //--- Call subScorer SetParameters, in case it is compound itself 
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->PropagatePrinterToSubScorers(prt);
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateTrkWeightToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetUseTrackWeight( UseTrackWeight() );
  }
    //--- Call subScorer SetParameters, in case it is compound itself 
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->PropagateTrkWeightToSubScorers();
  }
}

//--------------------------------------------------------------------
void GmCompoundScorer::PropagateScoreErrorsToSubScorers()
{
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->SetScoreErrors( ScoreErrors() );
  }
    //--- Call subScorer SetParameters, in case it is compound itself 
  for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) {
    theSubScorers[ii]->PropagateScoreErrorsToSubScorers();
  }
}

//--------------------------------------------------------------------
G4double GmCompoundScorer::GetNEvents( G4int index )
{
  //  G4cout << this << " " << GetName() << "  GmCompoundScorer::GetNEvents theNEventsType " << theNEventsType << G4endl; //GDEB
  switch (theNEventsType) {
    case SNET_ByRun:
      return 1;
    case SNET_ByEvent:
      return GmNumberOfEvent::GetNumberOfEvent();
    case SNET_ByNFilled:
      //      for( size_t ii = 0; ii < theSubScorers.size(); ii++ ) { //GDEB
	// G4int nev = theSubScorers[ii]->GetNFilled(index);
	//	G4cout << "  GmCompoundScorer::GetNEvents theNEventsType " << theNEventsType << " = " << theSubScorers[ii]->GetName() << " : " << theNFilled[index] << G4endl; //GDEB
	/*	if( nev == -1 ) {
	  G4Exception("GmVPrimitiveScorer::GetNEvents",
		      "",
		      JustWarning,
		      ("Scorer " + theSubScorers[ii]->GetName() + " : score does not exist for index " + GmGenUtils::itoa(index)).c_str());
		      }*/
      //      }
      return theNFilled[index];
  }
  
  return 1;
}


//--------------------------------------------------------------------
std::map<G4int,size_t> GmCompoundScorer::GetNFilled() const
{
  return theSubScorers[0]->GetNFilled();
}
