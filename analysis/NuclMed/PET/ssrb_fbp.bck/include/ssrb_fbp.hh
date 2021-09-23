using namespace std;  // if not commented "cout" instead of  "std::cout" etc...
#include <math.h>
#include <iostream>
#include <map>
#include <list>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fftw3.h>

//#include <iomanip>


typedef unsigned short SINO_TYPE;

void ReadSinoDim(void);
void read_sino3D(void);
void write_sino2D(void);
void free_sinos(void);
void write_INTERFILE(const char* filename, float*** data, int x, int y, int z, double pixD, double sliceD);
void free_data(void);

void calculate_filter(void);
void fastBP(int slice);

// Global variables:

FILE *theINfile_hv;
FILE *theINfile_v;
FILE *theOUTfile;
int typeINfile=0;
int verbos=0, norm=1;
int n_planes=40, n_bin=128, n_ang=128;
int n_planes_file=40;
int n_slices=40, P;

int MRDiff=-1, span=0, nSEG=0;
float pixDim=0.5;
float sliceDim=1.0; 

double delta_x=-1.0, delta_theta;
int M=-1;
int type_f=1;
double alpha=1.0, Butt_n=1.0, Nf_cut=0.75;
double delta_rho=0.5;
double delta_slice=1.0;

const char* fileOUTname="default_image";
const char* fileINname="default_sino3D";

SINO_TYPE ***sino3D;
float ***sino2D;
int *planesNorm;

double **sino_FFT;
double *filt;
double *costheta, *sintheta;
double **xc;
double **ys;

float ***image;

 
