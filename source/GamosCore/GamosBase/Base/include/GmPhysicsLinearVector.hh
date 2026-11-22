// --------------------------------------------------------------------
#ifndef GmPhysicsLinearVector_hh
#define GmPhysicsLinearVector_hh 1

#include "G4PhysicsLinearVector.hh"
#include "globals.hh"

class GmPhysicsLinearVector : public G4PhysicsLinearVector
{
public:
  // The vector will be filled from external file using Retrieve() method
  explicit GmPhysicsLinearVector(G4bool spline = false);

  // Energies will be computed and filled at construction, values will be 
  // filled with zeros. Required Nbin > 0 and Emax > Emin.
  // Use PutValue(..) to fill the data vector
  explicit GmPhysicsLinearVector(G4double Emin, G4double Emax, std::size_t Nbin,
                                 G4bool spline = false);

  void FillData( std::vector<G4double> binVec, std::vector<G4double> dataVec );
  
  ~GmPhysicsLinearVector() override = default;

  
protected:

};

#endif
