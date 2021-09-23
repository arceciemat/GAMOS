/***************************************************************************
 *   Copyright (C) 2007 by Juan Diego Soler                                *
 *   juan-sol@telemaco.ciemat.es                                           *
 *   Daniel P. A.    05/2008                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <vector>

using namespace std;

#include "FILE2MEMORY.h"
#include "general.h"
#include "histogram.h"
#include "matrix.h"
#include "utilities.h"
#include "egsclass.h"

int main(int argc, char *argv[]){

	string filename;
	string exitname;
	//	int first=1, last=3;
	//	int autoname=0;

	fstream * check_file = new fstream;
	phspheader* inputphsp;
	headerstats statsphsp[1];

	string aux;
	string outputname;
	//	char chain[9];

	int Nfiles=0;

	long Npar=0;
	long Npho=0;
	long Nele=0;
	long Npos=0;
	//	float Emax=0;
	//	float Emin=0;
	double Tpar=0;
	FILE* output;
	
	if( argc != 3 ) {
	  std::cerr << " !!! ERROR : Two arguments should be provided: INPUT_FILE_LIST_NAME OUTPUT_FILE_NAME" << std::endl;
	  std::exception();
	}
	filename = argv[1];
	exitname = argv[2];

	aux=exitname+".IAEAphsp";
	outputname=exitname+".IAEAheader";

	output = fopen(aux.c_str(), "wb");

	statsphsp[0].initialise();

	ifstream  finlis(filename.c_str());
	if( !finlis.is_open() ) {
	  cerr << "!!! ERROR :  Unable to read file with list of files: " << filename << endl;
	  std::exception();
	}
	//	for(int i=first; i<=last; i++){
	for(;;){
	  finlis >> aux;
	  if( finlis.eof() ) break;
	  Nfiles ++;
	  //	  aux += + ".IAEAheader";	  
	  check_file->open(aux.c_str());
	  if (check_file->is_open()){
	    check_file->close();
	    inputphsp = new phspheader(aux);
	    //			inputphsp -> print_phspheader();
	    Npar += inputphsp -> PARTICLES;
	    Npho += inputphsp -> PHOTONS;
	    Nele += inputphsp -> ELECTRONS;
	    Npos += inputphsp -> POSITRONS;
	    Tpar += inputphsp -> ORIG_HISTORIES;
	    cout << " PARTICLES " << float(inputphsp->PARTICLES) << " NPART_TOT " << Npar << " NPARTORIG_TOT " << (unsigned long)(Tpar) << " RATIO ";
	    // << float(inputphsp->PARTICLES)/inputphsp->ORIG_HISTORIES << " RATIO_TOT " << float(Npar)/Tpar << endl;
	    float partPS = float(inputphsp->PARTICLES);
	    float partORIG = inputphsp->ORIG_HISTORIES;
	    float div = partPS/partORIG;
	    float error = div*sqrt(1./partPS+1./partORIG);
	    error /= 1-div;
	    cout << div << " +- " << error << " RATIO_TOT " << float(Npar)/Tpar << endl;
	    inputphsp -> get_and_append(output,statsphsp);
	  }else{
	    cerr << "!!! ERROR : Unable to read file: " << aux << endl;
	    std::exception();
	    exit(1);
	    int tempor;
	    cin >> tempor;
	  }
	  if( finlis.eof() ) break;
	}
	
	cout << " * * * * N Particles   = " << Npar  << endl;
	cout << " * * * * N Photons   = " << statsphsp[0].nPhotons   << endl;
	cout << " * * * * N Electrons = " << statsphsp[0].nElectrons << endl;
	cout << " * * * * N Positrons = " << statsphsp[0].nPositrons << endl;
	cout << " * * * * N Original Histories =  " << Tpar << endl;
	inputphsp -> PARTICLES = Npar;
	inputphsp -> PHOTONS   = Npho;
	inputphsp -> ORIG_HISTORIES = Tpar;
	inputphsp -> ELECTRONS = Nele;
	inputphsp -> POSITRONS = Npos;

	//	cout << "Npar          " << Npar << endl;
	//	cout << "Npho          " << Npho << endl;
	// cout << "Tpar          " << Tpar << endl;

	aux.clear();

	cout << "Writing results to          " << outputname << endl;
	cout << " TOTAL N PARTICLE ORIG= " << Tpar << " IN PS= " << Npar << " in N FILES= " << Nfiles <<endl;
	cout << " TOTAL NPART_PS/NPART_ORIG= " << double(Npar)/Tpar <<endl;
 
	inputphsp -> write_IAEAheader(outputname,statsphsp);
	fclose(output);
/*
	if(autoname==0){
	} else if (autoname==1){
		inputphsp -> write_IAEAheader(exitname+".IAEAheader");
	}
*/
	return EXIT_SUCCESS;
}
