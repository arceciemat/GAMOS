#include "GmRandomKillByParticleUA.hh"
#include "G4Track.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "CLHEP/Random/RandFlat.h"

//---------------------------------------------------------------------------
GmRandomKillByParticleUA::GmRandomKillByParticleUA()
{

  theFileName = GmParameterMgr::GetInstance()->GetStringValue(theName+":FileName","");
  theMultFactor = GmParameterMgr::GetInstance()->GetNumericValue(theName+":MultFactor",1.);
  
  ReadFile();
  
}
//---------------------------------------------------------------------------
void GmRandomKillByParticleUA::ReadFile()
{
  if( theFileName == "" ) {
    G4Exception("GmRandomKillByParticleUA::ReadFile",
		"",
		FatalException,
		("No file name given, please use  /gamos/setParam "+theFileName+":FileName FILE_NAME").c_str());;
  }

  GmFileIn fin = GmFileIn::GetInstance(theFileName);
  std::vector<G4String> wl;
  for(;;) {
    if( !fin.GetWordsInLine(wl) ) break;
    if( wl.size() != 2 ) {
      for( size_t ii = 0; ii < wl.size(); ii++ ) {
	std::cerr << "WORD " << ii << " = " << wl[ii] << std::endl;
      }
      G4Exception("GmRandomKillByParticleUA::ReadFile",
		  "",
		  FatalException,
		  ("Line in " + theFileName + " must have tow words").c_str());
    }
    theParticleProb[wl[0]] = GmGenUtils::GetValue(wl[1]);
  }
  
}

//---------------------------------------------------------------------------
G4ClassificationOfNewTrack GmRandomKillByParticleUA::ClassifyNewTrack(const G4Track* aTrack )
{
  G4String particle = aTrack->GetDefinition()->GetParticleName();
  std::map<G4String,G4double>::const_iterator ite = theParticleProb.find(particle);
  if( ite == theParticleProb.end() ) {
    G4Exception("GmRandomKillByParticleUA::RClassifyNewTrack",
		"",
		JustWarning,
		("Particle " + particle + " does not exists in file " + theFileName).c_str());
  }

  if( CLHEP::RandFlat::shoot() > ite->second*theMultFactor ) {
    return fKill;
  } else {
    return fUrgent;
  }
}
