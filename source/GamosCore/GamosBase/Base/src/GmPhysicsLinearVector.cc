#include "GmPhysicsLinearVector.hh"

// --------------------------------------------------------------------
GmPhysicsLinearVector::GmPhysicsLinearVector(G4bool spline)
  : G4PhysicsLinearVector(spline)
{
  type = T_G4PhysicsLinearVector;
}

// --------------------------------------------------------------------
GmPhysicsLinearVector::GmPhysicsLinearVector(G4double Emin, G4double Emax,
                                             std::size_t Nbin, G4bool spline)
  : G4PhysicsLinearVector(Emin, Emax, Nbin, spline)
{
}

// --------------------------------------------------------------------
void GmPhysicsLinearVector::FillData( std::vector<G4double> binVec, std::vector<G4double> dataVec )
{
  binVector = binVec;
  dataVector = dataVec;
  //  G4cout << this << " GmPhysicsLinearVector::FillData( " << binVector.size() << " " << dataVector.size() << G4endl; //GDEB
}
