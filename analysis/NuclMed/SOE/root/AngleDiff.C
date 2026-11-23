TCanvas* m_c0 = NULL;
TCanvas* m_c1 = NULL;
TCanvas* m_c2 = NULL;

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

const double kPI = 3.14159265;
const double mass_electron = 510.99907;

// -------------------------------------

void Main()
{
	m_c0 = new TCanvas("m_c0", "m_c0", 1000, 500);
	m_c1 = new TCanvas("m_c1", "m_c1", 800, 600);
	m_c2 = new TCanvas("m_c2", "m_c2", 800, 600);

	std::ifstream infile("rootfile.dat");
	if (!infile.is_open())
	{
		cout << "NOT OPEN!" << endl;
		exit(1);
	}

	// Get configuration parameters used by SOE
	ReadConfigurationParameters();

	double hrmin = 0;
	double hrmax = m_xmax;
	int hrbins = (int) ( (hrmax - hrmin) / m_voxelSizeX ); 
	double rbinsize = m_voxelSizeX;

	// histoos

	TH1D* hAngleDiff = new TH1D("hAngleDiff", "hAngleDiff", 101, -2.0, 2.0);
	TH1D* hNearest = new TH1D("hNearest", "hNearest", hrbins, hrmin, hrmax);
	TH1D* hVoxDist = new TH1D("hVoxDist", "hVoxDist", 101, -0.05, 10.05);

	TH2D* hNearestVsAngle = new TH2D("hNearestVsAngle", "hNearestVsAngle", 100, 0.0, 120.0, hrbins, hrmin, hrmax);
	TH2D* hNearestVsEscat = new TH2D("hNearestVsEscat", "hNearestVsEscat", 100, 0.0, 350.0, hrbins, hrmin, hrmax);
	TH2D* hAngleDiffVsAngle = new TH2D("hAngleDiffVsAngle", "hAngleDiffVsAngle", 100, 0.0, 120.0, 101, -1.0, 1.0);

	// -----
	TH1D* hAngle = new TH1D("hAngle", "hAngle", 100, 0.0, 140.0);
	TH1D* hEscat = new TH1D("hEscat", "hEscat", 100, 0.0, 350.0);
	TH2D* hAngleDiffVsEscat = new TH2D("hAngleDiffVsEscat", "hAngleDiffVsEscat", 100, 0.0, 350.0, 101, -2.0, 2.0);

	TH1D* hAngleDiff_The1 = new TH1D("hAngleDiff_The1", "hAngleDiff_The1", 101, -1.0, 1.0);
	TH1D* hAngleDiff_The2 = new TH1D("hAngleDiff_The2", "hAngleDiff_The2", 101, -1.0, 1.0);
	TH1D* hAngleDiff_The3 = new TH1D("hAngleDiff_The3", "hAngleDiff_The3", 101, -1.0, 1.0);
	TH1D* hAngleDiff_The4 = new TH1D("hAngleDiff_The4", "hAngleDiff_The4", 101, -1.0, 1.0);

	TH1D* hAngle1 = new TH1D("hAngle1", "hAngle1", 10, 10.0, 40.0);
	TH1D* hAngle2 = new TH1D("hAngle2", "hAngle2", 10, 40.0, 60.0);
	TH1D* hAngle3 = new TH1D("hAngle3", "hAngle3", 10, 60.0, 80.0);
	TH1D* hAngle4 = new TH1D("hAngle4", "hAngle4", 10, 80.0, 120.0);
	
	double hit1Pos_X, hit1Pos_Y, hit1Pos_Z, hit1_E;
	double hit2Pos_X, hit2Pos_Y, hit2Pos_Z, hit2_E;

	double comptonAngle, thetaReal; 
	double realPos_X, realPos_Y, realPos_Z;
	double voxelReal_X, voxelReal_Y, voxelReal_Z;
	double nearestPos_X, nearestPos_Y, nearestPos_Z;
	double voxelNearest_X, voxelNearest_Y, voxelNearest_Z;

	while (!infile.eof())
	{
		infile >> hit1Pos_X >> hit1Pos_Y >> hit1Pos_Z >> hit1_E
			   >> hit2Pos_X >> hit2Pos_Y >> hit2Pos_Z >> hit2_E
			   >> comptonAngle >> thetaReal 
			   >> realPos_X >> realPos_Y >> realPos_Z 
			   >> voxelReal_X >> voxelReal_Y >> voxelReal_Z
			   >> nearestPos_X >> nearestPos_Y >> nearestPos_Z 
			   >> voxelNearest_X >> voxelNearest_Y >> voxelNearest_Z;

		if (!infile.eof())
		{
			double nearestDist = sqrt(   (nearestPos_X - realPos_X) * (nearestPos_X - realPos_X) 
								       + (nearestPos_Y - realPos_Y) * (nearestPos_Y - realPos_Y) 
								       + (nearestPos_Z - realPos_Z) * (nearestPos_Z - realPos_Z) );

			double voxDist = sqrt(   (voxelNearest_X - voxelReal_X) * (voxelNearest_X - voxelReal_X) 
								      + (voxelNearest_Y - voxelReal_Y) * (voxelNearest_Y - voxelReal_Y) 
								      + (voxelNearest_Z - voxelReal_Z) * (voxelNearest_Z - voxelReal_Z) );

			double Escat = CalcScatEnergy(comptonAngle);

			hAngleDiffVsAngle->Fill(thetaReal, comptonAngle - thetaReal);

			hAngleDiff->Fill(comptonAngle - thetaReal);
			hNearest->Fill(nearestDist);
			hVoxDist->Fill(voxDist);

			hNearestVsAngle->Fill(thetaReal, nearestDist);
			hNearestVsEscat->Fill(Escat, nearestDist);

			// ----------

			hAngle->Fill(thetaReal);
			hEscat->Fill(Escat);
			hAngleDiffVsEscat->Fill(Escat, comptonAngle - thetaReal);

			if (thetaReal > 10 && thetaReal < 40)
			{
				hAngle1->Fill(thetaReal);
				hAngleDiff_The1->Fill(comptonAngle - thetaReal);
			}
			else if (thetaReal > 40 && thetaReal < 60)
			{
				hAngle2->Fill(thetaReal);
				hAngleDiff_The2->Fill(comptonAngle - thetaReal);
			}
			else if (thetaReal > 60 && thetaReal < 80)
			{
				hAngle3->Fill(thetaReal);
				hAngleDiff_The3->Fill(comptonAngle - thetaReal);
			}
			else if (thetaReal > 80 && thetaReal < 120)
			{
				hAngle4->Fill(thetaReal);
				hAngleDiff_The4->Fill(comptonAngle - thetaReal);
			}
		}
	}

	// ---------------------------
	m_c0->Divide(2, 1);
	m_c0->cd(1);
	hAngleDiff->Draw();
	m_c0->cd(2);
	hNearest->Draw();

	// ---------------------------
	m_c1->Divide(2, 2);

	m_c1->cd(1);
	hVoxDist->Draw();

	m_c1->cd(2);
	hAngleDiffVsAngle->Draw();
	m_c1->cd(3);
	hNearestVsAngle->Draw();
	m_c1->cd(4);
	hNearestVsEscat->Draw();

	// ----------
	m_c1->cd(2);
	int nhbins = hNearest->GetNbinsX();
	int piekval = hNearest->GetBinContent(1);
	int restcon = 0;
	for (int luck = 2; luck < nhbins; luck++)
	{
		restcon += hNearest->GetBinContent(luck);
	}

	TString str = "CTR BIN: ";
	std::stringstream strstr;
	strstr << piekval;
	str += strstr.str();

	TPaveText* text1 = new TPaveText(0.7, 0.75, 0.95, 0.65, "NDC");
	text1->AddText(str);
	text1->Draw();
	str = "rest: ";
	strstr.str("");
	strstr << restcon;
	str += strstr.str();

	TPaveText* text2 = new TPaveText(0.7, 0.65, 0.95, 0.55, "NDC");
	text2->AddText(str);
	text2->Draw();

	m_c1->Update();

	// -------------------------

	// ---------------------------
	m_c2->Divide(4,3);
	m_c2->cd(1);
	hAngle->Draw();

	m_c2->cd(2);
	hEscat->Draw();

	m_c2->cd(3);
	hAngleDiffVsEscat->Draw();

	m_c2->cd(5);
	gStyle->SetOptFit();

	hAngleDiff_The1->Draw();
	hAngleDiff_The1->Fit("gaus","","", -0.4, 0.4);

	m_c2->cd(6);
	hAngleDiff_The2->Draw();
	hAngleDiff_The2->Fit("gaus","","", -0.4, 0.4);

	m_c2->cd(7);
	hAngleDiff_The3->Draw();
	hAngleDiff_The3->Fit("gaus","","", -0.7, 0.7);

	m_c2->cd(8);
	hAngleDiff_The4->Draw();
	hAngleDiff_The4->Fit("gaus","","", -0.7, 0.7);

	m_c2->cd(9);
	hAngle1->Draw();

	m_c2->cd(10);
	hAngle2->Draw();

	m_c2->cd(11);
	hAngle3->Draw();

	m_c2->cd(12);
	hAngle4->Draw();

	m_c2->Update();
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

// ================================================================================

double CalcScatEnergy(const double& in_angle_degrees)
{
	double angle_rad = kPI * (in_angle_degrees / 180.0);
	double cosTh = std::cos(angle_rad);
	// const double kPI = 3.14159265;
	// const double mass_electron = 510.99907;
	
	// TODO: not always true!!!
	double Etot = mass_electron;

	double term1 = 1.0 + Etot * (1.0 - cosTh)/(mass_electron);
	double E1 = Etot - Etot / term1;

	return E1;
}

// ================================================================================


