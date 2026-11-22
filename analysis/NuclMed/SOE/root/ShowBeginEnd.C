
std::ifstream beginfile;
std::ifstream endfile;

TCanvas* m_c1;

int		m_bins;
double	m_xmin;
double	m_xmax;
int		m_iterations;
double 	m_voxelSize;

/*
*  Compares the random positions at the beginning and at the end of the run.
*/

void
Main() 
{
	ReadConfigurationParameters();

	std::string beginfn = "data_begin.dat";
	beginfile.open(beginfn.c_str(), ios::in);
	if (!beginfile.is_open())
	{
		cout << "FILE NOT FOUND:" << beginfn << endl;
		return;
	}

	std::string endfn = "data_end.dat";
	endfile.open(endfn.c_str(), ios::in);
	if (!endfile.is_open())
	{
		cout << "FILE NOT FOUND:" << endfn << endl;
		return;
	}

	TH2D* h2begin = new TH2D("h2begin", "begin data", m_bins, m_xmin, m_xmax, m_bins, m_xmin, m_xmax );
	TH1D* h1begin_x = new TH1D("h1begin_x", "begin x profile", m_bins, m_xmin, m_xmax );
	TH1D* h1begin_y = new TH1D("h1begin_y", "begin y profile", m_bins, m_xmin, m_xmax );

	TH2D* h2end = new TH2D("h2end", "end data", m_bins, m_xmin, m_xmax, m_bins, m_xmin, m_xmax );
	TH1D* h1end_x = new TH1D("h1end_x", "end x profile", m_bins, m_xmin, m_xmax );
	TH1D* h1end_y = new TH1D("h1end_y", "end y profile", m_bins, m_xmin, m_xmax );

	double x, y, z;
	while (!beginfile.eof())
	{
		beginfile >> x >> y >> z;
		if (!beginfile.eof())
		{
			h2begin->Fill(x, y);
			h1begin_x->Fill(x);
			h1begin_y->Fill(y);
		}
	}
	
	while (!endfile.eof())
	{
		endfile >> x >> y >> z;
		if (!endfile.eof())
		{
			h2end->Fill(x, y);
			h1end_x->Fill(x);
			h1end_y->Fill(y);
		}
	}
	
	m_c1 = new TCanvas("m_c1", "m_c1", 700, 700);
	m_c1->Divide(3, 2);

	m_c1->cd(1);	h2begin->Draw();
	m_c1->cd(2);	h1begin_x->Draw();
	m_c1->cd(3);	h1begin_y->Draw();
	
	m_c1->cd(4);	h2end->Draw();
	m_c1->cd(5);	h1end_x->Draw();
	m_c1->cd(6);	h1end_y->Draw();

	// --------------------
	m_c1->Print("plot_Show.pdf");
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
