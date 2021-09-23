#include "GmClassifierByPhantomStructure.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GmReadPhantomStMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Track.hh"

//---------------------------------------------------------------
GmClassifierByPhantomStructure::GmClassifierByPhantomStructure(G4String name) : GmVClassifier( name )
{
  theReadPhantomStMgr = GmReadPhantomStMgr::GetInstance();
  if( theReadPhantomStMgr->GetStructs().size() == 0 ) {
    G4Exception("GmClassifierByPhantomStructure::GmClassifierByPhantomStructure",
		"",
		FatalErrorInArgument,
		" No structure found in G4 DICOM file");
  }
  
#ifdef WIN32
  theMaxIndex = int64_t(std::pow(double(2),int(32)-1)-1); // sizeof(int64_t) = 32 gives error 
#else
  theMaxIndex = int64_t(pow(double(2.),int(sizeof(int64_t)*8)-1)-1);
  //  G4cout << " MAXINDEX " << theMaxIndex << " sizeof " << sizeof(int64_t) << G4endl;
#endif
}

//---------------------------------------------------------------
void GmClassifierByPhantomStructure::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 0 ){
    G4Exception("GmClassifierByPhantomStructure::GmClassifierByPhantomStructure",
		"",
		FatalErrorInArgument,
		" Number of parameters different than 0");
  }

}

//---------------------------------------------------------------
int64_t GmClassifierByPhantomStructure::GetIndexFromStep(const G4Step* aStep)
{
  int64_t copyNo = aStep->GetTrack()->GetVolume()->GetCopyNo();

  G4String stid1 = theReadPhantomStMgr->GetStID( copyNo );
  std::vector<G4String> stidlistStr = GmGenUtils::StringSplit(stid1,":");
  int64_t stid;
  for( size_t ii = 0; ii < stidlistStr.size(); ii++ ) {
    stid += GmGenUtils::GetInt(stidlistStr[ii])*std::pow(100,ii);
  }

  return stid;

}

//---------------------------------------------------------------
int64_t GmClassifierByPhantomStructure::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t copyNo = aTrack->GetVolume()->GetCopyNo();

  G4String stid1 = theReadPhantomStMgr->GetStID( copyNo );
  std::vector<G4String> stidlistStr = GmGenUtils::StringSplit(stid1,":");
  int64_t stid;
  for( size_t ii = 0; ii < stidlistStr.size(); ii++ ) {
    stid += GmGenUtils::GetInt(stidlistStr[ii])*std::pow(100,ii);
  }

  return stid;

}


//-------------------------------------------------------------
void GmClassifierByPhantomStructure::SetIndices( std::vector<G4String> wl )
{

  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
    theIndexMap[int64_t(GmGenUtils::GetValue(wl[ii]))] = index;
    if( theMaxIndex < index) theMaxIndex = index;    
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
