
#include "CConeExtension.h"
#include "CCone.h"

#include "CUserParameters.h"
#include "constants.h"

//C/C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// #include <algorithm>
// #include <vector>

#include <cmath>
#include <cassert>

#define SCENARIO6_DEBUG 0

using namespace std;


// CConeExtension class *****************************************+

CConeExtension::CConeExtension()
	: m_angleY(0.0)
	, m_angleZ(0.0)
{
	m_phiRange.scenario = 0;
	m_phiRange.phi_min  = 0.0;
	m_phiRange.phi_max  = 2.0 * kPI;
}

CConeExtension::~CConeExtension()
{
}

// copy constructor
CConeExtension::CConeExtension(const CConeExtension& in_obj)
{
	*this = in_obj;
}

// assignment operator
CConeExtension&
CConeExtension::operator= (const CConeExtension& in_obj)
{
	if (this != &in_obj)
	{
		m_angleY = in_obj.GetAngleY();
		m_angleZ = in_obj.GetAngleZ();
		m_phiRange = in_obj.GetConePhiRange();
	}
	return *this;
}

bool
CConeExtension::operator==(const CConeExtension &in_obj) const
{
	ConePhiRange in_phiRange = in_obj.GetConePhiRange();
	return (	doubleEquals(m_angleY, in_obj.GetAngleY(), 0.001)
			 && doubleEquals(m_angleZ, in_obj.GetAngleZ(), 0.001)
			 && m_phiRange.scenario == in_phiRange.scenario
			 && doubleEquals(m_phiRange.phi_min, in_phiRange.phi_min, 0.001)
			 && doubleEquals(m_phiRange.phi_max, in_phiRange.phi_max, 0.001) );
}

bool
CConeExtension::operator!=(const CConeExtension &in_obj) const
{
    return !(*this == in_obj);
}

void
CConeExtension::Serialize( std::ostream& io_outstream ) const
{
	io_outstream.write((char*) &m_angleY, sizeof(m_angleY));
	io_outstream.write((char*) &m_angleZ, sizeof(m_angleZ));

	io_outstream.write((char*) &m_phiRange.scenario, sizeof(m_phiRange.scenario));
	io_outstream.write((char*) &m_phiRange.phi_min, sizeof(m_phiRange.phi_min));
	io_outstream.write((char*) &m_phiRange.phi_min, sizeof(m_phiRange.phi_max));
}


void
CConeExtension::Deserialize( std::istream& io_instream )
{
	io_instream.read((char*) &m_angleY, sizeof(m_angleY));
	io_instream.read((char*) &m_angleZ, sizeof(m_angleZ));

	io_instream.read((char*) &m_phiRange.scenario, sizeof(m_phiRange.scenario));
	io_instream.read((char*) &m_phiRange.phi_min, sizeof(m_phiRange.phi_min));
	io_instream.read((char*) &m_phiRange.phi_min, sizeof(m_phiRange.phi_max));
}

// /*

int
CConeExtension::CalculatePhiRanges( const CCone& in_cone, ofstream* in_ofile )
{
	double fovZ = CUserParameters::Instance()->GetUpperZ();
	DoCalculatePhiRange( in_cone, fovZ, in_ofile );

	int zuppScenario = m_phiRange.scenario;
	double zuppPhiMin = m_phiRange.phi_min;
	double zuppPhiMax = m_phiRange.phi_max;

	fovZ = CUserParameters::Instance()->GetLowerZ();
	DoCalculatePhiRange( in_cone, fovZ, in_ofile );

	int zlowScenario = m_phiRange.scenario;
	double zlowPhiMin = m_phiRange.phi_min;
	double zlowPhiMax = m_phiRange.phi_max;

	int err = 0;
	if ( zuppScenario == 1 && zlowScenario == 1 ) 		// N.B: throwing away all Scenario 1 events will result in cutting too much
	{
		err = 1;
	}
    else if (   (zuppScenario == 2 || zuppScenario == 3)
             && (zlowScenario == 2 || zlowScenario == 3) )
	{
        m_phiRange.phi_min = min(zuppPhiMin, zlowPhiMin);
		m_phiRange.phi_max = max(zuppPhiMax, zlowPhiMax);
	}

	return err;
}

int
CConeExtension::DoCalculatePhiRange( const CCone& in_cone, const double& in_fovZ, ofstream* in_ofile )
{
	// inverse rotation matrices
 	C3Matrix matZinv( C3Matrix::Axis_Z, -1*m_angleZ);
	C3Matrix matYinv( C3Matrix::Axis_Y, -1*m_angleY);

	CUserParameters* userPar = CUserParameters::Instance();
	double xmin = userPar->GetLowerX();
	double xmax = userPar->GetUpperX();
	double ymin = userPar->GetLowerY();
	double ymax = userPar->GetUpperY();

	C3Vector edge1_sol1, edge1_sol2;
	C3Vector edge2_sol1, edge2_sol2;
	C3Vector edge3_sol1, edge3_sol2;
	C3Vector edge4_sol1, edge4_sol2;

	const C3Vector& scatPos = in_cone.GetComptonAxisOrigin();
	const C3Vector& comptonAxis = in_cone.GetComptonAxisDirection();
	const double& theta_C = in_cone.GetComptonAngle();

	// Find Solutions In Fov
	int numSolutionsEdge1, numSolutionsEdge2, numSolutionsEdge3, numSolutionsEdge4;

	numSolutionsEdge1 = FindIntersectionsInFOV(scatPos, comptonAxis, theta_C, xmin, true,  /*in_angleMatY, in_angleMatZ,*/ in_fovZ, edge1_sol1, edge1_sol2 );
	numSolutionsEdge2 = FindIntersectionsInFOV(scatPos, comptonAxis, theta_C, ymax, false, /*in_angleMatY, in_angleMatZ,*/ in_fovZ, edge2_sol1, edge2_sol2 );
	numSolutionsEdge3 = FindIntersectionsInFOV(scatPos, comptonAxis, theta_C, xmax, true,  /*in_angleMatY, in_angleMatZ,*/ in_fovZ, edge3_sol1, edge3_sol2 );
	numSolutionsEdge4 = FindIntersectionsInFOV(scatPos, comptonAxis, theta_C, ymin, false, /*in_angleMatY, in_angleMatZ,*/ in_fovZ, edge4_sol1, edge4_sol2 );

	// Rotate solutions onto Circle
	if (numSolutionsEdge1 > 0)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge1_sol1, in_ofile);
	if (numSolutionsEdge1 == 2)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge1_sol2, in_ofile);

	if (numSolutionsEdge2 > 0)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge2_sol1, in_ofile);
	if (numSolutionsEdge2 == 2)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge2_sol2, in_ofile);

	if (numSolutionsEdge3 > 0)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge3_sol1, in_ofile);
	if (numSolutionsEdge3 == 2)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge3_sol2, in_ofile);

	if (numSolutionsEdge4 > 0)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge4_sol1, in_ofile);
	if (numSolutionsEdge4 == 2)
		DoRotateFOVUntoCircle(-7, theta_C, scatPos, matZinv, matYinv, edge4_sol2, in_ofile);

	// Find scenario and phi ranges
	int error = 0;

	// cout << "solutions, edge1: " << numSolutionsEdge1 << " 2: " << numSolutionsEdge2 
	// 			   << " 3: " << numSolutionsEdge3 << " 4: " << numSolutionsEdge4 << endl;

	int scenario = FindPhiRanges (  numSolutionsEdge1, edge1_sol1, edge1_sol2
								  , numSolutionsEdge2, edge2_sol1, edge2_sol2
								  , numSolutionsEdge3, edge3_sol1, edge3_sol2
								  , numSolutionsEdge4, edge4_sol1, edge4_sol2
								  , m_phiRange );
	/*
	if (scenario == 1)
	{
		error = 1;
	}
	else 
	*/
	if ( (scenario != 2) && (scenario != 3) )
	{
		m_phiRange.phi_min = 0.0;
		m_phiRange.phi_max = 2.0 * kPI;
	}

	// without the next line, we get artificial corners... 
	// if ( abs(m_phiRange.phi_max - m_phiRange.phi_min) < 0.01 )  	// too small: will give artificial corners
	if ( abs(m_phiRange.phi_max - m_phiRange.phi_min) < 0.05 ) 
	// if ( abs(m_phiRange.phi_max - m_phiRange.phi_min) < 50.05 ) 	// too big: will be like "no limit on range"
	{
		m_phiRange.phi_min = 0.0;
		m_phiRange.phi_max = 2.0 * kPI;
	}

	return error;
}


//  OTHER FUNCTIONS--------------------------------------------------------------------
// ************************************************************************************

bool DoRotateFOVUntoCircle(int in_histcolour, const double& in_ComptonAngle, const C3Vector& in_scatPos
        , const double& in_angleMatY, const double& in_angleMatZ
		, C3Vector& io_vector, ofstream* io_ofile
		, bool isCorner )
{
	// inverse rotation matrices
 	C3Matrix matZinv( C3Matrix::Axis_Z, -1*in_angleMatZ);
	C3Matrix matYinv( C3Matrix::Axis_Y, -1*in_angleMatY);

	bool ok = DoRotateFOVUntoCircle(in_histcolour, in_ComptonAngle, in_scatPos, matZinv, matYinv, io_vector, io_ofile, isCorner );
	return ok;
}

// ************************************************************************************

bool DoRotateFOVUntoCircle(int in_histcolour, const double& in_ComptonAngle, const C3Vector& in_scatPos
        , const C3Matrix& in_matZinv, const C3Matrix& in_matYinv, C3Vector& io_vector, ofstream* io_ofile
		, bool isCorner )
{
	// starting from FOV (real size)
	WriteOutVector(3, in_histcolour, 1.0, io_vector, io_ofile);

    // translate back w.r.t to scattering hit
    C3Vector vecpp = io_vector - in_scatPos;
	WriteOutVector(0, in_histcolour, 0.1, vecpp, io_ofile);

    // rotate BACK around z-axis
    C3Vector vecp = in_matZinv * vecpp;
	WriteOutVector(1, in_histcolour, 0.1, vecp, io_ofile);

    // rotate BACK around y-axis
    C3Vector vec = in_matYinv * vecp;

    // and project it on the plane where the circle is... (= cos(Theta_C))
    double z_circle_distance = cos(in_ComptonAngle);
	io_vector.Set(0, 0, 0);
	double factor = 0;
	bool ok = false;
	if (vec.GetZ() != 0)
	{
		factor = (z_circle_distance) / vec.GetZ();
		if (factor > 0)
		{
			ok = true;
			io_vector = vec * factor;
			WriteOutVector(2, in_histcolour, 1.0, io_vector, io_ofile);
		}
	}
	return ok;
}

// ************************************************************************************
// To later print out in ROOT (just for checking purposes)
void WriteOutVector(int in_histo, int in_colour, const double& in_scale, C3Vector& io_vector, ofstream* io_ofile)
{
	if (io_ofile)
	{
 	   *io_ofile << in_histo << " " << in_colour << " " << io_vector * in_scale << endl;
	}
}


// *****************************************************************************************************
int FindPhiRanges(    int numSolutionsEdge1, const C3Vector& edge1_sol1, const C3Vector& edge1_sol2
                    , int numSolutionsEdge2, const C3Vector& edge2_sol1, const C3Vector& edge2_sol2
                    , int numSolutionsEdge3, const C3Vector& edge3_sol1, const C3Vector& edge3_sol2
                    , int numSolutionsEdge4, const C3Vector& edge4_sol1, const C3Vector& edge4_sol2
					, ConePhiRange& io_phiRange  )
{
	// cout << "num solutions...: " << numSolutionsEdge1 << " " << numSolutionsEdge2 << " " << numSolutionsEdge3 << " " << numSolutionsEdge4 << endl;

	double phi_min_1 = 0;
	double phi_max_1 = 2 * kPI;
	double phi_min_2 = 0;
	double phi_max_2 = 0;
	double phi1, phi2, phi3, phi4;
	int scenario = 0;

	// scenario 1
	if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 0)
	{
        scenario = 1;
        // scenario 1, phi range = [0, 2pi]
        phi1 = 0;
        phi2 = 2.0 * kPI;
	}
	//
	// Scenario 2: circle crossing 1 side and one 1 adjacent side
	//
	else if (numSolutionsEdge1 == 1 && numSolutionsEdge2 == 1 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 0)
	{
		scenario = 2;
		phi1 = GetPhiFromXandY( edge1_sol1.GetX(), edge1_sol1.GetY());
		phi2 = GetPhiFromXandY( edge2_sol1.GetX(), edge2_sol1.GetY());
	}
	else if (numSolutionsEdge1 == 1 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 1)
	{
		scenario = 2;
		phi1 = GetPhiFromXandY( edge1_sol1.GetX(), edge1_sol1.GetY());
		phi2 = GetPhiFromXandY( edge4_sol1.GetX(), edge4_sol1.GetY());
	}
	else if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 1 && numSolutionsEdge3 == 1 && numSolutionsEdge4 == 0)
	{
		scenario = 2;
		phi1 = GetPhiFromXandY( edge2_sol1.GetX(), edge2_sol1.GetY());
		phi2 = GetPhiFromXandY( edge3_sol1.GetX(), edge3_sol1.GetY());
	}
	else if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 1 && numSolutionsEdge4 == 1)
	{
		scenario = 2;
		phi1 = GetPhiFromXandY( edge3_sol1.GetX(), edge3_sol1.GetY());
		phi2 = GetPhiFromXandY( edge4_sol1.GetX(), edge4_sol1.GetY());
	}
	//
	// Scenario 3: circle crossing two opposite sides
	//

	else if (numSolutionsEdge1 == 1 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 1 && numSolutionsEdge4 == 0)
	{
		scenario = 3;
		phi1 = GetPhiFromXandY( edge1_sol1.GetX(), edge1_sol1.GetY());
		phi2 = GetPhiFromXandY( edge3_sol1.GetX(), edge3_sol1.GetY());
	}
	else if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 1 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 1)
	{
		scenario = 3;
		phi1 = GetPhiFromXandY( edge2_sol1.GetX(), edge2_sol1.GetY());
		phi2 = GetPhiFromXandY( edge4_sol1.GetX(), edge4_sol1.GetY());
	}

	// For the moment, we only care about scenario 2 and 3 (and maybe 1, to skip it)

	/*
	// Scenario 4....

	// Scenario 5: circle crossing one side twice
	// ... never mind, will be small circle anyway
	else if (numSolutionsEdge1 == 2 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 0)
	{
		scenario = 5;
		phi1 = GetPhiFromXandY( edge1_sol1.GetX(), edge1_sol1.GetY());
		phi2 = GetPhiFromXandY( edge1_sol2.GetX(), edge1_sol2.GetY());
	}
	else if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 2 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 0)
	{
		scenario = 5;
		phi1 = GetPhiFromXandY( edge2_sol1.GetX(), edge2_sol1.GetY());
		phi2 = GetPhiFromXandY( edge2_sol2.GetX(), edge2_sol2.GetY());
	}
	else if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 2 && numSolutionsEdge4 == 0)
	{
		scenario = 5;
		phi1 = GetPhiFromXandY( edge3_sol1.GetX(), edge3_sol1.GetY());
		phi2 = GetPhiFromXandY( edge3_sol2.GetX(), edge3_sol2.GetY());
	}
	else if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 2)
	{
		scenario = 5;
		phi1 = GetPhiFromXandY( edge4_sol1.GetX(), edge4_sol1.GetY());
		phi2 = GetPhiFromXandY( edge4_sol2.GetX(), edge4_sol2.GetY());
	}

	// Scenario 6: crossing two opposite sides twice
	// forget it... will never happen
	else if (numSolutionsEdge1 == 2 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 2 && numSolutionsEdge4 == 0)
	{
		scenario = 6;
		phi1 = GetPhiFromXandY( edge1_sol1.GetX(), edge1_sol1.GetY());
		phi2 = GetPhiFromXandY( edge1_sol2.GetX(), edge1_sol2.GetY());
		phi3 = GetPhiFromXandY( edge3_sol1.GetX(), edge3_sol1.GetY());
		phi4 = GetPhiFromXandY( edge3_sol2.GetX(), edge3_sol2.GetY());
	}
	else if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 2 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 2)
	{
		scenario = 6;
		phi1 = GetPhiFromXandY( edge2_sol1.GetX(), edge2_sol1.GetY());
		phi2 = GetPhiFromXandY( edge2_sol2.GetX(), edge2_sol2.GetY());
		phi3 = GetPhiFromXandY( edge4_sol1.GetX(), edge4_sol1.GetY());
		phi4 = GetPhiFromXandY( edge4_sol2.GetX(), edge4_sol2.GetY());
	}
	*/

	// Additional stuff...
	// scenario 6
	/*
	if (scenario == 6)
	{
		// start with the max phi from one edge
		phi_min_1 = (phi1 > phi2) ? phi1 : phi2;
		// next, the min phi from opposite edge
		phi_max_1 = (phi3 < phi4) ? phi3 : phi4;
		// next, the max phi from opposite edge
		phi_min_2 = (phi3 < phi4) ? phi4 : phi3;
		// at last, the min phi from first edge
		phi_max_2 = (phi1 > phi2) ? phi2 : phi1;

		if (phi_min_1 > phi_max_1)
		{
			phi_min_1 = phi_min_1 - 2 * kPI;
		}
		if (phi_min_2 > phi_max_2)
		{
			phi_min_2 = phi_min_2 - 2 * kPI;
		}
	}
	// other cases
	else 
	*/

	// Organize phi-ranges (min is smallest, max is largest)
	if (scenario > 1)
	{
		phi_min_1 = (phi1 < phi2) ? phi1 : phi2;
		phi_max_1 = (phi1 < phi2) ? phi2 : phi1;
		// If range is bigger than 180 degrees, than take range from other half of the circle
		if ( scenario == 2 || scenario == 3 )
		{
			if (phi_max_1 > phi_min_1 + kPI)
			{
				double tmp = phi_min_1;
				phi_min_1 = phi_max_1 - 2 * kPI;
				phi_max_1 = tmp;
			}
		}
	}

	// Output...
	io_phiRange.phi_min = phi_min_1; 	
	io_phiRange.phi_max = phi_max_1; 	
	io_phiRange.scenario = scenario;

	return scenario;
}

// *********************************************************************************************************

double GetPhiFromXandY(const double& in_X, const double& in_Y)
{
	double out_phi;
	if (in_X == 0)
	{
		if (in_Y > 0) out_phi = 0.5 * kPI;
		else out_phi = 1.5 * kPI;
	}
	else if (in_Y == 0)
	{
		if (in_X > 0) out_phi = 0.0;
		else out_phi = kPI;
	}
	else
	{
		double quotient = fabs(in_Y / in_X);
		if (in_X > 0 && in_Y > 0)
		{
			out_phi = atan(quotient);
		}
		else if (in_X < 0 && in_Y < 0)
		{
			out_phi = kPI + atan(quotient);
		}
		else if (in_X < 0 && in_Y > 0)
		{
			out_phi = kPI - atan(quotient);
		}
		else if (in_X > 0 && in_Y < 0)
		{
			out_phi = 2 * kPI - atan(quotient);
		}
	}

	return out_phi;
}

// *********************************************************************************************************

int FindIntersectionsInFOV(const C3Vector& in_scatPos, const C3Vector& in_coneAxis, const double& in_coneAngle
        , double& in_val, bool isXedge
//		, const double& in_angleMatY, const double& in_angleMatZ
		, const double& in_fovZ
		, C3Vector& io_intersect1, C3Vector& io_intersect2 )
{
    io_intersect1.Set(-1,-1,-1);
    io_intersect2.Set(-1,-1,-1);

	double len = in_coneAxis.GetLength();
	double factor = len > 0 ? (1.0 / len) : 0.0;

    double nx = factor * in_coneAxis.GetX();
    double ny = factor * in_coneAxis.GetY();
    double nz = factor * in_coneAxis.GetZ();

	if (nz >= 0) return 0;

    double orig_x = in_scatPos.GetX();
    double orig_y = in_scatPos.GetY();
    double orig_z = in_scatPos.GetZ();

    double LTERM;   // left hand known term
    double RTERM;   // right hand known term
    if (isXedge)    // calculate unknown y from known x
    {
        LTERM = nx * (in_val - orig_x);
        RTERM = (in_val - orig_x) * (in_val - orig_x);
    }
    else            // calculate unknown x from known y
    {
        LTERM = ny * (in_val - orig_y);
        RTERM = (in_val - orig_y) * (in_val - orig_y);
    }
    LTERM += nz * (in_fovZ - orig_z);
    RTERM += (in_fovZ - orig_z) * (in_fovZ - orig_z);

    // root-square formula
	double cosAngle = cos(in_coneAngle); 
    double cosSQ = cosAngle * cosAngle;
    double cFactor = LTERM*LTERM - cosSQ * RTERM;
    double bFactor, aFactor;
    if (isXedge)
    {
        bFactor = 2 * LTERM * ny;
        aFactor = ny*ny - cosSQ;
    }
    else
    {
        bFactor = 2 * LTERM * nx;
        aFactor = nx*nx - cosSQ;
    }

    double q1, q2;
    int numSolutionsQ = solveQuadraticEquation(aFactor, bFactor, cFactor, q1, q2);

    int numSolutions = 0;
    if (numSolutionsQ > 0)
    {
        double x =  isXedge ? in_val : q1 + orig_x;
        double y = !isXedge ? in_val : q1 + orig_y;
		C3Vector tmp(x, y, in_fovZ);

		C3Vector direction = tmp - in_scatPos; 
		double inprod = direction.GetScalarProduct( in_coneAxis );
		bool cosPositive = (cos(in_coneAngle) > 0);
		bool inprodPositive = (inprod > 0);
		bool isOK = (cosPositive && inprodPositive) || (!cosPositive && !inprodPositive);

		if ( CUserParameters::Instance()->IsInsideFOVBounds(tmp) && isOK )
		{
			io_intersect1 = tmp;
        	numSolutions++;
		}
        if (numSolutionsQ > 1)
        {
            double x =  isXedge ? in_val : q2 + orig_x;
            double y = !isXedge ? in_val : q2 + orig_y;
			C3Vector tmp(x, y, in_fovZ);

			direction = tmp - in_scatPos; 
			inprod = direction.GetScalarProduct( in_coneAxis );
			cosPositive = (cos(in_coneAngle) > 0);
			inprodPositive = (inprod > 0);
			isOK = (cosPositive && inprodPositive) || (!cosPositive && !inprodPositive);

			if (CUserParameters::Instance()->IsInsideFOVBounds(tmp) && isOK )
			{
				if (numSolutions == 0)
					io_intersect1 = tmp;
				else
        	    	io_intersect2 = tmp;
            	numSolutions++;
			}
        }

    }

	return numSolutions;
}

// ****************************************************************************
/*
// 
// TESTING ALL THIS...

// QUESTION: problem occurs with "one solution" too???

if (numSolutions == 2)
{
//    CheckTwoSolutions( in_coneAngle, in_scatPos, in_angleMatY, in_angleMatZ
//		, C3Vector& io_intersect1, C3Vector& io_intersect1 );


    C3Vector dummy( io_intersect1 );
    bool isOK1_a = DoRotateFOVUntoCircle(-1, in_coneAngle, in_scatPos, in_angleMatY, in_angleMatZ, dummy);

	C3Vector direction = io_intersect1 - in_scatPos; 
    double inprod = direction.GetScalarProduct( in_coneAxis );

    // bool isOK1_b = (inprod > 0);

	double in_coneAngleDeg = in_coneAngle * 180 / kPI;
cout << "coneAngle: " << in_coneAngleDeg << " coneAxis: " << in_coneAxis << endl;

	double inprang = direction.GetScalarProductAngle( in_coneAxis );
	double inprangdeg = inprang * 180 / kPI;
	bool isOK1_c = doubleEquals( in_coneAngleDeg, inprangdeg, 0.0001 );

	bool smallerThan90 = (in_coneAngle < 0.5 * kPI);
	bool inprodPositive = (inprod > 0);
	bool isOK1_d = (smallerThan90 && inprodPositive) || (!smallerThan90 && !inprodPositive);

cout << "1st solution: " << io_intersect1 << " OK(rotF): " << isOK1_a << " OK (Th=Th): " << isOK1_c << " OK(quadrant):" << isOK1_d << endl;
cout << "direction: " << direction << " inprod: " << inprod << " inprodangle: " << inprang * 180 / kPI << endl;

    dummy = io_intersect2;
    bool isOK2_a = DoRotateFOVUntoCircle(-1, in_coneAngle, in_scatPos, in_angleMatY, in_angleMatZ, dummy);

	direction = io_intersect2 - in_scatPos; 

    inprod = direction.GetScalarProduct( in_coneAxis );
    // bool isOK2_b = (inprod > 0);

	inprang = direction.GetScalarProductAngle( in_coneAxis );
	inprangdeg = inprang * 180 / kPI;
	bool isOK2_c = doubleEquals( in_coneAngleDeg, inprangdeg, 0.0001 );

	smallerThan90 = (in_coneAngle < 0.5 * kPI);
	inprodPositive = (inprod > 0);
	bool isOK2_d = (smallerThan90 && inprodPositive) || (!smallerThan90 && !inprodPositive);

cout << "2nd solution: " << io_intersect2 << " OK(rotF): " << isOK2_a << " OK (Th=Th): " << isOK2_c << " OK(quadrant):" << isOK2_d << endl;
cout << "direction: " << direction << " inprod: " << inprod << " inprodangle: " << inprang * 180 / kPI << endl;

	assert (isOK1_a == isOK1_c);
	assert (isOK1_a == isOK1_d);

	assert (isOK2_a == isOK2_c);
	assert (isOK2_a == isOK2_d);


}

*/



















