#include "FileData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <set>

//-----------------------------------------------------------------------------
G4double FileData::theNEventsSum = 0;

//-----------------------------------------------------------------------------
FileData::FileData()
{
  bInvertX = false;
}

//-----------------------------------------------------------------------------
vpsmdpdd FileData::GetPlotData( G4bool bLogX )
{
  G4bool bLogXOrig = bLogX;
  vpsmdpdd plotData;
  for( unsigned int ii = 0; ii < theDoserData.size(); ii++ )  {
    bLogX = bLogXOrig;
    DoserData* doserData = theDoserData[ii];
    //--- Check name is not repeated
    G4String doserName = doserData->GetName();
    vpsmdpdd::iterator itevp;
    for( itevp = plotData.begin(); itevp != plotData.end(); itevp++ ) {
      if( (*itevp).first == doserName ) break;
    }
    if( itevp != plotData.end() ) {
      G4Exception("FileData::GetPlotData",
		  "",
		  JustWarning,
		  (G4String("Repeated doser name: ")+doserName).c_str());
    } 
    if( DoseData::verbosity >= 2 ) G4cout << "@@@ FileData::GetPlotData " << doserName << G4endl;
    plotData.push_back(psmdpdd(doserName,doserData->GetPlotData(bLogX)));
    if( bLogX && !IsConsistentLogX(doserData->GetPlotData(bLogX)) ) {
      bLogX = 0;
      plotData.pop_back();
      plotData.push_back(psmdpdd(doserName,doserData->GetPlotData(bLogX)));
    }
  }
  
  return plotData;
}

//-----------------------------------------------------------------------------
G4bool FileData::IsConsistentLogX( mdpdd* doserValues ) 
{
  mdpdd::const_iterator ite;
  for( ite = doserValues->begin(); ite != doserValues->end(); ite++ ) {
    if( (*ite).first <= 0. ) {
      G4Exception("FileData::IsConsistentLogX",
		  "",
		  JustWarning,
		  (G4String("Asking for logarithmic X axis while values are <= 0 : ") + GmGenUtils::ftoa((*ite).first)).c_str());
      return false;
    }
  }

  return true;

}

//-----------------------------------------------------------------------------
void FileData::Add( const FileData* fd2 )
{
  
  /*std::vector<DoserData*>::iterator ite1, ite2;
    for( ite1 = theDoserData.begin(), ite2.GetDoserData().begin(); ite1 != theDoserData.end(); ite++ ){
    DoserData* sd = *ite;
    
    }*/
  if( DoseData::verbosity >= 2 ) 
    G4cout << " FileData::operator+=: Number of dosers in file= " << theDoserData.size() << G4endl;
  
  std::set<DoserData*> doserData2Used;

  // loop to doser data in file1 and find correspoding doser in file2
  unsigned int siz1 = GetNumberOfDosers();
  unsigned int siz2 = fd2->GetNumberOfDosers();
  for( unsigned int ii = 0; ii < siz1; ii++ ){
    DoserData* sd1 = theDoserData[ii];
    for( unsigned int jj = 0; jj < siz2; jj++ ){
      DoserData* sd2 = fd2->GetDoserData(jj);
      //--- sum dosers in two files if they are == (same name)
      if( ! (*sd1 != *sd2) ) {
	*sd1 += *sd2;
	doserData2Used.insert(sd2);
      }    
    }
  }    

  for( unsigned int ii = 0; ii < siz2; ii++ ){
    DoserData* sd1 = fd2->GetDoserData(ii);
    if( doserData2Used.find(sd1) != doserData2Used.end() ) continue;
    
    for( unsigned int jj = 0; jj < siz1; jj++ ){
      DoserData* sd2 = theDoserData[jj];
      //--- sum dosers in two files if they are == (same name)
      if( ! (*sd1 != *sd2) ) {
	*sd1 += *sd2;
      }    
    }
  }    

}

//-----------------------------------------------------------------------------
void FileData::AddDoserData(DoserData* sd)
{
  theDoserData.push_back(sd);
  if( DoseData::verbosity >= 2 ) G4cout << "@@@  FileData::AddDoserData  N= " << theDoserData.size() << G4endl; 

}


//-----------------------------------------------------------------------------
DoserData* FileData::GetDoserData(unsigned int ii) const
{
  return theDoserData[ii];
}

