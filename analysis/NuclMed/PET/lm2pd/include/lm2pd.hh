
using namespace std;  // if not commented "cout" instead of  "std::cout" etc...
#include <math.h>
#include <iostream>
#include <map>
#include <list>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#include <iomanip>


//ttypedef unsigned short SINO_TYPE;  //!!NOTE:  Check "Write_sino3D" (.hv file) if SINO_TYPE changes !!
typedef float SINO_TYPE;  //!!NOTE:  Check "Write_sino3D" (.hv file) if SINO_TYPE changes !!

struct _3Dvector
{
  float X,Y,Z;
};

struct lor
{
  _3Dvector p1,p2;
};


struct arce_str
{
  char name[8];
  float xVtx,yVtx,zVtx,x1,y1,z1,x2,y2,z2;
};

lor ReadData(void);
void Load_sino3D(lor);
void free_sino3D(void);
void write_sino3D(void);

//arce_str arce_out;

// Global variables:

double _PI;

FILE *theINfile;
FILE *theOUTfile;
int typeINfile=0;
int verbos=0;
//float discretZ=0.0;
int n_planes=40, n_bin=128, n_ang=128;
int MRDiff=-1, span=5;
int oType=0, TOTaxial_pl;
float d_axial=100.0;
float d_FOV=300.0; 
float offsetX=0.0;

const char* fileOUTname="default_sino3D";
const char* fileINname="pet.out";

SINO_TYPE ***sino3D;
SINO_TYPE *sino_line;

unsigned long int nCoinciTot=0;
unsigned long int nCoinciVal=0;
unsigned long int MAX_Coinci=0;
  
 
