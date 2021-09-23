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

void
Main()
{
	std::ifstream binaryfile("density_matrix.img", ios::binary | ios::in );
	if (!binaryfile.is_open())
	{
		cout << "ERROR. Cannot open file: 'density_matrix.img' " << endl;
		return 1;
	}
	int tmp;
	std::ofstream outfile("ascii.img");

	// Get configuration parameters used by SOE
	ReadConfigurationParameters();

	for (int ix=0; ix < m_bins_x; ix++)
	{
		for (int jy=0; jy < m_bins_y; jy++)
		{
			for (int kz=0; kz < m_bins_z; kz++)
			{
				binaryfile.read((char*) &tmp, sizeof(tmp));
				outfile << tmp << " ";
			}
			outfile << endl;
		}
		outfile << endl;
	}
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



