#include"LinearSegment.hh"
#include"G4UIcmdWith3VectorAndUnit.hh"
#include"special_functions.hh"
#include"globals.hh"
#include"BiotSavartIntegrator.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;
////////////////////////////////////////////////////////////////////////////////
//
LinearSegment::LinearSegment(G4double D1, G4double D2,G4double L,G4double current)//current turn from y to z
{ d1=D1;
  d2=D2;
  l=L;
  I=current;
  
  
  theBiotSavartIntegrator = BiotSavartIntegrator::GetInstance();
} 
////////////////////////////////////////////////////////////////////////////////
//
LinearSegment::~LinearSegment()
{;
}
////////////////////////////////////////////////////////////////////////////////
//
void LinearSegment::GetFieldValue(const G4double yTrack[4],G4double *MagField) const
{ 
  
 
  //first section;
  G4double pos[3];
  pos[0]=yTrack[0];
  pos[1]=yTrack[1];
  pos[2]=yTrack[2];
  G4ThreeVector B = theBiotSavartIntegrator->BfieldFromLinearSegment(pos,d1, d2, l);
  
  
 
   
   //still need to multiply by the factor mu_0/4./pi
   G4double fac=mu0*I/4./pi;
   B=B*fac;
   
   
   
   MagField[0]=B.x();
   MagField[1]=B.y();
   MagField[2]=B.z(); 
   
 
}
