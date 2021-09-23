using namespace std;  // if not commented "cout" instead of  "std::cout" etc...
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
//#include "GamosCore/GamosBase/include/GmAnalysisMgr.hh"


typedef unsigned short SINO_TYPE;  //!!TO DO: read from .hv/.hs !!

unsigned long int read_data(const char* theName, SINO_TYPE* theSino);
void write_sino3D(SINO_TYPE* sino);

// Global variables:

double _PI;
int Ni, Ne, oType=1;
unsigned long int posTOT, sizeTOT; // total file positions

//const char *fileOUTname="projdata_ALL";
//const char *fileINname ="default_sino3D";

string fileOUTname ("projdata_ALL");
string fileINname ("default_sino3D");

SINO_TYPE *sino_total;

