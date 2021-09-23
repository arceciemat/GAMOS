///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef CoilType2_HH
#define CoilType2_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
class BiotSavartIntegrator;

class CoilType2: public G4MagneticField
{
  public:  // with description

    CoilType2(G4double RoutCorner, G4double D1, G4double D2, G4double LVertLeft, 
    	      G4double LVertRight, G4double LHorizUp, G4double LHorizDown,G4double current);
    ~CoilType2();

    virtual void GetFieldValue(const G4double yTrack[4],
                                     G4double *MagField) const ;
    
    inline void SetRoutCorner(G4double aVal) {Rout_corner= aVal;}
    inline void SetD1(G4double aVal) {d1= aVal;}
    inline void SetD2(G4double aVal) {d2= aVal;}
    inline void SetLVertLeft(G4double aVal) {Lvert_left= aVal;}
    inline void SetLVertRight(G4double aVal) {Lvert_right= aVal;}
    inline void SetLHorizUp(G4double aVal) {Lhoriz_up= aVal;}
    inline void SetLHorizDown(G4double aVal) {Lhoriz_down= aVal;}
    
    
    

  private:
    G4double d1,d2, I;
    G4double Lvert_left, Lvert_mid, Lvert_right, Lhoriz_up, Lhoriz_mid, Lhoriz_down, Rout_corner,Rin_corner;
    BiotSavartIntegrator* theBiotSavartIntegrator;
    G4double y_vert_left,y_vert_mid,y_vert_right,z_vert_left,z_vert_mid,z_vert_right;
    G4double y_horiz_up,y_horiz_mid,y_horiz_down,z_horiz_up,z_horiz_mid,z_horiz_down;
    G4double y_corner_left_up,y_corner_right_up,y_corner_right_mid,y_corner_mid_mid, y_corner_mid_down,  y_corner_left_down;
    G4double z_corner_left_up,z_corner_right_up,z_corner_right_mid,z_corner_mid_mid, z_corner_mid_down,  z_corner_left_down;
  
    
   
  
  
};

#endif

