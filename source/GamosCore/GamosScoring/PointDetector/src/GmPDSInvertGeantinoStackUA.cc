#include "G4StackManager.hh"
#include "GmPDSInvertGeantinoStackUA.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GmPDSUtils.hh"

#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"

#include "G4Geantino.hh"
#include "G4Track.hh"


GmPDSInvertGeantinoStackUA* GmPDSInvertGeantinoStackUA::theInstance = 0;

//---------------------------------------------------------------------------
GmPDSInvertGeantinoStackUA* GmPDSInvertGeantinoStackUA::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmPDSInvertGeantinoStackUA;
  } 

  return theInstance;
}


//---------------------------------------------------------------------------
GmPDSInvertGeantinoStackUA::GmPDSInvertGeantinoStackUA()
{

}

//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmPDSInvertGeantinoStackUA::ClassifyNewTrack(const G4Track* )
{
  return fWaiting; // so that NewStage will invert the order

  /*  G4TrackStack * waitingStack = stackManager->waitingStack;
  G4cout << " CNT stackManager " << stackManager << " waitingStack " << waitingStack  << " ntracks " << waitingStack->GetNTrack() << G4endl;

  if( aTrack->GetDefinition() == G4Geantino::Geantino() ) {
    return fWaiting;
  } else {
    return fUrgent;
  }
  */
}


//---------------------------------------------------------------------------
void GmPDSInvertGeantinoStackUA::NewStage()
{
  G4TrackStack * urgentStack = stackManager->urgentStack;

  G4StackedTrack aStackedTrack;

  std::vector<G4StackedTrack> invertedStack;
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSInvertGeantinoStackUA::NewStage ntracks urgent " << urgentStack->GetNTrack() << G4endl;
  while( urgentStack->GetNTrack() > 0 ) {
    //  while( (aStackedTrack=urgentStack->PopFromStack()) != 0 ) {
    if( ScoringVerb(debugVerb) && aStackedTrack.GetTrack() ) G4cout << "GmPDSInvertGeantinoStackUA::NewStage()  track popped " << " ID " << aStackedTrack.GetTrack()->GetTrackID() << " pos " << aStackedTrack.GetTrack()->GetPosition() << G4endl;
    invertedStack.push_back(aStackedTrack);
  }

  std::vector<G4StackedTrack>::const_iterator ite;
  for( ite = invertedStack.begin(); ite != invertedStack.end(); ite++ ){
    urgentStack->PushToStack( *ite );
  }

}

//---------------------------------------------------------------------------
void GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight( G4double weiFactor, G4int parentID, G4int detID )
{
  G4TrackStack * urgentStack = stackManager->urgentStack;

  G4StackedTrack  aStackedTrack;

  std::vector<G4StackedTrack> tmpStack;
  while( urgentStack->GetNTrack() > 0 ) {
    //  while( (aStackedTrack=urgentStack->PopFromStack()) != 0 ) {
    if( ScoringVerb(debugVerb) && aStackedTrack.GetTrack() ) G4cout << "GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight()  track " << " ID " << aStackedTrack.GetTrack()->GetTrackID()  << "  " << aStackedTrack.GetTrack()->GetDefinition()->GetParticleName()<< " pos " << aStackedTrack.GetTrack()->GetPosition() << G4endl;
    tmpStack.push_back(aStackedTrack);
    G4Track* aTrack = aStackedTrack.GetTrack();
    if( aTrack->GetDefinition() == G4Geantino::Geantino() ) {
      G4VUserTrackInformation* trki = aTrack->GetUserInformation();
      GmTrackInfo* gmtrki = dynamic_cast<GmTrackInfo*>( trki );
      if( gmtrki->GetIntValue("DetectorID") == detID && aTrack->GetParentID() == parentID ) {
	if( ScoringVerb(debugVerb) ) G4cout << "GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight() track "<< aTrack->GetTrackID() << " weight changed to " << GetWeight(aTrack)*weiFactor << G4endl;
	SetWeight( aTrack, GetWeight(aTrack)*weiFactor );
      }
    } else if ( aTrack->GetParentID() == parentID ) { 
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSInvertGeantinoStackUA::ChangeGeantinoWeight() track "<< aTrack->GetTrackID() << " weight changed to " << aTrack->GetWeight()*weiFactor << G4endl;
      aTrack->SetWeight( aTrack->GetWeight()*weiFactor );
    }
  }

  for( unsigned int ii = 0; ii < tmpStack.size(); ii++ ){
    urgentStack->PushToStack( tmpStack[ii] );
  }

}
