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
#endif
//---------------------------------------------------------------------
GmVNumericNDimDistribution::GmVNumericNDimDistribution(G4String name)
: GmVDistribution(name)
{
  Initialize();
}

//---------------------------------------------------------------------
void GmVNumericNDimDistribution::Initialize()
{
  bAllowOutOfLimits = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theName+":AllowOutOfLimits",1 ));

  this->BuildData();
  this->ReadFile();
}

//---------------------------------------------------------------------
void GmVNumericNDimDistribution::BuildData()
{  
  GmParameterMgr* parMgr = GmParameterMgr::GetInstance();
  std::vector<G4String> dataName;
  dataName = parMgr->GetVStringValue(theName+":Data",dataName);
  theNDim = dataName.size();
#ifndef GAMOS_NO_VERBOSE
  if( DistVerb(testVerb) ) {
    G4cout << " NDim::BuildData " <<theNDim  << G4endl; 
  }
#endif
  if(theNDim == 0 ) { 
    G4Exception("GmVNumericNDimDistribution::BuildData",
		"Data type is not defined",
		FatalException,
		G4String("Use command /gamos/setParam " + theName + ":Data").c_str());
  } 
  
  for( G4int ii = 0; ii < theNDim; ii++ ) {
    GmVData* data = GmDataMgr::GetInstance()->BuildData(dataName[ii]);
    if( data == 0 ) {
      G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		  "Data class not found",
		  FatalErrorInArgument,
		  G4String("DISTRIBUTION: "+theName 
			   + " Asking for data: " + data->GetName()).c_str());
    } else {
      if( ! data->CheckAvailability( DTStep ) ) {
	G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		    "Data class not available for DTStep",
		    FatalErrorInArgument,
		    G4String("DISTRIBUTION: "+theName 
			     + " Asking for data: " + data->GetName()).c_str());
      } 
      
      if( data->GetCType() == "char" ){ 
	G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		    "Data class cannot be of String type",
		    FatalErrorInArgument,
		    G4String("DISTRIBUTION: "+theName 
			     + " Asking for data: " + data->GetName()).c_str());
	
      }
    }
    theDataNDim.push_back(data);

  }

  /*  for( size_t ii = 0; ii < theNDim; ii++ ){
    theIndices.push_back(std::set<G4double>);
    }*/

}

//---------------------------------------------------------------------
void GmVNumericNDimDistribution::ReadFile()
{
  //  G4cout << " GmVNumericNDimDistribution::ReadFile " << G4endl;

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
  } else if( fileNameText != "" ) {
    CheckOtherFileNames( fileNameROOT, fileNameCSV, fileNameText, fileName, 2 );

    ReadFileText( fileNameText );
  } else if( fileName != "" ) {
    ReadFileUnknown( fileName );
  } else {
    G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		"No file name given",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+GetName()).c_str());
  }

}


//---------------------------------------------------------------------
void GmVNumericNDimDistribution::CheckOtherFileNames( G4String& fileNameROOT, G4String& fileNameCSV, G4String& fileNameText, G4String& fileName, G4int fNumber )
{
  std::vector<G4String> fileNames;
  fileNames.push_back(fileNameROOT);
  fileNames.push_back(fileNameCSV);
  fileNames.push_back(fileNameText);

  G4bool bDuplicated = false;
  for( unsigned int ii = 0; ii < 3; ii++ ){
    if( fileNames[ii] != "" && fileNames[ii] != fNumber ) {
      bDuplicated = true;
    }
  }

  if( bDuplicated ){
    G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
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
    G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		G4String("if "+fname + " is used, fileName will be overridden").c_str(),
		JustWarning,
		G4String("DISTRIBUTION: "+theName +
			 fname +
			 " fileName= " + fileName).c_str());
  }
  
}

//---------------------------------------------------------------------
void GmVNumericNDimDistribution::ReadFileUnknown( G4String& fileName )
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
void GmVNumericNDimDistribution::ReadFileROOT( G4String& )
{
  /*  G4String histoName = GmParameterMgr::GetInstance()->GetStringValue(theName+":ROOTHistoName","");
  if( histoName == "" ){
    G4Exception("GmVNumericNDimDistribution::ReadFileROOT",
		"ROOT histogram name not provided",
		FatalErrorInArgument,
		G4String("Please use /gamos/setParam "+theName+":ROOTHistoName").c_str());
  }

  TFile file( fileName );
  TH2F* his = (TH2F*)(file.Get(histoName));
  if( his == 0 ) {
    G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		"ROOT 2D histogram not found in file",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName+ " HISTOGRAM:" + histoName).c_str());
  }

  TAxis* xAxis = his->GetXaxis();
  TAxis* yAxis = his->GetYaxis();
  for( G4int ix = 1; ix < his->GetNbinsX()-1; ix++ ){
    for( G4int iy = 1; iy < his->GetNbinsY()-1; iy++ ){
      //   theIndices[0].insert(xAxis->GetBinCenter(ix));
      //t      theIndices[1].insert(yAxis->GetBinCenter(iy));
      theValueIndices.push_back(ix);
      theValueIndices.push_back(iy);
      theValuesNDim[ theValueIndices ] = his->GetBinContent(ix,iy);
    }
  }
   
  theMinimumNDim.push_back(((*(theValuesNDim.begin())).first)[0] - xAxis->GetBinWidth(1)/2.);
  theMinimumNDim.push_back(((*(theValuesNDim.begin())).first)[1] - xAxis->GetBinWidth(1)/2.);
  theMaximumNDim.push_back(((*(theValuesNDim.rbegin())).first)[0] + xAxis->GetBinWidth(1)/2.);
  theMaximumNDim.push_back(((*(theValuesNDim.rbegin())).first)[1] + xAxis->GetBinWidth(1)/2.);
  */			
}
#endif

//---------------------------------------------------------------------
void GmVNumericNDimDistribution::ReadFileCSV( G4String& )
{
  /*
  G4String histoName = GmParameterMgr::GetInstance()->GetStringValue(theName+":CSVHistoName","");
  if( histoName == "" ){
    G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		"CSV histogram name not provided",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName).c_str());
  }

  GmHistoReaderCSV* histoReader = new GmHistoReaderCSV( fileName );
  GmHisto1* his = histoReader->GetHisto1(histoName);
  if( his == 0 ) {
    G4Exception("GmVNumericNDimDistribution::GmVNumericNDimDistribution",
		"CSV histogram not found in file",
		FatalErrorInArgument,
		G4String("DISTRIBUTION: "+theName+ " HISTOGRAM:" + histoName).c_str());
  }

  for( G4int ii = 1; ii < his->GetNbinsX()-1; ii++ ){
    theValues[ his->GetBinCenter(ii) ] = his->GetBinContent(ii);
  }
    
  theMinimum = (*(theValues.begin())).first - his->GetBinWidth(1)/2.;
  theMaximum = (*(theValues.rbegin())).first + his->GetBinWidth(1)/2.;
  */
}

//---------------------------------------------------------------------
void GmVNumericNDimDistribution::ReadFileText( G4String& fileName )
{
  // Read energy - probability  pairs
  fileName = GmGenUtils::FileInPath( fileName);
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  //  G4int ii = 1;
  std::vector<G4int> changingIndices;
  std::vector<G4double> previousIndices;
  std::vector<G4double> currentIndices;
  std::vector<G4int> currentIds; // Id corresponding to index
  std::vector<G4int> lastLineChanged;
  std::vector<G4int> NLinesChanged; // check that for each dimension the change of index happens at a constant number of lines
  std::vector<G4int> firstChangeLine; // line where change happens for each dimension
  std::vector<G4int> NLinesChange; // number of lines change should happen  for each dimension (set when first change happens)
  std::vector< std::vector<G4double> > IndicesRead; // copy of all indices read
  std::vector<G4int> Dims;
  for( G4int ii = 0; ii < theNDim; ii++ ){
    changingIndices.push_back(0);
    previousIndices.push_back(0.);
    currentIndices.push_back(0.);
    currentIds.push_back(0);
    lastLineChanged.push_back(-1);
    NLinesChanged.push_back(-1);
    firstChangeLine.push_back(-1);
    NLinesChange.push_back(-1);
    Dims.push_back(0);
    theIndices.push_back( new std::set<G4double> );
  }
  G4int NLine = 0;
  for( ;; NLine++ ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( G4int(wl.size()) != theNDim+1) {
      G4Exception("GmVNumericNDimDistribution::ReadFileText",
		  "Error in number of words in line",
		  FatalErrorInArgument,
		  G4String("Reading file " + fileName 
			   + ", line number " + GmGenUtils::itoa(NLine) 
			   + " All lines must have " + GmGenUtils::itoa(theNDim) + " words: " + GmGenUtils::itoa(theNDim) + " VALUE(s) PROBABILITY").c_str());
    }
    /*#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(testVerb) ) {
       for( G4int ii = 0; ii < theNDim+1; ii++ ){
	 G4cout << " GmVNumericNDimDistribution::ReadFileText " << theValuesNDim.size() << " " << ii << " : " << GmGenUtils::GetValue( wl[ii] ) << G4endl;
       }
    }
    #endif */
    /*    if( NLine != 0 ) {
	  for( size_t ii = 0; ii < theNDim; ii++ ){
	  currentIndices[ii] = GmGenUtils::GetValue( wl[ii] );
	  }
	  }
    */    
    for( G4int ii = 0; ii < theNDim; ii++ ){
      currentIndices[ii] = GmGenUtils::GetValue( wl[ii] );
      std::set<G4double>* id1 = theIndices[ii];
      if( ii == 0 || theIndices[ii-1]->size() == 1 ) {  // if previous index has changed, the number of different values of this index is fixed
	//------ Check all values increase
    //	std::set<G4double>::reverse_iterator iterev = id1->rbegin();
	if( id1->size() > 0 && id1->find(currentIndices[ii]) == id1->end() && currentIndices[ii] <= (*(id1->rbegin())) ) {
	    G4Exception("GmVNumericNDimDistribution::ReadFileText",
			"NDD01",
			FatalException,
			("Wrong indices: there must be always crecent. Index " + GmGenUtils::itoa(ii) + " : " + GmGenUtils::ftoa(currentIndices[ii]) + " is not > " + GmGenUtils::ftoa((*(id1->rbegin())))).c_str());
	}
	id1->insert(currentIndices[ii]);
      }
      std::set<G4double>::const_iterator iteidx = id1->find(currentIndices[ii]);
      currentIds[ii] = std::distance(id1->begin(), iteidx);
#ifndef GAMOS_NO_VERBOSE
      if( DistVerb(testVerb) ) {
	G4cout << " NDim::ReadFileText NIndices " << ii << " "<< id1->size() << "=" << theIndices[ii]->size() << " Current Id " << currentIds[ii] << " val= " << currentIndices[ii] << G4endl;
      }
#endif
    }
    theValuesNDim[currentIds] = GmGenUtils::GetValue( wl[theNDim] );
    IndicesRead.push_back(currentIndices);
  } // END FILE READING

  //---------- check order of changing is correct
  for( G4int ii = 0; ii < theNDim; ii++ ){
    Dims[ii] = theIndices[ii]->size();
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(testVerb) ) {
      G4cout << " NDim::RFT NDIM " << ii << " "<< Dims[ii] << G4endl;
    }
#endif
  }
#ifndef GAMOS_NO_VERBOSE
  if( DistVerb(testVerb) ) {
    for( size_t ir = 0; ir < IndicesRead.size(); ir++ ){
      for( G4int ii = 0; ii < theNDim; ii++ ){
	G4cout << " NDim::RFT IndicesRead " << ir << ":" << ii << " "<< IndicesRead[ir][ii]  << G4endl;
      }
    }
  }
#endif
  
  for( G4int ii = 0; ii < theNDim; ii++ ){
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(testVerb) ) {
      G4cout << " NDim::RFT CO Dims " << ii << " " << Dims[ii]  << G4endl; 
    }
#endif
    G4int NFactor = 1;
    for( G4int jj = ii+1; jj < theNDim; jj++ ){
      NFactor *= Dims[jj];
    }
    G4int NFactorInv = 1;
    for( G4int jj = ii+1; jj < theNDim; jj++ ){
      NFactorInv *= Dims[jj];
    }
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(testVerb) ) {
      G4cout << " NDim::RFT CO NFactor " << ii << " " << NFactor  << G4endl; 
    }
#endif
    // check that indices are unique
    for( int iid = 0; iid < Dims[ii]; iid++ ){
      G4double firstIndex = IndicesRead[NFactor*iid][ii];
#ifndef GAMOS_NO_VERBOSE
      if( DistVerb(testVerb) ) {
	G4cout << " NDim::RFT CO firstIndex " << ii << " " << iid << " =" << firstIndex << " " << NFactor*iid << G4endl; 
      }
#endif
      for( size_t ir = 0; ir < IndicesRead.size(); ir++ ){
#ifndef GAMOS_NO_VERBOSE
	if( DistVerb(testVerb) ) {
	  G4cout << " NDim::RFT CO ir " << ir << G4endl;
	  G4cout << " NDim::RFT CO check id IndicesRead " << ii << " " << iid << " =? " <<  G4int(ir)/NFactor%Dims[ii] << " " << ir << " / " << NFactor << " % " << Dims[ii] << G4endl;
	}
#endif
	if( G4int(ir)/NFactor%Dims[ii] == iid ) {
	  if( firstIndex != IndicesRead[ir][ii] ) {
	    
	    G4cerr << "!! ERROR AT LINE " << ir+1 << " INDEX " << ii+1 << " = " <<  IndicesRead[ir][ii] << " IS DIFFERENT THAN INDEX AT LINE " << NFactor*iid+1 << " = " << firstIndex << G4endl;
	    G4Exception("GmVNumericNDimDistribution::ReadFileText",
			"NDD02",
			FatalException,
			"Wrong indices: there must be crecent from right to left: 2 3 5, 2 3 6, 2 3 8, 2 7 5, 2 7 6, 2 7 8, 3 3 5, ...");
	  }	 
	}
      }
    }

  }

    
  fin.Close();

  for( G4int ii = 0; ii < theNDim; ii++ ){
    theMinimumNDim.push_back( (*(IndicesRead.begin()))[ii]);
    theMaximumNDim.push_back( (*(IndicesRead.rbegin()))[ii]);
  }
  
}


//---------------------------------------------------------------------
G4int GmVNumericNDimDistribution::GetFileType( G4String& fileName )
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
G4double GmVNumericNDimDistribution::GetValueFromStep(const G4Step* aStep)
{
  std::vector<G4double> indexVals;
  for( G4int ii = 0; ii < theNDim; ii++ ) {
    indexVals.push_back(theDataNDim[ii]->GetValueFromStep( aStep ) );
  }
  return GetNumericValueFromIndex( indexVals );
}

//---------------------------------------------------------------------
G4double GmVNumericNDimDistribution::GetValueFromTrack(const G4Track* aTrack)
{

  std::vector<G4double> indexVals;
  for( G4int ii = 0; ii < theNDim; ii++ ) {
    indexVals.push_back(theDataNDim[ii]->GetValueFromTrack( aTrack ) );
    //    G4cout << ii <<  " GETTING VALUE " << theDataNDim[ii]->GetName() << " = " << indexVals[ii] << G4endl; //GDEB 
  }
  
  return GetNumericValueFromIndex( indexVals );
}

//---------------------------------------------------------------------
G4double GmVNumericNDimDistribution::GetNumericValueFromIndex(const std::vector<G4double> indexVals)
{
  std::vector<G4int> IndicesIdDown;
  std::vector<G4int> IndicesIdUp;
  for( G4int ii = 0; ii < theNDim; ii++ ) {
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(debugVerb) ) {
      G4cout << " NDim GNVFI indexVals:" << ii << " = " << indexVals[ii] << G4endl;
    }
#endif
    G4double PRECISION = fabs(1e-5*theMinimumNDim[ii]);
    if( (indexVals[ii]-theMinimumNDim[ii]) < -PRECISION ) {
      if( bAllowOutOfLimits ) {
	G4Exception(G4String(theName+"::GetIndexFromValue").c_str(),
		    "Value is smaller than mininum, output is 0.",
		    JustWarning,
		    G4String("DISTRIBUTION= " + theName + ": " + GmGenUtils::itoa(ii) + " VALUE= " + GmGenUtils::ftoa(indexVals[ii]) + " MINIMUM= " + GmGenUtils::ftoa(theMinimumNDim[ii])+" DIFF= "+GmGenUtils::ftoa(indexVals[ii]-theMinimumNDim[ii]) ).c_str());
	return 0;
      } else {
	G4Exception("GmVNumericNDimDistribution::GmGenUtils::GetValue",
		    "Value is smaller than mininum",
		    FatalException,
		    G4String("DISTRIBUTION= " + theName + ": " + GmGenUtils::itoa(ii) + " VALUE= " + GmGenUtils::ftoa(indexVals[ii]) + " MINIMUM= " + GmGenUtils::ftoa(theMinimumNDim[ii])).c_str());
      }
    }
    
    if( indexVals[ii] > theMaximumNDim[ii]*(1.+PRECISION) ) { // precision problems
      if( bAllowOutOfLimits ) {
	G4Exception(G4String(theName+"::GetIndexFromValue").c_str(),
		     "Value is bigger than the maximum, output is 0.",
		    JustWarning,
		    G4String("DISTRIBUTION= " + theName + ": " + GmGenUtils::itoa(ii) + " VALUE= " + GmGenUtils::ftoa(indexVals[ii]) + " MAXIMUM= " + GmGenUtils::ftoa(theMaximumNDim[ii])).c_str());
	return 0;
      } else {
	G4Exception("GmVNumericNDimDistribution::GmGenUtils::GetValue",
		    "Value is bigger than the maxinum",
		    FatalException,
		    G4String("DISTRIBUTION= " + theName + ": " + GmGenUtils::itoa(ii) + " VALUE= " + GmGenUtils::ftoa(indexVals[ii]) + " MAXIMUM= " + GmGenUtils::ftoa(theMaximumNDim[ii])).c_str());
      }
    }
    
    std::set<G4double>* id1 = theIndices[ii];
    std::set<G4double>::const_iterator iteUp = id1->upper_bound(indexVals[ii]);
    std::set<G4double>::const_iterator iteDown = iteUp;
    if( iteUp != id1->begin() ) iteDown--;
    if( iteUp == id1->end() ) { // value is = theMaximum
      iteUp--;
    }

    IndicesIdDown.push_back(std::distance(id1->begin(),iteDown));
    IndicesIdUp.push_back(std::distance(id1->begin(),iteUp));

#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(testVerb) ) {
      G4cout << " NDim GNVFI " << ii << " Setting IDDown " << IndicesIdDown[ii] << " IDUp " << IndicesIdUp[ii] << G4endl; 
      /* std::set<G4double>::const_iterator itese;
	  for( itese = id1->begin(); itese != id1->end(); itese++ ) {
      G4cout << " NDim GNVFI id1 " << *itese << G4endl;//GDEB
      } */
  }
#endif
  }

  //--build NDim X NDim values

  //  std::vector<std::set<G4double>::const_iterator> iteV;
  std::vector<G4int> iteLoop;
  G4int nIter = 1;
  std::vector<G4int> nIters;
  for( G4int ii = 0; ii < theNDim; ii++ ) {
    //    iteV.push_back(IndicesIdDown[ii]);
    iteLoop.push_back(0);
    G4int nIter1 = ( IndicesIdUp[ii] - IndicesIdDown[ii] +1);
    nIter *= nIter1;
    nIters.push_back(nIter1);
    //    G4cout << ii << " NDim GNVFI nIter " << nIter << G4endl; //GDEB
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( DistVerb(testVerb) ) {
    G4cout << " NDim GNVFI nIter " << nIter << G4endl;
  }
#endif
  std::vector<std::vector<G4int> > indices;
  for( G4int ii = 0; ii < nIter; ii++ ) {
   //check which iterator index advances
    /* gets stuck in infinite loop    if( std::distance(iteV[jj],IndicesIdUp[jj]) < 0 ) 
    for( G4int jj = theNDim-1; jj >= 0; jj-- ) {
      iteV[jj]++;
      if( std::distance(iteV[jj],IndicesIdUp[jj]) < 0 ) {
  G4cout << " NDim GNVFI iteV " << ii << ":" << jj << G4endl; //GDEB
	iteV[jj] = IndicesIdDown[jj];
      } else {
	break;
      }
      }
    std::vector<G4int> indices1;
    for( G4int jj = 0; jj < theNDim; jj++ ) {
      std::set<G4double>* id1 = theIndices[jj];
      iteV[jj] = IndicesIdDown[jj];
      indices1.push_back( std::distance(iteV[jj], id1->begin()) );
    }
    indices.push_back( indices1 );
  }
 */
#ifndef GAMOS_NO_VERBOSE
    if( DistVerb(testVerb) ) {
      for( G4int jj = theNDim-1; jj >= 0; jj-- ) {
	G4cout << " 0NDim GNVFI iteLoop " << ii << ":" << jj << " = " << iteLoop[jj] << " + " << IndicesIdDown[jj] << " UP " << IndicesIdUp[jj] <<G4endl;
      }
    }
#endif
    std::vector<G4int> indices1;
    for( G4int jj = 0; jj < theNDim; jj++ ) {
      indices1.push_back(IndicesIdDown[jj]+ iteLoop[jj] );
    }
    indices.push_back( indices1 );
    for( G4int jj = theNDim-1; jj >= 0; jj-- ) {
      iteLoop[jj]++;
      if( iteLoop[jj] >= nIters[jj] ) {
	iteLoop[jj] = 0;
      } else {
	break;
      }
      //      G4cout << " NDim GNVFI iteLoop " << ii << ":" << jj << " = " << iteLoop[jj] << G4endl; //GDEB
    }
  }
  #ifndef GAMOS_NO_VERBOSE
  if( DistVerb(testVerb) ) {
    for( size_t ii = 0; ii < indices.size(); ii++ ) {
      std::vector<G4int> indices1 = indices[ii];
      G4cout << " NDim GNVFI " << ii << ":"; 
      for( size_t jj = 0; jj < indices1.size(); jj++ ) {
	G4cout << jj << " = " << indices1[jj] << " ";
      }
      G4cout << G4endl;
    }
  }
#endif

  // interpolate
  G4double result = InterpolateValue( indices );

#ifndef GAMOS_NO_VERBOSE
  if( DistVerb(debugVerb) ) {
    /*    G4cout << " GmVNumericNDimDistribution::GetNumericValueFromIndex " 
	   << " indexVals[ii]= " << indexVals[ii]
	   << " indexDown= " << indexDown
	   << " indexUp= " << indexUp
	   << " valDown= " << valDown
	   << " valUp= " << valUp 
	   << " result= " << result
	   <<G4endl; */
  }
#endif

  return result;

}

//---------------------------------------------------------------------
void GmVNumericNDimDistribution::Normalize()
{
  /*
  std::map<G4double,G4double>::iterator ite;
  G4double valMax = -DBL_MAX;
  for( ite = theValues.begin(); ite != theValues.end(); ite++ ){
    valMax = std::max(valMax,(*ite).second);
  }

  for( ite = theValues.begin(); ite != theValues.end(); ite++ ){
    (*ite).second /= valMax;
  }
  */
}
