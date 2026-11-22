#include "FileData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <set>

//-----------------------------------------------------------------------------
G4double FileData::theNEventsSum = 0;

//-----------------------------------------------------------------------------
FileData::FileData()
{
}

//-----------------------------------------------------------------------------
msmspdd FileData::GetPlotData( G4bool bLogX )
{
  G4bool bLogXOrig = bLogX;
  msmspdd plotData;
  for( unsigned int ii = 0; ii < theScorerData.size(); ii++ )  {
    bLogX = bLogXOrig;
    ScorerData* scorerData = theScorerData[ii];
    //--- Check name is not repeated
    G4String scorerName = scorerData->GetName();
    if( plotData.find(scorerName) != plotData.end() ) {
      G4Exception("FileData::GetPlotData",
		  "",
		  JustWarning,
		  (G4String("Repeated scorer name: ")+scorerName).c_str());
    } 
    if( ScoreData::verbosity >= 2 ) G4cout << "@@@ FileData::GetPlotData " << scorerName << G4endl;
    plotData[scorerName] = scorerData->GetPlotData(bLogX);
    if( bLogX && !IsConsistentLogX(plotData[scorerName]) ) {
      bLogX = 0;
      plotData[scorerName] = scorerData->GetPlotData(bLogX);
    }
  }
  
  return plotData;
}

//-----------------------------------------------------------------------------
G4bool FileData::IsConsistentLogX( mspdd* scorerValues ) 
{
  mspdd::const_iterator ite;
  for( ite = scorerValues->begin(); ite != scorerValues->end(); ite++ ) {
    G4String key = ite->first;
    if( GmGenUtils::IsNumber(key) ) {
      G4double keyd = GmGenUtils::GetValue(key);
      if( keyd <= 0. ) {
	G4Exception("FileData::IsConsistentLogX",
		    "",
		    JustWarning,
		    (G4String("Asking for logarithmic X axis while values are <= 0 : ") + (*ite).first).c_str());
	return false;
      }
    } else {
      return true;
    }
  }

  return true;

}

//-----------------------------------------------------------------------------
void FileData::Add( const FileData* fd2 )
{
  
  /*std::vector<ScorerData*>::iterator ite1, ite2;
    for( ite1 = theScorerData.begin(), ite2.GetScorerData().begin(); ite1 != theScorerData.end(); ite++ ){
    ScorerData* sd = *ite;
    
    }*/
  if( ScoreData::verbosity >= 2 ) 
    G4cout << " FileData::operator+=: Number of scorers in file= " << theScorerData.size() << G4endl;
  
  std::set<ScorerData*> scorerData2Used;

  // loop to scorer data in file1 and find correspoding scorer in file2
  unsigned int siz1 = GetNumberOfScorers();
  unsigned int siz2 = fd2->GetNumberOfScorers();
  for( unsigned int ii = 0; ii < siz1; ii++ ){
    ScorerData* sd1 = theScorerData[ii];
    for( unsigned int jj = 0; jj < siz2; jj++ ){
      ScorerData* sd2 = fd2->GetScorerData(jj);
      //--- sum scorers in two files if they are == (same name)
      if( ! (*sd1 != *sd2) ) {
	*sd1 += *sd2;
	scorerData2Used.insert(sd2);
      }    
    }
  }    

  for( unsigned int ii = 0; ii < siz2; ii++ ){
    ScorerData* sd1 = fd2->GetScorerData(ii);
    if( scorerData2Used.find(sd1) != scorerData2Used.end() ) continue;
    
    for( unsigned int jj = 0; jj < siz1; jj++ ){
      ScorerData* sd2 = theScorerData[jj];
      //--- sum scorers in two files if they are == (same name)
      if( ! (*sd1 != *sd2) ) {
	*sd1 += *sd2;
      }    
    }
  }    

}

//-----------------------------------------------------------------------------
void FileData::AddScorerData(ScorerData* sd)
{
  theScorerData.push_back(sd);
  if( ScoreData::verbosity >= 1 ) G4cout << "@@@  FileData::AddScorerData  N= " << theScorerData.size() << G4endl; 

}


//-----------------------------------------------------------------------------
ScorerData* FileData::GetScorerData(unsigned int ii) const
{
  return theScorerData[ii];
}

