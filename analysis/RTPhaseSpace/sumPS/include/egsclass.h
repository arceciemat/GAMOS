#ifndef __EGSCLASS_H_
#define __EGSCLASS_H_
#endif

#include <string>

#define SEGMENT_BEG_TOKEN '$'
#define SEGMENT_END_TOKEN ':'
#define OK     0
#define FAIL  -1
#define MAX_NPART 5 // 1 photons
                    // 2 electrons
                    // 3 positrons
                    // 4 neutrons
                    // 5 protons

//DPA
class headerstats {
	public:
		headerstats();
		~headerstats();
		float sumParticleWeight[MAX_NPART];
		float minimumWeight[MAX_NPART];
		float maximumWeight[MAX_NPART];
		float averageKineticEnergy[MAX_NPART];
		float minimumKineticEnergy[MAX_NPART];
		float maximumKineticEnergy[MAX_NPART];
		float minimumX, maximumX;
		float minimumY, maximumY;
		float minimumZ, maximumZ;
		int nParticles, orig_hist;
		int nPhotons, nElectrons, nPositrons, nNeutrons, nProtons;
		void initialise();
		//void update();
};
//End DPA

class phspparticle {
	public:
		phspparticle(FILE* file, string mode);
		~phspparticle();
		long p_number;
		long int latch;
		float q;
		float e;
		float x, y, z;
		float u, v, w;
		float wt;
		int npass;
		char muse;
		long file_pointer;
		long file_pointer_out;
		int eof;
		vector<long> longs;
		vector<float> floats;

		int rc_x, rc_y, rc_z, rc_u, rc_v, rc_w, rc_wght, rc_flts, rc_lngs;
		int phsp_integrity(int i);
		int refresh_phspparticle(FILE* file, long number, string type);
		void write_phspparticle(FILE* outphspfile, long number, string type);
	private:
};

class phspheader {
	public:
		phspheader(string fegsname);
		~phspheader();

		string type;
		string firstname;

		int mode_n;
		int n_of_particles;
		int n_of_photons;
		float energy_max;
		float energy_min;
		float p_incident;

		int IAEA_INDEX;
		string TITLE;
		string FILE_TYPE;
		long CHECKSUM;
		float RECORD_CONSTANT;
		int RECORD_LENGTH, B_ORDER;
		long ORIG_HISTORIES, PARTICLES, PHOTONS, ELECTRONS, POSITRONS;
//DPA
		float PHOT_E_CUTOFF, PART_E_CUTOFF;
//End DPA
		float ph_emin, ph_emax;
		float el_emin, el_emax;

		float xmin, xmax, ymin, ymax;
		float deltax, totalx, deltay, totaly;
		float min_weight, max_weight;
		float min_energy, max_energy;
		float decrow, deccol;
		int n, rewrite, plotmode, enter;

		long file_pointer;
		long file_pointer_out;
		FILE* phspfile;
		FILE* IAEAfile;
		FILE* outphspfile;
		//FILE* outIAEAfile;

		matrix* mat;
		phspparticle* una;

		void load_limits(int in, float xmn, float xmx, float ymn, float ymx, float dx, float dy, float mn_wt, float mx_wt, float mn_e, float mx_e);

//DPA
		//(orig): void get_and_append(FILE* output, headerstats* stats);
		void get_and_append(FILE* output, headerstats stats[]);
//End DPA

		void print_phspheader();
		void write_EGSphsp(string outputphsp);

//DPA
		//(orig): void write_IAEAheader(string outputphsp);
		void write_IAEAheader(string outputphsp, headerstats stats[]);
//ENd DPA

	private:
		void load_particles(int plotmode, int write);
		void check_inputfile(string input);
};
