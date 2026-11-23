#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


int		m_iterations;

TCanvas* m_c1;

Main()
{
	// ------------
	ReadConfigurationParameters();

	std::ofstream outfile("averaged.dat");

	// " data_state_xy_iter"
	std::string basename = "data_state_xy_iter";
	int iter0 = m_iterations - 500;

	int nevents = 0;
	cout << "Number of events?" << endl;
	cin >> nevents;

	double x, y, z;
	double* sigmax = new double [nevents];
	double* sigmay = new double [nevents];
	double* sigmaz = new double [nevents];

	for (fuck = 0; fuck < nevents; fuck++)
	{
		sigmax[fuck] = 0.0;
		sigmay[fuck] = 0.0;
		sigmaz[fuck] = 0.0;
	}

	int numfiles = 0;

	for (int ii = iter0; ii <= m_iterations; ii += 20)
	{
		std::string fname = basename;
		std::stringstream fff;
		fff << ii;
		fname += fff.str();
		fname += ".dat";

		cout << "#iter: " << ii << " filename: " << fname << endl;

		std::ifstream datfile(fname.c_str());
		if (!datfile.is_open())
		{
			cout << "File with name: " << fname << " is not open." << endl;
			exit(1);
		}

		// loop over all events in file
		int ievent = 0;
		while (!datfile.eof())
		{
			datfile >> x >> y >> z;
			if (!datfile.eof())
			{
				if (ievent < nevents)
				{
					sigmax[ievent] = sigmax[ievent] + x;
					sigmay[ievent] = sigmay[ievent] + y;
					sigmaz[ievent] = sigmaz[ievent] + z;
				}	
				else
				{
					cout << "WARNING, ievent > nevents: " << ievent << " " << nevents << endl;
				}
				ievent++;
			}
		}
		// cout << "end of file: " << ievent << " " << nevents << endl;

		numfiles++;

		datfile.close();	
	} 

	// -----------------
	cout << "#files: " << numfiles << " #events: " << nevents << endl;
	for(int ievent = 0; ievent < nevents; ievent++)
	{	 
		double x = sigmax[ievent] / numfiles;
		double y = sigmay[ievent] / numfiles;
		double z = sigmaz[ievent] / numfiles;

		/*
		if (ievent == 438 || ievent == 439)
		{
			cout << ievent << " " << sigmax[ievent] << " " << x << endl;
		}
		*/

		outfile << x << " " << y << " " << z << endl;
	}

}


void ReadConfigurationParameters()
{
	std::string conffilename = "ir_soe_userparameters.conf";
	ifstream conffile(conffilename.c_str(), ios::in);
	if (!conffile.is_open())
	{
		cout << "Configure file not open: " << conffilename << endl;
		exit(1);
	}
	
	std::string tmpStr, tmpDum;
	double tmpVal;
	
	while (!conffile.eof())
	{
		conffile >> tmpStr >> tmpVal >> tmpDum;
		if (!conffile.eof())
		{
			if (tmpStr == "m_iterations")
			{
				m_iterations = (int) tmpVal;
			}
		}
	}
	
	if (true)
	{
		cout << "PROGRAM PARAMETERS: " << endl;
		cout << "m_iterations: " << m_iterations << endl;
		cout << endl;
	}
}
