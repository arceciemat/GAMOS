#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <algorithm>

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

int		m_bins_x;
double	m_xmin;
double	m_xmax;

int		m_bins_y;
double	m_ymin;
double	m_ymax;

int		m_iterations;
double 	m_voxelSizeX;
double 	m_voxelSizeY;

TH1D* m_h1_x = NULL;
TH1D* m_h1_y = NULL;
TH2D* m_h2contour = NULL;

// Special global constants...
bool m_doDrawGrid = 0;
bool m_doSetOptStat = 1;

// rotation matrix
double** m_a = NULL;

const double m_PI = 3.141592653;

void Main() 
{
	m_c1 = new TCanvas("m_c1", "m_c1", 500, 500);
	m_c2 = new TCanvas("m_c2", "m_c2", 500, 500);
	m_c3 = new TCanvas("m_c3", "m_c3", 500, 500);
	m_c4 = new TCanvas("m_c4", "m_c4", 500, 500);

m_c1->Update();
m_c2->Update();
m_c3->Update();
m_c4->Update();
	
	// ------------
	gROOT->SetStyle("Plain");  
	gStyle->SetOptStat(m_doSetOptStat);  
	gStyle->SetPalette(1); 

	TFile* myhistos = new TFile("img_hist_SOE.root","RECREATE");
	
	// ------------------------------------
	std::string fn = "soe_results.dat";
	datfile.open(fn.c_str(), ios::in);
	if (!datfile.is_open())
	{
		cout << "FILE NOT FOUND:" << fn << endl;
		return;
	}
	
	// Get configuration parameters used by SOE
	ReadConfigurationParameters();

	// Locate center point in center of bin (more or less)
	double realsourcex = 0.0;
	double realsourcey = 0.0;

	int sourceposknown = 0;
	cout << "Real position of source known (1/0)?" << endl;
	cin >> sourceposknown;
	if (sourceposknown) 
	{
		cout << "Give real source position, in x and y (if known, else, give 0.0, 0.0)" << endl;
		cin >> realsourcex;	
		cin >> realsourcey;	
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

	//******** 2D IMAGE *******	
	m_h2contour = new TH2D("m_h2contour", "", hxbins, hxmin, hxmax, hybins, hymin, hymax);
	
	double x, y, z;
	double x, y, z;
	std::vector<double> lameROOTx;
	std::vector<double> lameROOTy;
	int ncounts = 0;
	while (!datfile.eof())
	{
		datfile >> x >> y >> z;
		if (!datfile.eof())
		{
			m_h2contour->Fill(x, y);
			lameROOTx.push_back(x);
			lameROOTy.push_back(y);
			ncounts++;
		}
	}
	datfile.close();

	// get min and max index...
	// stupid ROOT starts counting bins from 1 ( bin0 = underflow )
	int maxContent = 0.0; 
	int maxX_index = 0, maxY_index = 0; 
	double maxx, maxy;
	double binsizex = (hxmax - hxmin) / hxbins; 
	double binsizey = (hymax - hymin) / hybins; 
	for(int iy = 1; iy <= m_bins_y; iy++)
	{	 
		for(int ix = 1; ix <= m_bins_x; ix++)
		{     
			double cont = m_h2contour->GetBinContent(ix, iy);
			if (cont > maxContent)
			{
				maxContent = cont;
				maxX_index = ix;       
				maxY_index = iy;     

				maxx = hxmin + (ix - 1) * binsizex + 0.5 * binsizex;
				maxy = hymin + (iy - 1) * binsizey + 0.5 * binsizey;
			}
		}
	}
	cout << "MAX points: "  
		 << maxX_index << ": " << maxx << " "
		 << maxY_index << ": " << maxy << endl;

	if (sourceposknown) 
	{
		maxX_index = 1 + ((realsourcex - hxmin) / binsizex);
		maxY_index = 1 + ((realsourcey - hymin) / binsizey);
		cout << "REAL CENTER points: "  
		 	<< maxX_index << ": " << realsourcex << " "
		 	<< maxY_index << ": " << realsourcey << endl;

		maxx = realsourcex;
		maxy = realsourcey;
	}

	m_c1->cd();	
	
	m_h2contour->SetMinimum(-1); 
	m_h2contour->GetXaxis()->SetTitle("X [mm]");
	m_h2contour->GetXaxis()->CenterTitle();
	m_h2contour->GetYaxis()->SetTitle("Y [mm]");
	m_h2contour->GetYaxis()->CenterTitle();
	m_h2contour->SetContour(10); 
	m_h2contour->Draw("CONTZ");   
	
	m_h2contour->Write("ImageXY"); 
	
	m_c1->Update();
	
	// ****** X AND Y and DIAG PROFILE LINE PLOTS ******
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

	// R PROFILE PLOTs -----------------------------------------
	double hrmin = 0;
	double hrmax = m_xmax;
	int hrbins = (int) ( (hrmax - hrmin) / m_voxelSizeX ); 
	double rbinsize = m_voxelSizeX;

	m_h1_r = new TH1D("m_h1_r", "", hrbins, hrmin, hrmax);

	m_h1_r->GetXaxis()->SetTitle("R [mm]");
	m_h1_r->GetXaxis()->CenterTitle();   
	m_h1_r->GetYaxis()->SetTitle("Intensity");   
	m_h1_r->GetYaxis()->CenterTitle();
	
	// Fill m_h1_x and m_h1_y
	for (int i=1; i <= m_bins_x; i++)
	{
		m_h1_x->SetBinContent(i, m_h2contour->GetBinContent(i, maxY_index) );
	}
	for (int i=1; i <= m_bins_y; i++)
	{
		m_h1_y->SetBinContent(i, m_h2contour->GetBinContent(maxX_index, i) );
	}

	// ========== R histogram
	int icount = 0;
	double ircount[6];
	for (int i = 0; i <= 5; i++)
		ircount[i] = 0;
	// area of a cirle goes with pi*r^2
	double factor = kPI * m_voxelSizeX * m_voxelSizeY;  
	for (int icount = 0; icount < ncounts; icount++)
	{
		double x = lameROOTx[icount];
		double y = lameROOTy[icount];

		double delX = x - maxx;
		double delY = y - maxy;

		double radius = std::sqrt(delX*delX + delY*delY);	

		int ir = (int) ( (radius) / rbinsize );		// bin numbering starting from centre (first binnr = 0)
		
		// normalize #counts with spatial area corresponding to this radius
		// area of a cirle goes with pi*r^2
		double weight = factor;
		double radborderX = (ir + 1) * m_voxelSizeX;
		double radborderY = (ir + 1) * m_voxelSizeY;
		double area = kPI * radborderX * radborderY;
		if (ir > 0)
		{
			radborderX = ir * m_voxelSizeX;
			radborderY = ir * m_voxelSizeY;
			area = area - (kPI * radborderX * radborderY);
		}
		weight = factor / area;
			// for first bin, weight == 1

		m_h1_r->Fill(radius, weight);
		if (ir <= 5 && ir >= 0) ircount[ir] += weight;
	}

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

	m_h1_r->Draw("");
	m_h1_r->Write("R-profile");

	cout << "R profile, Mean: " << m_h1_r->GetMean() <<"; RMS: " << m_h1_r->GetRMS() << endl;
	
	m_c4->Update();

	// -------------------------
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
	
	m_bins_x = 0;
	m_xmin = 0.0;
	m_xmin = 0.0;
	m_bins_y = 0;
	m_ymin = 0.0;
	m_ymin = 0.0;
	
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

		cout << endl;
	}
}

// =================================================================

CreateRotMatrixZAxis( const double& in_angle )
{
	int dim = 2;
	m_a = new double* [dim];
	for (int i=0; i < dim; i++)
		m_a[i] = new double [dim];

	for (int i=0; i < dim; i++)
		for (int j=0; j < dim; j++)
			m_a[j][i] = 0.0;

	double sinAngle = sin(in_angle);
	double cosAngle = cos(in_angle);

	// else if (in_axis == Axis_Z)
	{
		m_a[0][0] = cosAngle;  	m_a[1][0] = - sinAngle;
		m_a[0][1] = sinAngle;  	m_a[1][1] = cosAngle; 	
	}
}

RotateXandY(double& io_x, double& io_y) 
{
	assert (m_a != NULL);

	double tmp_x = m_a[0][0]*io_x + m_a[1][0]*io_y;
	double tmp_y = m_a[0][1]*io_x + m_a[1][1]*io_y;

	io_x = tmp_x;
	io_y = tmp_y;
}


void LineProfileX(int NBinX)
{
	if (myhistos == NULL)
	{ 
		myhistos =  new TFile("img_hist_SOE.root","READ");  
		cout << endl << "*** img_hist_SOE.root opened ***" << endl;
	} 
  
	ReadConfigurationParameters();
	
	TH2D* H2DforXLine = ImageXY;
	TH1D* XLineH = NULL;
	if (doCenterZeroBinX)
		XLineH = new TH1D("XLineH", "", m_bins+1, m_xmin-0.5, m_xmax+0.5); 
	else
		XLineH = new TH1D("XLineH", "", m_bins, m_xmin, m_xmax); 
	
	double Xpos = XLineH->GetBinCenter(NBinX);
	
	TLine* myXLine = new TLine(-5,Xpos,6,Xpos);
	
	TCanvas* c2DwLineX = new TCanvas("c2DwLineX","c2DwLineX", 500, 500);
	
	gROOT->SetStyle("Plain");  
	gStyle->SetOptStat(0);  
	gStyle->SetPalette(1);
	gStyle->SetErrorX(0);
	H2DforXLine->GetXaxis()->SetRangeUser(-5,5);  
	H2DforXLine->GetYaxis()->SetRangeUser(-5,5);
	H2DforXLine->Draw("CONTZ");
	myXLine->SetLineStyle(9);
	myXLine->SetLineWidth(3);
	myXLine->Draw();
		
	TCanvas* cXprofile = new TCanvas("cXprofile","cXprofile", 500, 500);
	
	for (int i=0;i<m_bins;i++)
	{
		XLineH->SetBinContent(i,H2DforXLine->GetBinContent(i,NBinX));
	}
	
	if (m_doDrawGrid)
	{
		cXprofile->SetGridx();
		cXprofile->SetGridy();
	}
	
	XLineH->SetMarkerStyle(21); 
	gStyle->SetErrorX(0);
	XLineH->GetXaxis()->SetTitle("X [mm]");
	XLineH->GetXaxis()->CenterTitle();
	XLineH->GetXaxis()->SetRangeUser(-5,5);
	XLineH->GetYaxis()->SetTitle("Intensity"); 
	XLineH->GetYaxis()->SetTitleOffset(1.5);
	XLineH->GetYaxis()->CenterTitle();
	XLineH->Draw("lp");
	
	TCanvas* cLineProfX = new TCanvas("cLineProfX","cLineProfX",1000,500);
	
	cLineProfX->Divide(2,1);
	
	cLineProfX->cd(1);
	if (m_doDrawGrid)
	{
		cLineProfX->cd(1)->SetGridx();
		cLineProfX->cd(1)->SetGridy();  
	}
	
	H2DforXLine->Draw("CONTZ");
	myXLine->Draw();
		
	cLineProfX->cd(2);
	if (m_doDrawGrid)
	{
		cLineProfX->cd(2)->SetGridx();
		cLineProfX->cd(2)->SetGridy();  
	}
	
	XLineH->Draw("lp");
}

void LineProfileY (int NBinY)
{
	if	(myhistos == NULL)
	{ 
		myhistos =  new TFile("img_hist_SOE.root","READ");  
		cout << endl << "*** img_hist_SOE.root opened ***" << endl;
	} 
	
	ReadConfigurationParameters();
	
	TH2D* H2DforYLine = ImageXY;
	TH1D* YLineH = NULL;
	if (doCenterZeroBinY)
		YLineH = new TH1D("YLineH", "Line profile - Y axis", m_bins+1, m_xmin-0.5, m_xmax+0.5); 
	else
		YLineH = new TH1D("YLineH", "Line profile - Y axis", m_bins, m_xmin, m_xmax); 
	
	double Ypos = YLineH->GetBinCenter(NBinY);
	
	TLine* myYLine = new TLine(Ypos,m_xmin,Ypos,m_xmax);
	
	TCanvas* c2DwLineY = new TCanvas("c2DwLineY","c2DwLineY", 500, 500);
	
	gStyle->SetPalette(1);  
	H2DforYLine->Draw("CONTZ");
	myYLine->Draw();
	
	TCanvas* cYprofile = new TCanvas("cYprofile","cYprofile", 500, 500);
	
	if (m_doDrawGrid)
	{
		cYprofile->SetGridx();
		cYprofile->SetGridy();
	}
	
	for(int i=0;i<m_bins;i++)
	{
		YLineH->SetBinContent(i,H2DforYLine->GetBinContent(NBinY,i));
	}
	
	YLineH->GetXaxis()->SetTitle("Y [mm]");
	YLineH->GetXaxis()->CenterTitle();
	YLineH->GetXaxis()->SetRangeUser(-11,10);
	YLineH->GetYaxis()->SetTitle("Intensity"); 
	YLineH->GetYaxis()->CenterTitle();
	YLineH->Draw("e");
	
	TCanvas* cLineProfY = new TCanvas("cLineProfY","cLineProfY",1000,500);
	
	cLineProfY->Divide(2,1);
	
	cLineProfY->cd(1);
	if (m_doDrawGrid)
	{
		cLineProfY->cd(1)->SetGridx();
		cLineProfY->cd(1)->SetGridy();  
	}
	
	H2DforYLine->Draw("CONTZ");
	myYLine->Draw();
	
	cLineProfY->cd(2);
	if (m_doDrawGrid)
	{
		cLineProfY->cd(2)->SetGridx();
		cLineProfY->cd(2)->SetGridy();  
	}
	
	YLineH->Draw("e");
}

void Convergence()
{  
	TCanvas* cSOE = new TCanvas("SOE Convergence","SOE Convergence",700,700);	
	cSOE->Divide(1, 2);
	
	std::ifstream dfile("convergence.dat");
	if (!dfile.is_open())
	{
		cout << "File convergence.dat NOT open" << endl;
		exit(1);
	}
	
	ReadConfigurationParameters();
	
	TH1D* h1 = new TH1D("h1", "vector diff", m_iterations, 0., m_iterations);
	TH1D* h2 = new TH1D("h2", "num of diff events", m_iterations, 0., m_iterations);
	
	int iter;
	double avedist;
	int nchevts;
	while ( !dfile.eof())
	{
		dfile >> iter >> avedist >> nchevts;
		if ( !dfile.eof())
		{
			h1->Fill(iter, avedist);
			h2->Fill(iter, nchevts);
		}
	}
	
	h1->SetMarkerSize(2);
	h1->SetMarkerStyle(7);
	
	cSOE->cd(1);
	gPad->SetLogy();
	h1->Draw("P");
	
	cSOE->cd(2);
	gPad->SetLogy();
	h2->Draw("P");
	
	cSOE->Print("convergence.pdf");
}


void myOpen()
{
	myHfile =  new TFile("img_hist_SOE.root","READ");  
	cout << endl << "*** img_hist_SOE.root opened ***" << endl;
}

void myGet(TString myHname)
{
	if(myHfile == NULL)
	{ 
		myHfile =  new TFile("img_hist_SOE.root","READ");  
		cout << endl << "*** img_hist_SOE.root opened ***" << endl;
	}
	myH = (TH1D*) myHfile->Get(myHname);
	cout << endl << "**************************************"; 
	cout << endl << "Histogram: " << myHname << " saved on pointer: TH1D* myH" << endl;
}


// FIT FUNCTION
Double_t parabolic(Double_t *x, Double_t *par)
{
	//cout << "x max pos: " << par[1]/(-2.0*par[0]) << endl;
	return par[0]*x[0]*x[0] + par[1]*x[0] + par[2];
}

double FWTM(int rebin)
{
	DoFullWidthXMax( rebin, 10 );
}

double FWHM(int rebin)
{
	DoFullWidthXMax( rebin, 2 );
}

double DoFullWidthXMax( int rebin, int maxFactor )
{
	bool getProj = true;
	std::string projCh;
	cout << "X or Y projection (Point: x/y ; Line: XL/YL)?" << endl;
	delete myH; myH = NULL;
	while (getProj) 
	{
		cin >> projCh;
		if (projCh == "x") 
		{
			myH = (TH1D*) m_h1_x->Clone("myH");
			getProj = false;
		}
		else if (projCh == "y") 
		{
			myH = (TH1D*) m_h1_y->Clone("myH");
			getProj = false;
		}
		else if (projCh == "XL") 
		{
			myH = (TH1D*) myXLine->Clone("myH");
			getProj = false;
		}
		else if (projCh == "YL") 
		{
			myH = (TH1D*) myYLine->Clone("myH");
			getProj = false;
		}
		else
			cout << "illegal character" << endl;
	}

	if (myH == NULL)
	{
		cout << "TH1D* myH is void! " << endl;		
		return;
	}

	if (maxFactor == 0)
	{
		cout << "Illegal factor for FW Maximum: " << maxFactor << endl;
		return;
	}
	
	myH->Rebin(rebin);
	
	double RMS = myH->GetRMS();
	int NumOfBins = myH->GetNbinsX();
	double BinWidth = (double) myH->GetBinWidth(NumOfBins);

	double lowx = (double) myH->GetBinLowEdge(1);
	double highx = (double) myH->GetBinLowEdge(NumOfBins)+BinWidth;
	
	double maximum = 0;
	double maximum_x = 0;
	int maximum_index = 0;

	for(int i=1; i <= NumOfBins; i++)
	{
		if(myH->GetBinContent(i) > maximum)
		{
			maximum = myH ->GetBinContent(i);
			maximum_x = myH ->GetBinCenter(i);
			maximum_index = i;
		}	
	} 

	cout << "maximum x: " << maximum_x << ", value: " << maximum << ", index: " << maximum_index << endl;
	cout << "Number of bins: " << NumOfBins << ", Bin width: " << BinWidth << endl;
  
	double bincenter_left = myH->GetBinCenter(maximum_index-1);
	double bincenter_mid  = myH->GetBinCenter(maximum_index);
	double bincenter_right = myH->GetBinCenter(maximum_index+1);

	int bincontents_left = myH->GetBinContent(maximum_index-1);
	int bincontents_mid  = myH->GetBinContent(maximum_index);
	int bincontents_right = myH->GetBinContent(maximum_index+1);

	lowx = std::max(lowx, bincenter_left - 20.0);
	highx = std::min(highx, bincenter_right + 20.0);

	// cout << "low X: " << lowx << endl;
	// cout << "high X: " << highx << endl;

	TH1D *temporal = new TH1D("temporal", "temporal",NumOfBins, lowx, highx);

	cout << "fit histo: bins: " << bincenter_left << " " << bincenter_mid << " " << bincenter_right << endl;
	cout << "fit histo: contents: " << bincontents_left << " " << bincontents_mid << " " << bincontents_right << endl;

	temporal->Fill(bincenter_left, bincontents_left);
	temporal->Fill(bincenter_mid, bincontents_mid);
	temporal->Fill(bincenter_right, bincontents_right);
	
	// cout << "temporal entries = " << temporal->GetEntries() << endl;
	TCanvas *c1 = new TCanvas();
	temporal->Draw();

	cout << "parabole min and max binvalues: " << lowx << " " << highx << endl;
	
	TF1 *func3 = new TF1("tempfit",parabolic, lowx, highx, 3);
		// function "parabolic" does: par[0]*x[0]*x[0] + par[1]*x[0] + par[2];
	func3->SetParameters(-500,1,-500);
	// func3->SetParameters(1,1, maximum);
	// func3->SetParLimits(2, 0.0, 2*maximum);

	temporal->Fit(func3,"","", -1000, 1000);
	// temporal->Fit(func3,"","", lowx, highx);
	
	double functionmaximum = -pow(func3->GetParameter(1),2)/(4*func3->GetParameter(0))+(func3->GetParameter(2));

	double fractionmaximum = functionmaximum / maxFactor ;

	cout << "max from parabolic fit = " << functionmaximum << endl;
	if (maxFactor == 2)
		cout << "half max = " << fractionmaximum << endl;
	else if (maxFactor == 10)
		cout << "tenth max = " << fractionmaximum << endl;
	else
		cout << maxFactor << "th max = " << fractionmaximum << endl;

	double x1up = 0;
	double x1down = 0;
	double x2up = 0;
	double x2down = 0;
	double x1up_index = 0;
	double x1down_index = 0;
	double x1up_index_plus = 0;
	double x1down_index_plus = 0;
	double x1up_index_minus = 0;
	double x1down_index_minus = 0;
	double x2up_index = 0;
	double x2down_index = 0;  
	double x2up_index_plus = 0;
	double x2down_index_plus = 0;
	double x2up_index_minus = 0;
	double x2down_index_minus = 0;

	for(int i=maximum_index;i>0;i--)
	{
		if (myH->GetBinContent(i) < fractionmaximum)
		{
			x1up = myH->GetBinContent(i+1);
			x1down = myH ->GetBinContent(i);
			x1up_index = myH->GetBinCenter(i+1);
			x1down_index = myH->GetBinCenter(i);
			x1up_index_plus = myH->GetBinLowEdge(i+2);
			x1down_index_plus = myH->GetBinLowEdge(i+1);
			x1up_index_minus = myH->GetBinLowEdge(i+1);
			x1down_index_minus = myH->GetBinLowEdge(i);
			break;
		}
	} 

	for( int i=maximum_index;i<NumOfBins;i++)
	{
		if(myH->GetBinContent(i) < fractionmaximum)
		{
			x2up = myH->GetBinContent(i-1);
			x2down = myH ->GetBinContent(i);
			x2up_index = myH->GetBinCenter(i-1);
			x2down_index = myH->GetBinCenter(i);
			x2up_index_plus = myH->GetBinLowEdge(i);
			x2down_index_plus = myH->GetBinLowEdge(i+1);
			x2up_index_minus = myH->GetBinLowEdge(i-1);
			x2down_index_minus = myH->GetBinLowEdge(i);
			break;
		}
	}

	cout << "x1up = " << x1up << "; x1down = " << x1down << "; x1up_index = " << x1up_index << "; x1down_index = " << x1down_index << endl;
	cout << "x1up_index_plus = " << x1up_index_plus << "; x1down_index_plus = " << x1down_index_plus << endl;
	cout << "x1up_index_minus = " << x1up_index_minus << "; x1down_index_minus = " << x1down_index_minus << endl;

	cout << "x2up = " << x2up << "; x2down = " << x2down << "; x2up_index = " << x2up_index << "; x2down_index = " << x2down_index << endl;
	cout << "x2up_index_plus = " << x2up_index_plus << "; x2down_index_plus = " << x2down_index_plus << endl;
	cout << "x2up_index_minus = " << x2up_index_minus << "; x2down_index_minus = " << x2down_index_minus << endl;
	
	TLine* line1 = new TLine(x1down_index,x1down,x1up_index,x1up);
	TLine* line1_plus = new TLine(x1down_index_plus,x1down,x1up_index_plus,x1up);
	TLine* line1_minus = new TLine(x1down_index_minus,x1down,x1up_index_minus,x1up);
	TLine* line2 = new TLine(x2down_index,x2down,x2up_index,x2up);
	TLine* line2_plus = new TLine(x2down_index_plus,x2down,x2up_index_plus,x2up);
	TLine* line2_minus = new TLine(x2down_index_minus,x2down,x2up_index_minus,x2up);
	
	double slope1 =(x1up - x1down)/ (x1up_index - x1down_index);
	double origin1 = x1down - (slope1*x1down_index);
	double slope1_plus =(x1up - x1down)/ (x1up_index_plus - x1down_index_plus);
	double origin1_plus = x1down - (slope1_plus*x1down_index_plus);
	double slope1_minus =(x1up - x1down)/ (x1up_index_minus - x1down_index_minus);
	double origin1_minus = x1down - (slope1_minus*x1down_index_minus);
	double slope2 =(x2up - x2down)/(x2up_index - x2down_index);
	double origin2 = x2down - (slope2*x2down_index);
	double slope2_plus =(x2up - x2down)/ (x2up_index_plus - x2down_index_plus);
	double origin2_plus = x2down - (slope2_plus*x2down_index_plus);
	double slope2_minus =(x2up - x2down)/ (x2up_index_minus - x2down_index_minus);
	double origin2_minus = x2down - (slope2_minus*x2down_index_minus);
	
	cout << " slope1 = " << slope1 << "; origin1 = " << origin1 << "; slope1_plus = " << slope1_plus << "; origin1_plus = " << origin1_plus << endl; 
	cout << " slope1_minus = " << slope1_minus << "; origin1_minus = " << origin1_minus << endl;
	cout << " slope2 = " << slope2 << "; origin2 = " << origin2 << "; slope2_plus = " << slope2_plus << "; origin2_plus = " << origin2_plus << endl; 
	cout << " slope2_minus = " << slope2_minus << "; origin2_minus = " << origin2_minus << endl; 
	
	double x1 = (-1/slope1)*((-fractionmaximum)+origin1);
	double x1_plus = (-1/slope1_plus)*((-fractionmaximum)+origin1_plus);
	double x1_minus = (-1/slope1_minus)*((-fractionmaximum)+origin1_minus);
	double x2 = (-1/slope2)*((-fractionmaximum)+origin2);
	double x2_plus = (-1/slope2_plus)*((-fractionmaximum)+origin2_plus);
	double x2_minus = (-1/slope2_minus)*((-fractionmaximum)+origin2_minus);
	TArrow* line3 = new TArrow(x1,fractionmaximum,x2,fractionmaximum, 0.01, "<|>");
	TLine* line3_max = new TLine(x1_minus,fractionmaximum,x2_plus,fractionmaximum);
	TLine* line3_min = new TLine(x1_plus,fractionmaximum,x2_minus,fractionmaximum);

	cout << "FWHM Line points:" << "; x1: " << x1 << "; x2: " << x2 << endl;
		
	double fullwidthfraction = (x2 - x1);
	double fullwidthfraction_max = (x2_plus - x1_minus);
	double fullwidthfraction_min = (x2_minus - x1_plus);
	
	gROOT->SetStyle("Plain");  
	
	TCanvas* cfwhm = new TCanvas("cfwhm","",500,500);

	myH->GetXaxis()->SetRangeUser(-5,5);
	myH->GetYaxis()->SetTitle("Arbitrary Units");
	myH->GetXaxis()->SetTitle("X [mm]");
	myH->GetXaxis()->SetTitleOffset(1.2);
	myH->SetLineWidth(3);
	myH->SetLineStyle(9);
	myH->SetMarkerColor(46);
	myH->Draw();

	line1->SetLineWidth(2);
	line1->SetLineColor(kRed);

	line2->SetLineWidth(2);
	line2->SetLineColor(kRed);

	line3->SetLineWidth(2);
	line3->SetLineColor(kBlue);
	line3->SetFillColor(kBlue);
	line3->Draw();

	line1_plus->SetLineWidth(1);
	line1_plus->SetLineColor(kRed+2);

	line2_plus->SetLineWidth(1);
	line2_plus->SetLineColor(kRed+2);

	line3_max->SetLineWidth(1);
	line3_max->SetLineColor(kBlue+2);

	line1_minus->SetLineWidth(1);
	line1_minus->SetLineColor(kRed-2);

	line2_minus->SetLineWidth(1);
	line2_minus->SetLineColor(kRed-2);

	line3_min->SetLineWidth(1);
	line3_min->SetLineColor(kBlue-2);

	cout << "RMS: " << RMS << endl;
	cout << "FWHM: " << fullwidthfraction << "; FWHM_max: " << fullwidthfraction_max << "; FWHM_min: " << fullwidthfraction_min << endl;
	cout << "Maximum peak value: " << functionmaximum << endl;
	if (maxFactor == 2)
		cout << "Half Max: " << fractionmaximum << endl;
	else if (maxFactor == 10)
		cout << "Tenth Max: " << fractionmaximum << endl;
	else
		cout << maxFactor << "th Max: " << fractionmaximum << endl;	

	return fullwidthfraction;
}

