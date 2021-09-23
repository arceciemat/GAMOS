//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//

// using namespace std;  
	// MK: replaced to cc file 
#include <math.h>
#include <iostream>
#include <map>
#include <list>

#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <string>

//#include <iomanip>

typedef unsigned short SINO_TYPE;  //!!NOTE:  Check "Write_sino3D" (.hv file) if SINO_TYPE changes !!

struct _3Dvector
{
	float X,Y,Z;
};

struct lor
{
	_3Dvector p1,p2;
};


struct arce_struct
{
	char name[8];
	float xVtx,yVtx,zVtx,x1,y1,z1,x2,y2,z2;
};

lor ReadData(void);
void Load_sino3D(lor);

// void free_sino3D(void);
void InitializeSino3D();
void DeleteSino3D(); 

void write_sino3D(void);

// Global variables:

double _PI;

// <<<<< File stuff:
enum FILE_TYPE
{
	FILETYPE_DEFAULT = 0,
	FILETYPE_OTHER		// never used???
};

int typeINfile = FILETYPE_DEFAULT;

std::string fileOUTname = "default_sino3D";
std::string fileINname = "pet.out";

std::ifstream theINfile;
std::ofstream theOUTfile;
// >>>>> File stuff

int verbos=0;

int n_planes=40, n_bin=128, n_ang=128;
int MRDiff=-1, span=5;
int oType=0, TOTaxial_pl;
float d_axial = 100.0;
float d_FOV = 300.0; 
float offsetX = 0.0;

// OUTPUT DATA pointers
SINO_TYPE*** sino3D;

unsigned long int nCoinciTot=0;
unsigned long int nCoinciVal=0;
unsigned long int MAX_Coinci=0;
  
 
