#include"MagneticDipole.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"

////////////////////////////////////////////////////////////////////////////////
//
MagneticDipole::MagneticDipole(const G4ThreeVector& aDipoleMoment, const G4ThreeVector& aPos)
{theDipoleMoment = aDipoleMoment;
 theDipoleCenter = aPos;
} 
////////////////////////////////////////////////////////////////////////////////
//
MagneticDipole::~MagneticDipole()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void MagneticDipole::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  //Formulae taken from Classical Electrodynamics, Jackson J. D., 3rd Edition, p. 186, eq. 5.56 
  G4ThreeVector pos_rel= G4ThreeVector(yTrack[0], yTrack[1], yTrack[2])-theDipoleCenter;
  G4double r=pos_rel.mag();
  G4double r3=r*r*r;
  pos_rel = pos_rel/r;
  G4double fac_mag=1.; //change for mu/4*pi
  G4ThreeVector Bvec=3.*pos_rel*(pos_rel.dot(theDipoleMoment)) - theDipoleMoment;
  Bvec=fac_mag*Bvec/r3;
  MagField[0]=Bvec.x();
  MagField[1]=Bvec.y();
  MagField[2]=Bvec.z();
  
  
  
    
}
