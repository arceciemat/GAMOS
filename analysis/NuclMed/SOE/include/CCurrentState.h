#pragma once

#ifndef _SOE_CCURRENTSTATE___
#define _SOE_CCURRENTSTATE___

#include "CEvent.h"
#include "C3Vector.h"
#include "CDensityMatrix.h"
#include "CEventManager.h"

#include "CDataSetManager.h"

#include <map>

typedef std::map<long, C3Vector*> CStateVectorMap;		// Mapping event ID to position vector

class CCurrentState
{
public:
							CCurrentState();
	virtual 				~CCurrentState();

	void					AddToStateVector( int in_eventID, const C3Vector& in_position );

	void					GetPosition( long in_eventID, C3Vector& io_position);
	void					Update( long in_eventID, const C3Vector& in_position );

	int						GetDensity( const C3Vector& position ) const;

	void					DumpCurrentState( const std::string& in_filename ) const;
	bool					ReadCurrentState( const std::string& in_filename );

	bool					DumpDensityMatrix() const;
	bool					ReadDensityMatrix();

	// void					GetStateStatistics(C3Vector& io_mean, C3Vector& io_sigma);

private:
	// private methods
	void					Initialize();

	// friend test function has access to private methods...
	friend void TestCCurrentState();

	// disabled copy and assign constructors
	// not implemented, to avoid copying this object
							CCurrentState(const CCurrentState& in_obj);
	CCurrentState& 			operator= (const CCurrentState& in_obj);

	// private date
	CDensityMatrix*					m_densityMatrix;	// an NxMxL sized map with counters for each bin
	CDataSetManager<C3Vector>		m_dataSetManager;	// Friend class. Takes care of data (set) management
};

#endif

