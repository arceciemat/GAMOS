#include"JCurrentCalculator.hh"
#include <fstream>
#include <ctime>
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;

JCurrentCalculator* JCurrentCalculator::instance = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
//
JCurrentCalculator::JCurrentCalculator()
{theField=0;  
}
///////////////////////////////////////////////////////////////////////
JCurrentCalculator::~JCurrentCalculator()
{; 
}
////////////////////////////////////////////////////////////////////////////////
//
JCurrentCalculator* JCurrentCalculator::GetInstance()
{ 
  if (!instance) instance = new JCurrentCalculator();
  return instance;
}
////////////////////////////////////////////////////////////////////////////////
//
void JCurrentCalculator::SetField(G4MagneticField* aField)
{theField = aField;
}
////////////////////////////////////////////////////////////////////////////////
//
G4ThreeVector JCurrentCalculator::CalculateJCurrent(G4ThreeVector pos,G4double t, G4double ds)
{ G4double position[4],B0[3],B1[3],B2[3],B3[3],B11[3],B22[3],B33[3];
  position[0] =pos.x();
  position[1] =pos.y();
  position[2] =pos.z();
  position[3] =t;
  theField->GetFieldValue(position,B0);
  
  position[0] =pos.x()+ds;
  position[1] =pos.y();
  position[2] =pos.z();
  position[3] =t;
  
  theField->GetFieldValue(position,B1);
  
  position[0] =pos.x()-ds;
  position[1] =pos.y();
  position[2] =pos.z();
  position[3] =t;
  
  theField->GetFieldValue(position,B11);
  
  
  position[0] =pos.x();
  position[1] =pos.y()+ds;
  position[2] =pos.z();
  position[3] =t;
  
  theField->GetFieldValue(position,B2);
  
  position[0] =pos.x();
  position[1] =pos.y()-ds;
  position[2] =pos.z();
  position[3] =t;
  
  theField->GetFieldValue(position,B22);
  
  
  position[0] =pos.x();
  position[1] =pos.y();
  position[2] =pos.z()+ds;
  position[3] =t;
  
  theField->GetFieldValue(position,B3);
  
  position[0] =pos.x();
  position[1] =pos.y();
  position[2] =pos.z()-ds;
  position[3] =t;
  
  theField->GetFieldValue(position,B33);
  
  
  
  G4double Jx,Jy,Jz;
  Jx=.5*(B2[2]-B22[2] -(B3[1]-B33[1]))/ds;
  Jy=.5*(B3[0]-B33[0] -(B1[2]-B11[2]))/ds;
  Jz=.5*(B1[1]-B11[1] -(B2[0]-B22[0]))/ds;
  //still need to multiply by the vacuum permeability
  G4ThreeVector J=G4ThreeVector(Jx,Jy,Jz)/mu0;
  return J; 

}
////////////////////////////////////////////////////////////////////////////////
//
void JCurrentCalculator::CalculateJCurrentBetweenTwoPositions(G4ThreeVector pos1,G4ThreeVector pos2,G4double t, G4int npos,G4double ds, G4String output_file_name)
{ //open file
  std::fstream FileOutput(output_file_name, std::ios::out);
  FileOutput.setf(std::ios::scientific);
  FileOutput.precision(6);
  FileOutput<<"X[m]"<<'\t'<<"Y[m]"<<'\t'<<"Z[m]"<<'\t'<<
  	      "JX[A/m2]"<<'\t'<<"JY[A/m2]"<<'\t'<<"JZ[A/m2]"<<std::endl;
  G4ThreeVector dpos=(pos2-pos1)/(npos-1.);
  for (size_t i=0;i<size_t(npos);i++){
   	G4ThreeVector pos=pos1+dpos*double(i);
  	G4ThreeVector J= CalculateJCurrent(pos,t,ds);
	FileOutput<<pos.x()/m<<'\t'<<pos.y()/m<<'\t'<<pos.z()/m<<'\t'<<J.x()*m2/ampere<<'\t'<<J.y()*m2/ampere<<'\t'<<J.z()*m2/ampere<<std::endl;
	
	
  }
  
  
}
////////////////////////////////////////////////////////////////////////////////
//
void JCurrentCalculator::CalculateJCurrentOnACartesianGrid(
				G4double x0,G4double  dx, G4int nX,
				G4double y0,G4double  dy, G4int nY,
				G4double z0,G4double  dz, G4int nZ,G4double ds,G4double t,
				G4String file_output) 
{ clock_t clock1,clock2;
  clock1=clock();
  
  
  std::fstream OutputFile(file_output, std::ios::out);
  OutputFile.setf(std::ios::scientific);
  OutputFile<<G4String("Cartesian")<<std::endl;
  OutputFile<<nX<<'\t'<<nY<<'\t'<<nZ<<std::endl;
  OutputFile.precision(6);
  OutputFile<<'\t'<<"X[m] "<<'\t'<<"Y[m] "<<'\t'<<"Z[m] "
				  <<"Jx"<<'\t'<<"Jy"<<'\t'
				  <<"Jz"<<'\t'
				  <<"J [A/m2]"<<std::endl;
  
  
  G4double x,y,z;
 
  for (int  i=0; i<nX; i++){
  	x=x0 + double(i)*dx;
  	for (int  j=0; j<nY; j++){
		y=y0 + double(j)*dy;
		for (int  k=0; k<nZ; k++){
			z=z0 + double(k)*dz;
			G4ThreeVector pos= G4ThreeVector(x,y,z);
			G4ThreeVector J= CalculateJCurrent(pos,t,ds)*m2/ampere;
			OutputFile<<x/m<<'\t'<<'\t'
				  <<y/m<<'\t'<<'\t'
				  <<z/m<<'\t'<<'\t'
				  <<J.x()<<'\t'<<J.y()<<'\t'
				  <<J.z()<<'\t'<<J.mag()<<'\t'
				  <<std::endl;	              	
		}
	}
  
  }
  
  OutputFile.close();
   
  clock2=clock();
  double tclock=double(clock2-clock1)/(double(CLOCKS_PER_SEC));
  G4cout<<"time used for the computation: "<<tclock<<" s"<<std::endl;//open file
  
  
  
}

