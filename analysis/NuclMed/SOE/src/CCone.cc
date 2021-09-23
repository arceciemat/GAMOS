
#include "CCone.h"
#include "CUserParameters.h"

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandFlat.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include "constants.h"

using namespace std;

// define output stream operator (for outputting contents of CCone with cout)
ostream& operator<<(ostream& os, const CCone& cone)
{
	os << cone.GetComptonAngle() << " " << cone.GetComptonAxisDirection() << " " << cone.GetComptonAxisOrigin() << endl;
	C3Matrix matrix;
	cone.GetRotationMatrix(matrix);
	os << matrix;
	return os;
}

// --------- CCone --------------------------------------

CCone::CCone()
	: m_comptonAngle(0.0)
	, m_comptonAxisDirection(0.0, 0.0, 0.0)
	, m_comptonAxisOrigin(0.0, 0.0, 0.0)
	, m_rotMatrixZxY()
{
}

CCone::~CCone()
{
}

// copy constructor
CCone::CCone(const CCone& in_obj)
{
	*this = in_obj;
}

// assignment operator
CCone&
CCone::operator= (const CCone& in_obj)
{
	if (this != &in_obj)
	{
		m_comptonAngle = in_obj.GetComptonAngle();
		m_comptonAxisDirection = in_obj.GetComptonAxisDirection();
		m_comptonAxisOrigin = in_obj.GetComptonAxisOrigin();
		in_obj.GetRotationMatrix( m_rotMatrixZxY );

		m_coneExtension = in_obj.GetConeExtension();

	}
	return *this;
}

bool
CCone::operator==(const CCone &in_obj) const
{
	C3Matrix matrix;
	in_obj.GetRotationMatrix( matrix );

    return (    doubleEquals(m_comptonAngle, in_obj.GetComptonAngle(), 0.0001)
		    && 	m_comptonAxisDirection == in_obj.GetComptonAxisDirection()
			&&  m_comptonAxisOrigin == in_obj.GetComptonAxisOrigin()
			&& 	m_rotMatrixZxY == matrix 
			&&  m_coneExtension == in_obj.GetConeExtension() );
}

bool
CCone::operator!=(const CCone &in_obj) const
{
    return !(*this == in_obj);
}

int
CCone::Calculate(  const C3Vector& in_position1, const double& in_e1
				 , const C3Vector& in_position2, const double& in_e2 )
{
	int err = CalculateComptonAngle(in_e1, in_e2);
	if (!err)
	{
		err = CalculateComptonGeometrics(in_position1, in_position2);
	}
	return err;
}

int
CCone::CalculateComptonAngle(const double& in_e1, const double& in_e2)
{
	// Etot
	double Etot = CUserParameters::Instance()->GetSourceEnergy();
	if (Etot <= 0.0)
	{
		Etot = in_e1 + in_e2;
	}
	double cosTh = 1.0 - mass_electron*((1/(Etot - in_e1)) - (1/(Etot)));

	if (fabs(cosTh) > 1.0)
	{
		return 1;
	}

	m_comptonAngle = acos(cosTh);
		// NOTE: since phi is unknown, we don't care about the cuadrant
		// (i.e., angle can be +10 degrees or -10 degrees, will give same cosTh)

	return 0;
}

int
CCone::CalculateComptonGeometrics(const C3Vector& in_position1, const C3Vector& in_position2)
{
	m_comptonAxisDirection = in_position1 - in_position2;
	m_comptonAxisOrigin = in_position1;

	int error = CalculateRotationMatrices();
	return error;
}

void
CCone::VaryComptonAngle( const double& in_dopplerResolution )
{
	if (in_dopplerResolution > 0.0)
	{
		double degrees = (m_comptonAngle/kPI) * 180.0;
		double gaussVal = 1.0;
		if (degrees < 40.0)
		{
			double delLow = CUserParameters::Instance()->GetDopplerEffectSigmaThetaLow();
			gaussVal = CLHEP::RandGauss::shoot(1.0, delLow);
		}
		else
		{
			gaussVal = CLHEP::RandGauss::shoot(1.0, in_dopplerResolution);
		}

		m_comptonAngle = m_comptonAngle * gaussVal;
	}
}

const C3Vector&
CCone::GetComptonAxisDirection() const
{
	return m_comptonAxisDirection;
}

const C3Vector&
CCone::GetComptonAxisOrigin() const
{
	return m_comptonAxisOrigin;
}

void
CCone::GetRotationMatrix(C3Matrix& io_rotMatZxY) const
{
	io_rotMatZxY = m_rotMatrixZxY;
}

int
CCone::CalculateRotationMatrices()
{
	double angleMatY;
	double angleMatZ;

	DoCalculateRotationAngles(angleMatY, angleMatZ);
	m_coneExtension.SetAngles( angleMatY, angleMatZ );

	C3Matrix rotMatrixY ( C3Matrix::Axis_Y, angleMatY);
	C3Matrix rotMatrixZ ( C3Matrix::Axis_Z, angleMatZ);

	m_rotMatrixZxY = rotMatrixZ * rotMatrixY;
	
	int error = 0;
	/*
	if ( CUserParameters::Instance()->GetDoLimitRange() )
	{
		error = m_coneExtension.CalculatePhiRange( *this, angleMatY, angleMatZ );
	}
	*/
	return error;
}

void
CCone::DoCalculateRotationAngles(double& io_angleMatY, double& io_angleMatZ)
{
	// The next code is verified in TestCode.cxx, with the function "RotationTests()"

	// Calculate Rotation Matrices to rotate vector into right frame
	C3Vector axisZ( 0.0, 0.0, 1.0);
	C3Vector axisX( 1.0, 0.0, 0.0);

	// Rotation around Y-axis, to rotate z_axis to X component of Compton axis
	io_angleMatY = m_comptonAxisDirection.GetScalarProductAngle(axisZ);
	// 	Correct for sign-ambiguity of angle ( acos(X) = plus/minus theta)
	if (m_comptonAxisDirection.GetX() < 0)
	{
		io_angleMatY = -1.0 * io_angleMatY;
		// Note:
		// Only checking for sign in X-direction (because we rotate around Y anyway....)
		// 1. We don't care about the size of the rotated vector, only the direction
		// 2. As a matter of fact, we don't care about the y component of the vector, only the x-component
		// 			(because we will rotate the Z axis around the Y-axis, which will ALWAYS give 0 for the y component.
		// 3. The angle between z-axis and the compton-axis is equal to the angle
		// 			between z-axis and compton-axis rotated onto x-z plane
	}

	// Rotation around Z-axis, to rotate the rotate x-axis to the projection of the Compton axis in the X-Y plane
	// 	1. Calculate value of angle with vector pointing in 1st quadrant
	C3Vector xyVector( fabs(m_comptonAxisDirection.GetX()), fabs(m_comptonAxisDirection.GetY()), 0.0);
	io_angleMatZ = xyVector.GetScalarProductAngle(axisX);

	// 	2. Decide real sign of angle (depending on real quadrant)
	if (   (m_comptonAxisDirection.GetX() >= 0 && m_comptonAxisDirection.GetY() <  0)
	    || (m_comptonAxisDirection.GetX() <  0 && m_comptonAxisDirection.GetY() >= 0) )
	{
		io_angleMatZ = -1.0 * io_angleMatZ;
	}
}

double 
CCone::GetRandomPhi() const
{
	double zCone_phi;
	double phi_min = 0;
	double phi_max = 2.0 * kPI;
	double phi_range = phi_max - phi_min;

	if ( CUserParameters::Instance()->GetDoLimitRange() )
	{
		ConePhiRange phiRange = m_coneExtension.GetConePhiRange();
		if ( phiRange.scenario == 2 || phiRange.scenario == 3 )
		{
			phi_min = phiRange.phi_min;
			phi_max = phiRange.phi_max;
			phi_range = phi_max - phi_min;
		}
		// cout << "getphi: " << " sc: " << phiRange.scenario << " range:" << phiRange.phi_min << " " << phiRange.phi_max << endl;
	}

	zCone_phi = CLHEP::RandFlat::shoot(phi_range);
	zCone_phi = phi_min + zCone_phi;

	return zCone_phi;
}

void
CCone::GetRandomDirectionVectorOnCone(C3Vector& io_vector) const
{
	// 1. 	Generate random phi
	// Use a random number to get a random value for phi
	// on a cone that is directed along the z-axis with origin (0, 0, 0).

	double zCone_phi = GetRandomPhi();

	double zCone_cosPhi = cos(zCone_phi);
	double zCone_sinPhi = sin(zCone_phi);

	// 	cosTheta comes from Compton angle
	double zCone_cosTheta = cos(m_comptonAngle);
	double zCone_sinTheta = sin(m_comptonAngle);

	// 2.	Fill cone vector
	C3Vector zCone_vector(  zCone_sinTheta * zCone_cosPhi
				 	  	  , zCone_sinTheta * zCone_sinPhi
				 	      , zCone_cosTheta );

	// 3.	Rotate the Vector.
	io_vector = (m_rotMatrixZxY) * zCone_vector;
}

void
CCone::GetRandomPositionOnCone(C3Vector& io_vector) // const
{
    int zbins = CUserParameters::Instance()->GetNumberOfBinsZ();
    if (zbins == 1)
    {
        GetRandomPositionOnEllips( io_vector );
        return;
    }

	// Get a random position in the z direction
	double zmin = CUserParameters::Instance()->GetLowerZ();
	double zmax = CUserParameters::Instance()->GetUpperZ();
	assert (zmax >= zmin);
	double zrange = zmax - zmin;
	assert (zrange > 0);

	// Get A Point on the Cone with the chosen Z distance
	// zmin is further away from scatterer than zmax because both zmin and zmax are negative values
	// (i.e.: z-axis is negative in direction from scatterer to source)

	double zrandom = CLHEP::RandFlat::shoot(zrange);
	double zposition = zmax - zrandom;

	// Make sure phi-range is correct for this zposition
	if ( CUserParameters::Instance()->GetDoLimitRange() )
	{
		int error = m_coneExtension.DoCalculatePhiRange( *this, zposition );
		if (error)
		{
			// ConePhiRange phiRange = m_coneExtension.GetConePhiRange();
			// cout << "calc, z: " << zposition << " sc: " << phiRange.scenario << " range:" << phiRange.phi_min << " " << phiRange.phi_max << endl;

			return;
		}
	}

	// Get a random angle in phi
	C3Vector dir_vector;
	GetRandomDirectionVectorOnCone(dir_vector);

	// Project the vector (with selected phi) onto FOV (with selected Z). 
	double zcoord = zposition - m_comptonAxisOrigin.GetZ();
	double coeff = (dir_vector.GetZ() != 0) ? zcoord / dir_vector.GetZ() : 0.0;

	dir_vector.Set(  coeff * dir_vector.GetX()
				   , coeff * dir_vector.GetY()
				   , coeff * dir_vector.GetZ() );

	io_vector = m_comptonAxisOrigin + dir_vector;
}

void
CCone::GetRandomPositionOnEllips(C3Vector& io_vector) const
{
	C3Vector dir_vector;
	GetRandomDirectionVectorOnCone(dir_vector);

	// Get A Point on the Cone with the chosen Z distance
	double zcoord = CUserParameters::Instance()->GetLowerZ() - m_comptonAxisOrigin.GetZ();
	double coeff = (dir_vector.GetZ() != 0) ? zcoord / dir_vector.GetZ() : 0.0;

	dir_vector.Set(  coeff * dir_vector.GetX()
				   , coeff * dir_vector.GetY()
				   , coeff * dir_vector.GetZ() );

	io_vector = m_comptonAxisOrigin + dir_vector;
}

// (De)Serialization
void
CCone::Serialize( std::ostream& io_outstream ) const
{
	// cout << "Serializing: " << m_comptonAngle << " " << m_comptonAxisDirection << " " << m_comptonAxisOrigin << endl;
	io_outstream.write((char*) &m_comptonAngle, sizeof(m_comptonAngle));

	m_comptonAxisDirection.Serialize( io_outstream );
	m_comptonAxisOrigin.Serialize( io_outstream );
	m_rotMatrixZxY.Serialize( io_outstream );

	m_coneExtension.Serialize( io_outstream );
}

void
CCone::Deserialize( std::istream& io_instream )
{
	io_instream.read((char*) &m_comptonAngle, sizeof(m_comptonAngle));

	m_comptonAxisDirection.Deserialize( io_instream );
	m_comptonAxisOrigin.Deserialize( io_instream );
	m_rotMatrixZxY.Deserialize( io_instream );

	m_coneExtension.Deserialize( io_instream );
}

// debugging purposes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
bool
CCone::GetNearestPointOnCone( const C3Vector& in_Point, C3Vector& io_PointOnCone
		, double& io_DelTh_PointMinOnCone ) const
//
// Here, we suppose the angle is wrong: the Origin and the ^k axis is correct
// With a new theta, we recalculate a vector ^q with respecto the ^k axis
// and then we look for a point on ^q nearest to the point P (on vector ^p)
//
{
    // Calculate Real Compton Angle
    C3Vector pntVec = in_Point - m_comptonAxisOrigin;
    double thetaReal = pntVec.GetScalarProductAngle(m_comptonAxisDirection);
    double pntVecLen = pntVec.GetLength();

    double axisLen = m_comptonAxisDirection.GetLength();
	C3Vector axisDirection = m_comptonAxisDirection * (1.0 / axisLen);

    double perpenRealLen = pntVecLen * sin(thetaReal);
    double xLen = pntVec * axisDirection;
    C3Vector xPoint = m_comptonAxisOrigin + (axisDirection * xLen);

    C3Vector perpenRealVec = in_Point - xPoint;

    double perpenShiftedLen = xLen * tan(m_comptonAngle);
	C3Vector perpenRealDirection = perpenRealVec * ( 1.0 / perpenRealLen );
    C3Vector perpenShiftedVec = ( perpenRealDirection * perpenShiftedLen );

    C3Vector checkRealPos = xPoint + perpenRealVec;
	assert( checkRealPos == in_Point );

    C3Vector shiftedPos = xPoint + perpenShiftedVec;

	C3Vector shiftVec = in_Point - shiftedPos;
	C3Vector qVec = shiftedPos - m_comptonAxisOrigin;
	double qLen = qVec.GetLength();
	C3Vector qDirection = qVec * (1.0 / qLen);

	double nearestLen = (shiftVec * qDirection);
	C3Vector nearestVec = qDirection * nearestLen;
	C3Vector nearestPos = shiftedPos + nearestVec;

	io_PointOnCone = nearestPos;
	io_DelTh_PointMinOnCone = thetaReal - m_comptonAngle;

    return 0;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>






