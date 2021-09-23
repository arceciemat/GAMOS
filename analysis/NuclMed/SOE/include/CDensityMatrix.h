
#pragma once

#ifndef _SOE_DENSITYMATRIX___
#define _SOE_DENSITYMATRIX___

#include "C3Vector.h"

// 2 DIMENSION implementation

class CDensityMatrix 
{
public:
				CDensityMatrix( int in_nbins_x, int in_nbins_y, int in_nbins_z );
	virtual		~CDensityMatrix( );

	void		IncrementCount( const C3Vector& position );	
	void		DecrementCount( const C3Vector& position );

	int			GetDensity( const C3Vector& position ) const;

	bool		DumpDensityMatrix() const;
	bool		ReadDensityMatrix();

private:
	// private methods
	void		CalculateVoxel( const C3Vector& position, C3Vector& io_voxelIndex ) const;
	void		DumpHeaderFile() const;
	
	// private data
	int***		m_densities;

	int			m_nbins_x;
	int			m_nbins_y;
	int			m_nbins_z;
};


#endif
