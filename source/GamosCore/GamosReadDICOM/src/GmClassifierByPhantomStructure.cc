#include "GmClassifierByPhantomStructure.hh"
#include "GmReadDICOMVerbosity.hh"
#include "GmReadPhantomStMgr.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Classifiers/include/GmClassifierVerbosity.hh"

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
  int64_t copyNo = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();

  G4String stid1 = theReadPhantomStMgr->GetStID( copyNo );
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhantomStructure::GetIndexFromStep stid1 " << stid1 << G4endl;
#endif
  std::vector<G4String> stidlistStr = GmGenUtils::StringSplit(stid1,":");
  //  for( size_t ii = 0 ; ii < stidlistStr.size(); ii++ ) {
  //  G4cout << ii << " stidlistStr " << stidlistStr[ii] << G4endl; //GDEB
  // }
  int64_t stid = 0;
  for( size_t ii = 0; ii < stidlistStr.size(); ii++ ) {
    stid += GmGenUtils::GetInt(stidlistStr[ii])*std::pow(100,ii);
    // G4cout << ii << " stidlistStr[ii] " << stidlistStr[ii] << " * " << std::pow(100,ii) << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhantomStructure::GetIndexFromStep stid " << stid << " ii " << ii << G4endl;
#endif
  }

  return stid;

}

//---------------------------------------------------------------
int64_t GmClassifierByPhantomStructure::GetIndexFromTrack(const G4Track* aTrack)
{
  int64_t copyNo = aTrack->GetTouchable()->GetCopyNumber();

  G4String stid1 = theReadPhantomStMgr->GetStID( copyNo );
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhantomStructure::GetIndexFromTrack stid1 " << stid1 << G4endl;
#endif

  std::vector<G4String> stidlistStr = GmGenUtils::StringSplit(stid1,":");
  int64_t stid;
  for( size_t ii = 0; ii < stidlistStr.size(); ii++ ) {
    stid += GmGenUtils::GetInt(stidlistStr[ii])*std::pow(100,ii);
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhantomStructure::GetIndexFromTrack stid " << stid << " ii " << ii << G4endl;
#endif
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
#ifndef GAMOS_NO_VERBOSE
    if( ClassifierVerb(debugVerb) ) G4cout << " GmClassifierByPhantomStructure::SetIndices " << int64_t(GmGenUtils::GetValue(wl[ii])) << " = " << index << G4endl;
#endif

  }

  theMaxIndex -= theIndexMap.size()+1;  
}
