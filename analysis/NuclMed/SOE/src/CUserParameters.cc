
#include "CUserParameters.h"

#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

CUserParameters* CUserParameters::sInstance = 0;	// initialize pointer

CUserParameters*
CUserParameters::Instance()
{
	if (sInstance == NULL)
	{
		sInstance = new CUserParameters();
	}
	return sInstance;
}

void
CUserParameters::Destroy()
{
	delete sInstance;
	sInstance = NULL;
}

CUserParameters::CUserParameters()
	: m_bins_z(1)
	, m_zmin(-100)
	, m_zmax(-100)
	, m_voxelSizeZ(0.0)
	, m_bins_x(1)
	, m_xmin(0)
	, m_xmax(0)
	, m_bins_y(1)
	, m_ymin(0)
	, m_ymax(0)
	, m_voxelSizeX(0.0)
	, m_voxelSizeY(0.0)
	, m_iterations(100)
	, m_sourceEnergy(-1)
	, m_eventSetSize(0)
	, m_maxNumberOfEvents(0)
	, m_energyResolutionFactorSigma(0.0)
	, m_spatialDeltaX(0.0)
	, m_spatialDeltaY(0.0)
	, m_spatialDeltaZ(0.0)
	, m_dopplerEffectSigmaTheta(0.0)
	, m_dopplerEffectSigmaThetaLow(0.0)
	, m_doLimitRange(false)
{
	ReadConfigurationParameters();
}

CUserParameters::~CUserParameters()
{
}

void
CUserParameters::ReadConfigurationParameters()
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

	double zPosition1 = m_zmin;
	double zPosition2 = m_zmax;

	while (!conffile.eof())
    {
		conffile >> tmpStr >> tmpVal >> tmpDum;
		if (conffile.fail() && !conffile.eof())
		{
			cout << "ERROR! FORMAT ERROR in parameters file" << endl;
			exit(1);
		}
		if (!conffile.eof())
		{
			if (   tmpStr == "m_distanceZ")
			{
				zPosition1 = tmpVal;
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
			else if (tmpStr == "DataFileName")
			{
				m_dataFileName = tmpDum;
			}
			else if (tmpStr == "EventSetSize")
			{
				m_eventSetSize = (int) tmpVal;
			}
			else if (tmpStr == "m_maxNumberOfEvents")
			{
				m_maxNumberOfEvents = (int) tmpVal;
			}
			else if (tmpStr == "m_sourceEnergy")
			{
				m_sourceEnergy = tmpVal;
			}
			else if (tmpStr == "UseEnergyResolutionFactorFWHM")
			{
				// sigma = FWHM / 2.35
				m_energyResolutionFactorSigma = tmpVal / 2.35;
			}
			else if (tmpStr == "UseEnergyResolutionFactorSigma")
			{
				m_energyResolutionFactorSigma = tmpVal;
			}
			else if (tmpStr == "UseSpatialDeltaX")
			{
				m_spatialDeltaX = tmpVal;
			}
			else if (tmpStr == "UseSpatialDeltaY")
			{
				m_spatialDeltaY = tmpVal;
			}
			else if (tmpStr == "UseSpatialDeltaZ")
			{
				m_spatialDeltaZ = tmpVal;
			}
			else if (tmpStr == "UseDopplerEffectSigmaTheta")
			{
                m_dopplerEffectSigmaTheta = tmpVal;
			}
			else if (tmpStr == "UseDopplerEffectSigmaThetaLow")
			{
				m_dopplerEffectSigmaThetaLow = tmpVal;
			}
			else if (!conffile.eof())
			{
				cout << "Unknown entry in configure file: " << tmpStr << endl;
				exit(1);
			}
		}
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
	if (m_bins_z <= 0)
	{
		cout << "ERROR: illegal number of z bins: " << m_bins_z << endl;
		exit(1);
	}
	m_voxelSizeZ = (m_zmax - m_zmin) / m_bins_z;

	if (m_dopplerEffectSigmaThetaLow == 0)
	{
		m_dopplerEffectSigmaThetaLow = m_dopplerEffectSigmaTheta;
	}

	if (true)
	{
		cout << "PROGRAM PARAMETERS: " << endl;
		cout << "# iterations: " << m_iterations << endl;
		cout << "#_bins (x axis): " << m_bins_x << endl;
		cout << "x min: " << m_xmin << ", x max: " << m_xmax << endl;
		cout << "Voxel size X: " << m_voxelSizeX << endl;
		cout << "#_bins (y axis): " << m_bins_y << endl;
		cout << "y min: " << m_ymin << ", y max: " << m_ymax << endl;
		cout << "Voxel size Y: " << m_voxelSizeY << endl;
		cout << "# bins (z-axis): " << m_bins_z << endl;
		cout << "Z position 1: " << zPosition1 << ", 2: " << zPosition2 << endl;
		cout << "Voxel size Z: " << m_voxelSizeZ << endl;
		cout << "Data Filename: " << m_dataFileName << endl;
		cout << "Data set size: " << m_eventSetSize << endl;
		cout << "Max number of events: " << m_maxNumberOfEvents << endl;
		cout << "Gamma source energy: " << m_sourceEnergy << endl;

		cout << "Use Energy Resolution Recovery (sigma) [%]: " << m_energyResolutionFactorSigma << endl;
		cout << "Use Spatial Delta X [mm]: " << m_spatialDeltaX << endl;
		cout << "Use Spatial Delta Y [mm]: " << m_spatialDeltaY << endl;
		cout << "Use Spatial Delta Z [mm]: " << m_spatialDeltaZ << endl;
        cout << "Use DopplerEffect SigmaTheta: " << m_dopplerEffectSigmaTheta << endl;
        cout << "Use DopplerEffect SigmaTheta(Low): " << m_dopplerEffectSigmaThetaLow << endl;

		cout << endl;
	}
}

void
CUserParameters::CalculateVoxel( const C3Vector& in_position, C3Vector& io_voxelIndex ) const
{
	// double amin = -0.5 * m_voxelSize * m_bins;	// mm
	// double amax = amin + m_voxelSize * m_bins;

	int voxel_xindex = -1;
	if (m_voxelSizeX == 0)
		voxel_xindex = 0;
	else if (   in_position.GetX() >= m_xmin && in_position.GetX() < m_xmax)
		voxel_xindex = int ( (in_position.GetX() - m_xmin)/m_voxelSizeX );

	int voxel_yindex = -1;
	if (m_voxelSizeY == 0)
		voxel_yindex = 0;
	else if (   in_position.GetY() >= m_ymin && in_position.GetY() < m_ymax)
		voxel_yindex = int ( (in_position.GetY() - m_ymin)/m_voxelSizeY );

	int voxel_zindex = -1;
	if (m_voxelSizeZ == 0)
		voxel_zindex = 0;
	else if (   in_position.GetZ() >= m_zmin && in_position.GetZ() < m_zmax)
		voxel_zindex = int ( (in_position.GetZ() - m_zmin)/m_voxelSizeZ );

	// return voxel indices in a 3D vector
	io_voxelIndex.Set(voxel_xindex, voxel_yindex, voxel_zindex);
}

// ****************************************************************************

bool 
CUserParameters::IsInsideFOVBounds(const C3Vector& in_position) const
{
	bool insideBounds = (  in_position.GetX() >= GetLowerX()
						&& in_position.GetX() <= GetUpperX()
						&& in_position.GetY() >= GetLowerY()
						&& in_position.GetY() <= GetUpperY()
						&& in_position.GetZ() >= GetLowerZ()
						&& in_position.GetZ() <= GetUpperZ() );
	return insideBounds;
}




