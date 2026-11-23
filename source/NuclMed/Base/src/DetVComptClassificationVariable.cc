#include "DetVComptClassificationVariable.hh"
#include "DetVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
DetVComptClassificationVariable::DetVComptClassificationVariable()
{

}

//----------------------------------------------------------------------
void DetVComptClassificationVariable::InitVariables()
{
  theStep = GmParameterMgr::GetInstance()->GetNumericValue("DetCClassif"+theName+":Step",-1.);
  if( theStep != -1. ) { 
   theMin = GmParameterMgr::GetInstance()->GetNumericValue("DetCClassif"+theName+":Min",0.);
    theMax = GmParameterMgr::GetInstance()->GetNumericValue("DetCClassif"+theName+":Max",0.5);
    for( G4double val = theMin; val <= theMax; val += theStep ) {
      theIntervalLimits.insert(val);
    }
  } else {
    std::vector<G4double> intervalLimitsV;
    intervalLimitsV = GmParameterMgr::GetInstance()->GetVNumericValue("DetCClassif"+theName+":IntervalLimits",intervalLimitsV);
    if( intervalLimitsV.size() == 0 ) {
      G4Exception("DetVComptClassificationVariable::DetVComptClassificationVariable",
		  "No value steps are set",
		  FatalErrorInArgument,
		  G4String("Set at least one value with the command /gamos/setParam DetCClassif"+theName+":IntervalLimits").c_str());
    }
    for( unsigned int ii = 0; ii < intervalLimitsV.size(); ii++ ){
      theIntervalLimits.insert(intervalLimitsV[ii]);
    }
    std::set<G4double>::iterator ite = theIntervalLimits.begin();
    theMin = (*ite);
    std::set<G4double>::reverse_iterator rite = theIntervalLimits.rbegin();
    theMax = (*rite);

  } 

  if( theIntervalLimits.size() >= 100 ) {
      G4Exception("DetVComptClassificationVariable::DetVComptClassificationVariable",
		  "Too many value steps are set",
		  FatalErrorInArgument,
		  G4String("Maximum is 100, you are setting " + GmGenUtils::itoa(theIntervalLimits.size()) + " for DetCClassif"+theName).c_str());
    }

  theCurrentIndex = 0;
}


//----------------------------------------------------------------------
void DetVComptClassificationVariable::BookHistos(G4int histoNumber)
{
  theAnaMgr = GmAnalysisMgr::GetInstance("compton") ;

  BookGeneralHistos(histoNumber);
  BookSpecificHistos(histoNumber);
}

//----------------------------------------------------------------------
void DetVComptClassificationVariable::BookGeneralHistos(G4int histoNumber)
{

  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hgnam = "DetCompton" + sepa + "Classif" + sepa + " " + theName + sepa + " ";
  G4String hnam;
  G4int ii = 0;
  std::set<G4double>::const_iterator ite = theIntervalLimits.begin();
  std::set<G4double>::const_iterator ite2 = theIntervalLimits.begin();

  ite2++;
  for(; ite != theIntervalLimits.end(); ite++, ite2++, ii++ ){
    G4String valName2;
    if( ite2 != theIntervalLimits.end() ) valName2 = GmGenUtils::ftoa(*ite2);
    G4String valName = GmGenUtils::ftoa(*ite) + "-" + valName2;
    hnam = hgnam + valName + G4String(" Dist RecHitSet - Associated Interaction");
    theAnaMgr->CreateHisto1D(hnam,200,0,200.,histoNumber+100*ii+90);
    hnam = hgnam + valName + G4String(" Dist RecHitSet - 1st Interaction");
    theAnaMgr->CreateHisto1D(hnam,200,0,200.,histoNumber+100*ii+91);
    hnam = hgnam + valName + G4String(" Dist RecHitSet - 1st Interaction, Wrong assoc.");
    theAnaMgr->CreateHisto1D(hnam,200,0,200.,histoNumber+100*ii+92);
    hnam = hgnam + valName + G4String(" Dist RecHitSet - 1st Interaction, Good assoc.");
    theAnaMgr->CreateHisto1D(hnam,200,0,200.,histoNumber+100*ii+93);
  }
  
}



//----------------------------------------------------------------------
void DetVComptClassificationVariable::SetCurrentClassificationAndFillHistos( DetRecHitSet& prhs, G4int nh )
{
  //  G4cout << theName << " SetCurrentClassificationAndFillHistos "<< nh << G4endl;
  theCurrentIndex = GetClassification(prhs, nh);
}

//----------------------------------------------------------------------
int64_t DetVComptClassificationVariable::GetIndexFromValue( G4double val )
{
  std::set< G4double>::const_iterator itev = theIntervalLimits.upper_bound(val);
  int64_t index = std::distance( theIntervalLimits.begin(), itev ) -1;
  if( index == -1 ) index = 0;

  return index;
}

//----------------------------------------------------------------------
void DetVComptClassificationVariable::FillHisto( G4int hn, G4double val)
{

  theAnaMgr->GetHisto1(hn + theCurrentIndex*100 )->Fill(val);

}

