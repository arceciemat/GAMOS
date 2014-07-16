#include "GamosCore/GamosUtils/include/GmLine.hh"

//--------------------------------------------------------------------
GmLine::GmLine(const G4ThreeVector& pnt, const G4ThreeVector& dir): thePoint(pnt)
{
  if( dir.mag() == 0. ) {
    G4Exception("GmLine::GmLine",
		"Wrong internal argument",
		FatalException,
		"trying to create a line with direction = (0.,0.,0.)");
  }
  theDir = dir.unit();
  // G4cout << " Created GmLine pnt " << thePoint << " dir " << theDir << G4endl;
}


//--------------------------------------------------------------------
G4double GmLine::GetDist( const G4ThreeVector& pt ) const
{
  G4ThreeVector vect = pt - thePoint;
  //  G4cout << " GmLine::GetDist enter " << " pt " << pt << " " << *this << G4endl;
  if( vect.mag() == 0. ) return 0.;
  G4double ang = acos( theDir * vect.unit() ); 
  //  G4cout << " GmLine::GetDist " << vect.mag() * sin( ang ) << " pt " << pt << " " << " ang " << ang << *this << G4endl;
  return vect.mag() * sin( ang );

}


//--------------------------------------------------------------------
G4ThreeVector GmLine::GetDistPoint( const G4ThreeVector& pt ) const
{
  G4ThreeVector vect = pt - thePoint;
  G4double ang = acos( theDir * vect.unit() ); 
  return thePoint + theDir * vect.mag() * cos( ang );
}


//--------------------------------------------------------------------
std::ostream& operator<<( std::ostream& os, const GmLine& lin )
{
  os << " GmLine: Point= " << lin.thePoint << "  Dir= " << lin.theDir;
  return os;

}
