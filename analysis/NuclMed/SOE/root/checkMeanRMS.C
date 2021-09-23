#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

int		m_bins;
double	m_xmin;
double	m_xmax;
int		m_iterations;
double 	m_voxelSize;

using namespace std;

std::ifstream datfile;

void Main() 
{
	ReadConfigurationParameters();

	// ------------------------------------
	std::string fn = "soe_results.dat";
	datfile.open(fn.c_str(), ios::in);
	if (!datfile.is_open())
	{
		cout << "FILE NOT FOUND:" << fn << endl;
		return;
	}

	double factor = 0.1;
	double m_bandmin = factor * m_xmin;
	double m_bandmax = factor * m_xmax;

	cout << "bounds: " << m_xmin << " " << m_xmax << endl;
	
	// ----------------------------------------
	double x, y, z;
	double meanx = 0.0;
	double meany = 0.0;
	double meanz = 0.0;
	int countx = 0;
	int county = 0; 
	int countz = 0;
	while (!datfile.eof())
	{
		datfile >> x >> y >> z;
		if (!datfile.eof())
		{
			if (    x > m_xmin && x < m_xmax
				 && y > m_xmin && y < m_xmax)
			{
				if (y > m_bandmin && y < m_bandmax) 
				{
					meanx += x;
					countx++;
				}
				if (x > m_bandmin && x < m_bandmax) 
				{
					meany += y;
					county++;
				}
				if (   x > m_bandmin && x < m_bandmax
					&& y > m_bandmin && y < m_bandmax) 
				{
					meanz += z;
					countz++;
				}
			}
		}
	}
	meanx = meanx / countx;
	meany = meany / county;
	meanz = meanz / countz;


	// the stupid way, whatever --------------------------
	// 
	datfile.close();
	datfile.open(fn.c_str(), ios::in);
	if (!datfile.is_open())
	{
		cout << "FILE NOT FOUND:" << fn << endl;
		return;
	}

	// Note that this function returns the Standard Deviation (Sigma) of the distribution (not RMS).
	// The Sigma estimate is computed as Sqrt((1/N)*(Sum(x_i-x_mean)^2))
	// The name "RMS" was introduced many years ago (Hbook/PAW times). We kept the name for continuity.

	double rmsx = 0.0;
	double rmsy = 0.0;
	double rmsz = 0.0;
	while (!datfile.eof())
	{
		datfile >> x >> y >> z;
		if (!datfile.eof())
		{
			if (    x > m_xmin && x < m_xmax
				 && y > m_xmin && y < m_xmax)
			{
				if (y > m_bandmin && y < m_bandmax) 
				{
					rmsx += (x - meanx) * (x - meanx);
				}
				if (x > m_bandmin && x < m_bandmax) 
				{
					rmsy += (y - meany) * (y - meany);
				}
				if (   x > m_bandmin && x < m_bandmax
					&& y > m_bandmin && y < m_bandmax) 
				{
					rmsz += (z - meanz) * (z - meanz);
				}
			}
		}
	}
	rmsx = std::sqrt(rmsx/countx);
	rmsy = std::sqrt(rmsy/county);
	rmsz = std::sqrt(rmsz/countz);

	cout << "X mean: " << meanx << " sigma:" << rmsx << endl;
	cout << "Y mean: " << meany << " sigma:" << rmsy << endl;
	cout << "Z mean: " << meanz << " sigma:" << rmsz << endl;
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
	
	m_bins = 0;
	m_xmin = 0.0;
	m_xmin = 0.0;
	
	while (!conffile.eof())
	{
		conffile >> tmpStr >> tmpVal >> tmpDum;
		if (!conffile.eof())
		{
			if (tmpStr == "m_bins")
			{
				m_bins = (int) tmpVal;
			}
			else if (tmpStr == "m_xmin")
			{
				m_xmin = tmpVal;
			}
			else if (tmpStr == "m_xmax")
			{
				m_xmax = tmpVal;
			}
			else if (tmpStr == "m_iterations")
			{
				m_iterations = (int) tmpVal;
			}
		}
	}
	
	if (m_bins > 0)
	{
		m_voxelSize = (m_xmax - m_xmin) / m_bins;
	}
	
	if (true)
	{
		cout << "PROGRAM PARAMETERS: " << endl;
		cout << "m_iterations: " << m_iterations << endl;
		cout << "m_bins: " << m_bins << endl;
		cout << "x/y min: " << m_xmin << ", x/y max: " << m_xmax << endl;
		cout << "Voxel size: " << m_voxelSize << endl;
		cout << endl;
	}
}

