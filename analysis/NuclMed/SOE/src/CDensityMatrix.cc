
#include "CDensityMatrix.h" 
#include "CUserParameters.h"

#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

CDensityMatrix::CDensityMatrix( int in_nbins_x, int in_nbins_y, int in_nbins_z )
	: m_nbins_x(in_nbins_x)
	, m_nbins_y(in_nbins_y)
	, m_nbins_z(in_nbins_z)
{
	m_densities = new int** [m_nbins_x];
	for (int ix=0; ix < m_nbins_x; ix++)
	{
		m_densities[ix] = new int* [m_nbins_y];
		for (int jy=0; jy < m_nbins_y; jy++)
		{
			m_densities[ix][jy] = new int [m_nbins_z];
		}
	}

	for (int ix=0; ix < m_nbins_x; ix++)
		for (int jy=0; jy < m_nbins_y; jy++)
			for (int kz=0; kz < m_nbins_z; kz++)
				m_densities[ix][jy][kz] = 0;
}

CDensityMatrix::~CDensityMatrix( )
{
	for (int i=0; i < m_nbins_x; i++)
	{
		for (int j=0; j < m_nbins_y; j++)
		{
			delete [] m_densities[i][j];
		}
		delete [] m_densities[i];
	}

	delete [] m_densities;
}

void
CDensityMatrix::IncrementCount( const C3Vector& in_position )
{
	// Calculate voxel, increment +1 voxel-density in CDensityMatrix
	C3Vector voxelIndex;
	CUserParameters::Instance()->CalculateVoxel( in_position, voxelIndex );

	int ix = (int) voxelIndex.GetX();
	int iy = (int) voxelIndex.GetY();	
	int iz = (int) voxelIndex.GetZ();

	// TODO: what to do when position is outside range of density matrix?
	if (   ix >= 0 && ix < m_nbins_x 
		&& iy >= 0 && iy < m_nbins_y 
		&& iz >= 0 && iz < m_nbins_z)
	{
		m_densities[ix][iy][iz]++;
	}
}
	
void
CDensityMatrix::DecrementCount( const C3Vector& in_position )
{
	// Calculate voxel, decrement -1 voxel-density in CDensityMatrix
	C3Vector voxelIndex;
	CUserParameters::Instance()->CalculateVoxel( in_position, voxelIndex );

	int ix = (int) voxelIndex.GetX();
	int iy = (int) voxelIndex.GetY();
	int iz = (int) voxelIndex.GetZ();

	// TODO what to do when position is outside range of density matrix?
	if (   ix >= 0 && ix < m_nbins_x 
		&& iy >= 0 && iy < m_nbins_y
		&& iz >= 0 && iz < m_nbins_z)
	{
		m_densities[ix][iy][iz]--;
	}
}

int
CDensityMatrix::GetDensity( const C3Vector& in_position ) const
{
	C3Vector voxelIndex;
	CUserParameters::Instance()->CalculateVoxel( in_position, voxelIndex );

	int ix = (int) voxelIndex.GetX();
	int iy = (int) voxelIndex.GetY();
	int iz = (int) voxelIndex.GetZ();

	// TODO what to do when position is outside range of density matrix?
	if (   ix < 0 || ix >= m_nbins_x 
	    || iy < 0 || iy >= m_nbins_y 
		|| iz < 0 || iz >= m_nbins_z)
	{
		return 0;		
	}

	return m_densities[ix][iy][iz];
}

bool		
CDensityMatrix::DumpDensityMatrix() const
{
	std::ofstream binaryfile("density_matrix.img", ios::binary | ios::out );
	if (!binaryfile.is_open())
	{
		cout << "ERROR. Cannot open file: 'density_matrix.img' " << endl;
		return 1;
	}
	int tmp;
	// FOR SOME REASON, the inner loop is over x in AMIDE FILES
	for (int kz=0; kz < m_nbins_z; kz++)
	{
		for (int jy=0; jy < m_nbins_y; jy++)
		{
			for (int ix=0; ix < m_nbins_x; ix++)
			{
				tmp = m_densities[ix][jy][kz];
				binaryfile.write((char*) &tmp, sizeof(tmp));
			}
		}
	}
	binaryfile.close();
	DumpHeaderFile();
	return 0;
}

bool		
CDensityMatrix::ReadDensityMatrix()
{
	std::ifstream binaryfile("in_density_matrix.img", ios::binary | ios::in );
	if (!binaryfile.is_open())
	{
		cout << "ERROR. Cannot open file: 'in_density_matrix.img' " << endl;
		return 1;
	}
	int tmp;
	// FOR SOME REASON, the inner loop is over x in AMIDE FILES
	for (int kz=0; kz < m_nbins_z; kz++)
	{
		for (int jy=0; jy < m_nbins_y; jy++)
		{
			for (int ix=0; ix < m_nbins_x; ix++)
			{
				binaryfile.read((char*) &tmp, sizeof(tmp));
				m_densities[ix][jy][kz] = tmp;
			}
		}
	}
	binaryfile.close();
	return 0;
}

void		
CDensityMatrix::DumpHeaderFile() const
{
	std::ofstream asciifile("density_matrix.hdr", ios::out);

	CUserParameters* uP = CUserParameters::Instance();
	assert (uP != NULL);

	asciifile << "!INTERFILE  := " << endl;
	asciifile << "name of data file := " << "density_matrix.img" << endl;	// TODO: make a variable?
	asciifile << "!GENERAL DATA := " << endl;
	asciifile << "!GENERAL IMAGE DATA := " << endl;
	asciifile << "!type of data := tomographic " << endl;
	asciifile << "!version of keys := 3.3 " << endl;
	asciifile << "!data offset in bytes := 0 " << endl;
	asciifile << "imagedata byte order := littleendian " << endl;
	asciifile << "!PET STUDY (General) := " << endl;
	asciifile << "!PET data type := Image " << endl;
	asciifile << "process status := Reconstructed " << endl;
	asciifile << "!number format := short float " << endl;
	asciifile << "!number of bytes per pixel := 4 " << endl;
	asciifile << "number of dimensions := 3 " << endl;
	asciifile << "matrix axis label [1] := X " << endl;
	asciifile << "!matrix size [1] := " << uP->GetNumberOfBinsX() << endl;
	asciifile << "scaling factor (mm/pixel) [1] := " << uP->GetVoxelSizeX() << endl;
	asciifile << "matrix axis label [2] := Y " << endl;
	asciifile << "!matrix size [2] := " << uP->GetNumberOfBinsY() << endl;
	asciifile << "scaling factor (mm/pixel) [2] := " << uP->GetVoxelSizeY() << endl;
	asciifile << "matrix axis label [3] := Z " << endl;
	asciifile << "!matrix size [3] := " << uP->GetNumberOfBinsZ() << endl;
	asciifile << "scaling factor (mm/pixel) [3] := " << uP->GetVoxelSizeZ() << endl;
	asciifile << "number of slices := " << uP->GetNumberOfBinsZ() << endl;		// TODO: is this correct?
	asciifile << "number of time frames := 1 " << endl;
	asciifile << "image scaling factor[1] := 1 " << endl;
	asciifile << "data offset in bytes[1] := 0 " << endl;
	asciifile << "quantification units := 1 " << endl;
	asciifile << "!END OF INTERFILE :=  " << endl;

	asciifile.close();
}












