/*
	UserParameters for the SOE Image Reconstruction algorithm
*/

#pragma once

#ifndef _SOE_CUSERPARAMETERS_H__
#define _SOE_CUSERPARAMETERS_H__


#include <string>
#include "C3Vector.h"

class CUserParameters
{
public:
	// singleton access
	static CUserParameters*			Instance();
	static void 					Destroy();

	void							CalculateVoxel( const C3Vector& in_position, C3Vector& io_voxelIndex ) const;
	bool							IsInsideFOVBounds(const C3Vector& in_position) const;

	// Get Functions
	int								GetNumberOfBinsZ() const { return m_bins_z; }
	double							GetLowerZ() const { return m_zmin; }
	double							GetUpperZ() const { return m_zmax; }
	int								GetNumberOfBinsX() const { return m_bins_x; }
	double							GetLowerX() const { return m_xmin; }
	double							GetUpperX() const { return m_xmax; }
	int								GetNumberOfBinsY() const { return m_bins_y; }
	double							GetLowerY() const { return m_ymin; }
	double							GetUpperY() const { return m_ymax; }
	int								GetNumberOfIterations() const { return m_iterations; }

	int								GetEventSetSize() const	{ return m_eventSetSize; }
	int								GetMaxNumberOfEvents() const	{ return m_maxNumberOfEvents; }

	void							SetDoLimitRange( bool doLimitRange ) { m_doLimitRange = doLimitRange; }
	bool							GetDoLimitRange() const { return m_doLimitRange; }

	const std::string& 				GetDataFileName() const { return m_dataFileName; }
	double 							GetVoxelSizeX() const { return m_voxelSizeX; }
	double 							GetVoxelSizeY() const { return m_voxelSizeY; }
	double 							GetVoxelSizeZ() const { return m_voxelSizeZ; }
	double							GetSourceEnergy() const { return m_sourceEnergy; }

	double							GetEnergyResolutionFactorSigma() const { return m_energyResolutionFactorSigma; }
	double 							GetSpatialDeltaX() const { return m_spatialDeltaX; }
	double 							GetSpatialDeltaY() const { return m_spatialDeltaY; }
	double 							GetSpatialDeltaZ() const { return m_spatialDeltaZ; }
    double                          GetDopplerEffectSigmaTheta() const { return m_dopplerEffectSigmaTheta; }
    double                          GetDopplerEffectSigmaThetaLow() const { return m_dopplerEffectSigmaThetaLow; }

private:

	// constructor and destructor
									CUserParameters();
	virtual							~CUserParameters();

	// other methods
	void							ReadConfigurationParameters();

	// friend test function has access to private methods...
	friend void TestEventManager();
	friend void TestCCurrentState();
	friend void TestCEvent();
	friend void RotateFOVInitialize();

	// static instance
	static CUserParameters*			sInstance;

	// Private data
	int								m_bins_z;
	double							m_zmin;
	double							m_zmax;
	double 							m_voxelSizeZ;

	int								m_bins_x;
	double							m_xmin;
	double							m_xmax;
	int								m_bins_y;
	double							m_ymin;
	double							m_ymax;
	double 							m_voxelSizeX;
	double 							m_voxelSizeY;

	int								m_iterations;

	std::string 					m_dataFileName;
	double							m_sourceEnergy;

	int								m_eventSetSize;
	int								m_maxNumberOfEvents;

	double							m_energyResolutionFactorSigma;
	double 							m_spatialDeltaX;
	double 							m_spatialDeltaY;
	double 							m_spatialDeltaZ;
	double                          m_dopplerEffectSigmaTheta;
	double                          m_dopplerEffectSigmaThetaLow;

	bool							m_doLimitRange;
};

#endif

/*
Example singleton

class Singleton
{

public:

	Singleton* GetInstance()
	{
	if(instance == NULL) instance = new Singleton;
	return instance;
	}

	void DeleteInstance()
	{
	delete instance;
	instance = NULL;
	}


private:

	static Singleton *instance;

	Singleton() { }
	~Singleton() { }
};
*/
