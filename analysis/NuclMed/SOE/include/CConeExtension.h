
#pragma once

#ifndef _SOE_CCONEEXTENSION___
#define _SOE_CCONEEXTENSION___

#include "C3Matrix.h"
#include "C3Vector.h"

class CCone;

struct ConePhiRange
{
	int scenario;
	double phi_min;
	double phi_max;
};

// *******************************************************************************************************

class CConeExtension
{
public: 
						CConeExtension();
	virtual				~CConeExtension();

	// copy and assign constructors
						CConeExtension(const CConeExtension& in_obj);
	CConeExtension& 	operator= (const CConeExtension& in_obj);

	void				SetAngles(const double& in_angleY, const double& in_angleZ)
						{
							m_angleY = in_angleY;
							m_angleZ = in_angleZ;
						}
	double				GetAngleY() const { return m_angleY; }
	double				GetAngleZ() const { return m_angleZ; }
	ConePhiRange 		GetConePhiRange() const { return m_phiRange; }

	int 				CalculatePhiRanges( const CCone& in_cone, ofstream* in_ofile = NULL );
	int 				DoCalculatePhiRange( const CCone& in_cone, const double& in_fovZ, ofstream* in_ofile = NULL );

	// operators
	bool				operator==(const CConeExtension& in_obj) const; 		// ==
	bool				operator!=(const CConeExtension& in_obj) const;		// !=

	// (De)Serialization
	void				Serialize( std::ostream& outstream ) const;
	void				Deserialize( std::istream& instream );

private:

	double 				m_angleY;
	double 				m_angleZ;
	ConePhiRange		m_phiRange;
};

// *******************************************************************************************************

// static functions....

bool DoRotateFOVUntoCircle(int in_histcolour, const double& in_ComptonAngle, const C3Vector& in_scatPos
        , const double& in_angleMatY, const double& in_angleMatZ
		, C3Vector& io_vector, ofstream* io_ofile = NULL, bool isCorner = false);

bool DoRotateFOVUntoCircle(int in_histcolour, const double& in_ComptonAngle, const C3Vector& scatPos
        , const C3Matrix& in_matInvZ, const C3Matrix& in_matInvY, C3Vector& io_vector, ofstream* io_ofile = NULL, bool isCorner = false );

int FindPhiRanges (  int numSolutionsEdge1, const C3Vector& edge1_sol1, const C3Vector& edge1_sol2
                    , int numSolutionsEdge2, const C3Vector& edge2_sol1, const C3Vector& edge2_sol2
                    , int numSolutionsEdge3, const C3Vector& edge3_sol1, const C3Vector& edge3_sol2
                    , int numSolutionsEdge4, const C3Vector& edge4_sol1, const C3Vector& edge4_sol2
					, ConePhiRange& io_phiRange );

int FindIntersectionsInFOV(const C3Vector& in_scatPos, const C3Vector& in_coneAxis, const double& coneAngle
        , double& in_val, bool isXedge
//		, const double& in_angleMatY, const double& in_angleMatZ
		, const double& in_fovZ
		, C3Vector& io_intersect1, C3Vector& io_intersect2 );

double GetPhiFromXandY(const double& in_X, const double& in_Y);

void WriteOutVector(int in_histo, int in_colour, const double& in_scale, C3Vector& io_vector, ofstream* io_ofile);

#endif



