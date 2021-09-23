
std::ifstream datfile;

TCanvas* m_c1;

int		m_bins;
double	m_xmin;
double	m_xmax;
int		m_iterations;
double 	m_voxelSize;

TFile* myhistos = NULL;

void Main() 
{
	// ------------
	gROOT->SetStyle("Plain");  
	gStyle->SetOptStat(0);  
	gStyle->SetPalette(1); 

	myhistos = new TFile("img_hist_SOE.root","RECREATE");

	// ------------------------------------
	std::string fn = "soe_results.dat";
	datfile.open(fn.c_str(), ios::in);
	if (!datfile.is_open())
	{
		cout << "FILE NOT FOUND:" << fn << endl;
		return;
	}

	ReadConfigurationParameters();

	//******** 2D IMAGE *******

	TH2D* h2contour = new TH2D("h2contour", "", m_bins, m_xmin, m_xmax, m_bins, m_xmin, m_xmax);

	double x, y, z;
	while (!datfile.eof())
	{
		datfile >> x >> y >> z;
		if (!datfile.eof())
		{
			h2contour->Fill(x, y);
		}
	}

	// get min and max index...
	// stupid ROOT starts counting bins from 1 ( bin0 = underflow )
	int maxContent = 0.0; 
	int maxX_index = 0, maxY_index = 0;  
	for(int jy = 1; jy <= m_bins; jy++)
	{	 
		for(int ix = 1; ix <= m_bins; ix++)
		{     
			double cont = h2contour->GetBinContent(ix, jy);
			if (cont > maxContent)
			{
				maxContent = cont;
				maxX_index = ix;       
				maxY_index = jy;     
			}
		}
	}

	m_c1 = new TCanvas("m_c1", "m_c1", 900, 600);
	m_c2 = new TCanvas("m_c2", "m_c2", 500, 500);
	m_c3 = new TCanvas("m_c3", "m_c3", 500, 500);

	m_c1->cd();	

	h2contour->SetMinimum(-1); 
	h2contour->GetXaxis()->SetTitle("X [mm]");
	h2contour->GetXaxis()->CenterTitle();
	h2contour->GetYaxis()->SetTitle("Y [mm]");
	h2contour->GetYaxis()->CenterTitle();
	h2contour->SetContour(10); 
	h2contour->Draw("CONTZ");   
	
	h2contour->Write("ImageXY"); 
	
	m_c1->Update();

	// ****** X AND Y PROFILE LINE PLOTS ******
	
	TH1D* h1_x = new TH1D("h1_x", "", m_bins, m_xmin, m_xmax);
	h1_x->GetXaxis()->SetTitle("X [mm]");
	h1_x->GetXaxis()->CenterTitle();
	h1_x->GetYaxis()->SetTitle("Intensity"); 
	h1_x->GetYaxis()->CenterTitle();
	
	
	TH1D* h1_y = new TH1D("h1_y", "", m_bins, m_xmin, m_xmax);
	h1_y->GetXaxis()->SetTitle("Y [mm]");
	h1_y->GetXaxis()->CenterTitle();   
	h1_y->GetYaxis()->SetTitle("Intensity");   
	h1_y->GetYaxis()->CenterTitle();
	
	// Fill h1_x and h1_y

	for(int i=1; i <= m_bins; i++)
	{
		h1_x->SetBinContent(i, h2contour->GetBinContent(i, maxY_index) );
		h1_y->SetBinContent(i, h2contour->GetBinContent(maxX_index, i) );
	}

	// -------------------------
	m_c2->cd();	
	
	// m_c2->SetGridx();
	// m_c2->SetGridy();

	cout << "Center Bin: " << (int) m_bins/2. << endl;
	
	h1_x->Draw();
	h1_x->Write("Xprofile");
	
	m_c2->Update();

	// -------------------------
	m_c3->cd();	
	
	// m_c3->SetGridx();
	// m_c3->SetGridy();
	
	h1_y->Draw();
	h1_y->Write("Yprofile");
	
	m_c3->Update();

	// -------------------------------
	m_c1->Print("Soe_compton.pdf");
}

void 
ReadConfigurationParameters()
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
