#pragma once

#ifndef _SOE_CCONE___
#define _SOE_CCONE___

#include "C3Vector.h"
#include "C3Matrix.h"
#include "CConeExtension.h"

class CCone
{
public:
						CCone();
	virtual				~CCone();

	// copy and assign constructors
						CCone(const CCone& in_obj);
	CCone& 				operator= (const CCone& in_obj);

	// other methods
	int					Calculate(  const C3Vector& in_position1, const double& in_e1
				   				  , const C3Vector& in_position2, const double& in_e2 );

	// resolution recovery functions
	int					CalculateComptonAngle(const double& in_e1, const double& in_e2);
	int					CalculateComptonGeometrics(const C3Vector& in_position1, const C3Vector& in_position2);
	void				VaryComptonAngle( const double& in_dopplerResolution = 0.0 );

	// get functions
	inline double		GetComptonAngle() const 	{ return m_comptonAngle; }
	const C3Vector&		GetComptonAxisDirection() const;
	const C3Vector&		GetComptonAxisOrigin() const;

	inline const CConeExtension&	GetConeExtension() const { return m_coneExtension; }

	// (De)Serialization
	void				Serialize( std::ostream& outstream ) const;
	void				Deserialize( std::istream& instream );

	// Methods to get random position on the Compton Cone
	void				GetRandomPositionOnCone(C3Vector& io_vector); // const;
    void				GetRandomDirectionVectorOnCone(C3Vector& io_vector) const;
	inline int			CalculatePhiRanges() { return m_coneExtension.CalculatePhiRanges(*this); }

	bool				operator==(const CCone& in_obj) const; 		// ==
	bool				operator!=(const CCone& in_obj) const;		// !=

	// debugging purposes
	bool				GetNearestPointOnCone( const C3Vector& in_Point, C3Vector& io_PointOnCone, double& io_DelTh_PointMinOnCone ) const;
							// Shift theta (correct the compton angle, for Doppler resolution recovery)
	void				SetComptonAngle(const double& in_angle) { m_comptonAngle = in_angle; }

private:
	// private methods
	int					CalculateRotationMatrices();
	void				DoCalculateRotationAngles(double& io_angleMatY, double& io_angleMatZ);
	void				GetRotationMatrix(C3Matrix& io_rotMatZxY) const;
    void				GetRandomPositionOnEllips(C3Vector& io_vector) const;

	double 				GetRandomPhi() const;

	// friend function with access to data
	friend void RotationTests();
	friend void TestSerializeCone();
	friend void RotateFOVAllEvents();
	friend void RotateFOVExamples();

	// private data
	double					m_comptonAngle;
	C3Vector				m_comptonAxisDirection;
	C3Vector				m_comptonAxisOrigin;

	CConeExtension			m_coneExtension;

	// variables needed to get random vector on a cone (and hence, random point)
	C3Matrix 				m_rotMatrixZxY;

	// define output stream operator
	friend ostream& 		operator<<(ostream& os, const CCone& cone);
};

#endif
