#include "GmBOptrForceCollisionTrackData.hh"
#include "GmBOptrForceCollision.hh"

GmBOptrForceCollisionTrackData::GmBOptrForceCollisionTrackData( const GmBOptrForceCollision* optr )
: G4VAuxiliaryTrackInformation(),
  fForceCollisionOperator( optr )
{
  fForceCollisionState = ForceCollisionState::free;
}

GmBOptrForceCollisionTrackData::~GmBOptrForceCollisionTrackData()
{
  if ( fForceCollisionState != ForceCollisionState::free )
    {
      G4ExceptionDescription ed;
      ed << "Track deleted while under GmBOptrForceCollision biasing scheme of operator `";
      if ( fForceCollisionOperator == nullptr ) ed << "(none)"; else ed << fForceCollisionOperator->GetName();
      ed <<"'. Will result in inconsistencies.";
      G4Exception(" GmBOptrForceCollisionTrackData::~GmBOptrForceCollisionTrackData()",
		  "BIAS.GEN.19",
		  JustWarning,
		  ed);
    }
}

void GmBOptrForceCollisionTrackData::Print() const
{
  G4cout << " GmBOptrForceCollisionTrackData object : " << this << G4endl;
  G4cout << "     Force collision operator : "; if ( fForceCollisionOperator == nullptr ) G4cout << "(none)"; else G4cout << fForceCollisionOperator->GetName(); G4cout << G4endl;
  G4cout << "     Force collision state    : ";
  switch ( fForceCollisionState )
    {
    case ForceCollisionState::free :
      G4cout << "free from biasing ";
      break;
    case ForceCollisionState::toBeCloned :
      G4cout << "to be cloned ";
      break;
    case ForceCollisionState::toBeForced :
      G4cout << "to be interaction forced ";
      break;
    case ForceCollisionState::toBeFreeFlight :
      G4cout << "to be free flight forced (under weight = 0) ";
      break;
    default:
      break;
    }
  G4cout << G4endl;
}
