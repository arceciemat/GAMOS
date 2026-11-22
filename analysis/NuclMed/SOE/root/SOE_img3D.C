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

const double kPI = 3.14159265;

std::ifstream datfile;

TCanvas* m_c1;
TCanvas* m_c2;
TCanvas* m_c3;
TCanvas* m_c4;
TCanvas* m_c5;
TCanvas* m_c6;

int		m_bins_x;
double	m_xmin;
double	m_xmax;

int		m_bins_y;
double	m_ymin;
double	m_ymax;

int		m_bins_z;
double 	m_zmin;
double 	m_zmax;

int		m_iterations;
double 	m_voxelSizeX;
double 	m_voxelSizeY;
double 	m_voxelSizeZ;

TH1D* m_h1_x = NULL;
TH1D* m_h1_y = NULL;
TH1D* m_h1_z = NULL;

TH2D* m_h2contour = NULL;
TH3D* m_h3contour = NULL;

// Special global constants...
bool m_doDrawGrid = 0;
bool m_doSetOptStat = 1;

struct 
{
	double m_x;
	double m_y;
	double m_z;
} myvector;

// ===================================================

void Main() 
{
	// ------------
	gROOT->SetStyle("Plain");  
	gStyle->SetOptStat(m_doSetOptStat);  
	gStyle->SetPalette(1); 

	TFile* myhistos = new TFile("img_hist_SOE_3D.root","RECREATE");
	
	// ------------------------------------
	std::string fn = "soe_results.dat";
	datfile.open(fn.c_str(), ios::in);
	if (!datfile.is_open())
	{
		cout << "FILE NOT FOUND:" << fn << endl;
		return;
	}

	m_c1 = new TCanvas("m_c1", "m_c1", 500, 500);
	m_c2 = new TCanvas("m_c2", "m_c2", 500, 500);
	m_c3 = new TCanvas("m_c3", "m_c3", 500, 500);
	m_c4 = new TCanvas("m_c4", "m_c4", 500, 500);
	m_c5 = new TCanvas("m_c5", "m_c5", 500, 500);
	m_c6 = new TCanvas("m_c6", "m_c6", 500, 500);

m_c5->Update();
m_c6->Update();

	// Get configuration parameters used by SOE
	ReadConfigurationParameters();

	// Locate center point in center of bin (more or less)
	double realsourcex = 0.0;
	double realsourcey = 0.0;
	double realsourcez = -100.0;

	int sourceposknown = 0;
	cout << "Real position of source known (1/0)?" << endl;
	cin >> sourceposknown;
	
	if (sourceposknown) 
	{
		cout << "Give real source position, in x, y and z (if known, else, give 0.0, 0.0 and -100.0)" << endl;
		cin >> realsourcex;	
		cin >> realsourcey;	
		cin >> realsourcez;	
	}

	int centershift = 0;
	cout << "shift known source or 0 point to center of bin (1/0)?" << endl;
	cin >> centershift;

	int nquot = (int) ( realsourcex / m_voxelSizeX );
	double rest = realsourcex - m_voxelSizeX * nquot;
	double delta = m_voxelSizeX / 2;

	bool doCenterZeroBinX = (rest == 0.0 && centershift == 1) ? true : false;
	int hxbins = doCenterZeroBinX ? m_bins_x+1 : m_bins_x;
	double hxmin = doCenterZeroBinX ? m_xmin-delta : m_xmin;
	double hxmax = doCenterZeroBinX ? m_xmax+delta : m_xmax;

	nquot = (int) ( realsourcey / m_voxelSizeY );
	rest = realsourcey - m_voxelSizeY * nquot;
	delta = m_voxelSizeY / 2;

	bool doCenterZeroBinY = (rest == 0.0 && centershift == 1) ? true : false;
	int hybins = doCenterZeroBinY ? m_bins_y+1 : m_bins_y;
	double hymin = doCenterZeroBinY ? m_ymin-delta : m_ymin;
	double hymax = doCenterZeroBinY ? m_ymax+delta : m_ymax;

	nquot = (int) ( realsourcez / m_voxelSizeZ );
	rest = realsourcez - m_voxelSizeZ * nquot;
	delta = m_voxelSizeZ / 2;

	bool doCenterZeroBinZ = (rest == 0.0 && centershift == 1) ? true : false;
	int hzbins = doCenterZeroBinZ ? m_bins_z+1 : m_bins_z;
	double hzmin = doCenterZeroBinZ ? m_zmin-delta : m_zmin;
	double hzmax = doCenterZeroBinZ ? m_zmax+delta : m_zmax;

	//******** 2D IMAGE *******	
	m_h3contour = new TH3D("m_h3contour", "", hxbins, hxmin, hxmax, hybins, hymin, hymax, hzbins, hzmin, hzmax);

	double x, y, z;
	std::vector<double> lameROOTx;
	std::vector<double> lameROOTy;
	std::vector<double> lameROOTz;
	int ncounts = 0;
	while (!datfile.eof())
	{
		datfile >> x >> y >> z;
		if (!datfile.eof())
		{
			m_h3contour->Fill(x, y, z);
			lameROOTx.push_back(x);
			lameROOTy.push_back(y);
			lameROOTz.push_back(z);
			ncounts++;
		}
	}
	datfile.close();

	// Get min and max index...
	// Stupid ROOT starts counting bins from 1 ( bin0 = underflow )
	int maxContent = 0.0; 
	int maxX_index = 0, maxY_index = 0, maxZ_index = 0;
	double maxx, maxy, maxz;
	double binsizex = (hxmax - hxmin) / hxbins; 
	double binsizey = (hymax - hymin) / hybins; 
	double binsizez = (hzmax - hzmin) / hzbins; 
	for(int ix = 1; ix <= m_bins_x; ix++)
	{     
		for(int iy = 1; iy <= m_bins_y; iy++)
		{	 
			for(int iz = 1; iz <= m_bins_z; iz++)
			{	     
				double cont = m_h3contour->GetBinContent(ix, iy, iz);
				if (cont > maxContent)
				{
					maxContent = cont;
					maxX_index = ix;       
					maxY_index = iy;     
					maxZ_index = iz;

					maxx = hxmin + (ix - 1) * binsizex + 0.5 * binsizex;
					maxy = hymin + (iy - 1) * binsizey + 0.5 * binsizey;
					maxz = hzmin + (iz - 1) * binsizez + 0.5 * binsizez;
				}
			}
		}
	}
	cout << "MAX points: "  
		 << maxX_index << ": " << maxx << " "
		 << maxY_index << ": " << maxy << " "
		 << maxZ_index << ": " << maxz << endl;

	if (sourceposknown) 
	{
		// Stupid ROOT starts counting bins from 1 ( bin0 = underflow )
		maxX_index = 1 + ((realsourcex - hxmin) / binsizex);
		maxY_index = 1 + ((realsourcey - hymin) / binsizey);
		maxZ_index = 1 + ((realsourcez - hzmin) / binsizez);
		cout << "REAL CENTER points: "  
		 	<< maxX_index << ": " << realsourcex << " "
		 	<< maxY_index << ": " << realsourcey << " "
		 	<< maxZ_index << ": " << realsourcez << endl;

		maxx = realsourcex;
		maxy = realsourcey;
		maxz = realsourcez;
	}

	m_c1->cd();	
	
	m_h3contour->SetMinimum(-1); 
	m_h3contour->GetXaxis()->SetTitle("X [mm]");
	m_h3contour->GetXaxis()->CenterTitle();
	m_h3contour->GetYaxis()->SetTitle("Y [mm]");
	m_h3contour->GetYaxis()->CenterTitle();
	m_h3contour->SetContour(10); 
	m_h3contour->Draw("LEGO");   
	
	// m_h3contour->Write("ImageXYZ"); 
	
	m_c1->Update();
	
	// ****** X AND Y AND Z AND R PROFILE LINE PLOTS ******
	m_h1_x = new TH1D("m_h1_x", "", hxbins, hxmin, hxmax);

	m_h1_x->GetXaxis()->SetTitle("X [mm]");
	m_h1_x->GetXaxis()->CenterTitle();
	m_h1_x->GetYaxis()->SetTitle("Intensity"); 
	m_h1_x->GetYaxis()->CenterTitle();
	
	m_h1_y = new TH1D("m_h1_y", "", hybins, hymin, hymax);

	m_h1_y->GetXaxis()->SetTitle("Y [mm]");
	m_h1_y->GetXaxis()->CenterTitle();   
	m_h1_y->GetYaxis()->SetTitle("Intensity");   
	m_h1_y->GetYaxis()->CenterTitle();

	m_h1_z = new TH1D("m_h1_z", "", hzbins, hzmin, hzmax);

	m_h1_z->GetXaxis()->SetTitle("Z [mm]");
	m_h1_z->GetXaxis()->CenterTitle();   
	m_h1_z->GetYaxis()->SetTitle("Intensity");   
	m_h1_z->GetYaxis()->CenterTitle();

	// R PROFILE PLOTs -----------------------------------------
	double hrmin = 0;
	double hrmax = m_xmax;
	int hrbins = (int) ( (hrmax - hrmin) / m_voxelSizeX ); 
	double rbinsize = m_voxelSizeX;
	hrmax = hrbins * rbinsize;
	
	m_h1_r = new TH1D("m_h1_r", "R", hrbins, hrmin, hrmax);
	// cout << "R histo: 1st bin: " << hrmin << " " << hrmin + rbinsize << " binwidth: " << m_h1_r->GetBinWidth(1) << endl;

	m_h1_r->GetXaxis()->SetTitle("R [mm]");
	m_h1_r->GetXaxis()->CenterTitle();   
	m_h1_r->GetYaxis()->SetTitle("Intensity");   
	m_h1_r->GetYaxis()->CenterTitle();

	// -----
	double raccmin = 0;
	double raccmax = hrmax; // std::sqrt(hrmax*hrmax + hrmax*hrmax + hrmax*hrmax);
	int raccbins = hrbins;
	
	m_h1_racc = new TH1D("m_h1_racc", "R acc", raccbins, raccmin, raccmax);
	// cout << "Racc histo: 1st bin: " << raccmin << " " << raccmin + rbinsize << " binwidth: " << m_h1_racc->GetBinWidth(1) << endl;

	m_h1_racc->GetXaxis()->SetTitle("Racc [mm]");
	m_h1_racc->GetXaxis()->CenterTitle();   
	m_h1_racc->GetYaxis()->SetTitle("Intensity");   
	m_h1_racc->GetYaxis()->CenterTitle();
	
	// Fill m_h1_x and m_h1_y and m_h1_z ------------------------------------
	for(int ix=1; ix <= m_bins_x; ix++)
	{
		m_h1_x->SetBinContent(ix, m_h3contour->GetBinContent(ix, maxY_index, maxZ_index) );
	}
	for(int iy=1; iy <= m_bins_y; iy++)
	{
		m_h1_y->SetBinContent(iy, m_h3contour->GetBinContent(maxX_index, iy, maxZ_index) );
	}
	for(int iz=1; iz <= m_bins_z; iz++)
	{
		m_h1_z->SetBinContent(iz, m_h3contour->GetBinContent(maxX_index, maxY_index, iz) );
	}

	// ========== FILL R histograms
	int icount = 0;
	int icheck = 0;
	// volume of a sphere goes with (4/3)*pi*r^3
	double factor = kPI * m_voxelSizeX * m_voxelSizeY * m_voxelSizeZ;
	for (int icount = 0; icount < ncounts; icount++)
	{
		double x = lameROOTx[icount];
		double y = lameROOTy[icount];
		double z = lameROOTz[icount];

		double delX = x - maxx;			// x, y and z location of a hit, compared with "max x, y, z" ( == target position)
		double delY = y - maxy;
		double delZ = z - maxz;

		double radius = std::sqrt(delX*delX + delY*delY + delZ*delZ);	
		m_h1_racc->Fill(radius);
		if (radius >= raccmin && radius < raccmax)
		{
			icheck++;
		}

		int ir = (int) ( (radius) / rbinsize );	

		double weight = factor;
		double radborderx = (ir + 1) * m_voxelSizeX;
		double radbordery = (ir + 1) * m_voxelSizeY;
		double radborderz = (ir + 1) * m_voxelSizeZ;
		double volume = kPI * radborderx * radbordery * radborderz;
		if (ir > 0)
		{
			radborderx = ir * m_voxelSizeX;
			radbordery = ir * m_voxelSizeY;
			radborderz = ir * m_voxelSizeZ;
			volume = volume - (kPI * radborderx * radbordery * radborderz);
		}
		weight = factor / volume;
			// for first bin, weight == 1

		m_h1_r->Fill(radius, weight); 
	}

cout << "CHECK raccfil: " << icount << " in bounds: " << icheck << endl;


	// -------------------------
	m_c2->cd();	
	
	if (m_doDrawGrid)
	{
		m_c2->SetGridx();
		m_c2->SetGridy();
	}
	
	m_h1_x->Draw("");
	m_h1_x->Write("Xprofile");

	cout << "X profile, Mean: " << m_h1_x->GetMean() <<"; RMS: " << m_h1_x->GetRMS() << endl;
	
	m_c2->Update();
	
	// -------------------------
	m_c3->cd();	
	
	if (m_doDrawGrid)
	{
		m_c3->SetGridx();
		m_c3->SetGridy();
	}
	
	m_h1_y->Draw("");
	m_h1_y->Write("Yprofile");

	cout << "Y profile, Mean: " << m_h1_y->GetMean() <<"; RMS: " << m_h1_y->GetRMS() << endl;
	
	m_c3->Update();

	// -------------------------
	m_c4->cd();	
	
	if (m_doDrawGrid)
	{
		m_c4->SetGridx();
		m_c4->SetGridy();
	}
	
	m_h1_z->Draw("");
	m_h1_z->Write("Zprofile");

	cout << "Z profile, Mean: " << m_h1_z->GetMean() <<"; RMS: " << m_h1_z->GetRMS() << endl;
	
	m_c4->Update();

	// -------------------------
	m_c5->cd();	

	m_h1_r->Draw("");
	m_h1_r->Write("R-profile");

	cout << "R profile, Mean: " << m_h1_r->GetMean() <<"; RMS: " << m_h1_r->GetRMS() << endl;

	std::stringstream strstr;
	int rad0con = m_h1_r->GetBinContent(1);
	int radacc0con = m_h1_racc->GetBinContent(1);
	int restcon = 0;
	int restacccon = 0;
	for(int ir = 2; ir <= hrbins; ir++)
	{     
		restcon += m_h1_r->GetBinContent(ir);
		restacccon += m_h1_racc->GetBinContent(ir);
	}
	TString str = "CTR BIN: ";
	strstr << rad0con;
	str += strstr.str();

	TPaveText* text1 = new TPaveText(0.7, 0.75, 0.95, 0.65, "NDC");
	text1->AddText(str);
	text1->Draw();

	str = "all rest: ";
	strstr.str("");
	strstr << restcon;
	str += strstr.str();

	TPaveText* text2 = new TPaveText(0.7, 0.65, 0.95, 0.55, "NDC");
	text2->AddText(str);
	text2->Draw();

	m_c5->Update();

	// -------------------------
	m_c6->cd();	

	m_h1_racc->Draw("");
	m_h1_racc->Write("RACC-profile");

	str = "CTR BIN: ";
	strstr.str("");
	strstr << radacc0con;
	str += strstr.str();

	TPaveText* text3 = new TPaveText(0.7, 0.75, 0.95, 0.65, "NDC");
	text3->AddText(str);
	text3->Draw();

	str = "all rest: ";
	strstr.str("");
	strstr << restacccon;
	str += strstr.str();

	TPaveText* text4 = new TPaveText(0.7, 0.65, 0.95, 0.55, "NDC");
	text4->AddText(str);
	text4->Draw();

	m_c6->Update();
}

// ================================================================================

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
	
	m_bins_x = 0;
	m_xmin = 0.0;
	m_xmax = 0.0;
	m_bins_y = 0;
	m_ymin = 0.0;
	m_ymax = 0.0;

	double zPosition1 = 0.0; 
	double zPosition2 = 0.0;
	
	while (!conffile.eof())
	{
		conffile >> tmpStr >> tmpVal >> tmpDum;
		if (!conffile.eof())
		{
			if (tmpStr == "m_bins")			// OLD VERSIONS OF SOE (< 2012 02 20)
			{
				m_bins_x = (int) tmpVal;
				m_bins_y = (int) tmpVal;
			}
			else if (tmpStr == "m_bins_x")
			{
				m_bins_x = (int) tmpVal;
			}
			else if (tmpStr == "m_xmin")
			{
				m_xmin = tmpVal;
			}
			else if (tmpStr == "m_xmax")
			{
				m_xmax = tmpVal;
			}
			else if (tmpStr == "m_bins_y")
			{
				m_bins_y = (int) tmpVal;
			}
			else if (tmpStr == "m_ymin")
			{
				m_ymin = tmpVal;
			}
			else if (tmpStr == "m_ymax")
			{
				m_ymax = tmpVal;
			}
			else if (tmpStr == "m_bins_z") 
			{
				m_bins_z = (int) tmpVal;
			}
			else if (tmpStr == "zPosition1")
			{
				zPosition1 = tmpVal;
			}
			else if (tmpStr == "zPosition2")
			{
				zPosition2 = tmpVal;
			}
			else if (tmpStr == "m_iterations")
			{
				m_iterations = (int) tmpVal;
			}
		}
	}
	
	if (m_ymax == m_ymin && m_ymin == 0.0)		// OLD VERSIONS OF SOE (< 2012 02 20)
	{
		m_ymin = m_xmin;
		m_ymax = m_xmax;
	}

	if (m_bins_x > 0)
	{
		m_voxelSizeX = (m_xmax - m_xmin) / m_bins_x;
	}
	if (m_bins_y > 0)
	{
		m_voxelSizeY = (m_ymax - m_ymin) / m_bins_y;
	}

	m_zmin = (zPosition1 < zPosition2) ? zPosition1 : zPosition2;
	m_zmax = (zPosition1 > zPosition2) ? zPosition1 : zPosition2;
	if (m_bins_z > 0)
	{
		m_voxelSizeZ = (m_zmax - m_zmin) / m_bins_z;
	}
	
	if (true)
	{
		cout << "PROGRAM PARAMETERS: " << endl;
		cout << "m_iterations: " << m_iterations << endl;

		cout << "#_bins (x axis): " << m_bins_x << endl;
		cout << "x min: " << m_xmin << ", x max: " << m_xmax << endl;
		cout << "Voxel size X: " << m_voxelSizeX << endl;

		cout << "#_bins (y axis): " << m_bins_y << endl;
		cout << "y min: " << m_ymin << ", y max: " << m_ymax << endl;
		cout << "Voxel size Y: " << m_voxelSizeY << endl;

		cout << "#_bins (z axis): " << m_bins_z << endl;
		cout << "z min: " << m_zmin << ", z max: " << m_zmax << endl;
		cout << "Voxel size: " << m_voxelSizeZ << endl;
		cout << endl;
	}
}

