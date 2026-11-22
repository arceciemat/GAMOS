///////////////////////////////////////////////////////////
/////////Code developed by SpaceIt GmbH, Bern Switzerland
/////////For the Space european Agency
/////////First Author :L. Desorgher
///////////////////////////////////////////////////////////
#ifndef BiotSavartIntegrator_HH
#define BiotSavartIntegrator_HH 1 
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"


class BiotSavartIntegrator
{
  public:  // with description

    BiotSavartIntegrator();
    virtual ~BiotSavartIntegrator();
    
    static BiotSavartIntegrator* GetInstance();
    
    G4ThreeVector BfieldFromLinearSegment(const G4double pos[3],double dx,  double dy, double dz);
    
    G4ThreeVector BfieldFromCurvedSegmentInYZ(const G4double pos[3],double phi1,  double dphi,double Rout,double Rin,double dx,int nphi_to_use);
    G4ThreeVector BfieldFromCurvedSegmentInYZ(const G4double pos[3],double phi1,  double dphi,double Rout,double Rin,double dx);
    G4ThreeVector BfieldFromCurvedSegmentInYZ_1(const G4double pos[3],double phi1,  double dphi,double Rout,double Rin,double dx);
    G4ThreeVector BfieldFromCurvedSegmentInYZ_test(const G4double pos[3],double phi1,  double dphi,double Rout,double Rin,double dx);
    
    inline void SetNx(G4int n) {nx= n;}
    inline void SetNy(G4int n) {ny= n;} 
    inline void SetNphi(G4int n) {nphi= n;}
    inline void SetNphiForHighPrecision(G4int n) {nphi1= n;}
    inline void SetNphiForMiddlePrecision(G4int n) {nphi2= n;}
   
    
    
    
    inline void SetCurvedSegmentTypeOfIntegration (G4int n) {CurvedSegmentTypeOfIntegration = n;}
   // inline void SetLinearSegmentTypeOfIntegration (G4int n) {LinearSegmentTypeOfIntegration = n;}
    
    
    
  private: //methods
  
    G4double BxFromYZCurrentSheet(double& x, double& y,double& z);
    G4double BfieldFromXZCurrentSheet(double& x, double& y,double& z, double& bx, double& by);
    
    G4double ByFromXZCurrentSheet(double& x, double& y,double& z);
    G4double dBfieldFromCurvedSegmentInYZdPhiPrime(double& x, double& y,double& z, double& dBxdPhi, double& dBrhoPrimdPhi);
    
    void BfFromIZLine(double& x, double& y,double& z, double& by, double& bz);
 
  
  private:
  
  static BiotSavartIntegrator* instance; 

  private:
    
    //Characteristic of current  linear segment
    G4double lx, ly, lz; // half x ,y and z extension of  the current segment
   
    //nb of step for integrations
    G4int nx,ny,nphi,nphi1,nphi2; // nb of steps for integration
    
    //boolean variable that tells if this the begghinig of the integration
    G4bool beginning_of_integration;
    G4int  CurvedSegmentTypeOfIntegration;
    //G4int  LinearSegmentTypeOfIntegration;
    
    //variables that should be computed only once at the beginning of the integration
    G4double zm,zm2,zp,zp2,ym,ym2,yp,yp2,y2,xm,xm2,xp,xp2,x2,rhomm2,rhomp2,rhopm2,rhopp2,xpzm,xmzp,xmzm,xpzp;
    G4double aa,bb,rhoout,rhoin,xu,xd,xu2,xd2;
    
    

};

#endif
