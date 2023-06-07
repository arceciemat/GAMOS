#include "GmVNumericDistribution.hh"
#include "GmNumericDistributionUpper.hh"
#include "GmNumericDistributionLinLin.hh"
#include "GmNumericDistributionLinLog.hh"
#include "GmNumericDistributionLogLin.hh"
#include "GmNumericDistributionLogLog.hh"
#include "GmNumericDistributionLower.hh"
#include "GmNumericDistributionUpper.hh"
#include "GmVNumericNDimDistribution.hh"
#include "GamosCore/GamosData/Distributions/include/GmDistributionVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoReaderCSV.hh"

#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#ifndef GAMOS_NO_ROOT
#include "TFile.h"
#include "TH1F.h"
#include "TAxis.h"
#else
#include "GamosCore/GamosBase/Base/include/GmHisto1.hh"
#endif
//---------------------------------------------------------------------
GmVNumericDistribution::GmVNumericDistribution(G4String name)
: GmVDistribution(name)
{
  Initialize();
}

//---------------------------------------------------------------------
void GmVNumericDistribution::Initialize()
{
  theAllowOutOfLimits = GmParameterMgr::GetInstance()->GetNumericValue(theName+":AllowOutOfLimits",1 );

  this->ReadFile();
  this->BuildData();
}

//---------------------------------------------------------------------
void GmVNumericDistribution::BuildData()
{
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String dataName = parMgr->GetStringValue(theName+":Data","");
  theData = GmDataMgr::GetInstance()->BuildData(dataName);
  if( theData == 0 ) {
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		"Data class not found",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName 
			 + " Asking for data: " + dataName).c_str());
  } else {
    if( ! theData->CheckAvailability( DTStep ) ) {
      G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		  "Data class not available for DTStep",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName 
			   + " Asking for data: " + dataName).c_str());
    } 

    if( theData->GetCType() == "char" ){ 
      G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		  "Data class cannot be of String type",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName 
			   + " Asking for data: " + dataName).c_str());
      
    }

  }

}

//---------------------------------------------------------------------
void GmVNumericDistribution::ReadFile()
{
  //  G4cout << " GmVNumericDistribution::ReadFile " << G4endl;
  G4bool bCheckNValues = true;

  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  G4String fileName = parMgr->GetStringValue(theName+":FileName","");
  G4String fileNameROOT = parMgr->GetStringValue(theName+":FileNameROOT","");
  G4String fileNameCSV = parMgr->GetStringValue(theName+":FileNameCSV","");
  G4String fileNameText = parMgr->GetStringValue(theName+":FileNameText","");
#ifndef GAMOS_NO_ROOT
  if( fileNameROOT != "" ) {
    CheckOtherFileNames( fileNameROOT, fileNameCSV, fileNameText, fileName, 0 );

    ReadFileROOT( fileNameROOT );
  } 
  else 
#endif
    if( fileNameCSV != "" ) {
    CheckOtherFileNames( fileNameROOT, fileNameCSV, fileNameText, fileName, 1 );

    ReadFileCSV( fileNameCSV );
  }
  else if( fileNameText != "" ) {
    CheckOtherFileNames( fileNameROOT, fileNameCSV, fileNameText, fileName, 2 );

    ReadFileText( fileNameText );
  }
  else if( fileName != "" ) {
    ReadFileUnknown( fileName );
  }
  else {
    if( dynamic_cast<GmNumericDistributionLinLin*>(this) || dynamic_cast<GmNumericDistributionLinLog*>(this) || 
	dynamic_cast<GmNumericDistributionLogLin*>(this) || dynamic_cast<GmNumericDistributionLogLog*>(this) || 
	dynamic_cast<GmNumericDistributionLower*>(this) || dynamic_cast<GmNumericDistributionUpper*>(this) || 
	dynamic_cast<GmVNumericNDimDistribution*>(this) ) {
      G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		  "No file name given",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName).c_str());
    }
    bCheckNValues = false;
  }
  if( bCheckNValues ) CheckNValues();
}

//---------------------------------------------------------------------
void GmVNumericDistribution::CheckNValues()
{
  if( theValues.size() < 1 ) {
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		"Only one value found in file!",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName).c_str());
  }

}


//---------------------------------------------------------------------
void GmVNumericDistribution::CheckOtherFileNames( G4String& fileNameROOT, G4String& fileNameCSV, G4String& fileNameText, G4String& fileName, G4int fNumber )
{
  std::vector<G4String> fileNames;
  fileNames.push_back(fileNameROOT);
  fileNames.push_back(fileNameCSV);
  fileNames.push_back(fileNameText);

  G4bool bDuplicated = false;
  for( unsigned int ii = 0; ii < 3; ii++ ){
    if( fileNames[ii] != G4String("") && fileNames[ii] != GmGenUtils::itoa(fNumber) ) {
      bDuplicated = true;
    }
  }

  if( bDuplicated ){
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		"Only one type of files can be provided",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName +
			 " fileNameROOT= " + fileNameROOT +
			 " fileNameCSV= " + fileNameCSV +
			 " fileNameText= " + fileNameText).c_str());
  }

  if( fileName != "" ){
    G4String fname;
    switch(fNumber){
    case 0:
      fname = "fileNameROOT " + fileNameROOT;
      break;
    case 1:
      fname = "fileNameCSV " + fileNameCSV;
      break;
    case 2:
      fname = "fileNameText" + fileNameText;
      break;
    }
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		G4String("if "+fname + " is used, fileName will be overridden").c_str(),
		JustWarning,
		G4String("DISTRIBUTION: "+theName +
			 fname +
			 " fileName= " + fileName).c_str());
  }
  
}

//---------------------------------------------------------------------
void GmVNumericDistribution::ReadFileUnknown( G4String& fileName )
{
  G4int fileType = GetFileType( fileName );
  switch (fileType) {
#ifndef GAMOS_NO_ROOT
  case 0:
    ReadFileROOT( fileName );
    break;
#endif
  case 1:
    ReadFileCSV( fileName );
    break;
  case 2:
    ReadFileText( fileName );
    break;
  }

}

#ifndef GAMOS_NO_ROOT
//---------------------------------------------------------------------
void GmVNumericDistribution::ReadFileROOT( G4String& fileName )
{
#ifndef GAMOS_NO_ROOT
  G4String histoName = GmParameterMgr::GetInstance()->GetStringValue(theName+":ROOTHistoName","");
  if( histoName == "" ){
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		"ROOT histogram name not provided",
		FatalErrorInArgument,
		G4String("Please use /gamos/setParam "+theName+":ROOTHistoName").c_str());
  }

  TFile file( fileName );
  TH1F* his = (TH1F*)(file.Get(histoName));
  if( his == 0 ) {
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		"ROOT 1D histogram not found in file",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName+ " HISTOGRAM:" + histoName).c_str());
  }

  for( G4int ii = 1; ii < his->GetNbinsX()-1; ii++ ){
    theValues[ his->GetBinCenter(ii) ] = his->GetBinContent(ii);
  }
    
  theMinimum = GetMinimum(his);
  theMaximum = (*(theValues.rbegin())).first + his->GetBinWidth(1)/2.;
#endif
			
}
#endif

//---------------------------------------------------------------------
G4double GmVNumericDistribution::GetMinimum(GmHisto1* his )
{
  /*?  if( ! dynamic_cast<GmNumericDistributionUpper*>(this) ) {
    return 0.;
    } */
  if( his ) {
    return (*(theValues.begin())).first - his->GetBinWidth(1)/2.;
  } else {
    return (*(theValues.begin())).first;
  }
}

//---------------------------------------------------------------------
void GmVNumericDistribution::ReadFileCSV( G4String& fileName )
{
  G4String histoName = GmParameterMgr::GetInstance()->GetStringValue(theName+":CSVHistoName","");
  if( histoName == "" ){
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		"CSV histogram name not provided",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName).c_str());
  }

  GmHistoReaderCSV* histoReader = new GmHistoReaderCSV( fileName );
  GmHisto1* his = histoReader->GetHisto1(histoName);
  if( his == 0 ) {
    G4Exception("GmVNumericDistribution::GmVNumericDistribution",
		"CSV histogram not found in file",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName+ " HISTOGRAM:" + histoName).c_str());
  }

  for( G4int ii = 1; ii < his->GetNbinsX()-1; ii++ ){
    theValues[ his->GetBinCenter(ii) ] = his->GetBinContent(ii);
  }
    
  theMinimum = GetMinimum(his);
  theMaximum = (*(theValues.rbegin())).first + his->GetBinWidth(1)/2.;

}

//---------------------------------------------------------------------
void GmVNumericDistribution::ReadFileText( G4String& fileName )
{
  G4int iValColumn = G4int(GmParameterMgr::GetInstance()->GetNumericValue(theName+":ValueColumn",2 )-1);

 // Read energy - probability  pairs
  fileName = GmGenUtils::FileInPath( fileName);
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  G4int ii = 1;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( G4int(wl.size()) >= iValColumn+1) {
#ifndef GAMOS_NO_VERBOSE
      if( DistVerb(debugVerb) ) {
	G4cout << this << " GmVNumericDistribution::ReadFileText " <<  GmGenUtils::GetValue( wl[0] ) << " -> " <<  GmGenUtils::GetValue( wl[iValColumn] ) << G4endl;
      }
#endif
      
      theValues[ GmGenUtils::GetValue( wl[0] ) ] = GmGenUtils::GetValue( wl[iValColumn] );
    } else if( wl.size() == 1 ) {
      G4Exception("GmVNumericDistribution::ReadFileText",
		  "Warning in number of words in line",
		  JustWarning,
		  G4String("Reading file " + fileName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: VALUE PROBABILITY, taking as probability the line number").c_str());
      theValues[ GmGenUtils::GetValue( wl[0] ) ] = theValues.size();
    } else {
      G4Exception("GmVNumericDistribution::ReadFileText",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + fileName 
			   + ", line number " + GmGenUtils::itoa(ii) 
			   + " All lines must have two words: VALUE PROBABILITY").c_str());
    }

    ii++;
  }
  
  fin.Close();

  theMinimum = this->GetMinimum((GmHisto1*)0);
  theMaximum = (*(theValues.rbegin())).first;
  
}

//---------------------------------------------------------------------
G4int GmVNumericDistribution::GetFileType( G4String& fileName )
{

  G4int fLen = fileName.length();

  if( fLen > 5 && fileName.substr(fLen-5,5) == ".root" ) {
    return 0; 
  } else if( fLen > 4 && fileName.substr(fLen-4,4) == ".csv" ) {
    return 1;
  }

  return 2;

}


//---------------------------------------------------------------------
G4double GmVNumericDistribution::GetValueFromStep(const G4Step* aStep)
{
  if( !theData ) { 
    G4Exception("GmVNumericDistribution::GetValueFromStep",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 

  G4double indexVal = theData->GetValueFromStep( aStep );

  return GetNumericValueFromIndex( indexVal );
}

//---------------------------------------------------------------------
G4double GmVNumericDistribution::GetValueFromTrack(const G4Track* aTrack)
{

  if( !theData ) { 
    G4Exception("GmVNumericDistribution::GetValueFromTrack",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 

  G4double indexVal = theData->GetValueFromTrack( aTrack );

  return GetNumericValueFromIndex( indexVal );
}

//---------------------------------------------------------------------
G4double GmVNumericDistribution::GetNumericValueFromIndex(const G4double indexVal)
{
  if( indexVal < theMinimum ) {
    std::map<G4double,G4double>::const_iterator ite = theValues.begin();
    for( ; ite != theValues.end(); ite++ ) {
      G4cerr << " VALUE " << ite->first << " = " << ite->second << G4endl;
    }
    if( theAllowOutOfLimits == 1 ) {
      G4Exception(G4String(theName+"::GetIndexFromValue").c_str(),
		  "Value out of limits",
		  JustWarning,
		  G4String(GmGenUtils::ftoa(theMinimum)+ " <=? " + GmGenUtils::ftoa(indexVal)+" <=? "+GmGenUtils::ftoa(theMaximum)).c_str());
      return 0;
    } else if( theAllowOutOfLimits == 0 ) {
      G4Exception("GmVNumericDistribution::GmGenUtils::GetValue",
		  "Value is smaller than mininum",
		  FatalException,
		  G4String("DISTRIBUTION= " + theName + ": VALUE= " + GmGenUtils::ftoa(indexVal) + " MINIMUM= " + GmGenUtils::ftoa(theMinimum)).c_str());
    } else {
      return 0;
    } 
  }
  if( indexVal > theMaximum*1.000001 ) { // precision problems
    if( theAllowOutOfLimits == 1 ) {
      G4Exception(G4String(theName+"::GetIndexFromValue").c_str(),
		  "Value out of limits",
		  JustWarning,
		  G4String(GmGenUtils::ftoa(theMinimum)+ " <=? " + GmGenUtils::ftoa(indexVal)+" <=? "+GmGenUtils::ftoa(theMaximum)).c_str());
      return INT_MAX;
    } else if( theAllowOutOfLimits == 0 ) {
      G4Exception("GmVNumericDistribution::GmGenUtils::GetValue",
		  "Value is bigger than the maxinum",
		  FatalException,
		  G4String("DISTRIBUTION= " + theName + ": VALUE= " + GmGenUtils::ftoa(indexVal) + " MAXIMUM= " + GmGenUtils::ftoa(theMaximum)).c_str());
    } else {
      return INT_MAX;
    }
    
  }

  std::map<G4double,G4double>::const_iterator iteUp = theValues.upper_bound(indexVal);
  std::map<G4double,G4double>::const_iterator iteDown = iteUp;
  if( iteUp != theValues.begin() ) iteDown--;
  if( iteUp == theValues.end() ) { // value is = theMaximum
    iteUp--;
    return (*iteUp).second;
  }

  G4double indexDown = (*iteDown).first;
  G4double indexUp = (*iteUp).first;
  G4double valDown = (*iteDown).second;
  G4double valUp = (*iteUp).second;

  // interpolate
  G4double result = InterpolateValue( indexVal, indexDown, indexUp, valDown, valUp );

#ifndef GAMOS_NO_VERBOSE
  if( DistVerb(debugVerb) ) {
    G4cout << " GmVNumericDistribution::GetNumericValueFromIndex " 
	   << " indexVal= " << indexVal
	   << " indexDown= " << indexDown
	   << " indexUp= " << indexUp
	   << " valDown= " << valDown
	   << " valUp= " << valUp 
	   << " result= " << result
	   <<G4endl;
  }
#endif

  return result;
}

//---------------------------------------------------------------------
void GmVNumericDistribution::Normalize()
{

  std::map<G4double,G4double>::iterator ite;
  G4double valMax = -DBL_MAX;
  for( ite = theValues.begin(); ite != theValues.end(); ite++ ){
    valMax = std::max(valMax,(*ite).second);
  }

  for( ite = theValues.begin(); ite != theValues.end(); ite++ ){
    (*ite).second /= valMax;
  }

}
