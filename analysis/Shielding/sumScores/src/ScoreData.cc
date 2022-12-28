#include "ScoreData.hh"
#include "ScorerData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

int ScoreData::verbosity = 0;
ScoreIndexValType ScoreData::theIndexValType= SIVT_MIDDLE;

//-----------------------------------------------------------------
void ScoreData::SetScorerData( ScorerData* sd )
{
  theScorerData = sd;
}

/*
void ScoreData::MultiplyByNEvents()
{
  // for summing each score is multiplied by the number of events
  double nev = theScorerData->GetNEvents();
  theNEvents = G4int(nev);
  //  G4cout << " SetScorerDataPRE " << theName << " TOT " << theScoreTot << " nev " << nev << G4endl;
  theScoreTot *= nev; 
  if( ScoreData::verbosity >= 3 ) G4cout << " SetScorerData " << theName << " TOT " << theScoreTot << G4endl;
  // only used for dispersion  theScoreSq *= nev*nev; 
}
*/

bool ScoreData::operator!=( const ScoreData& sd2 )
{
  return !(*this == sd2);

}
bool ScoreData::operator==( const ScoreData& sd2 )
{
  //     G4cout << " CHECKSEPAR " << theName << " " << sd2.GetName() << G4endl;
 // compare name also checking that 1.E-05-0.0001 is equal than 1.E-05-1.E-04
  if (theName == sd2.GetName() ) {
    /*    G4cout << " FATAL error, the Score in Scorer don't have the same format  " 
	   << " First Score " << theName
	   << " Second Score " << sd2.GetName() 
	   << G4endl; */
    return true;
  } else {
    // try to decompose: look for a '-' separating min and max
    int iSepar1 = -1;
    for( size_t ii = 1; ii < theName.length(); ii++ ) {
      if( theName[ii] == '-' ) {
	if( theName[ii-1] != 'E' && theName[ii-1] != 'e' ) {
	  iSepar1 = ii;
	  break;
	}
      }
    }
    int iSepar2 = -1;
    G4String theName2 = sd2.GetName();
    for( size_t ii = 1; ii < theName2.length(); ii++ ) {
      if( theName2[ii] == '-' ) {
	if( theName2[ii-1] != 'E' && theName2[ii-1] != 'e' ) {
	  iSepar2 = ii;
	  break;
	}
      }
    }
    //    G4cout << " SEPAR " << theName << " " << theName2 << " " << iSepar1 << " " << iSepar2 <<  G4endl;
    if( iSepar1 != -1 && iSepar2 != -1 ) {
      G4String name1Sub1 = theName.substr(0,iSepar1);
      if( !GmGenUtils::IsNumber(name1Sub1) ) return false;
      G4String name2Sub1 = theName2.substr(0,iSepar2);
      if( !GmGenUtils::IsNumber(name2Sub1) ) return false;
      G4String name1Sub2 = theName.substr(iSepar1+1,theName.length());
      if( !GmGenUtils::IsNumber(name2Sub1) ) return false;
      G4String name2Sub2 = theName2.substr(iSepar2+1,theName.length());
      if( !GmGenUtils::IsNumber(name2Sub2) ) return false;
      if( GmGenUtils::GetValue(name1Sub1) == GmGenUtils::GetValue(name2Sub1) && 
	  GmGenUtils::GetValue(name1Sub2) == GmGenUtils::GetValue(name2Sub2) ) {
	return true;
      }

    }

  }
  return false;

}


ScoreData ScoreData::operator+=( const ScoreData& ed2 )
{ 
  //  if( theEnergy == 0.01)  G4cout << " 1theInvErrorExp2 " << this << " " << theInvErrorExp2 << G4endl;
  if( ScoreData::verbosity >= 2 ) {
    G4cout <<  " theScore " << theScore << " * " << GetNEvents() << " + " << ed2.GetScore() << " * " << ed2.GetNEvents() << G4endl; 
    G4cout << "   ScoreData::operator+=ed1 " << theName << " : " << theScore << " +- " << theScoreError << " ScoreSq " << theScoreSq << G4endl; 
    G4cout << "   ScoreDatad::operator+=ed2 " << theName << " : " << ed2.GetScore() << " +- " << ed2.GetScoreError() << " ScoreSq " << ed2.GetScoreSq() << G4endl; 
  }

  theScore = (theScore*GetNEvents()+ed2.GetScore()*ed2.GetNEvents())/(GetNEvents()+ed2.GetNEvents());
  //  theScore = (theScore+ed2.GetScore()*ed2.GetNEvents())/(GetNEvents()+ed2.GetNEvents());

  //  theScoreTot += ed2.GetScoreTot();
  //  theScore += ed2.GetScore();
  theNumberOfFiles++;

  theScoreSq += ed2.GetScoreSq();

  if ( ed2.GetScoreError() != 0. ) {    
    theScoreDivideError += (ed2.GetScore())/sqr(ed2.GetScoreError());
    theInvErrorExp2 += 1/sqr(ed2.GetScoreError());
    if( theInvErrorExp2 != 0. ) {
      theScoreError = 1./sqrt(theInvErrorExp2);
    } else {
      theScoreError = 0.;
    }

    //    if( ScoreData::verbosity >= 2 )
      //      G4cout << "   ScoreData::operator+=: " << bTotal << " Add ScoreData " << theName << " WITH " << ed2.GetName() << " " << theScore << " scoreSq " << theScoreSq << " theScoreDivideError " << theScoreDivideError << " theInvErrorExp2 " << theInvErrorExp2 << G4endl;
  }  
  
  if( ScoreData::verbosity >= 2 )
    G4cout << "   ScoreData::operator+= " << theName << " : " << theScore << " +- " << theScoreError << " ScoreSq " << theScoreSq << " ScoreDivideError " << theScoreDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;

  return *this;
}

//-----------------------------------------------------------------
ScoreData & ScoreData::operator*=( G4double wei )
{
  theScore *= wei;

  theScoreSq *= wei*wei;

  if ( GetScoreError() != 0. ) {    
    theInvErrorExp2 *= 1./(wei*wei);
  }  
  
  if( ScoreData::verbosity >= 2 )
    G4cout << "   ScoreData::operator*=: " << theScore << " +- " << theScoreError << " ScoreSq " << theScoreSq << " ScoreDivideError " << theScoreDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;

  return *this;

}

long double ScoreData::GetScore() const
{
  return theScore;
}

long double ScoreData::GetScoreSq() const
{
  return theScoreSq;
}

long double ScoreData::GetScoreError() const
{
  return theScoreError;
}


G4double ScoreData::GetNEvents() const
{
  return theScorerData->GetNEvents();
}


G4bool ScoreData::SelfCheck() const
{
  if( GetName() == "=" ) { 
    G4cerr << "!!!ERROR in Score " << theName << " " << theScore << " +- " << theScoreError << " " << theScoreSq << G4endl;
    G4Exception("ScoreData::SelfCheck",
		"Wrong data",
		JustWarning,
		"Score name dose not exists");
    return false;
  } 

  return true;
}
