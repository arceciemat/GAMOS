#include "GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//---------------------------------------------------------
GmVClassifier::GmVClassifier(G4String name ) : theName( name )
{
  theMaxIndex = 0;
  bIsCompound = FALSE;

  bOnSecondary = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(name+":OnSecondary",0));

}

//---------------------------------------------------------
G4String GmVClassifier::GetIndexName(int64_t index )
{
  return GmGenUtils::itoa( index );
}

//---------------------------------------------------------
int64_t GmVClassifier::GetIndexFromTrack(const G4Track*)
{
  return 0;
}

#include "G4Track.hh" //GDEB
//---------------------------------------------------------
int64_t GmVClassifier::GetIndexFromSecoTrack(const G4Track*, const G4Track* aTrack2)
{
  return GetIndexFromTrack(aTrack2);
}

//---------------------------------------------------------
void GmVClassifier::CheckNumberOfIndices( std::vector<G4String> wl )
{
  if( wl.size()%2 == 1 ) {
    G4Exception("GmVClassifier::SetIndices",
		("Wrong number of parameters in classifier " + theName).c_str(),
		FatalErrorInArgument,
		"There should be an even number of parameters after classifier name in command /gamos/base/setClassifierIndices : KEY_1 INDEX_2 KEY_2 INDEX_2 ...");
  }
}
