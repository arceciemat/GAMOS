///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef InterpolatedField_HH
#define InterpolatedField_HH 1 


#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "globals.hh"
#include "vector"
class BiotSavartIntegrator;

class InterpolatedField: public G4MagneticField
{
  public:  // with description

    InterpolatedField(G4String file_name);
    ~InterpolatedField();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;      
  private:
    
     void Read3DGrid(G4String file_name);
    
  
  private:
     G4bool IsCartesian;
  
    
    
    //Matrices for  interpolation
    //////////////////////////////
	
     std::vector< std::vector< std::vector<float> > > B1_grid3D; //X or rho component
     std::vector< std::vector< std::vector<float> > > B2_grid3D; //Y or phi component
     std::vector< std::vector< std::vector<float> > > B3_grid3D; //Z component
     G4double xmin,xmax,ymin,ymax,zmin,zmax,dx,dy,dz;
     G4double rho_min,rho_max,drho;
     G4double phi1,phi2,dphi;    

};

#endif

