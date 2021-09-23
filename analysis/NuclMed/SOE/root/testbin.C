#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

using namespace std;

TFile* myHfile = NULL;
TH1D* myH = NULL;
Int_t Max = 0, maxX = 0, maxY = 0;  

std::ifstream datfile;

TCanvas* m_c1;
TCanvas* m_c2;
TCanvas* m_c3;

int		m_bins;
double	m_xmin;
double	m_xmax;
int		m_iterations;
double 	m_voxelSize;

TH1D* m_h1_x = NULL;
TH1D* m_h1_y = NULL;
TH2D* m_h2contour = NULL;

// Special global constants...
bool m_doDrawGrid = 0;
bool m_doSetOptStat = 1;


void Main() 
{
	ReadConfigurationParameters();

	double realsourcex = 0.0;
	double realsourcey = 0.0;
	cout << "Give real source position, in x and y (if known, else, give 0.0 and 0.0)" << endl;
	cin >> realsourcex;	
	cin >> realsourcey;	

	int nquot = (int) ( realsourcex / m_voxelSize );
	double rest = realsourcex - m_voxelSize * nquot;

	bool doCenterZeroBinX = (rest == 0.0) ? true : false;
	int hxbins = doCenterZeroBinX ? m_bins+1 : m_bins;
	double hxmin = doCenterZeroBinX ? m_xmin-0.5 : m_xmin;
	double hxmax = doCenterZeroBinX ? m_xmax+0.5 : m_xmax;

	cout << "X trgt/voxS: " << nquot << ", rest: " << rest << "  ";

	nquot = (int) ( realsourcey / m_voxelSize );
	rest = realsourcey - m_voxelSize * nquot;

	bool doCenterZeroBinY = (rest == 0.0) ? true : false;
	int hybins = doCenterZeroBinY ? m_bins+1 : m_bins;
	double hymin = doCenterZeroBinY ? m_xmin-0.5 : m_xmin;
	double hymax = doCenterZeroBinY ? m_xmax+0.5 : m_xmax;

	cout << "Y trgt/voxS: " << nquot << ", rest: " << rest << "  ";
	
	cout << "doCenterZeroBinX: " << doCenterZeroBinX << " ";
	cout << "doCenterZeroBinY: " << doCenterZeroBinY << " ";
	cout << endl;

	cout << "X: " << hxbins << "[" << hxmin << "," << hxmax << "]" << endl;
	cout << "Y: " << hybins << "[" << hymin << "," << hymax << "]" << endl;


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

