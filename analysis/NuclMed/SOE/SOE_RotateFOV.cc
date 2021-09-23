
#include "CUserParameters.h"
#include "C3Matrix.h"
#include "C3Vector.h"
#include "CCone.h"
#include "constants.h"

#include "CConeExtension.h"
#include "CCone.h"

//C/C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// #include <algorithm>
// #include <vector>

#include <cmath>
#include <cassert>
#include <cstdlib>

using namespace std;


// Functions....
void RotateFOVExamples();

void RotateFOVAllEvents();

void RotateFOVInitialize();

void OpenVisualisationFiles( int in_eventID, std::ofstream& io_file1, std::ofstream& io_file2 );

void DoRotations(const double& in_angleMatY, const double& in_angleMatZ, const CCone& in_cone, int in_eventID);

bool DoRotateCircleUntoFOV(int in_angle, const double& in_ComptonAngle, const C3Vector& scatPos
        , const C3Matrix& in_matY, const C3Matrix& in_matZ, C3Vector& io_vector, ofstream& io_ofile);

bool IsCircleOutsideRotatedFOV(const double& in_circle_radius
        , const C3Vector& in_corner1, const C3Vector& in_corner2
        , const C3Vector& in_corner3, const C3Vector& in_corner4);

bool IsPointOutsideRotatedFOV(const C3Vector& in_position
        , const C3Vector& in_corner1, const C3Vector& in_corner2
        , const C3Vector& in_corner3, const C3Vector& in_corner4);

void GetRotatedFOV(	  const C3Vector& in_corner1, const C3Vector& in_corner2
			        , const C3Vector& in_corner3, const C3Vector& in_corner4
					, double& io_rotatedFOV_xmin, double& io_rotatedFOV_xmax
					, double& io_rotatedFOV_ymin, double& io_rotatedFOV_ymax );

void TestUnityMatrix(const C3Matrix& in_mat1, const C3Matrix& in_mat2);

void RotateCorners(C3Vector& in_corner1, C3Vector& in_corner2, C3Vector& in_corner3, C3Vector& in_corner4
		, const double& in_ComptonAngle, const C3Vector& scatPos
       	, const C3Matrix& in_matZinv, const C3Matrix& in_matYinv, ofstream& io_ofile);

void FindSolutionsInCircle(const double& in_circle_radius
                , const C3Vector& in_corner1, const C3Vector& in_corner2
                , const C3Vector& in_corner3, const C3Vector& in_corner4
                , int& numSolutionsEdge1, C3Vector& edge1_sol1, C3Vector& edge1_sol2
                , int& numSolutionsEdge2, C3Vector& edge2_sol1, C3Vector& edge2_sol2
                , int& numSolutionsEdge3, C3Vector& edge3_sol1, C3Vector& edge3_sol2
                , int& numSolutionsEdge4, C3Vector& edge4_sol1, C3Vector& edge4_sol2);

int FindIntersectionsInCircle(const double& in_circle_radius
        , const C3Vector& in_corner1, const C3Vector& in_corner2, C3Vector& out_coord1, C3Vector& out_coord2);

void OldWay( const double& in_circle_radius, const double& in_theta_C, const C3Vector& in_scatPos
	, const C3Matrix& in_matZinv, const C3Matrix& in_matYinv, std::ofstream& io_ofile2 );

void VisualisationStuff( const double& in_theta_C, const C3Vector& in_scatPos, const double& in_angleMatY, const double& in_angleMatZ
	, std::ofstream& in_ofile1, std::ofstream& in_ofile2 );

// ***********************************************************************************
// For each event, the matrix is different, so, also the FOV corners...
// At Initialisation: for each event, we calculate the inverse matrix and with that, the four corners...
// 					  for each event, we can calculate the "scenario" (how does the ellips fit in the detector)
// 					  for each event, we can proceed to calculate the mininum phi and max phi (1, 2, ...) which we store
// Later, during iteration, we use the min_phi and max_phi
// ***********************************************************************************

bool m_debug = false;

// MAIN
int main()
{
	// RotateFOVExamples();

	RotateFOVAllEvents();
}

// *****************************************************************************

void RotateFOVExamples()
{
	// initialize userparameters...
	RotateFOVInitialize();

	// TARGET SCENARIOS
	// scenario: 1, 1bad, 2, 3, 5, 6
	int targetsc[7] = {1, 1, 2, 3, 5, 6, 1};
	std::string tstrings[7] = {"NO SOLUTIONS", "NO SOLUTIONS", "1 + 1 ADJACENT"
                              , "2 OPPOSITE", "2 SAME SIDE", "2 + 2 OPPOSITE"
							  , "NO SOLUTIONS"};

	double anglesMatY[7] = {kPI, kPI, kPI*0.8, kPI*0.75, kPI*0.75, kPI, kPI*0.5};
	double anglesMatZ[7] = {0.5*kPI, 0, 0.25*kPI, 0, 0, 0.5*kPI, 0};
	const double thetas_C[7] = {0.15*kPI, 0.40*kPI, 0.25*kPI, 0.35*kPI, 0.25*kPI, 0.25*kPI, 0.25*kPI };

	// TODO!!!! Somehow the axis origin plays a role too!!!!
	C3Vector scatPos(50, 0, 0);

	CCone cone;
	for (int ievent = 0; ievent < 7; ievent++)
	{
		std::string fuck = tstrings[ievent];
		cout << "*****  " << ievent << "  ***** SCENARIO: " << targetsc[ievent] << " = " << fuck << endl;
		double angleMatY = anglesMatY[ievent];
		double angleMatZ = anglesMatZ[ievent];

		// Calculate Compton axis
		C3Vector zaxis(0.0, 0.0, 1.0);
		C3Matrix matY( C3Matrix::Axis_Y, angleMatY );
		C3Matrix matZ( C3Matrix::Axis_Z, angleMatZ );
		C3Vector tmp = matY * zaxis;
		C3Vector comptonAxis = matZ * tmp;
		double len = comptonAxis.GetLength();
		if (len > 0)
		{
			comptonAxis = comptonAxis * (1.0 / len);
		}

		cone.m_comptonAngle = thetas_C[ievent];
		cone.m_comptonAxisDirection = comptonAxis;
		cone.m_comptonAxisOrigin = scatPos;
		cone.m_rotMatrixZxY = matZ * matY;

		DoRotations( angleMatY, angleMatZ, cone, ievent);

		cout << endl;
	}
}

// *********************************************************************+

void RotateFOVAllEvents()
{
	std::string datafilename = "test_data.dat";
	std::ifstream datafile(datafilename.c_str(), ios::in);
	if (!datafile.is_open())
	{
		cout << "Input Data file not open: " << datafilename << endl;
		exit(1);
	}
	double z1, y1, x1, e1, z2, y2, x2, e2;
	C3Vector hitPosition1, hitPosition2;
	CCone cone;
	int ievent = 0;
	int linecnt = 0;
	while (!datafile.eof() && ievent < 150 )
    {
		datafile >> z1 >> y1 >> x1 >> e1 >> z2 >> y2 >> x2 >> e2;
		if (!datafile.eof())
		{
			if (datafile.fail())
			{
				cout << "badbit: " << datafile.bad() << ", failbit: " << datafile.fail() << endl;
				cout << "Last read values: " << z1 << " " << y1 << " " << x1 << " " << e1 << " " << z2 << " " << y2 << " " << x2 << " " << e2 << endl;
				exit(2);
			}
            hitPosition1.Set(x1, y1, z1);
            hitPosition2.Set(x2, y2, z2);

			int err = cone.CalculateComptonAngle(e1, e2);
			if (!err)
			{
				cone.m_comptonAxisDirection = hitPosition1 - hitPosition2;
				cone.m_comptonAxisOrigin = hitPosition1;
	
//				cout << "event: " << ievent << endl;
				double angleMatY;
				double angleMatZ;
			
				cone.DoCalculateRotationAngles(angleMatY, angleMatZ);
			
				C3Matrix rotMatrixY ( C3Matrix::Axis_Y, angleMatY);
				C3Matrix rotMatrixZ ( C3Matrix::Axis_Z, angleMatZ);
			
				cone.m_rotMatrixZxY = rotMatrixZ * rotMatrixY;

				// to debug scenario 6 solutions...
				// if ( ievent == 101 || ievent == 102 || ievent == 207 || ievent == 318 || ievent == 366 || ievent == 414 ) m_debug = true;

				if (m_debug) 
				{
					cout << endl;
					cout << "ANGLES, compton: " << cone.m_comptonAngle*180.0/kPI << " angleY: " << angleMatY*180/kPI << " angleZ: " << angleMatZ*180/kPI << endl;
				}

				DoRotations( angleMatY, angleMatZ, cone, ievent);
		
				m_debug = false;

				ievent++;
			}
		}
		if (m_debug) cout << endl;
	}
}

// ***********************************************************************************
void RotateFOVInitialize()
{
	double m_xmin = -75.0;
	double m_xmax = 75.0;
	double m_ymin = -150.0;
	double m_ymax = 150.0;

	double m_FOV_frontZ = -100.0;

	CUserParameters* userPar = CUserParameters::Instance();

    userPar->m_xmin = m_xmin;
    userPar->m_xmax = m_xmax;

    userPar->m_ymin = m_ymin;
    userPar->m_ymax = m_ymax;

	userPar->m_zmin = m_FOV_frontZ;
	userPar->m_zmax = m_FOV_frontZ;
}

// ***********************************************************************************

void OpenVisualisationFiles( int in_eventID, std::ofstream& io_file1, std::ofstream& io_file2 )
{
	std::string str = "picturecones_forth_";
	std::stringstream strstr;
	strstr << in_eventID;
	str += strstr.str();
	str += ".dat";
	io_file1.open(str.c_str());

	str = "picturecones_back_";
	strstr.str("");
	strstr << in_eventID;
	str += strstr.str();
	str += ".dat";
	io_file2.open(str.c_str());
}

// ***********************************************************************************

void DoRotations(const double& in_angleMatY, const double& in_angleMatZ, const CCone& in_cone, int in_eventID)
{
	// TestUnityMatrix(matZ*matY, matYinv*matZinv);

 	ofstream ofile1;
 	ofstream ofile2;

// to debug scenario 6 solutions...
// 101 207 318 366 414

	// if ( in_eventID == 101 || in_eventID == 102|| in_eventID == 207 || in_eventID == 318 || in_eventID == 366 || in_eventID == 414 )
	{
		OpenVisualisationFiles( in_eventID, ofile1, ofile2 );

		// VisualisationStuff
		VisualisationStuff( in_cone.GetComptonAngle(), in_cone.GetComptonAxisOrigin(), in_angleMatY, in_angleMatZ, ofile1, ofile2 );
		ofile1.close();
	}

	// "old" way --------------------------------------
	//	OldWay( circle_radius, in_theta_C, in_scatPos, matZinv, matYinv, ofile2 );

	// new way ----------------------------------------
	// if ( in_eventID == 101 || in_eventID == 102|| in_eventID == 207 || in_eventID == 318 || in_eventID == 366 || in_eventID == 414 )
	{
		// cout << "NEW WAY---------------------------------------" << endl;
		CConeExtension coneExtension;
		coneExtension.SetAngles( in_angleMatY, in_angleMatZ );
		coneExtension.CalculatePhiRanges( in_cone, &ofile2 );

		ConePhiRange phiRange = coneExtension.GetConePhiRange();

//		cout << phiRange.scenario << " " << phiRange.phi_min*180/kPI << " " << phiRange.phi_max*180/kPI << endl;

		// to debug scenario 6 solutions...
		if (phiRange.scenario == 6)
		{
			cout << "GOTCHA! " << in_eventID << " " << phiRange.scenario << " " << phiRange.phi_min*180/kPI << " " << phiRange.phi_max*180/kPI << endl;
			cout << endl;
		}
	}
	ofile2.close();
}

// ************************************************************************************
//
bool DoRotateCircleUntoFOV(int in_angle, const double& in_ComptonAngle, const C3Vector& in_scatPos
        , const C3Matrix& in_matY, const C3Matrix& in_matZ, C3Vector& io_vector, ofstream& io_ofile)
{
    // starting vector with length 1 on a circle
    io_ofile << 0 << " " << in_angle << " "
             << io_vector.GetX() << " " << io_vector.GetY() << " " << io_vector.GetZ() << endl;

    // rotate around Y-axis
    C3Vector vecp = in_matY * io_vector;

    // rotate around Z-axis
    C3Vector vecpp = in_matZ * vecp;

    // extrapolate to image source z-position
	CUserParameters* userPar = CUserParameters::Instance();
    double target_z = userPar->GetUpperZ() - in_scatPos.GetZ();
    double factor = (vecpp.GetZ() != 0) ? target_z / vecpp.GetZ() : 0.0;

    // CHECK that vecpp length IS 1 BEFORE PROJECTING IT ON THE DETECTOR !!!!!!!!!!!!!!!!!!
    double length = sqrt(vecpp.GetX()*vecpp.GetX() + vecpp.GetY()*vecpp.GetY() + vecpp.GetZ()*vecpp.GetZ());
    assert ( (int) (length + 0.5) == 1);

	bool ok = true;
	if (factor > 0.0)
	{
		io_vector.Set(   factor * vecpp.GetX()
					, factor * vecpp.GetY()
					, factor * vecpp.GetZ() );

		// Translate to scatter point (vertex origin of cone)
		io_vector = in_scatPos + io_vector;
	    io_ofile << 1 << " " << in_angle << " " << vecp.GetX() << " " << vecp.GetY() << " " << vecp.GetZ() << endl;

	    io_ofile << 2 << " " << in_angle << " " << vecpp.GetX() << " " << vecpp.GetY() << " " << vecpp.GetZ() << endl;

	    io_ofile << 3 << " " << in_angle << " "
             << io_vector.GetX() << " " << io_vector.GetY() << " " << io_vector.GetZ() << endl;
	}
	else
	{
		io_vector.Set(-1, -1, -1);
		ok = false;
	}

	return ok;
}

// *************************************************************

bool IsCircleOutsideRotatedFOV(const double& in_circle_radius
        , const C3Vector& in_corner1, const C3Vector& in_corner2
        , const C3Vector& in_corner3, const C3Vector& in_corner4)
{
    double rotatedFOV_xmin;
    double rotatedFOV_xmax;
    double rotatedFOV_ymin;
    double rotatedFOV_ymax;

	GetRotatedFOV(	  in_corner1, in_corner2, in_corner3, in_corner4
					, rotatedFOV_xmin, rotatedFOV_xmax, rotatedFOV_ymin, rotatedFOV_ymax );

    if (    in_circle_radius > 0.5 * (rotatedFOV_ymax - rotatedFOV_ymin)
         || in_circle_radius > 0.5 * (rotatedFOV_xmax - rotatedFOV_xmin))
    {
        return true;
    }
    return false;
}

// *************************************************************

bool IsPointOutsideRotatedFOV(const C3Vector& in_position
        , const C3Vector& in_corner1, const C3Vector& in_corner2
        , const C3Vector& in_corner3, const C3Vector& in_corner4)
{

    double rotatedFOV_xmin;
    double rotatedFOV_xmax;
    double rotatedFOV_ymin;
    double rotatedFOV_ymax;

	GetRotatedFOV(	in_corner1, in_corner2, in_corner3, in_corner4
				  , rotatedFOV_xmin, rotatedFOV_xmax, rotatedFOV_ymin, rotatedFOV_ymax );

	// is position inside rotated FOV?
	if (    rotatedFOV_xmin < in_position.GetX() && in_position.GetX() < rotatedFOV_xmax
		 && rotatedFOV_ymin < in_position.GetY() && in_position.GetY() < rotatedFOV_ymax )
	{
		return false;
	}
	// else...
	return true;
}

// *************************************************************

void GetRotatedFOV(	  const C3Vector& in_corner1, const C3Vector& in_corner2
			        , const C3Vector& in_corner3, const C3Vector& in_corner4
					, double& io_rotatedFOV_xmin, double& io_rotatedFOV_xmax
					, double& io_rotatedFOV_ymin, double& io_rotatedFOV_ymax )
{
    double xarr[4], yarr[4];
    xarr[0] = in_corner1.GetX();
    xarr[1] = in_corner2.GetX();
    xarr[2] = in_corner3.GetX();
    xarr[3] = in_corner4.GetX();
    yarr[0] = in_corner1.GetY();
    yarr[1] = in_corner2.GetY();
    yarr[2] = in_corner3.GetY();
    yarr[3] = in_corner4.GetY();

    bubbleSort( 4, xarr );
    bubbleSort( 4, yarr );

    io_rotatedFOV_xmin = xarr[0];
    io_rotatedFOV_xmax = xarr[3];
    io_rotatedFOV_ymin = yarr[0];
    io_rotatedFOV_ymax = yarr[3];
}


// *************************************************************

void TestUnityMatrix(const C3Matrix& in_mat1, const C3Matrix& in_mat2)
{
	// test unity matrix ------------------------------
	C3Matrix tmp = in_mat2 * in_mat1;

	cout << "unity matrix: " << endl;
	for (int irow = 0; irow < 3; irow++)
	{
		for (int icol = 0; icol < 3; icol++)
		{
			int kdigit = (int) (tmp.GetElement(icol, irow) * 1000.0);
			double value =  (double) ( kdigit / 1000.0 ) ;
			int digit1 = (int) (value + 0.5);	// rounding off...
			cout << digit1 << "  ";
		}
		cout << endl;
	}
}

// ************************************************************************************
void RotateCorners(C3Vector& in_corner1, C3Vector& in_corner2, C3Vector& in_corner3, C3Vector& in_corner4
		, const double& in_ComptonAngle, const C3Vector& in_scatPos
       	, const C3Matrix& in_matZinv, const C3Matrix& in_matYinv, ofstream& io_ofile )
//        , const C3Matrix* in_matY, const C3Matrix* in_matZ)
{
	CUserParameters* userPar = CUserParameters::Instance();

	// GetUpperZ is nearest to the scatterer
	double zposition = userPar->GetUpperZ();
    in_corner1.Set(userPar->GetLowerX(), userPar->GetLowerY(), zposition);
	in_corner2.Set(userPar->GetLowerX(), userPar->GetUpperY(), zposition);
	in_corner3.Set(userPar->GetUpperX(), userPar->GetUpperY(), zposition);
	in_corner4.Set(userPar->GetUpperX(), userPar->GetLowerY(), zposition);

    int iangle = -2;

    DoRotateFOVUntoCircle(iangle, in_ComptonAngle, in_scatPos, in_matZinv, in_matYinv, in_corner1, &io_ofile); 
    DoRotateFOVUntoCircle(iangle, in_ComptonAngle, in_scatPos, in_matZinv, in_matYinv, in_corner2, &io_ofile); 
    DoRotateFOVUntoCircle(iangle, in_ComptonAngle, in_scatPos, in_matZinv, in_matYinv, in_corner3, &io_ofile); 
    DoRotateFOVUntoCircle(iangle, in_ComptonAngle, in_scatPos, in_matZinv, in_matYinv, in_corner4, &io_ofile); 
}

// ************************************************************************************
// Find various

void FindSolutionsInCircle(const double& in_circle_radius
                , const C3Vector& in_corner1, const C3Vector& in_corner2
                , const C3Vector& in_corner3, const C3Vector& in_corner4
                , int& io_numSolutionsEdge1, C3Vector& io_edge1_sol1, C3Vector& io_edge1_sol2
                , int& io_numSolutionsEdge2, C3Vector& io_edge2_sol1, C3Vector& io_edge2_sol2
                , int& io_numSolutionsEdge3, C3Vector& io_edge3_sol1, C3Vector& io_edge3_sol2
                , int& io_numSolutionsEdge4, C3Vector& io_edge4_sol1, C3Vector& io_edge4_sol2)
{
	//
	// See which scenario this event is (and calculate phi range)
	//

//			2
//       	-------------
//	1  	|		   	|  3
//	   	|			|
//		-------------
//			4
//

	// Apply formula to find solution
	io_numSolutionsEdge1 = FindIntersectionsInCircle(in_circle_radius, in_corner1, in_corner2, io_edge1_sol1, io_edge1_sol2);
	// cout << "edge1: " << edge1_sol1.X << " " << edge1_sol1.Y << " " << edge1_sol2.X << " " << edge1_sol2.Y << endl;

	// Apply formula to find solution
	io_numSolutionsEdge2 = FindIntersectionsInCircle(in_circle_radius, in_corner2, in_corner3, io_edge2_sol1, io_edge2_sol2);
	// cout << "edge2: " << io_edge2_sol1.GetX() << " " << io_edge2_sol1.GetY() << " " << io_edge2_sol2.GetX() << " " << io_edge2_sol2.GetY() << endl;

	// Apply formula to find solution
	io_numSolutionsEdge3 = FindIntersectionsInCircle(in_circle_radius, in_corner3, in_corner4, io_edge3_sol1, io_edge3_sol2);
	// cout << "edge3: " << io_edge3_sol1.GetX() << " " << io_edge3_sol1.GetY() << " " << io_edge3_sol2.GetX() << " " << io_edge3_sol2.GetY() << endl;

	// Apply formula to find solution
	io_numSolutionsEdge4 = FindIntersectionsInCircle(in_circle_radius, in_corner4, in_corner1, io_edge4_sol1, io_edge4_sol2);
	// cout << "edge4: " << io_edge4_sol1.GetX() << " " << io_edge4_sol1.GetY() << " " << io_edge4_sol2.GetX() << " " << io_edge4_sol2.GetY() << endl;
}

// *********************************************************************************************************

// Apply formula to find solution, for equation where the circle crosses the lines of the back-rotated square

int FindIntersectionsInCircle(const double& in_circle_radius, const C3Vector& in_corner1, const C3Vector& in_corner2
					, C3Vector& out_coord1, C3Vector& out_coord2)
{
	int numSolutions = 0;

	out_coord1.Set(0, 0, 0);
	out_coord2.Set(0, 0, 0);

	double x_1 = in_corner1.GetX();
	double y_1 = in_corner1.GetY();
	double x_2 = in_corner2.GetX();
	double y_2 = in_corner2.GetY();

	double xcmin = (x_1 <  x_2) ? x_1 : x_2;
	double xcmax = (x_1 >= x_2) ? x_1 : x_2;
	double ycmin = (y_1 <  y_2) ? y_1 : y_2;
	double ycmax = (y_1 >= y_2) ? y_1 : y_2;

	double xsol1, ysol1, xsol2, ysol2;

	double radius2 = ( in_circle_radius * in_circle_radius );
	//
	if (x_2 == x_1) // straight vertical line ( == xcmin == xcmax )
	{
		double dis = radius2 - x_1*x_1;
		if (dis >= 0)  // at least 1 solution
		{
			xsol1 = x_1;
			ysol1 = sqrt(dis);
			bool valid1 = (ysol1 >= ycmin && ysol1 <= ycmax);
			if (valid1)
			{
				numSolutions++;
				out_coord1.SetX(x_1);
				out_coord1.SetY(ysol1);
			}

			if (dis > 0) 	// two solutions
			{
				xsol2 = xsol1;
				ysol2 = -sqrt(dis);
				bool valid2 = (ysol2 >= ycmin && ysol2 <= ycmax);
				if (valid2)
				{
					numSolutions++;
					if (valid1)
					{
						out_coord2.SetX(x_2);
						out_coord2.SetY(ysol2);
					}
					else
					{
						out_coord1.SetX(x_2);
						out_coord1.SetY(ysol2);
					}
				}
			}
		}
	}
	else if (y_2 == y_1) // straight horizontal line ( == yxmin == yxmax )
	{
		double dis = radius2 - y_1*y_1;
		if (dis >= 0)
		{
			xsol1 = sqrt(dis);
			ysol1 = y_1;
			bool valid1 = (xsol1 >= xcmin && xsol1 <= xcmax);
			if (valid1)
			{
				numSolutions++;
				out_coord1.SetX(x_1);
				out_coord1.SetY(ysol1);
			}

			if (dis > 0) 	// two solutions
			{
				xsol2 = -sqrt(dis);
				ysol2 = ysol1;
				bool valid2 = (xsol2 >= xcmin && xsol2 <= xcmax);
				if (valid2)
				{
					numSolutions++;
					if (valid1)
					{
						out_coord2.SetX(x_2);
						out_coord2.SetY(ysol2);
					}
					else
					{
						out_coord1.SetX(x_2);
						out_coord1.SetY(ysol2);
					}
				}
			}
		}
	}
	else // not a horizontal or vertical line
	{
		double a_12 = (y_2 - y_1) / (x_2 - x_1);
		double b_12 = y_1 + a_12 * ( 0 - x_1);

		// line-equation:
		//
		// y = b_12 + a_12 * x;
		// combine with circle: x^2 + y^2 = circle_radius*circle_radius
		// x^2 + b_12^2 + 2*b_21*a_12*x + a_12^2 * x^2 = circle_radius^2
		// (1 + a_12^2) * x^2 + (2*b_12*a_12) * x + b_12^2 - circle_radius^2 = 0
		//
		double eq_a = 1 + (a_12 * a_12);
		double eq_b = 2 * b_12 * a_12;
		double eq_c = (b_12 * b_12) - radius2;

		double dis = (eq_b*eq_b - 4.0 * eq_a * eq_c);
		if (dis >= 0)
		{
			// cout << idum << "  solution dis: " << dis << " a: " << eq_a << " b: " << eq_b << " c: " << eq_c << endl;
			dis = sqrt(dis);
			xsol1 = (-eq_b + dis) / (2.0 * eq_a);
			ysol1 = b_12 + a_12 * xsol1;

			bool valid1 = (xsol1 >= xcmin && xsol1 <= xcmax && ysol1 >= ycmin && ysol1 <= ycmax);
			if (valid1)
			{
				numSolutions++;
				out_coord1.SetX(xsol1);
				out_coord1.SetY(ysol1);
			}

			if (dis > 0) // two solutions
			{
				xsol2 = (-eq_b - dis) / (2.0 * eq_a);
				ysol2 = b_12 + a_12 * xsol2;
				bool valid2 = (xsol2 >= xcmin && xsol2 <= xcmax && ysol2 >= ycmin && ysol2 <= ycmax);
				if (valid2)
				{
					numSolutions++;
					if (valid1)
					{
						out_coord2.SetX(xsol2);
						out_coord2.SetY(ysol2);
					}
					else
					{
						out_coord1.SetX(xsol2);
						out_coord1.SetY(ysol2);
					}
				}
			}
		}
	}
	return numSolutions;
}

// *********************************************************************************************************

void OldWay( const double& in_circle_radius, const double& in_theta_C, const C3Vector& in_scatPos
	, const C3Matrix& in_matZinv, const C3Matrix& in_matYinv, std::ofstream& io_ofile2 )
{
	cout << "OLD WAY---------------------------------------" << endl;

	double phi1 = 0;
	double phi2 = 0;
	double phi3 = 0;
	double phi4 = 0;

	C3Vector edge1_sol1, edge1_sol2;
	C3Vector edge2_sol1, edge2_sol2;
	C3Vector edge3_sol1, edge3_sol2;
	C3Vector edge4_sol1, edge4_sol2;

	int numSolutionsEdge1, numSolutionsEdge2, numSolutionsEdge3, numSolutionsEdge4;

	// Back rotate corners of FOV (= boundaries of SOE image density matrix)
	// I want my corners...
	C3Vector corner1;
	C3Vector corner2;
	C3Vector corner3;
	C3Vector corner4;

	RotateCorners(corner1, corner2, corner3, corner4, in_theta_C, in_scatPos, in_matZinv, in_matYinv, io_ofile2); 

	FindSolutionsInCircle(in_circle_radius, corner1, corner2, corner3, corner4
				, numSolutionsEdge1, edge1_sol1, edge1_sol2
				, numSolutionsEdge2, edge2_sol1, edge2_sol2
				, numSolutionsEdge3, edge3_sol1, edge3_sol2
				, numSolutionsEdge4, edge4_sol1, edge4_sol2);

	// Scenario 1: circle too big or completely inside FOV
	//
	bool isOutside = false;
	if (numSolutionsEdge1 == 0 && numSolutionsEdge2 == 0 && numSolutionsEdge3 == 0 && numSolutionsEdge4 == 0)
	{
		isOutside = IsCircleOutsideRotatedFOV(in_circle_radius, corner1, corner2, corner3, corner4);
	}
	if (!isOutside)
	{
		ConePhiRange io_phiRange;
		int scenario = FindPhiRanges (    numSolutionsEdge1, edge1_sol1, edge1_sol2
										, numSolutionsEdge2, edge2_sol1, edge2_sol2
										, numSolutionsEdge3, edge3_sol1, edge3_sol2
										, numSolutionsEdge4, edge4_sol1, edge4_sol2
										, io_phiRange);

		if (scenario == 5)
		{
			C3Vector point(0, 0, 0);
			isOutside = IsPointOutsideRotatedFOV(point, corner1, corner2, corner3, corner4);
			if (!isOutside)
			{
				// if the centre of the circle is inside rotated FOV, take biggest phi range
				if (io_phiRange.phi_max < io_phiRange.phi_min + kPI)
				{
					double tmp = io_phiRange.phi_min;
					io_phiRange.phi_min = io_phiRange.phi_max - 2 * kPI;
					io_phiRange.phi_max = tmp;
				}
				cout << "CORRECTION, scenario: " << scenario 
					 << " phi_min_1: " << io_phiRange.phi_min*180/kPI << " phi_max_1: " << io_phiRange.phi_max*180/kPI << endl;
			}
		}
	}
}

// ****************************************************************************************

void VisualisationStuff( const double& in_theta_C, const C3Vector& in_scatPos, const double& in_angleMatY, const double& in_angleMatZ
	, std::ofstream& in_ofile1, std::ofstream& in_ofile2 )
{
	// the front matrices
	C3Matrix matY( C3Matrix::Axis_Y, in_angleMatY);
 	C3Matrix matZ( C3Matrix::Axis_Z, in_angleMatZ);

	// and the back matrices
 	C3Matrix matZinv( C3Matrix::Axis_Z, -1*in_angleMatZ);
	C3Matrix matYinv( C3Matrix::Axis_Y, -1*in_angleMatY);

    // loop over various different phi-angles...
	double z_circle_distance = cos(in_theta_C);
	double circle_radius = fabs(sin(in_theta_C));
	for (int iangle = 0; iangle < 360; iangle++)
	{
		double phi = iangle * (2.* kPI) / 360;

		// vector with length 1 (on cone with angle Theta_C)
		double x = cos(phi) * circle_radius;
		double y = sin(phi) * circle_radius;
		double z = z_circle_distance;

		// ROTATION CIRCLE UNTO FOV
		C3Vector cirvec(x, y, z);
		bool ok = DoRotateCircleUntoFOV(iangle, in_theta_C, in_scatPos, matY, matZ, cirvec, in_ofile1);

		// NOW GOING BACK ---------------------------
		if (ok)
			DoRotateFOVUntoCircle(iangle, in_theta_C, in_scatPos, matZinv, matYinv, cirvec, &in_ofile2);
	}

    // Back-rotate FOV edges
	int iangle = -1;
    double x, y;

	CUserParameters* userPar = CUserParameters::Instance();
    double xmin = userPar->GetLowerX();
    double xmax = userPar->GetUpperX();
   	double ymin = userPar->GetLowerY();
    double ymax = userPar->GetUpperY();

    for (int iedge = 0; iedge < 4; iedge++)
    {
        double hmin = (iedge < 2) ? ymin : xmin;
        double hmax = (iedge < 2) ? ymax : xmax;

        if (iedge == 0)
		{
			x = xmin; hmin = ymin; hmax = ymax;
		}
        else if (iedge == 1)
		{
			x = xmax; hmin = ymin; hmax = ymax;
		}
        else if (iedge == 2)
		{
			y = ymin; hmin = xmin; hmax = xmax;
		}
        else if (iedge == 3)
		{
			y = ymax; hmin = xmin; hmax = xmax;
		}

        for (double h = hmin; h <= hmax; h += 1.0)
        {
            if ( iedge < 2 ) y = h;
            else x = h;

            C3Vector fovvec(x, y, userPar->GetUpperZ());
			bool isCorner = (iedge<2) && (h == hmin || h == hmax);
            DoRotateFOVUntoCircle(iangle, in_theta_C, in_scatPos, matZinv, matYinv, fovvec, &in_ofile2, isCorner );
        }
    }
}

// ****************************************************************************************






