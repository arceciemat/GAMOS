#include "GmVClassifier.hh"
#include "G4UIcommand.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//---------------------------------------------------------
GmVClassifier::GmVClassifier(G4String name ) : theName( name )
{
  theMaxIndex = 0;
  bIsCompound = FALSE;

  bOnSecondary = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(name+":OnSecondary",0));

}

//---------------------------------------------------------
G4String GmVClassifier::GetIndexName(G4int index )
{
  return G4UIcommand::ConvertToString( G4int(index) );
}

//---------------------------------------------------------
G4int GmVClassifier::GetIndexFromTrack(const G4Track*)
{
  return 0;
}

#include "G4Track.hh" //GDEB
//---------------------------------------------------------
G4int GmVClassifier::GetIndexFromSecoTrack(const G4Track*, const G4Track* aTrack2)
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
