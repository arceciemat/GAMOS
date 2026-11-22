
#include "CCurrentState.h"
#include "CUserParameters.h"

#include "CEvent.h"

#include <cassert>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;


CCurrentState::CCurrentState()
	: m_densityMatrix(0)
{
    Initialize();
}

CCurrentState::~CCurrentState()
{
	delete m_densityMatrix;
}

void
CCurrentState::Initialize()
{
	// Initialize Density Matrix
	CUserParameters* userPars = CUserParameters::Instance();
	m_densityMatrix = new CDensityMatrix(userPars->GetNumberOfBinsX(), userPars->GetNumberOfBinsY(), userPars->GetNumberOfBinsZ() );
}

void
CCurrentState::GetPosition( long in_eventID, C3Vector& io_position)
{
	C3Vector* vector = m_dataSetManager.GetDataElement(in_eventID);
	if (vector == NULL)
	{
		cout << "ERROR CCurrentState::GetPosition." << endl;
		cout << "Event with this ID does not exist: " << in_eventID << endl;
		exit(1);
	}
	io_position = *(vector);
}

void
CCurrentState::Update( long in_eventID, const C3Vector& in_newposition )
{
	// get correct set
	m_dataSetManager.LoadCorrectSet( in_eventID );

	// get position
	// CStateVectorMap::iterator mapiter = m_stateVectorMap.find(in_eventID);
	CStateVectorMap& vectorMap = m_dataSetManager.m_dataSetMap;
	CStateVectorMap::iterator mapiter = vectorMap.find(in_eventID);
	if (mapiter == vectorMap.end())
	{
		cout << "ERROR CCurrentState::Update." << endl;
		cout << "Event with this ID does not exist: " << in_eventID << endl;
		exit(1);
	}

	// Update State vector
	C3Vector* oldposition = ((*mapiter).second);

	C3Vector* vec = new C3Vector( in_newposition );
	((*mapiter).second) = vec;

	// update density matrix
	m_densityMatrix->DecrementCount( *oldposition );
	m_densityMatrix->IncrementCount( in_newposition );

	// Important! Delete old vector, not longer used by stateVectorMap.
	delete oldposition;
}

void
CCurrentState::AddToStateVector( int in_eventID, const C3Vector& in_position )
{
	assert (   (in_eventID == 0 && m_dataSetManager.m_maxEventID == 0)
            || (in_eventID == m_dataSetManager.m_maxEventID + 1));

	// Add event to dataSetManager
	m_dataSetManager.AddToMap( in_eventID, in_position );

    // update density matrix
	m_densityMatrix->IncrementCount( in_position );
}

int
CCurrentState::GetDensity( const C3Vector& in_position ) const
{
	return m_densityMatrix->GetDensity( in_position );
}

void
CCurrentState::DumpCurrentState( const std::string& in_filename ) const
{
	std::ofstream outfile(in_filename.c_str(), ios::out );

	C3Vector* vec = NULL;
	const CStateVectorMap& vectorMap = m_dataSetManager.m_dataSetMap;
	CStateVectorMap::const_iterator mapiter = vectorMap.begin();

	int cnt = 0;
	for (mapiter = vectorMap.begin(); mapiter != vectorMap.end(); mapiter++)
	{
		vec = ((*mapiter).second);
		assert (vec != NULL);

		outfile << *vec << endl;
	}
}

bool
CCurrentState::ReadCurrentState( const std::string& in_filename )
{
	std::ifstream infile(in_filename.c_str(), ios::in );
	if (!infile.is_open())
	{
		cout << "ERROR! CCurrentState::ReadCurrentState. Could not open file: " << in_filename << endl;
		return false;
	}

	m_dataSetManager.CleanCurrentMap();

	C3Vector vec;
	double x, y, z;
	int eventID = 0;
	while (!infile.eof())
	{
		infile >> x >> y >> z;
		if (!infile.eof())
		{
			vec.Set(x, y, z);
			m_dataSetManager.AddToMap(eventID, vec);
		}
		eventID++;
	}
	return true;
}

bool
CCurrentState::DumpDensityMatrix() const
{
	if (m_densityMatrix)
	{
		m_densityMatrix->DumpDensityMatrix();
		return 0;
	}
	return 1;
}

bool
CCurrentState::ReadDensityMatrix()
{
	if (m_densityMatrix)
	{
		m_densityMatrix->ReadDensityMatrix();
		return 0;
	}
	return 1;
}

/*
void
CCurrentState::GetStateStatistics(C3Vector& io_mean, C3Vector& io_sigma)
{
	C3Vector* vec = NULL;
	const CStateVectorMap& vectorMap = m_dataSetManager.m_dataSetMap;
	CStateVectorMap::const_iterator mapiter = vectorMap.begin();

	io_mean.Set(0.0, 0.0, 0.0);
	io_sigma.Set(0.0, 0.0, 0.0);
	int nentries = vectorMap.size();
	assert(nentries > 1);

	double xsum = 0.0;
	double ysum = 0.0;
	double zsum = 0.0;
	int nused = 0;
	for (mapiter = vectorMap.begin(); mapiter != vectorMap.end(); mapiter++)
	{
		vec = ((*mapiter).second);
		assert (vec != NULL);	

		bool insideBounds = (  vec->GetX() > CUserParameters::Instance()->GetLowerX()
							&& vec->GetX() < CUserParameters::Instance()->GetUpperX()
							&& vec->GetY() > CUserParameters::Instance()->GetLowerY()
							&& vec->GetY() < CUserParameters::Instance()->GetUpperY()
							&& vec->GetZ() >= CUserParameters::Instance()->GetLowerZ()
							&& vec->GetZ() <= CUserParameters::Instance()->GetUpperZ() );

		if (insideBounds)
		{
			nused++;
			xsum += vec->GetX();
			ysum += vec->GetY();
			zsum += vec->GetZ();
		}
	}
	io_mean.Set(xsum/nused, ysum/nused, zsum/nused);

	//     s = \sqrt{\frac{1}{N-1} \sum_{i=1}^N (x_i - \overline{x})^2}, 
	xsum = 0.0;
	ysum = 0.0;
	zsum = 0.0;
	for (mapiter = vectorMap.begin(); mapiter != vectorMap.end(); mapiter++)
	{
		vec = ((*mapiter).second);
		assert (vec != NULL);

		bool insideBounds = (  vec->GetX() > CUserParameters::Instance()->GetLowerX()
							&& vec->GetX() < CUserParameters::Instance()->GetUpperX()
							&& vec->GetY() > CUserParameters::Instance()->GetLowerY()
							&& vec->GetY() < CUserParameters::Instance()->GetUpperY()
							&& vec->GetZ() >= CUserParameters::Instance()->GetLowerZ()
							&& vec->GetZ() <= CUserParameters::Instance()->GetUpperZ() );
		if (insideBounds)
		{
			xsum += (vec->GetX() - io_mean.GetX()) * (vec->GetX() - io_mean.GetX());
			ysum += (vec->GetY() - io_mean.GetY()) * (vec->GetY() - io_mean.GetY());
			zsum += (vec->GetZ() - io_mean.GetZ()) * (vec->GetZ() - io_mean.GetZ());
		}
	}
	io_sigma.Set(sqrt(xsum/(nused-1)), sqrt(ysum/(nused-1)), sqrt(zsum/(nused-1)));
}
*/











