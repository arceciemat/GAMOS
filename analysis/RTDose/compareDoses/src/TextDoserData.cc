#include "TextDoserData.hh"
#include "DoseData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <set>

//----------------------------------------------------------------------
TextDoserData::TextDoserData() : DoserData()
{
  theDoseType = ST_TEXT;

}

//----------------------------------------------------------------------
void TextDoserData::Print( std::ofstream& fout )
{
  fout << " Doser: " << theName << G4endl;
  fout << " Number of entries= " << theNDoses << G4endl;
  
  if( DoseData::verbosity >= 2 ) G4cout << "  TextDoserData::Print: " << theName << G4endl; 
 
  DoseData* totalDoseData = 0;
  for( unsigned int ii = 0; ii < theDoseData.size(); ii++ ) {
    DoseData* doseData =  theDoseData[ii];
    if( !doseData->IsTotal() ) { 
      doseData->Print(fout);
    } else {
      totalDoseData = doseData;
    }
  }

  totalDoseData->Print(fout);

}


//----------------------------------------------------------------------
G4double TextDoserData::GetIndexVal( G4String doseName, G4bool bLogX ) 
{
  DoseIndexValType theIndexValType = SIVT_LOWER;

  G4double indexVal = 0.;
  //--- Find separating '-' 
  // if found get value between the two values, if not take single value
  size_t igf = std::string::npos;
  for( size_t ii = 1; ii < doseName.length(); ii++ ) {
    if( doseName[ii] == '-' && doseName[ii-1] != 'E' && doseName[ii-1] != 'e' ){
      igf = ii;
      //      G4cout << doseName <<  " FOUN " << igf << " " << doseName [igf-1] << G4endl;//GDEB
    }
  }
  if( igf == std::string::npos ) { // NOT FOUND
    return GmGenUtils::GetValue(doseName);
  }

  //  size_t igInterval = igf; // position of '-' defining an interva
  //  G4cout << " IGF " << igf << " " << doseName.substr(0,igf) << " " << doseName.substr(igf+1,doseName.length()) << G4endl;//GDEB
  G4double valLower = GmGenUtils::GetValue(doseName.substr(0,igf));
  G4double valUpper = GmGenUtils::GetValue(doseName.substr(igf+1,doseName.length()));
  if( theIndexValType == SIVT_MIDDLE ) {
    if( !bLogX ) {
      indexVal = (valLower+valUpper)/2.;
    } else {
      G4double valSq = valLower*valUpper;
      if( valSq >= 0 ) {
	indexVal = std::sqrt(valSq) ;
	if( valLower < 0. || valUpper < 0. ) indexVal *= -1;
      } else {
	indexVal = 0.;
      }
    }
  } else if( theIndexValType == SIVT_LOWER ) {
    indexVal = valLower;
  } else if( theIndexValType == SIVT_UPPER ) {
    indexVal = valUpper;
  }
  if( DoseData::verbosity >= 2 ) G4cout << " GetIndexVal " << doseName << " -> " << indexVal << " Lower " << valLower << " Upper " << valUpper << " theIndexValType " << DoseData::theIndexValType << G4endl;

  if( DoseData::verbosity >= 2 ) G4cout << " GetIndexVal " << doseName << " -> " << indexVal  << G4endl;

  return indexVal;
}
