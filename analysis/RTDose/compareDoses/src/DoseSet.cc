#include "DoseSet.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------------
DoseSet::DoseSet(std::vector<G4String> wl, G4bool bLogXDef, G4bool bLogYDef  )
{
 if( wl.size() != 3 &&
      wl.size() != 4 && 
      wl.size() != 6 ) {
    G4Exception("ScoreSet::ScoreSet",
		"",
		FatalException,
		(G4String("COMPARE line must have a second word : 0/1, a third word: HISTO_FILE_NAME, an optional fourth word: DIV, and two optional fifth and sixth words: logx(0/1) logy(0/1),   it has " + GmGenUtils::itoa(wl.size()) + " words ").c_str()));
  }
  if( wl[1] != "0" && wl[1] != "1" ) {
    G4Exception("ScoreSet::ScoreSet",
		"",
		FatalException,
		(G4String("COMPARE line must have a second word : 0/1, while it is " + wl[1]).c_str()));
  }
  bNormalized = GmGenUtils::GetBool(wl[1]);

  theHisFileName = wl[2];

  bPlotDivided = false;
  if( wl.size() >= 4 ) {
    if( wl[3] == "DIV" ) {
      bPlotDivided = true;
    } else if( wl[3] == "NODIV" ) {
      bPlotDivided = false;
    } else {
      G4Exception("ScoreSet::ScoreSet",
		"",
		  FatalException,
		  (G4String("COMPARE line fourth word must be 'DIV' or 'NODIV', while it is " + wl[3]).c_str()));
    }
  }
  
  if( wl.size() == 6 ) {
    if( wl[4] == "0" ) {
      bLogX = 0;
    } else if( wl[4] == "1" ) {
      bLogX = 1;
    } else {
      G4Exception("ScoreSet::ScoreSet",
		"",
		  FatalException,
		  (G4String("COMPARE line fifth word must be '0' or '1', while it is " + wl[4]).c_str()));
    }
    if( wl[5] == "0" ) {
      bLogY = 0;
    } else if( wl[5] == "1" ) {
      bLogY = 1;
    } else {
      G4Exception("ScoreSet::ScoreSet",
		"",
		  FatalException,
		  (G4String("COMPARE line fifth word must be '0' or '1', while it is " + wl[5]).c_str()));
    }

  } else {
    bLogX = bLogXDef;
    bLogY = bLogYDef;
  }
  
}


//-----------------------------------------------------------------------------
void DoseSet::AddDoser(G4String fileName, G4String doseName, G4double multFactor, G4double dispFactor, G4int iColor, G4double dispYFactor)
{
  DoseInfo doseInfo;
  doseInfo.fileName = fileName;
  doseInfo.doseName = doseName;
  doseInfo.multFactor = multFactor;
  doseInfo.dispFactor = dispFactor;
  doseInfo.iColor = iColor ;
  doseInfo.dispYFactor = dispYFactor;
  theDoseInfos.push_back( doseInfo );

}

