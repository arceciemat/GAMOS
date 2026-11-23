
#ifndef _VIP_SSRB_FBP_H__
#define _VIP_SSRB_FBP_H__

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
#include <math.h>
#include <iostream>
#include <map>
#include <list>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fftw3.h>
#include <fstream>

//#include <iomanip>

typedef unsigned short SINO_TYPE;

namespace VIP_ssrb
{
// methods
	void 	ReadSinoDim(void);
	void 	read_sino3D(void);
	void 	write_sino2D(void);
	
	void 	write_INTERFILE(const char* filename, float*** data, int x, int y, int z
						, double pixD, double sliceD);

	void 	DeleteSino3D(); 
	void 	CreateSino2D();
	void 	DeleteSino2D(); 
	void 	CreateImage();
	void 	DeleteData();

	void 	calculate_filter(void);
	void 	fastBP(int slice);

    // Global variables:
	FILE* theINfile_hv;
	FILE* theINfile_v;
	FILE* theOUTfile;

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

	SINO_TYPE*** sino3D;
	float*** sino2D;
	int* planesNorm;

	double** sino_FFT;
	double* filt;
	double* costheta; 
	double* sintheta;
	double** xc;
	double** ys;

	float*** image;
};

 
#endif

