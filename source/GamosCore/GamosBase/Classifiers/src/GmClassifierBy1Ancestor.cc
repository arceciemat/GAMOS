#include "GmClassifierBy1Ancestor.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4RegularNavigationHelper.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
//#include "G4RegNavHelper.hh"

//-------------------------------------------------------------
GmClassifierBy1Ancestor::GmClassifierBy1Ancestor(G4String name) : GmVClassifier( name )
{
} 

//-------------------------------------------------------------
void GmClassifierBy1Ancestor::SetParameters( std::vector<G4String>& params )
{
  if( params.size() != 1 ){
    G4Exception("GmClassifierBy1Ancestor::GmClassifierBy1Ancestor"," Number of parameters different than 1",FatalErrorInArgument," Only parameter is ancestor level");
  }

  theIndexDepth = GmGenUtils::GetInteger(params[0]);
  //  G4cout << this << " GmClassifierBy1Ancestor::SetParameters indexDepth " << theIndexDepth << G4endl;
}

//-------------------------------------------------------------
int64_t GmClassifierBy1Ancestor::GetIndexFromStep(const G4Step* aStep)
{
  int64_t index;
  
  if( aStep->GetTrack()->GetDefinition()->GetPDGCharge() == 0 &&  G4RegularNavigationHelper::Instance()->GetStepLengths().size() > 1 ) {
    G4StepPoint* postStep = aStep->GetPostStepPoint();
    G4TouchableHistory* th = (G4TouchableHistory*)(postStep->GetTouchable()); 
    index = th->GetReplicaNumber(theIndexDepth);//ts0
  } else {
    G4StepPoint* preStep = aStep->GetPreStepPoint();
    G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  // G4cout << "GmClassifierBy1Ancestor::GetIndexFromStep "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetVolume()->GetCopyNo() << G4endl;
  //-  th = (G4TouchableHistory*)( aStep->GetPostStepPoint()->GetTouchable());
  /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndexFromStep "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;

    /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndexFromStep "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;
  //  G4cout << " GmClassifierBy1Ancestor::GetIndexFromStep( " <<  th->GetReplicaNumber(theIndexDepth) << " " << theIndexDepth << " touch " << th->GetVolume()->GetName() << G4endl;
    index = th->GetReplicaNumber(theIndexDepth);//ts0
  }
  
  if( theIndexMap.size() != 0 ) {
    std::map<int64_t,int64_t>::const_iterator ite = theIndexMap.find(index);
    if( ite == theIndexMap.end() ) {
      G4Exception("GmClassifierBy1Ancestor::GetIndexFromStep",
		  "Index not found",
		  FatalErrorInArgument,
		  G4String("Index = " + GmGenUtils::itoa(index)).c_str());
    } else {
      return (*ite).second;
    }
  }

  //  G4cout << " BY1ANCESTOR " << index << " " << indexPost << G4endl; //GDEB
 
  return index;

}

//-------------------------------------------------------------
int64_t GmClassifierBy1Ancestor::GetIndexFromTrack( const G4Track* aTrack )
{
  const G4Step* aStep = aTrack->GetStep();
  if( !aStep ) return 0;

  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4TouchableHistory* th = (G4TouchableHistory*)(preStep->GetTouchable());
  // G4cout << "GmClassifierBy1Ancestor::GetIndex "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetVolume()->GetCopyNo() << G4endl;
  //-  th = (G4TouchableHistory*)( aStep->GetPostStepPoint()->GetTouchable());
  /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndex "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;


    /// G4cout << aStep->GetTrack()->GetPosition().z() << "  GmClassifierBy1Ancestor::GetIndex "<< th->GetReplicaNumber(theIndexDepth) << " = " << aStep->GetTrack()->GetNextVolume()->GetCopyNo() << G4endl;
  //  G4cout << " GmClassifierBy1Ancestor::GetIndex( " <<  th->GetReplicaNumber(theIndexDepth) << " " << theIndexDepth << " touch " << th->GetVolume()->GetName() << G4endl;
  int64_t index = th->GetReplicaNumber(theIndexDepth);//ts0
  if( theIndexMap.size() != 0 ) {
    std::map<int64_t,int64_t>::const_iterator ite = theIndexMap.find(index);
    if( ite == theIndexMap.end() ) {
      G4Exception("GmClassifierBy1Ancestor::GetIndex",
		  "Index not found",
		  FatalErrorInArgument,
		  G4String("Index = " + GmGenUtils::itoa(index)).c_str());
    } else {
      return (*ite).second;
    }
  }
  
  return index;

}

//-------------------------------------------------------------
void GmClassifierBy1Ancestor::SetIndices( std::vector<G4String> wl )
{

  for( unsigned int ii = 0; ii < wl.size(); ii+=2 ){
    int64_t index = int64_t(GmGenUtils::GetValue(wl[ii+1]));
    theIndexMap[int64_t(GmGenUtils::GetValue(wl[ii]))] = index;
    if( theMaxIndex < index) theMaxIndex = index;    
  }

  theMaxIndex -= theIndexMap.size()+1;  
}
