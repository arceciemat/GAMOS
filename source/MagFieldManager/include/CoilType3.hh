///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////

#ifndef CoilType3_HH
#define CoilType3_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
class BiotSavartIntegrator;

class CoilType3: public G4MagneticField
{
  public:  // with description
  
    // Coil formed by  two big circular sections covering the same angular sector of angle dAlpha,
    // and two linear sections that are situated between the two big circular sections and oriented in the radial direction.
    //The coil is oriented parallel to the YZ plane.
    //Big circular and linear sections are joined on their extrimity by four small circular section that we call here corners.
    //The circular section start at an angle alpha0 from the Z axis (with positive angle direction from Y to Z)
    //The current if given as positif turn in the outer big circular section from the Y to the Z axis.
    
    //Input to define the coil
    //------------------------
    //The coil has a d1 x d2 rectangular section (d1 is relative to the X direction)
    //Rout = outer radius of the external circular big section
    //Rin = inner radius of the external circular big section 
    //RoutCorner = outer radius of the corners
    //Alpha0 = angular start of the big circular section   from the Z axis (with positive angle direction from Y to Z)
    //dAlpha = angle covered by the big circular section, should always by positive (with positive angle direction from Y to Z)
    //current = current contained in the coil
    

    CoilType3(G4double RoutCorner,G4double D1, G4double D2,G4double Rin, G4double Rout,G4double Alpha0,G4double dAlpha,G4double current);
    ~CoilType3();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;
    
    
    

  private:
    G4double d1,d2, I;
    G4double l,rout,rin,alpha0,dalpha,rout_corner; //alpha0 is the 
    G4double rin_corner,cos_alpha0,sin_alpha0,cos_alpha1,sin_alpha1;
    
    //The (y0,z0) system of coordinate is obtained from a rotation of angle alpha0 around the X axis
    //We have then : 	y0=cos_alpha0*y+sin_alpha0*z  z0=-sin_alpha0*y+cos_alpha0*z;
    //			y=cos_alpha0*y0-sin_alpha0*z0  z= sin_alpha0*y0+cos_alpha0*z0;	
    G4double y0_lin1, z0_lin1; // Represents the (y0,z0) coordinate of the centrum of the first linear section
    
    //The (y1,z1) system of coordinate is obtained from a rotation of angle alpha1=alpha0+dalpha around the X axis
    //We have then : 	y1=cos_alpha1*y+sin_alpha1*z  z1=-sin_alpha1*y+cos_alpha1*z;
    //			y=cos_alpha1*y1-sin_alpha1*z1  z= sin_alpha1*y1+cos_alpha1*z1;	
     G4double y1_lin2, z1_lin2; // Represents the (y1,z1) coordinate of the centrum of the second linear section
    
    BiotSavartIntegrator* theBiotSavartIntegrator;
    
    

    
   
  
  
};

#endif

