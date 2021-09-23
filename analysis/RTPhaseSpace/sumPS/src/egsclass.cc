/*==================================================================

      PLOTPHSP UTILITIES LIBRARY

      Juan Diego Soler Pulido
      email: juandiego.soler@ciemat.es
      Madrid, Spain
      December 2006
      Daniel P. A.
      05/2008

 =================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "utilities.h"
#include "FILE2MEMORY.h"
#include "general.h"
#include "matrix.h"

// DPA
//#include "iaeaHeader.h"
//#include "iaeaRecord.h"
//#include "iaeaUtilities.h"
// End DPA

#include "egsclass.h"


/////////////////////////////////////////////////////////////////////////////
//    PHSP STATS PHSP STATS PHSP STATS PHSP STATS PHSP STATS PHSP STATS    //
/////////////////////////////////////////////////////////////////////////////


headerstats::headerstats(){
}

headerstats::~headerstats(){
}

void headerstats::initialise(){
	nParticles = orig_hist = 0;
	nPhotons = nElectrons = nPositrons = nNeutrons = nProtons = 0;
	for(int i=0;i<MAX_NPART;i++) {
		sumParticleWeight[i] = 0.f;
		minimumWeight[i] = 32000.f;
		maximumWeight[i] = 0.f;
		averageKineticEnergy[i] = 0.f;
		minimumKineticEnergy[i] = 32000.f;
		maximumKineticEnergy[i] = 0.f;
	}
	minimumX = minimumY = minimumZ = 32000.f;
	maximumX = maximumY = maximumZ = 0.f;
}

/*
void headerstats::update() {
	if (una->x > maximumX)  maximumX = una->x;
	if (una->x < minimumX)  minimumX = una->x;
	if (una->y > maximumY)  maximumY = una->y;
	if (una->y < minimumY)  minimumY = una->y;
	if (una->z > maximumZ)  maximumZ = una->z;
	if (una->z < minimumZ)  minimumZ = una->z;

	nParticles++;
	particle = (short) una->muse;
	if (particle == 1) {
		nPhotons++;
		sumParticleWeight[0]    += una->wt;
		averageKineticEnergy[0] += una->wt*fabs(una->e);
		if (una->wt > maximumWeight[0])
			maximumWeight[0] = una->wt;
		if (una->wt < minimumWeight[0])
			minimumWeight[0] = una->wt;
		if (fabs(una->e) > maximumKineticEnergy[0])
			maximumKineticEnergy[0] = fabs(una->e);
		if (fabs(una->e) < minimumKineticEnergy[0])
			minimumKineticEnergy[0] = fabs(una->e);
	}
	if (particle == 2) {
		nElectrons++;
		sumParticleWeight[1]    += una->wt;
		averageKineticEnergy[1] += una->wt*fabs(una->e);
		if (una->wt > maximumWeight[1])
			maximumWeight[1] = una->wt;
		if (una->wt < minimumWeight[1])
			minimumWeight[1] = una->wt;
		if (fabs(una->e) > maximumKineticEnergy[1])
			maximumKineticEnergy[1] = fabs(una->e);
		if (fabs(una->e) < minimumKineticEnergy[1])
			minimumKineticEnergy[1] = fabs(una->e);
	}
	if (particle == 3) {
		nPositrons++;
		sumParticleWeight[2]    += una->wt;
		averageKineticEnergy[2] += una->wt*fabs(una->e);
		if (una->wt > maximumWeight[2])
			maximumWeight[2] = una->wt;
		if (una->wt < minimumWeight[2])
			minimumWeight[2] = una->wt;
		if (fabs(una->e) > maximumKineticEnergy[2])
			maximumKineticEnergy[2] = fabs(una->e);
		if (fabs(una->e) < minimumKineticEnergy[2])
			minimumKineticEnergy[2] = fabs(una->e);
	}
	if (particle == 4) {
		nNeutrons++ ;
		sumParticleWeight[3]    += una->wt;
		averageKineticEnergy[3] += una->wt*fabs(una->e);
		if (una->wt > maximumWeight[3] )
			maximumWeight[3] = una->wt;
		if (una->wt < minimumWeight[3] )
			minimumWeight[3] = una->wt;
		if (fabs(una->e) > maximumKineticEnergy[3])
			maximumKineticEnergy[3] = fabs(una->e);
		if (fabs(una->e) < minimumKineticEnergy[3])
			minimumKineticEnergy[3] = fabs(una->e);
	}
	if (particle == 5) {
		nProtons++ ;
		sumParticleWeight[4] +=  una->wt;
		averageKineticEnergy[4] += una->wt*fabs(una->e);
		if (una->wt > maximumWeight[4])
			maximumWeight[4] = una->wt;
		if (una->wt < minimumWeight[4])
			minimumWeight[4] = una->wt;
		if (fabs(una->e) > maximumKineticEnergy[4])
			maximumKineticEnergy[4] = fabs(una->e);
		if (fabs(una->e) < minimumKineticEnergy[4])
			minimumKineticEnergy[4] = fabs(una->e);
	}
}
*/

/////////////////////////////////////////////////////////////////////////////
// PHSP HEADER PHSP HEADER PHSP HEADER PHSP HEADER PHSP HEADER PHSP HEADER //
/////////////////////////////////////////////////////////////////////////////


phspheader::phspheader(string fegsname){
	int i;
	string aux;
	string fname;

	check_inputfile(fegsname);
	cout << "Opening phase space contained in     " << fegsname << " of type " << type << endl;
	char mode[6];

	if (type == "EGS"){
		TITLE = fegsname;
		phspfile = fopen(fegsname.c_str(), "rb");
		fread(mode,5*sizeof(char),1,phspfile);
		mode[5]='\0';

		if(mode[4] == '0')
			mode_n = 0;
		else
			mode_n = 2;

		fread(&n_of_particles,sizeof(int),1,phspfile);
		fread(&n_of_photons,sizeof(int),1,phspfile);
		fread(&energy_max,sizeof(float),1,phspfile);
		fread(&energy_min,sizeof(float),1,phspfile);
		fread(&p_incident,sizeof(float),1,phspfile);

		file_pointer = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3;

		una = new phspparticle(phspfile,type);
	}

	if (type == "IAEA"){
		FILE2MEMORY header(fegsname);

		fname=firstname+".IAEAphsp";
		IAEAfile = fopen(fname.c_str(), "rb");
		una = new phspparticle(IAEAfile,type);

		i=header.find_line("IAEA_INDEX",0, 1);
		IAEA_INDEX=atoi(header.get_line(i+1).c_str());

		i=header.find_line("TITLE",0, 1);
		TITLE=header.get_line(i+1);

		i=header.find_line("FILE_TYPE",0, 1);
		FILE_TYPE=header.get_line(i+1);

		i=header.find_line("CHECKSUM",0, 1);
		aux=header.get_line(i+1);
		CHECKSUM=atoi(aux.c_str());

		i=header.find_line("RECORD_CONTENTS",0, 1);
		una->rc_x   = atoi(header.get_stringFromLine(i+1," ",1).c_str());
		una->rc_y   = atoi(header.get_stringFromLine(i+2," ",1).c_str());
		una->rc_z   = atoi(header.get_stringFromLine(i+3," ",1).c_str());
		una->rc_u   = atoi(header.get_stringFromLine(i+4," ",1).c_str());
		una->rc_v   = atoi(header.get_stringFromLine(i+5," ",1).c_str());
		una->rc_w   = atoi(header.get_stringFromLine(i+6," ",1).c_str());
		una->rc_wght= atoi(header.get_stringFromLine(i+7," ",1).c_str());
		una->rc_flts= atoi(header.get_stringFromLine(i+8," ",1).c_str());
		una->rc_lngs= atoi(header.get_stringFromLine(i+9," ",1).c_str());

		i=header.find_line("RECORD_CONSTANT",0, 1);
		RECORD_CONSTANT = atof(header.get_stringFromLine(i+1," ",1).c_str());

		i=header.find_line("RECORD_LENGTH",0, 1);
		RECORD_LENGTH   = atoi(header.get_stringFromLine(i+1," ",1).c_str());

		i=header.find_line("BYTE_ORDER",0, 1);
		B_ORDER         = atoi(header.get_stringFromLine(i+1," ",1).c_str());

		i=header.find_line("ORIG_HISTORIES",0, 1);
		ORIG_HISTORIES  = atoi(header.get_stringFromLine(i+1," ",1).c_str());

		i=header.find_line("PARTICLES",0, 1);
		PARTICLES       = atoi(header.get_stringFromLine(i+1," ",1).c_str());

		i=header.find_line("PHOTONS",0, 1);
		PHOTONS         = atoi(header.get_stringFromLine(i+1," ",1).c_str());

		i=header.find_line("ELECTRONS",0, 1);
		ELECTRONS       = atoi(header.get_stringFromLine(i+1," ",1).c_str());

		i=header.find_line("POSITRONS",0, 1);
		POSITRONS       = atoi(header.get_stringFromLine(i+1," ",1).c_str());

//DPA
		i=header.find_line("GLOBAL_PHOTON_ENERGY_CUTOFF",0, 1);
		PHOT_E_CUTOFF   = atof(header.get_stringFromLine(i+1," ",1).c_str());
		i=header.find_line("GLOBAL_PARTICLE_ENERGY_CUTOFF",0, 1);
		PART_E_CUTOFF   = atof(header.get_stringFromLine(i+1," ",1).c_str());
//End DPA

		i=header.find_line("STATISTICAL_INFORMATION_PARTICLES",0, 1);
		ph_emin        = atof(header.get_stringFromLine(i+2," ",5).c_str());
		ph_emax        = atof(header.get_stringFromLine(i+2," ",6).c_str());
		el_emin        = atof(header.get_stringFromLine(i+3," ",5).c_str());
		el_emax        = atof(header.get_stringFromLine(i+3," ",6).c_str());
	}
}


phspheader::~phspheader(){
}


void phspheader::get_and_append(FILE* output,headerstats stats[]){
	int control;
	if (type == "IAEA"){
		for(long irecord=0; irecord<PARTICLES; irecord++){
			control=una->refresh_phspparticle(IAEAfile,1,type);
			if(!control){
				una->write_phspparticle(output,irecord+1,type);
				if (una->x > stats[0].maximumX)  stats[0].maximumX = una->x;
				if (una->x < stats[0].minimumX)  stats[0].minimumX = una->x;
				if (una->y > stats[0].maximumY)  stats[0].maximumY = una->y;
				if (una->y < stats[0].minimumY)  stats[0].minimumY = una->y;
				if (una->z > stats[0].maximumZ)  stats[0].maximumZ = una->z;
				if (una->z < stats[0].minimumZ)  stats[0].minimumZ = una->z;
				stats[0].nParticles++;
				short particle = (short) una->muse;
				//cout << " * * * * particle = " << particle << endl;
				if (particle == 1) {
					stats[0].nPhotons++;
					stats[0].sumParticleWeight[0]    += una->wt;
					stats[0].averageKineticEnergy[0] += una->wt*fabs(una->e);
					if (una->wt > stats[0].maximumWeight[0])
						stats[0].maximumWeight[0] = una->wt;
					if (una->wt < stats[0].minimumWeight[0])
						stats[0].minimumWeight[0] = una->wt;
					if (fabs(una->e) > stats[0].maximumKineticEnergy[0])
						stats[0].maximumKineticEnergy[0] = fabs(una->e);
					if (fabs(una->e) < stats[0].minimumKineticEnergy[0])
						stats[0].minimumKineticEnergy[0] = fabs(una->e);
				}
				if (abs(particle) == 2) {
					stats[0].nElectrons++;
					stats[0].sumParticleWeight[1]    += una->wt;
					stats[0].averageKineticEnergy[1] += una->wt*fabs(una->e);
					if (una->wt > stats[0].maximumWeight[1])
						stats[0].maximumWeight[1] = una->wt;
					if (una->wt < stats[0].minimumWeight[1])
						stats[0].minimumWeight[1] = una->wt;
					if (fabs(una->e) > stats[0].maximumKineticEnergy[1])
						stats[0].maximumKineticEnergy[1] = fabs(una->e);
					if (fabs(una->e) < stats[0].minimumKineticEnergy[1])
						stats[0].minimumKineticEnergy[1] = fabs(una->e);
				}
				if (particle == 3) {
					stats[0].nPositrons++;
					stats[0].sumParticleWeight[2]    += una->wt;
					stats[0].averageKineticEnergy[2] += una->wt*fabs(una->e);
					if (una->wt > stats[0].maximumWeight[2])
						stats[0].maximumWeight[2] = una->wt;
					if (una->wt < stats[0].minimumWeight[2])
						stats[0].minimumWeight[2] = una->wt;
					if (fabs(una->e) > stats[0].maximumKineticEnergy[2])
						stats[0].maximumKineticEnergy[2] = fabs(una->e);
					if (fabs(una->e) < stats[0].minimumKineticEnergy[2])
						stats[0].minimumKineticEnergy[2] = fabs(una->e);
				}
				if (particle == 4) {
					stats[0].nNeutrons++ ;
					stats[0].sumParticleWeight[3]    += una->wt;
					stats[0].averageKineticEnergy[3] += una->wt*fabs(una->e);
					if (una->wt > stats[0].maximumWeight[3] )
						stats[0].maximumWeight[3] = una->wt;
					if (una->wt < stats[0].minimumWeight[3] )
						stats[0].minimumWeight[3] = una->wt;
					if (fabs(una->e) > stats[0].maximumKineticEnergy[3])
						stats[0].maximumKineticEnergy[3] = fabs(una->e);
					if (fabs(una->e) < stats[0].minimumKineticEnergy[3])
						stats[0].minimumKineticEnergy[3] = fabs(una->e);
				}
				if (particle == 5) {
					stats[0].nProtons++ ;
					stats[0].sumParticleWeight[4] +=  una->wt;
					stats[0].averageKineticEnergy[4] += una->wt*fabs(una->e);
					if (una->wt > stats[0].maximumWeight[4])
						stats[0].maximumWeight[4] = una->wt;
					if (una->wt < stats[0].minimumWeight[4])
						stats[0].minimumWeight[4] = una->wt;
					if (fabs(una->e) > stats[0].maximumKineticEnergy[4])
						stats[0].maximumKineticEnergy[4] = fabs(una->e);
					if (fabs(una->e) < stats[0].minimumKineticEnergy[4])
						stats[0].minimumKineticEnergy[4] = fabs(una->e);
				}
			}
		}
	}
/*
	int control;
	float temp;
	float xcorr, ycorr;
	int p,q;  //these are the matrix indexes
	int count_photons=0;
	int count_particles=0;

	if (type == "IAEA"){
		for(long irecord=0; irecord<n; irecord++){
			control=una->refresh_phspparticle(IAEAfile,1,type);
			if(!control){
				una->write_phspparticle(outphspfile,irecord+1);
				if(una->q == 0){
	 				count_photons++;
       				}
       				count_particles++;
			}
		}
	}

	if(type == "EGS"){
		//una = new phspparticle(phspfile,type);
		for(long irecord=0; irecord<n; irecord++){
     			control=una->refresh_phspparticle(phspfile,irecord+1,type);
			if(!control){
     				xcorr = (una->x - xmin)/deltax;
     				ycorr = (una->y - ymin)/deltay;
     				p = (int) (decrow-ycorr);
     				q = (int) xcorr;
     				temp = evaluate_event(plotmode, una->wt, una->e, min_weight, max_weight, min_energy, max_energy);
				mat -> addto_matrix(p,q,temp);
				if(temp != 0){
					if(write==1){
       						una->write_phspparticle(outphspfile,irecord+1);
					}
       					if(una->q == 0){
	 					count_photons++;
       					}
       					count_particles++;
    				}
			}
		}
		if(write==1){
			fclose(phspfile);
			n_of_particles=count_particles;
			n_of_photons=count_photons;
			p_incident=n;
		}
	}

	if(write==0){
		mat -> save_matrix(firstname+"MATRIX");
		mat -> colorscale("colors");
		mat -> plot_matrix(firstname+"MATRIX","colors");
	}
*/
}

void phspheader::load_limits(int in, float xmn, float xmx, float ymn, float ymx, float dx, float dy, float mn_wt, float mx_wt, float mn_e, float mx_e){

	int rowSize, colSize;
	if (type == "EGS"){
		if(in > n_of_particles){
			n=n_of_particles;
		} else if (in == 0) {
			n=n_of_particles;
		} else {
			n=in;
		}
	}
	if (type == "IAEA"){
		if(in > PARTICLES){
			n=PARTICLES;
		} else if (in == 0) {
			n=PARTICLES;
		} else {
			n=in;
		}
	}

	xmin = xmn;
	xmax = xmx;
	ymin = ymn;
	ymax = ymx;
	deltax = dx;
	deltay = dy;
	min_weight = mn_wt;
	max_weight = mx_wt;
	min_energy = mn_e;
	max_energy = mx_e;

	deccol = (xmax- xmin)/deltax;
	decrow = (ymax- ymin)/deltay;
	rowSize = (int) decrow;
	colSize = (int) deccol;

	mat = new matrix(rowSize,colSize);
	mat -> get_grid(deltax,deltay,xmin,xmax);

	load_particles(0,0);
}


void phspheader::load_particles(int plotmode2, int write){
	int control;
	float temp;
	float xcorr, ycorr;
	int p,q;  //these are the matrix indexes
	int count_photons=0;
	int count_particles=0;

	if (type == "IAEA"){
		for(long irecord=0; irecord<n; irecord++){
			control=una->refresh_phspparticle(IAEAfile,1,type);
			if(!control){
     				xcorr = (una->x - xmin)/deltax;
     				ycorr = (una->y - ymin)/deltay;
     				p = (int) (decrow-ycorr);
     				q = (int) xcorr;
				temp = evaluate_event(plotmode2, una->wt, una->e, min_weight, max_weight, min_energy, max_energy);
				mat -> addto_matrix(p,q,temp);
				if(temp != 0){
					if(write==1){
       						una->write_phspparticle(outphspfile,irecord+1, type);
					}
       					if(una->q == 0){
	 					count_photons++;
       					}
       					count_particles++;
    				}
			}

		}

		if(write==1){
			fclose(IAEAfile);
			PARTICLES=count_particles;
			PHOTONS=count_photons;
			p_incident=n;
		}
	}

	if(type == "EGS"){
		//una = new phspparticle(phspfile,type);

		for(long irecord=0; irecord<n; irecord++){
		control=una->refresh_phspparticle(phspfile,irecord+1,type);
			if(!control){
     				xcorr = (una->x - xmin)/deltax;
     				ycorr = (una->y - ymin)/deltay;
     				p = (int) (decrow-ycorr);
     				q = (int) xcorr;
     				temp = evaluate_event(plotmode2, una->wt, una->e, min_weight, max_weight, min_energy, max_energy);
				mat -> addto_matrix(p,q,temp);
				if(temp != 0){
					if(write==1){
       						una->write_phspparticle(outphspfile,irecord+1, type);
					}
       					if(una->q == 0){
	 					count_photons++;
       					}
       					count_particles++;
    				}
			}
		}

		if(write==1){
			fclose(phspfile);
			n_of_particles=count_particles;
			n_of_photons=count_photons;
			p_incident=n;
		}
	}

	if(write==0){
		mat -> save_matrix(firstname+"MATRIX");
		mat -> colorscale("colors");
		mat -> plot_matrix(firstname+"MATRIX","colors");
	}
}


void phspheader::print_phspheader(){
	if (type == "EGS"){
		cout << "EGS FILE" << endl << endl;
		cout << "Mode                         " << mode_n         << endl
		     << "Total N of particles         " << n_of_particles << endl
		     << "N of photons                 " << n_of_photons   << endl
		     << "Minimal energy               " << energy_min     << endl
		     << "Maximal energy               " << energy_max     << endl
		     << "Particles from org source    " << p_incident     << endl;
	}

	std::cout << " print_phspheader " << type << std::endl;
	if (type == "IAEA"){
		cout << "IAEA FILE " << endl;
		cout << "Title                  " << TITLE         << endl
		     << "File type              " << FILE_TYPE     << endl
		     << "Checksum               " << CHECKSUM      << endl;
		cout << "Record contents        " << endl	
		     << "X stored               " << una->rc_x << endl
		     << "Y stored               " << una->rc_y << endl
		     << "Z stored               " << una->rc_z << endl
		     << "U stored               " << una->rc_u << endl
		     << "V stored               " << una->rc_v << endl
		     << "W stored               " << una->rc_w << endl
		     << "Weight stored          " << una->rc_wght << endl
		     << "Extra floats stored    " << una->rc_flts << endl
		     << "Extra longs stored     " << una->rc_lngs << endl;

		cout << "Record constant        " << RECORD_CONSTANT << endl
		     << "Record length          " << RECORD_LENGTH   << endl
		     << "Byte order             " << B_ORDER << endl
		     << "Original histories     " << ORIG_HISTORIES << endl
		     << "Particles              " << PARTICLES << endl
		     << "Photons                " << PHOTONS << endl
		     << "Electrons              " << ELECTRONS << endl
		     << "Positrons              " << POSITRONS << endl;

		cout << "Photons Emin           " << ph_emin << " MeV" << endl
		     << "Photons Emax           " << ph_emax << " MeV" << endl
		     << "Electrons Emin         " << el_emin << " MeV" << endl
		     << "Electrons Emax         " << el_emax << " MeV" << endl;
	}
}


void phspheader::write_EGSphsp(string outputphsp){
	outphspfile = fopen(outputphsp.c_str(), "wb");

	char mode[6];
	char dummy;
	float aux;

	(void) fseek(outphspfile,(long) 0,SEEK_SET);

	if(type == "EGS"){
		file_pointer_out = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3;

		load_particles(0,1);

		(void) fseek(outphspfile,(long) 0,SEEK_SET);
		sprintf(mode,"MODE0 %1d",mode_n);
		fwrite(mode,5*sizeof(char),1,outphspfile);
		fwrite(&n_of_particles,sizeof(int),1,outphspfile); 
		fwrite(&n_of_photons,sizeof(int),1,outphspfile);
		fwrite(&energy_max,sizeof(float),1,outphspfile);
		fwrite(&energy_min,sizeof(float),1,outphspfile);
		fwrite(&p_incident,sizeof(float),1,outphspfile);
	}

	if(type == "IAEA"){
		aux = (float) ORIG_HISTORIES;

		file_pointer_out = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3;

		load_particles(0,1);

		(void) fseek(outphspfile,(long) 0,SEEK_SET);
		sprintf(mode,"MODE0 %1d",0);
		fwrite(mode,5*sizeof(char),1,outphspfile);
		fwrite(&PARTICLES,sizeof(int),1,outphspfile); 
		fwrite(&PHOTONS,sizeof(int),1,outphspfile);
		fwrite(&ph_emax,sizeof(float),1,outphspfile);
		fwrite(&ph_emin,sizeof(float),1,outphspfile);
		fwrite(&aux,sizeof(float),1,outphspfile);
		dummy = 0;
		fwrite(&dummy,sizeof(char),1,outphspfile);
		fwrite(&dummy,sizeof(char),1,outphspfile);
		fwrite(&dummy,sizeof(char),1,outphspfile);
	}

	fclose(outphspfile);
}


void phspheader::write_IAEAheader(string outputphsp, headerstats stats[]){
	FILE* outIAEAfile;
	outIAEAfile = fopen(outputphsp.c_str(), "wb");

	if (type == "EGS"){
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"IAEA_INDEX",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%i   // Test header\n \n",0);
		
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"TITLE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile," IAEA phase space from EGS file %s\n \n", TITLE.c_str());
		
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"FILE_TYPE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%s\n\n", "0");

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"CHECKSUM",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%d\n\n", 0);

		fprintf(outIAEAfile,"%c%s%c\n ",SEGMENT_BEG_TOKEN,"RECORD_CONTENTS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"    %i     // X is stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // Y is stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // Z is stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // U is stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // V is stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // W is stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // Weight is stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // Extra floats stored ?\n", 1);
		fprintf(outIAEAfile,"    %i     // Extra longs stored ?\n\n", 1);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"RECORD_CONSTANT",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"   %f     // Constant Z\n\n", 0.);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"RECORD_LENGTH",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%i\n\n", 0);
	
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"B_ORDER",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%i\n\n", 0);
	
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"ORIG_HISTORIES",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%f\n\n", p_incident);
		
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"PARTICLES",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%d\n\n", n_of_particles);
	
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"PHOTONS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%d\n\n", n_of_photons);
	
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"ELECTRONS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%d\n\n", n_of_particles-n_of_photons);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"POSITRONS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%d\n\n", 0);

		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"TRANSPORT_PARAMETERS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"MACHINE_TYPE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"MONTE_CARLO_CODE_VERSION",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"GLOBAL_PHOTON_ENERGY_CUTOFF",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"GLOBAL_PARTICLE_ENERGY_CUTOFF",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"COORDINATE_SYSTEM_DESCRIPTION",SEGMENT_END_TOKEN);
	}

	if (type == "IAEA"){
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"IAEA_INDEX",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%i   // Test header\n\n",IAEA_INDEX);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"TITLE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%s\n\n", TITLE.c_str());

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"FILE_TYPE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%s\n\n", FILE_TYPE.c_str());
	
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"CHECKSUM",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%ld \n\n", RECORD_LENGTH*PARTICLES);

		fprintf(outIAEAfile,"%c%s%c\n ",SEGMENT_BEG_TOKEN,"RECORD_CONTENTS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"   %i     // X is stored ?\n ", una->rc_x);
		fprintf(outIAEAfile,"   %i     // Y is stored ?\n ", una->rc_y);
		fprintf(outIAEAfile,"   %i     // Z is stored ?\n ", una->rc_z);
		fprintf(outIAEAfile,"   %i     // U is stored ?\n ", una->rc_u);
		fprintf(outIAEAfile,"   %i     // V is stored ?\n ", una->rc_v);
		fprintf(outIAEAfile,"   %i     // W is stored ?\n ", una->rc_w);
		fprintf(outIAEAfile,"   %i     // Weight is stored ?\n ", una->rc_wght);
		fprintf(outIAEAfile,"   %i     // Extra floats stored ?\n ", una->rc_flts);
		fprintf(outIAEAfile,"   %i     // Extra longs stored ?\n\n", una->rc_lngs);

		fprintf(outIAEAfile,"%c%s%c\n ",SEGMENT_BEG_TOKEN,"RECORD_CONSTANT",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"  %6.4f     // Constant Z\n\n", RECORD_CONSTANT);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"RECORD_LENGTH",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%i\n\n", RECORD_LENGTH);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"BYTE_ORDER",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%i\n\n", B_ORDER);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"ORIG_HISTORIES",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%ld\n\n", ORIG_HISTORIES);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"PARTICLES",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%ld\n\n", PARTICLES);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"PHOTONS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%ld\n\n", PHOTONS);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"ELECTRONS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%ld\n\n", ELECTRONS);

		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"POSITRONS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%ld\n\n", POSITRONS);

		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"TRANSPORT_PARAMETERS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"MACHINE_TYPE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"MONTE_CARLO_CODE_VERSION",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"GLOBAL_PHOTON_ENERGY_CUTOFF",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%1.3f\n",PHOT_E_CUTOFF);
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"GLOBAL_PARTICLE_ENERGY_CUTOFF",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%1.3f\n",PART_E_CUTOFF);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"COORDINATE_SYSTEM_DESCRIPTION",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"//  OPTIONAL INFORMATION\n\n");
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"BEAM_NAME",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"FIELD_SIZE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"NOMINAL_SSD",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"MC_INPUT_FILENAME",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"VARIANCE_REDUCTION_TECHNIQUES",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"INITIAL_SOURCE_DESCRIPTION",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"PUBLISHED_REFERENCE",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"AUTHORS",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"INSTITUTION",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n\n",SEGMENT_BEG_TOKEN,"LINK_VALIDATION",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"ADDITIONAL_NOTES",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"This is IAEA header as defined in the technical \n");
		fprintf(outIAEAfile,"report IAEA(NDS)-0484, Vienna, 2006\n\n");
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"STATISTICAL_INFORMATION_PARTICLES",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile,"//    Weight     Wmin    Wmax       <E>       Emin        Emax    Particle\n");
		float eaver; char buffer[15];
		for(int i=0;i<MAX_NPART;i++) {
			switch (i) {
				case 0: strcpy(buffer," PHOTONS"); break;
				case 1: strcpy(buffer," ELECTRONS"); break;
				case 2: strcpy(buffer," POSITRONS"); break;
				case 3: strcpy(buffer," NEUTRONS"); break;
				case 4: strcpy(buffer," PROTONS"); break;
			}
			if( (i == 0) && (stats[0].nPhotons == 0 ) ) continue;
			if( (i == 1) && (stats[0].nElectrons == 0 ) ) continue;
			if( (i == 2) && (stats[0].nPositrons == 0 ) ) continue;
			if( (i == 3) && (stats[0].nNeutrons == 0 ) ) continue;
			if( (i == 4) && (stats[0].nProtons == 0 ) ) continue;
			eaver = 0.f;
			if(stats[0].sumParticleWeight[i]>0) {
				stats[0].averageKineticEnergy[i] /= stats[0].sumParticleWeight[i];
				eaver = stats[0].averageKineticEnergy[i];
			}
			fprintf(outIAEAfile,"  %12.2f %6.5f %6.2f %10.2f    %6.5f  %10.2f  %s\n", stats[0].sumParticleWeight[i], stats[0].minimumWeight[i], stats[0].maximumWeight[i], eaver, stats[0].minimumKineticEnergy[i], stats[0].maximumKineticEnergy[i], buffer);
		}
		fprintf(outIAEAfile,"\n");
		fprintf(outIAEAfile,"%c%s%c\n",SEGMENT_BEG_TOKEN,"STATISTICAL_INFORMATION_GEOMETRY",SEGMENT_END_TOKEN);
		fprintf(outIAEAfile," %f  %f\n",stats[0].minimumX,stats[0].maximumX);
		fprintf(outIAEAfile," %f  %f\n",stats[0].minimumY,stats[0].maximumY);
		fprintf(outIAEAfile," %f  %f\n",stats[0].minimumZ,stats[0].maximumZ);
	}
	//load_particles(0);
	fclose(outIAEAfile);
}


void phspheader::check_inputfile(string input){
  char aux[MAX_STR_LEN]={"\0"};
	char* aux2;
	char aux3[MAX_STR_LEN]={"\0"};
	int i;
	string name;

	strcpy(aux,input.c_str());
	aux2 = strstr(aux,".");

	if(strcmp(aux2,".egsphsp1")==0 || strcmp(aux2,".egsphsp2")==0 || strcmp(aux2,".egsphsp3")==0){
		type = "EGS";
	}else if(strcmp(aux2,".IAEAheader")==0){
		type = "IAEA";
	}else {
		type = "UNKNOWN";
	}

	i = strcspn (input.c_str(),".");
	strncpy (aux3,input.c_str(),i);
	firstname=aux3;

	std::string egssuff = input.substr(input.length()-9,9);
	if(egssuff == ".egsphsp1" || egssuff == ".egsphsp2" || egssuff == ".egsphsp3" ) {
	  type = "EGS";
	  firstname = input.substr(0,input.length()-9);
	}else if(input.substr(input.length()-11,11) == ".IAEAheader" ){
	  firstname = input.substr(0,input.length()-11);
	  type = "IAEA";
	}else {
	  std::cerr << "!!! FATAL ERROR: Unknown suffix type of file " << input << std::endl;
	  std::exception();
	}

	//	std::cout << " check_inputfile " << input << " firstname " << firstname << std::endl;

}


///////////////////////////////////////////////////////////////////////////
// PHSP PARTICLE PHSP PARTICLE PHSP PARTICLE PHSP PARTICLE PHSP PARTICLE //
///////////////////////////////////////////////////////////////////////////


phspparticle::phspparticle(FILE* , string mode) {
	if (mode == "EGS"){
		//  file_pointer = point;
		file_pointer = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3;
	}
	if (mode == "IAEA"){
	}
}


phspparticle::~phspparticle(){
}


int phspparticle::phsp_integrity(int i){
  char str[50];

  sprintf(str, "%e", x);
  if (strcmp(str,"nan")==0) 
    {
      printf("x coord in record number %d\n",i);
      return(FALSE);
    }
  sprintf(str,"%e",y);
  if (strcmp(str,"nan")==0) 
    {
      printf("y coord in record number %d\n",i);
      return(FALSE);
    }
  sprintf(str,"%e",u);
  if (strcmp(str,"nan")==0) 
    {
      printf("u direction in record number %d\n",i);
      return(FALSE);
    }
  sprintf(str,"%e",v);
  if (strcmp(str,"nan")==0) 
    {
      printf("v direction in record number %d\n",i);
      return(FALSE);
    }
  sprintf(str,"%ld",latch);
  if (strcmp(str,"nan")==0) 
    {
      printf("latch variable in record number %d\n",i);
      return(FALSE);
    }
  sprintf(str,"%e",w);
  if (strcmp(str,"nan")==0) 
    {
      printf("weight in record number %d\n",i);
      return(FALSE);
    }
  sprintf(str,"%e",e);
  if (strcmp(str,"nan")==0) 
    {
      printf("energy in record number %d\n",i);
      return(FALSE);
    }
  return(TRUE);
}


// Include data in phshparticle object

int phspparticle::refresh_phspparticle(FILE* file, long number, string type){
	float aux;
	int reset_phsp;
	int a1;
	long step;
	short particle;

	if(type=="IAEA"){
		a1 = fread(&muse,sizeof(char),1,file);
//		cout << "Particle  " << (short) muse << endl;
		particle = (short) muse;

		if(particle==1){
			q=0;
		} else if (particle==2){
			q=1;
		}

		a1 = fread(&e,sizeof(e),1,file);
		
		if(rc_x==1){
			a1 = fread(&x,sizeof(x),1,file);
			//x/=10;
		}
		if(rc_y==1){
			a1 = fread(&y,sizeof(y),1,file);
			//y/=10;
		}
		if(rc_z==1){
			a1 = fread(&z,sizeof(z),1,file);
			//z/=10;
		}
		if(rc_u==1){
			a1 = fread(&u,sizeof(u),1,file);
		}
		if(rc_v==1){
			a1 = fread(&v,sizeof(v),1,file);
		}
		if(rc_wght==1){
			a1 = fread(&wt,sizeof(wt),1,file);
		}

		// temporary
		latch = 0;
		aux = u*u+v*v;
			if (aux<=1.0)
				w = (float) sqrt((float)(1.0-aux));
			else{
				aux= (float)sqrt((float)aux);
				u/=aux;
				v/=aux;
				w=0.0f;
			}
			if(w < 0.0f)
				w *= -1.0f;
		npass=0;

		//include extra floats and extra longs

		reset_phsp = FALSE;
	}

	if(type=="EGS"){
		step = sizeof(long int) + 6*sizeof(float);
		p_number = number;
		(void) fseek(file,file_pointer,SEEK_SET);
		a1 = fread(&latch,sizeof(long int),1,file);
		a1 &= fread(&e,sizeof(float),1,file);
		a1 &= fread(&x,sizeof(float),1,file);
		a1 &= fread(&y,sizeof(float),1,file);
		a1 &= fread(&u,sizeof(float),1,file);
		a1 &= fread(&v,sizeof(float),1,file);
		a1 &= fread(&wt,sizeof(float),1,file);

		if(a1){
			reset_phsp = FALSE;
			e *= (-1);
			if(!phsp_integrity(p_number)){
				printf("Phase space problem on record %ld!\n",p_number);
				return TRUE;
			}
			// calculate w
			aux = u*u+v*v;
			if (aux<=1.0)
				w = (float) sqrt((float)(1.0-aux));
			else{
				aux= (float)sqrt((float)aux);
				u/=aux;
				v/=aux;
				w=0.0f;
			}
			if(w < 0.0f)
				w *= -1.0f;
			// obtain npass from latch
	
			if ((latch & 0x80000000) != 0){
				// printf("Multiple crosser\n");
				npass=1;
				latch &= 0x7FFFFFFF;
			} else
				npass=0;

			// obtain q from latch

			if((latch & 0x40000000) != 0){
				q = -1;
				latch &= 0xBFFFFFFF;
			} else if ((latch & 0x20000000) != 0){
				q = +1;
				latch &= 0xDFFFFFFF;
			} else
				q = 0;

			file_pointer += step;

			if(p_number <= MAX_PRINT){
				cout << "# " <<p_number;
				cout << " Q " << q;
				cout << " E " << e;
				cout << " x " << x;
				cout << " y " << y << endl;
				cout << "\tu " << u;
				cout << " v " << v;
				cout << " w " << w;
				cout << " W " << wt;
				cout << " LATCH " << latch;
				cout << " NPASS " << npass <<  endl;
			}
		}else{
			printf("phsp_rw: unsuccesful fread at record %ld\n",file_pointer/step);
			printf("End of file\n");

			//     if(fseek(file,(long)0,SEEK_SET) != 0){
			//       printf("In phsp_rw: phsp_read_particle() file could not be reset\n");
			//       exit(-1);
			//     }
			//     eof = eof + 1; //increase the number of restarts
			//     reset_phsp = TRUE;
			//     file_pointer += sizeof(long int)+6*sizeof(float);
			//    return reset_phsp;
		}
	}
	return reset_phsp;
}


void phspparticle::write_phspparticle(FILE* outphspfile, long number, string type){
	//char muse;

	if(type=="IAEA"){
		//cout << "Particle " << (short) muse << endl;
		fwrite(&muse, sizeof(char),1, outphspfile);
		fwrite(&e,sizeof(e),1,outphspfile);
		fwrite(&x,sizeof(x),1,outphspfile);

		if(rc_y==1){
			fwrite(&y,sizeof(y),1,outphspfile);
		}
		if(rc_z==1){
			fwrite(&z,sizeof(z),1,outphspfile);
		}
		if(rc_u==1){
			fwrite(&u,sizeof(u),1,outphspfile);
		}
		if(rc_v==1){
			fwrite(&v,sizeof(v),1,outphspfile);
		}
		if(rc_wght==1){
			fwrite(&wt,sizeof(wt),1,outphspfile);
		}
	}

	if(type=="EGS"){
		long int aux_latch;
		if(number == 1){
			file_pointer_out = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3;
			(void) fseek(outphspfile,file_pointer_out,SEEK_SET);
		}

		//  (void) fseek(outphspfile,file_pointer_out,SEEK_SET);

		if (q == -1)
			aux_latch |= 0x40000000; // set bit 30
		else if (q == 0)
			aux_latch &= 0x9FFFFFFF; // clear bit 30,29
		else {
			aux_latch &= 0xBFFFFFFF; 
			aux_latch |= 0x20000000; // set 29, clr 30
		}

		if (npass == 0)
			aux_latch &= 0x7FFFFFFF;  // clear bit 31
		else
			aux_latch |= 0x80000000;  // set bit 31

		fwrite(&aux_latch,sizeof(long int),1,outphspfile);
		fwrite(&e,sizeof(float),1,outphspfile);
		fwrite(&x,sizeof(float),1,outphspfile);
		fwrite(&y,sizeof(float),1,outphspfile);
		fwrite(&u,sizeof(float),1,outphspfile);
		fwrite(&v,sizeof(float),1,outphspfile);
		fwrite(&wt,sizeof(float),1,outphspfile);
		file_pointer_out += sizeof(long int) + 6*sizeof(float);
	}
}
