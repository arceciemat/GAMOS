#include "ScoreSet.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------------
ScoreSet::ScoreSet(std::vector<G4String> wl)
{
  if( wl.size() != 4) {
    G4Exception("ScoreSet::ScoreSet",
		"",
		FatalException,
		(G4String("COMPARE line must have a second word: FILE_NAME, a third word: DETECTOR_NAME, and a fourth word: SCORE_NAME,   it has " + GmGenUtils::itoa(wl.size()) + " words ").c_str()));
  }
  theOutFileName = wl[1];
  theDetectorName = wl[2];
  theScorerName = wl[3];

}


//-----------------------------------------------------------------------------
void ScoreSet::AddScorer(G4String fileName, G4String scorerName, G4double weight)
{
  theFileScorerNames.push_back(pss(fileName,scorerName));
  theScoreWeights.push_back(weight);
}

