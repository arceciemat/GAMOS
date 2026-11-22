TCanvas* m_c1 = NULL;

int		m_bins;
double	m_xmin;
double	m_xmax;

int		m_bins_z;
double 	m_zmin;
double 	m_zmax;

int		m_iterations;
double 	m_voxelSize;
double 	m_voxelSizeZ;

const double kPI = 3.14159265;
const double mass_electron = 510.99907;

// -------------------------------------

void Main()
{
	m_c1 = new TCanvas("m_c1", "m_c1", 800, 600);

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
	int hrbins = (int) ( (hrmax - hrmin) / m_voxelSize ); 
	double rbinsize = m_voxelSize;

	// histoos
	TH1D* hAngleDiff = new TH1D("hAngleDiff", "hAngleDiff", 101, -1.0, 1.0);
	TH1D* hDist = new TH1D("hDist", "hDist", hrbins, hrmin, hrmax);
	TH1D* hVoxDist = new TH1D("hVoxDist", "hVoxDist", 101, -0.05, 10.05);

	TH2D* hAngleDiffVsAngle = new TH2D("hAngleDiffVsAngle", "hAngleDiffVsAngle", 100, 0.0, 120.0, 101, -1.0, 1.0);
	TH2D* hDistVsAngle = new TH2D("hDistVsAngle", "hDistVsAngle", 100, 0.0, 120.0, hrbins, hrmin, hrmax);
	TH2D* hVoxDistVsAngle = new TH2D("hVoxDistVsAngle", "hVoxDistVsAngle", 100, 0.0, 120.0, 100, -0.05, 10.05);

	TH2D* hDistVsEscat = new TH2D("hDistVsEscat", "hDistVsEscat", 100, 0.0, 350.0, hrbins, hrmin, hrmax);
	
	double comptonAngle, thetaReal; 
	double measPos_X, measPos_Y, measPos_Z;
	double realPos_X, realPos_Y, realPos_Z;
	double perpenRealLen, perpenMeasLen;
	double voxelMeas_X, voxelMeas_Y, voxelMeas_Z;
	double voxelReal_X, voxelReal_Y, voxelReal_Z;
	double distPos_X, distPos_Y, distPos_Z;
	double voxelDist_X, voxelDist_Y, voxelDist_Z;

	while (!infile.eof())
	{
		infile >> comptonAngle >> thetaReal 
			   >> measPos_X >> measPos_Y >> measPos_Z 
			   >> realPos_X >> realPos_Y >> realPos_Z 
			   >> perpenRealLen >> perpenMeasLen
			   >> voxelMeas_X >> voxelMeas_Y >> voxelMeas_Z 
			   >> voxelReal_X >> voxelReal_Y >> voxelReal_Z
			   >> distPos_X >> distPos_Y >> distPos_Z 
			   >> voxelDist_X >> voxelDist_Y >> voxelDist_Z;

		if (!infile.eof())
		{
			double distPos = sqrt(   (distPos_X - realPos_X) * (distPos_X - realPos_X) 
								   + (distPos_Y - realPos_Y) * (distPos_Y - realPos_Y) 
								   + (distPos_Z - realPos_Z) * (distPos_Z - realPos_Z) );

			// if (perpenRealLen < perpenMeasLen) distPos = -1.0 * distPos;

			double distVox = sqrt(   (voxelDist_X - voxelReal_X) * (voxelDist_X - voxelReal_X) 
								   + (voxelDist_Y - voxelReal_Y) * (voxelDist_Y - voxelReal_Y) 
								   + (voxelDist_Z - voxelReal_Z) * (voxelDist_Z - voxelReal_Z) );

			double Escat = calcScatEnergy(comptonAngle);
// cout << "Escat: " << Escat << endl;

			hAngleDiff->Fill(comptonAngle - thetaReal);

			hDist->Fill(distPos);

			hVoxDist->Fill(distVox);

// cout << distPos << " " << perpenMeasLen - perpenRealLen << endl;

			hAngleDiffVsAngle->Fill(thetaReal, comptonAngle - thetaReal);
			hDistVsAngle->Fill(thetaReal, distPos);
			hVoxDistVsAngle->Fill(thetaReal, distVox);

			hDistVsEscat->Fill(Escat, distPos);
		}
	}

	m_c1->Divide(3, 2);

	m_c1->cd(1);
	hAngleDiff->Draw();
	m_c1->cd(2);
	hDist->Draw();
	m_c1->cd(3);
	hVoxDist->Draw();

	m_c1->cd(4);
	hAngleDiffVsAngle->Draw();
	m_c1->cd(5);
	hDistVsAngle->Draw();

	m_c1->cd(6);
	//	hVoxDistVsAngle->Draw();
	hDistVsEscat->Draw();

	m_c1->cd(2);

	int nhbins = hDist->GetNbinsX();
	int piekval = hDist->GetBinContent(1);
	int restcon = 0;
	for (int luck = 2; luck < nhbins; luck++)
	{
		restcon += hDist->GetBinContent(luck);
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
	
	m_bins = 0;
	m_xmin = 0.0;
	m_xmin = 0.0;

	double zPosition1 = 0.0; 
	double zPosition2 = 0.0;
	
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
	
	if (m_bins > 0)
	{
		m_voxelSize = (m_xmax - m_xmin) / m_bins;
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
		cout << "m_bins: " << m_bins << endl;
		cout << "x/y min: " << m_xmin << ", x/y max: " << m_xmax << endl;
		cout << "Voxel size: " << m_voxelSize << endl;
		cout << "z min: " << m_zmin << ", z max: " << m_zmax << endl;
		cout << "Voxel size: " << m_voxelSizeZ << endl;
		cout << endl;
	}
}

// ================================================================================

double calcScatEnergy(const double& in_angle_degrees)
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


