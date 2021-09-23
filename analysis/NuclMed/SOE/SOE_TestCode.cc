
#include "CUserParameters.h"
#include "CCurrentState.h"
#include "C3Vector.h"
#include "C3Matrix.h"
#include "CEvent.h"
#include "CHit.h"
#include "CCone.h"
#include "CEventManager.h"
#include "constants.h"

// #include "CUniformRandomGenerator.h"

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandFlat.h"

//C/C++ headers
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <cmath>

#include <cassert>

void TestC3Vector();
void TestC3Matrix();
void TestCEvent();
void TestCCone();

void RotationTests();

void TestCCurrentState();

// void TestBoostRandomGenerator();
void TestCLHEPRandomGenerator();

void TestSerializeVector();
void TestSerializeMatrix();
void TestSerializeStdMap();
void TestSerializeCone();
void TestSerializeCEventMap();
void TestSerializeCHit();

void TestEventManager();

void GetValidEvents();

CUserParameters* m_userPar = NULL;

using namespace std;

std::string basedir = "tmp/";

// =============================================================================================================

int main(int argc, char *argv[])
{
	// --------------------------------------------
	// TEST ROTATIONS...
	RotationTests();

	// --------------------------------------------
	// TEST USERPARAMETERS
	cout << "TEST CUserParameters: ";
	{
		CUserParameters::Destroy();
		CUserParameters::Instance();
		CUserParameters::Destroy();
		m_userPar = CUserParameters::Instance();
		cout << endl;
	}
	cout << "OK" << endl;
	cout << endl;

	// --------------------------------------------
	// TEST C3Vector
	TestC3Vector();

	// --------------------------------------------
	// TEST C3Matrix
	TestC3Matrix();

	// --------------------------------------------
	// TEST CEvent
	TestCEvent();

	// --------------------------------------------
	// TEST CCone
	TestCCone();

	// --------------------------------------------
	// TEST CUniformRandomGenerator
	// TestBoostRandomGenerator();
	TestCLHEPRandomGenerator();

	// --------------------------------------------
	// TEST (de)serialization C3Vector, C3Matrix, CCone

	TestSerializeVector();
	TestSerializeMatrix();
	// TestSerializeStdMap();
	TestSerializeCone();
	TestSerializeCHit();
	TestSerializeCEventMap();

	TestCCurrentState();

	// --------------------------------------------
	// TEST CEventManager
 	TestEventManager();


	CUserParameters::Destroy();
}

// =============================================================================================================

void TestC3Vector()
{
	cout << "TEST C3Vector: ";
	// adding two vectors
	{
		C3Vector v0(0.0, 0.0, 0.0);
		C3Vector v1, v2, v3;
		v3 = v1 + v2;
		assert (v3 == v0);
	}
	// adding two vectors
	{
		double a1 = CLHEP::RandFlat::shoot( 100.0 );
		double a2 = CLHEP::RandFlat::shoot( 100.0 );
		double a3 = CLHEP::RandFlat::shoot( 100.0 );
		double b1 = CLHEP::RandFlat::shoot( 100.0 );
		double b2 = CLHEP::RandFlat::shoot( 100.0 );
		double b3 = CLHEP::RandFlat::shoot( 100.0 );
		C3Vector vplus(a1 + b1, a2 + b2, a3 + b3);
		C3Vector a( a1, a2, a3 );
		C3Vector b( b1, b2, b3 );
		C3Vector c;
		c = a + b;
		assert (c == vplus);
		assert (a != vplus);
		assert (b != vplus);
		assert (a != c);
		assert (b != c);
	}
	// subtracting two vectors
	{
		double a1 = CLHEP::RandFlat::shoot( 100.0 );
		double a2 = CLHEP::RandFlat::shoot( 100.0 );
		double a3 = CLHEP::RandFlat::shoot( 100.0 );
		double b1 = CLHEP::RandFlat::shoot( 100.0 );
		double b2 = CLHEP::RandFlat::shoot( 100.0 );
		double b3 = CLHEP::RandFlat::shoot( 100.0 );
		C3Vector vmin(a1 - b1, a2 - b2, a3 - b3);
		C3Vector a( a1, a2, a3 );
		C3Vector b( b1, b2, b3 );
		C3Vector c;
		c = a - b;
		assert (c == vmin);
		assert (a != vmin);
		assert (b != vmin);
		assert (a != c);
		assert (b != c);
	}

	// scalarproduct
	{
		C3Vector v1( 1.0, 0.0,  0);
		C3Vector v2( 0.0, -3.2, 0);

		double inp = v1 * v2;
		assert(inp == 0);

		double angle1 = v1.GetScalarProductAngle(v2);
		double angle2 = v2.GetScalarProductAngle(v1);

		assert (angle1 == angle2);
		assert (sin(angle1) == 1);

		assert (cos(angle1) < 0.00001);	// cos doubles never get really 0
	}
	cout << " OK" << endl;
	cout << endl;
}

// =============================================================================================================

void TestC3Matrix()
{
	cout << "TEST C3Matrix: ";
	{
		// 0 degrees
		C3Matrix matX( C3Matrix::Axis_X, 0.0 );
		C3Matrix matY( C3Matrix::Axis_Y, 0.0 );
		C3Matrix matZ( C3Matrix::Axis_Z, 0.0 );
		C3Vector v0(1.0, 0.0, 0.0);
		C3Vector v1(0.0, 1.0, 0.0);
		C3Vector v2(0.0, 0.0, 1.0);
		C3Matrix mat1( v0, v1, v2 );

		assert (matX == mat1);
		assert (matY == mat1);
		assert (matZ == mat1);
	}
	{
		// 90 degrees around Z		vy = MatZ * vx
		C3Matrix matZ( C3Matrix::Axis_Z, 0.5 * kPI );
		C3Vector vx(1.0, 0.0, 0.0);
		C3Vector vy(0.0, 1.0, 0.0);

		C3Vector vec = matZ * vx;

		assert (vec == vy);
	}
	{
		// 90 degrees around Y		vx = MatY * vZ
		C3Matrix matY( C3Matrix::Axis_Y, 0.5 * kPI );
		C3Vector vx(1.0, 0.0, 0.0);
		C3Vector vz(0.0, 0.0, 1.0);

		C3Vector vec = matY * vz;

		assert (vec == vx);
	}
	{
		// 90 degrees around X		vz = MatX * vy
		C3Matrix matX( C3Matrix::Axis_X, 0.5 * kPI );
		C3Vector vy(0.0, 1.0, 0.0);
		C3Vector vz(0.0, 0.0, 1.0);

		C3Vector vec = matX * vy;

		assert (vec == vz);
	}
	{
		// matrix * vector multiplication;
		C3Vector vec(1.0, 2.0, 3.0);
		C3Vector col1(1.0, 2.0, 3.0);
		C3Matrix mat(col1, col1, col1);

		C3Vector check(6.0, 12.0, 18.0);
		C3Vector out = mat*vec;

		assert (out == check);
	}

	cout << " OK" << endl;
	cout << endl;
}

// =============================================================================================================

void RotationTests()
{
	cout << "TEST ROTATIONs: ";
	{
		// Rotation tests...
		C3Vector axisZ( 0.0, 0.0, 1.0);
		C3Vector axisX( 1.0, 0.0, 0.0);

		CCone aCone;
		double anAngle = 0.35;
		C3Vector vec0( 0.0, 0.0, 0.0 );

		for (int ix = -100; ix < 100; ix += 20)
		{
			for (int iy = -100; iy < 100; iy += 20)
			{
				// Rotation around Y-axis
				C3Vector axisDirection( ix, iy, -100.0);
				double len = axisDirection.GetLength();
				double coeff = (1.0/len);
				axisDirection = axisDirection * coeff;

				aCone.m_comptonAngle = anAngle;
				aCone.m_comptonAxisDirection = axisDirection;
				aCone.m_comptonAxisOrigin = vec0;

				aCone.CalculateRotationMatrices();

				C3Matrix rotMatrixZxY;
				aCone.GetRotationMatrix(rotMatrixZxY);

				// multiply
				C3Vector newvec2 = (rotMatrixZxY) * axisZ;

				// normalize to length = 1;
				len = newvec2.GetLength();
				coeff = (1.0/len);
				newvec2 = newvec2 * coeff;

				if (newvec2 != axisDirection /*|| true*/)
				{
					cout << "axisDir: " << axisDirection << endl;
					cout << "newvec step 2: " << newvec2 << endl;
					cout << endl;
				}

				assert (newvec2 == axisDirection);

				// interesting example:
				/*
				axisDir: -0.486664 0.324443 -0.811107
				angle 1: -2.51684
				newvec step 1: -0.584898 0 -0.811107
				angle 2: -0.588003
				newvec step 2: -0.486664 0.324443 -0.811107
				*/
			}
		}
	}

	// Picture Ellipses
	{
		double angleMatY = 0.785;
		C3Matrix matY( C3Matrix::Axis_Y, angleMatY);

		double angleMatZ = 3.9285;
 		C3Matrix matZ( C3Matrix::Axis_Z, angleMatZ);

		double sinTh= 1.0;
		double z = sinTh;
		std::string fff = basedir;
		fff += "/picturecones.dat";
 		ofstream ofile(fff.c_str());
		for (int iangle = 0; iangle < 360; iangle++)
		{
			double phi = iangle * (2.* kPI) / 360;
			double x = cos(phi);
			double y = sin(phi);

			ofile << 0 << " " << iangle << " " << x << " " << y << endl;

			C3Vector vec(x, y, z);

			C3Vector vecp = matY * vec;
			double len = vecp.GetLength();
			vecp.Set(vecp.GetX()/len, vecp.GetY()/len, vecp.GetZ()/len);

			ofile << 1 << " " << iangle << " " << vecp.GetX() << " " << vecp.GetY() << endl;

			C3Vector vecpp = matZ * vecp;
			len = vecpp.GetLength();
			vecpp.Set(vecpp.GetX()/len, vecpp.GetY()/len, vecpp.GetZ()/len);

			ofile << 2 << " " << iangle << " " << vecpp.GetX() << " " << vecpp.GetY() << endl;
		}
	}

	cout << "OK " << endl;
	cout << endl;
}

// =============================================================================================================

void TestCEvent()
{
	cout << "TEST CEvent: ";

    // Event ID
	{
		// check event IDs
		for (int id = 0; id < 200; id+=17)
		{
			CEvent event(id);
			assert(id == event.GetID());
		}
	}

	cout << " OK" << endl;
	cout << endl;
}

// =============================================================================================================

void TestCCone()
{
	cout << "TEST CCone: ";

	cout << "OK" << endl;
	cout << endl;
}

// =============================================================================================================

void TestCCurrentState()
{
	cout << "TEST CCurrentState: ";
	m_userPar->m_eventSetSize = 2;

	// serialization
	{
		int eventSetSize = m_userPar->GetEventSetSize();
		assert (eventSetSize == 2);
		{
			double z1, y1, x1, e1, z2, y2, x2, e2;
			C3Vector hitPosition1, hitPosition2, position;
            CCone cone;
            int error = 0;

            CCurrentState currentState;

			x1 = 19.5; y1 = 9.52; z1 = 6.5; e1 = 72.0883; x2 = 133.5; y2 = -209.74; z2 = 127.5; e2 = 434.852;
            hitPosition1.Set(x1, y1, z1);
            hitPosition2.Set(x2, y2, z2);

            error = cone.Calculate(hitPosition1, e1, hitPosition2, e2);
            assert( error == 0);
            cone.GetRandomPositionOnCone( position );
            currentState.AddToStateVector( 0, position );

			x1 = -161.5; y1 = 214.13; z1 = 18.5; e1 = 284.393; x2 = -135.5; y2 = 88.46; z2 = 127.5; e2 = 226.214;
            hitPosition1.Set(x1, y1, z1);
            hitPosition2.Set(x2, y2, z2);

            error = cone.Calculate(hitPosition1, e1, hitPosition2, e2);
            assert( error == 0);
            cone.GetRandomPositionOnCone( position );
            currentState.AddToStateVector( 1, position );

			x1 = 80.5; y1 = 160.75; z1 = 4.5; e1 = 230.395; x2 = 71.5; y2 = 39.47; z2 = 130.5; e2 = 278.03;
            hitPosition1.Set(x1, y1, z1);
            hitPosition2.Set(x2, y2, z2);

            error = cone.Calculate(hitPosition1, e1, hitPosition2, e2);
            assert( error == 0);
            cone.GetRandomPositionOnCone( position );
            currentState.AddToStateVector( 2, position );

			x1 = -64.5; y1 = 41.6; z1 = 14.5; e1 = 173.004; x2 = -65.5; y2 = 24.56; z2 = 139.5; e2 = 337.921;
            hitPosition1.Set(x1, y1, z1);
            hitPosition2.Set(x2, y2, z2);

            error = cone.Calculate(hitPosition1, e1, hitPosition2, e2);
            assert( error == 0);
            cone.GetRandomPositionOnCone( position );
            currentState.AddToStateVector( 3, position );

			x1 = -43.5; y1 = -118.15; z1 = 1.5; e1 = 44.3054; x2 = -35.5; y2 = -241.69; z2 = 169.5; e2 = 466.263;
            hitPosition1.Set(x1, y1, z1);
            hitPosition2.Set(x2, y2, z2);

            error = cone.Calculate(hitPosition1, e1, hitPosition2, e2);
            assert( error == 0);
            cone.GetRandomPositionOnCone( position );
            currentState.AddToStateVector( 4, position );

			// check for serialized file for set 0
			{
				std::string fname = "CStateVector_";
				std::stringstream setNrStr;
				setNrStr << 0;
				fname += setNrStr.str();
				fname += ".dat";
				std::ifstream infile(fname.c_str());
				assert (infile.is_open());
				infile.close();
			}

			// check for serialized file for set 1
			{
				std::string fname = "CStateVector_";
				std::stringstream setNrStr;
				setNrStr << 1;
				fname += setNrStr.str();
				fname += ".dat";
				std::ifstream infile(fname.c_str());
				assert (infile.is_open());
				infile.close();
			}
			// Update( long in_eventID, const C3Vector& in_position );
			// (Go back to set 0)
			C3Vector dummy(1.0, 2.0, 3.0);
			currentState.Update(0, dummy);

			// check for serialized file for set 2
			{
				std::string fname = "CStateVector_";
				std::stringstream setNrStr;
				setNrStr << 2;
				fname += setNrStr.str();
				fname += ".dat";
				std::ifstream infile(fname.c_str());
				assert (infile.is_open());
				infile.close();
			}

			currentState.Update(1, dummy);
			currentState.Update(2, dummy);
			currentState.Update(3, dummy);
			currentState.Update(4, dummy);
		}
	}

	cout << " OK " << endl;
}


// =============================================================================================================

/*
void TestBoostRandomGenerator()
{
	std::string fff = basedir;
	fff += "/random.dat";
	ofstream outfile(fff.c_str());
	cout << "TEST CUniformRandomGenerator: ";
	{
		for (int i = 0; i < 100000; i++)
		{
			outfile << CUniformRandomGenerator::Instance()->GetUniformRandomValue(1.0) << endl;
		}
	}

	// random seed, each time different...
	{
		CUniformRandomGenerator::Destroy();
		double a1 = CUniformRandomGenerator::Instance()->GetUniformRandomValue(100.0);
		for (int i = 0; i < 100; i++)
		{
			CUniformRandomGenerator::Destroy();
			double a2 = CUniformRandomGenerator::Instance()->GetUniformRandomValue(100.0);
 			if ( (int) (10000*a2) == (int) (10000*a1) )
			{
				cout << a1 << " " << a2 << endl;
			}
			assert ( (int) (10000*a2) != (int) (10000*a1));
		}
	}

	cout << "OK " << endl;
	cout << endl;
}
*/

void TestCLHEPRandomGenerator()
{
	/*
	long seed;
	ifstream urandom;
	urandom.open("/dev/urandom");
	urandom.read(reinterpret_cast<char*> (&seed), sizeof (seed));
	urandom.close();
	*/

    long seed;
    time_t systime = time(NULL);
    seed = (long) systime;

	CLHEP::HepRandom::setTheSeed(seed);

	cout << "UNIFORM value 1: " << CLHEP::RandFlat::shoot(2.0 * kPI) << endl;
	cout << "UNIFORM value 2: " << CLHEP::RandFlat::shoot(2.0 * kPI) << endl;
	cout << "UNIFORM value 3: " << CLHEP::RandFlat::shoot(2.0 * kPI) << endl;

	cout << "GAUSS value 1: " << CLHEP::RandGauss::shoot(0.0, 2.0) << endl;
	cout << "GAUSS value 2: " << CLHEP::RandGauss::shoot(0.0, 2.0) << endl;
	cout << "GAUSS value 3: " << CLHEP::RandGauss::shoot(0.0, 2.0) << endl;

	std::string fff = basedir;
	fff += "/random.dat";
	ofstream outfile(fff.c_str());
	cout << "TEST CLHEP::RandFlat: ";
	{
		for (int i = 0; i < 100000; i++)
		{
			outfile << CLHEP::RandFlat::shoot(1.0) << endl;
		}
	}

	// random seed, each time different...
	{
		double a1 = CLHEP::RandFlat::shoot(100.0);
		for (int i = 0; i < 100; i++)
		{
			double a2 = CLHEP::RandFlat::shoot(100.0);
			bool isEqual = doubleEquals(a1, a2, 0.0001);
 			if ( isEqual )
			{
				cout << a1 << " " << a2 << endl;
			}
			assert ( !isEqual );
		}
	}

	cout << "OK " << endl;
	cout << endl;
}


// =============================================================================================================

void TestSerializeVector()
{
	cout << "TESTING BOOST SERIALIZE VECTOR: ";

	// object of class C3Vector
	const C3Vector vector1 (5.1, 6.2, 7.3);
		// IMPORTANT, should be defined as a const!!!

	std::stringstream ss;
	vector1.Serialize(ss);

	C3Vector vector2;
	vector2.Deserialize(ss);

	if (vector1 != vector2)
	{
		cout << "vector1: " << vector1 << endl;
		cout << "vector2: " << vector2 << endl;
	}
	assert (vector1 == vector2);

	cout << " OK" << endl;
}

// =============================================================================================================

void TestSerializeMatrix()
{
	cout << "TESTING BOOST SERIALIZE MATRIX: " ;

	// object of class C3Matrix
	const C3Matrix matrix1a (C3Matrix::Axis_X, 0.3);
		// IMPORTANT, should be defined as a const!!!
	const C3Matrix matrix1b (C3Matrix::Axis_Y, 2.7);

	// Serialization
	std::string fff = basedir;
	fff += "/matrixarch.dat";
	std::ofstream ofs(fff.c_str(), ios::binary);
	if (!ofs.is_open())
	{
		cout << "WTF! file not open: " << fff.c_str() << endl;
		exit(1);
	}

	matrix1a.Serialize(ofs);
	matrix1b.Serialize(ofs);

	ofs.close();

	// Deserialization
	C3Matrix matrix2a;
	C3Matrix matrix2b;

	std::ifstream ifs(fff.c_str());
	matrix2a.Deserialize(ifs);
	matrix2b.Deserialize(ifs);

	assert (matrix1a == matrix2a);
	assert (matrix1b == matrix2b);

	cout << " OK" << endl;
}

// =============================================================================================================

/*
void TestSerializeStdMap()
{
	cout << "TESTING BOOST SERIALIZE std::map: " ;

	// test 1
	{
		std::map<int, std::string> theMap1;
		theMap1[0] = std::string("zero");
		theMap1[1] = std::string("ett");
		theMap1[2] = std::string("two");
		theMap1[3] = std::string("drie");

		const std::map<int, std::string>& tmpMap1 = theMap1;

		std::stringstream ss;
		boost::archive::binary_oarchive oarch(ss);
		oarch << tmpMap1;

		// ------------

		std::map<int, std::string> theMap2;
		boost::archive::binary_iarchive iarch(ss);
		iarch >> theMap2;

		for (int i = 0; i < theMap2.size(); i++)
		{
			assert( theMap1[i] == theMap2[i] );
		}

		assert( theMap1 == theMap2 );
	}

	// test 2
	{
		std::map<int, CCone*> theMap1;

		double z1, y1, x1, e1, z2, y2, x2, e2;

		CCone* cone0 = new CCone();
		x1 = 19.5; y1 = 9.52; z1 = 6.5; e1 = 72.0883; x2 = 133.5; y2 = -209.74; z2 = 127.5; e2 = 434.852;
		cone0->Calculate( C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2 );
		double comptonAngle0 = cone0->GetComptonAngle();

		CCone* cone1 = new CCone();
		x1 = -161.5; y1 = 214.13; z1 = 18.5; e1 = 284.393; x2 = -135.5; y2 = 88.46; z2 = 127.5; e2 = 226.214;
		cone1->Calculate( C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2 );
		double comptonAngle1 = cone1->GetComptonAngle();

		CCone* cone2 = new CCone();
		x1 = 80.5; y1 = 160.75; z1 = 4.5; e1 = 230.395; x2 = 71.5; y2 = 39.47; z2 = 130.5; e2 = 278.03;
		cone2->Calculate( C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2 );
		double comptonAngle2 = cone2->GetComptonAngle();

		CCone* cone3 = new CCone();
		x1 = -64.5; y1 = 41.6; z1 = 14.5; e1 = 173.004; x2 = -65.5; y2 = 24.56; z2 = 139.5; e2 = 337.921;
		cone3->Calculate( C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2 );
		double comptonAngle3 = cone3->GetComptonAngle();

		theMap1[0] = cone0;
		theMap1[1] = cone1;
		theMap1[2] = cone2;
		theMap1[3] = cone3;

		const std::map<int, CCone*>& tmpMap1 = theMap1;

		std::stringstream ss;
		boost::archive::binary_oarchive oarch(ss);

		oarch << tmpMap1;

		// cout << endl;
		for (int i = 0; i < theMap1.size(); i++)
		{
			CCone* cone1 = theMap1[i];
			assert ( cone1 != 0 );
			// cout << "Map1, angle[" << i << "]: " << cone1->GetComptonAngle() << endl;
		}

		delete cone0;
		delete cone1;
		delete cone2;
		delete cone3;

		// ------------

		std::map<int, CCone*> theMap2;
		boost::archive::binary_iarchive iarch(ss);
		iarch >> theMap2;

		for (int i = 0; i < theMap2.size(); i++)
		{
			CCone* cone2 = theMap2[i];
			assert ( cone2 != 0 );
		}

		assert ( (int) (kMILLION * comptonAngle0 ) == (int) (kMILLION * theMap2[0]->GetComptonAngle() ) );
		assert ( (int) (kMILLION * comptonAngle1 ) == (int) (kMILLION * theMap2[1]->GetComptonAngle() ) );
		assert ( (int) (kMILLION * comptonAngle2 ) == (int) (kMILLION * theMap2[2]->GetComptonAngle() ) );
		assert ( (int) (kMILLION * comptonAngle3 ) == (int) (kMILLION * theMap2[3]->GetComptonAngle() ) );

		// N.B. IMPORTANT!!! Clean map up!!!
		for (int i = 0; i < theMap2.size(); i++)
		{
			CCone* cone2 = theMap2[i];
			delete cone2;
		}
	}

	cout << " OK" << endl;
}
*/

// =============================================================================================================

void TestSerializeCone()
{
	cout << "TESTING BOOST SERIALIZE CONE: ";

	double x1 = 80.5; double y1 = -69.29; double z1 = 2.5; double e1 = 71.7055;
	double x2 = 156.5; double y2 = -58.51; double z2 = 146.5; double e2 = 439.293;

	CCone cone0;
	cone0.Calculate( C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);

	const CCone& cone1 = cone0;
	//
	// HAS TO BE CONST!!! If not:
	// /usr/include/boost/archive/detail/oserializer.hpp:567:
	// error: invalid application of �sizeof� to incomplete type �boost::STATIC_ASSERTION_FAILURE<false>�
	//

	std::stringstream ss1;

	cone1.Serialize(ss1);

	C3Matrix matrix1;
	cone1.GetRotationMatrix(matrix1);

	// ---- 2 ----

	CCone cone2;
	std::stringstream ss2;
	std::string str1 = ss1.str();
	ss2.str(str1);

	cone2.Deserialize(ss1);

	C3Matrix matrix2;
	cone2.GetRotationMatrix(matrix2);

	assert (cone1 == cone2);
	assert (matrix1 == matrix2);

	cout << "OK" << endl;
}

// =============================================================================================================

void TestSerializeCEventMap()
{
	cout << "TESTING BOOST SERIALIZE CEVENT MAP: ";

	// CEvent::Set(  const double& x1, const double& y1, const double& z1, const double& e1
	// 		       , const double& x2, const double& y2, const double& z2, const double& e2 );

	// COMPTON EVENT
	{
		std::stringstream ss;

		double x1 = 80.5; double y1 = -69.29; double z1 = 2.5; double e1 = 71.7055;
		double x2 = 156.5; double y2 = -58.51; double z2 = 146.5; double e2 = 439.293;
		int err = 0;
		CEvent event0(0);
		event0.Set(C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);

		int size1;
		//serialize
		{
			CEventMap eventMap1;
			CEvent tmp0(event0);
			eventMap1[0] = &tmp0;

			const CEventMap& tmpMap = eventMap1;

			std::map<long, CEvent*>::const_iterator mapiter;
			// ss << tmpMap.size() << " ";
			size1 = eventMap1.size();
			ss.write((char*) &size1, sizeof(size1));
			int eventID;
			for (mapiter = tmpMap.begin(); mapiter != tmpMap.end(); mapiter++)
			{
				// ss << ((*mapiter).first) << " ";
				eventID = ((*mapiter).first);
				ss.write((char*) &eventID, sizeof(eventID));

				CEvent* event = ((*mapiter).second);
				assert( event );
				event->Serialize( ss );
			}
		}

		//deserialize
		{
			CEventMap eventMap2;
			/*
			boost::archive::binary_iarchive iarch(ss);
			iarch >> eventMap2;
			*/

			int size2, eventID;
			// ss >> size2;
			ss.read((char*) &size2, sizeof(size2));
			assert(size2 == size1);

			// ss >> eventID;
			ss.read((char*) &eventID, sizeof(eventID));
			assert (eventID == 0);

			CEvent event2;
			event2.Deserialize( ss );

			if ( event2 != event0 )
			{
				cout << "e0 type: " << event0.GetEventType() << " ev type: " << event2.GetEventType() << endl;
				cout << "e0 pos1: " << event0.GetHitPosition1() << " ev pos1: " << event2.GetHitPosition1() << endl;
				cout << "e0 pos2: " << event0.GetHitPosition2() << " ev pos2: " << event2.GetHitPosition2() << endl;
				cout << "e0 E1: " << event0.GetHitE1() << " ev E1: " << event2.GetHitE1() << endl;
				cout << "e0 E2: " << event0.GetHitE2() << " ev E2: " << event2.GetHitE2() << endl;
			}

			assert( event2 == event0);
		}
	}

	cout << "OK" << endl;
}

// =============================================================================================================

void TestSerializeCHit()
{
	cout << "TESTING BOOST SERIALIZE CHIT: ";

	std::stringstream ss;

	double x1 = 80.5; double y1 = -69.29; double z1 = 2.5; double e1 = 71.7055;
	C3Vector position1(x1, y1, z1);
	double x2 = 156.5; double y2 = -58.51; double z2 = 146.5; double e2 = 439.293;
	C3Vector position2(x2, y2, z2);
	int err = 0;
	//serialize
	{
		CHit hit1(position1, e1);
		CHit hit2(position2, e2);

		hit1.Serialize(ss);
		hit2.Serialize(ss);
	}

	//deserialize
	{
		CHit tmpHit;
		tmpHit.Deserialize(ss);

		assert(tmpHit.GetE() == e1);
		assert(tmpHit.GetPosition() == position1);

		tmpHit.Deserialize(ss);

		assert(tmpHit.GetE() == e2);
		assert(tmpHit.GetPosition() == position2);
	}
	cout << "OK" << endl;
}

// =============================================================

void TestEventManager()
{
	cout << "TESTING CEVENTMANAGER: " << endl;

	int oldSetSize = m_userPar->m_eventSetSize;
	m_userPar->m_eventSetSize = 2;
	int eventSetSize = m_userPar->GetEventSetSize();
	assert (eventSetSize == 2);
	// GetEventSetNumber(int in_eventID)
	{
		CEventManager evtMger;

		// SET 0
		int check = evtMger.m_dataSetManager.GetSetNumber(0);
		assert (check == 0);
		check = evtMger.m_dataSetManager.GetSetNumber(1);
		assert (check == 0);
		// SET 1
		check = evtMger.m_dataSetManager.GetSetNumber(2);
		assert (check == 1);
		check = evtMger.m_dataSetManager.GetSetNumber(3);
		assert (check == 1);
		// SET 2
		check = evtMger.m_dataSetManager.GetSetNumber(4);
		assert (check == 2);
	}

	// AddEvent(CEvent& in_event)
	// CEvent*			GetEvent(int in_eventID)
	{
		CEventManager evtMger;
		double z1, y1, x1, e1, z2, y2, x2, e2;

		// SET 0, event ID = 0 and 1
		CEvent event1(0);
		x1 = 19.5; y1 = 9.52; z1 = 6.5; e1 = 72.0883; x2 = 133.5; y2 = -209.74; z2 = 127.5; e2 = 434.852;
		event1.Set(C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);
		evtMger.AddEvent(event1);

		CEvent event2(1);
		x1 = -161.5; y1 = 214.13; z1 = 18.5; e1 = 284.393; x2 = -135.5; y2 = 88.46; z2 = 127.5; e2 = 226.214;
		event2.Set(C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);
		evtMger.AddEvent(event2);

		CEvent* eventCheck = evtMger.GetEvent(1);
		assert (eventCheck != NULL);
		assert (*eventCheck == event2);
		assert (event2.GetID() == 1);

		int evtSetNr = evtMger.m_dataSetManager.GetSetNumber(1);
		assert (evtSetNr == 0);

		// SET 1, event ID = 2 and 3
		CEvent event3(2);
		x1 = 80.5; y1 = 160.75; z1 = 4.5; e1 = 230.395; x2 = 71.5; y2 = 39.47; z2 = 130.5; e2 = 278.03;
		event3.Set(C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);
		evtMger.AddEvent(event3);

		eventCheck = dynamic_cast<CEvent*>( evtMger.GetEvent(2) );
		assert (eventCheck);
		assert (*eventCheck == event3);
		assert (event3.GetID() == 2);

		evtSetNr = evtMger.m_dataSetManager.GetSetNumber(2);
		assert (evtSetNr == 1);

		CEvent event4(3);
		event4.Set(C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);
		evtMger.AddEvent(event4);

		// SET 2, event ID = 4
		CEvent event5 (4);
		x1 = -64.5; y1 = 41.6; z1 = 14.5; e1 = 173.004; x2 = -65.5; y2 = 24.56; z2 = 139.5; e2 = 337.921;
		event5.Set(C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);
		evtMger.AddEvent(event5);

		// check for serialized file for set 1
		{
			std::string fname = "CEventMap_";
			std::stringstream setNrStr;
			setNrStr << 1;
			fname += setNrStr.str();
			fname += ".dat";
			std::ifstream infile(fname.c_str());
			assert (infile.is_open());
			infile.close();
		}

		evtSetNr = evtMger.m_dataSetManager.GetSetNumber(4);
		assert (evtSetNr == 2);

		// back to SET 0, event ID = 75 (not existing)
		CEvent* event6 = dynamic_cast<CEvent*> ( evtMger.GetEvent(75) );
		assert (event6 == NULL);

		// Now, really back to SET 0, event ID = 0
		event6 = dynamic_cast<CEvent*> ( evtMger.GetEvent(0) );
		// check for serialized file for set 2
		{
			std::string fname = "CEventMap_";
			std::stringstream setNrStr;
			setNrStr << 2;
			fname += setNrStr.str();
			fname += ".dat";
			std::ifstream infile(fname.c_str());
			assert (infile.is_open());
			infile.close();
		}

		// check file names
		const std::map<int, std::string>& fnames = evtMger.m_dataSetManager.m_setFileNames;
		cout << "Number of files: " << fnames.size() << endl;
		std::map<int, std::string>::const_iterator iter;
		for (iter = fnames.begin(); iter != fnames.end(); iter++)
		{
			cout << "Filename: " << ((*iter).second) << endl;
		}
	}

	m_userPar->m_eventSetSize = oldSetSize;

	cout << "OK" << endl;
}

// =============================================================

void
GetValidEvents()
{
	std::ifstream infile("CC_img.out");
	if (!infile.is_open())
	{
		cout << "CC_img.out is not open" << endl;
		return;
	}
	bool donext = true;
	double z1, y1, x1, e1, z2, y2, x2, e2;
	int count = 0;
	CCone cone;
	while (donext && infile.is_open())
	{
		infile >> z1 >> y1 >> x1 >> e1 >> z2 >> y2 >> x2 >> e2;
		if (infile.is_open())
		{
			// int err = event.Set(C3Vector(x1, y1, z1), e1, "Scatterer", C3Vector(x2, y2, z2), e2, "Absorber");
			int	err = cone.Calculate(C3Vector(x1, y1, z1), e1, C3Vector(x2, y2, z2), e2);
			if (!err)
			{
				cout << "x1 = " << x1 << "; y1 = " << y1 << "; z1 = " << z1 << "; e1 = " << e1
					 << "; x2 = " << x2 << "; y2 = " << y2 << "; z2 = " << z2 << "; e2 = " << e2 << endl;
				count++;
			}
		}
		if (count == 10) donext = false;
	}
/*
x1 = 19.5; y1 = 9.52; z1 = 6.5; e1 = 72.0883; x2 = 133.5; y2 = -209.74; z2 = 127.5; e2 = 434.852
x1 = -161.5; y1 = 214.13; z1 = 18.5; e1 = 284.393; x2 = -135.5; y2 = 88.46; z2 = 127.5; e2 = 226.214
x1 = 80.5; y1 = 160.75; z1 = 4.5; e1 = 230.395; x2 = 71.5; y2 = 39.47; z2 = 130.5; e2 = 278.03
x1 = -64.5; y1 = 41.6; z1 = 14.5; e1 = 173.004; x2 = -65.5; y2 = 24.56; z2 = 139.5; e2 = 337.921
x1 = -43.5; y1 = -118.15; z1 = 1.5; e1 = 44.3054; x2 = -35.5; y2 = -241.69; z2 = 169.5; e2 = 466.263
x1 = 99.5; y1 = 56.38; z1 = 3.5; e1 = 109.648; x2 = 162.5; y2 = -60.64; z2 = 132.5; e2 = 403.537
x1 = 2.5; y1 = -62.9; z1 = 3.5; e1 = 77.5591; x2 = 14.5; y2 = -98.98; z2 = 131.5; e2 = 434.034
x1 = -13.5; y1 = 56.51; z1 = 0.5; e1 = 150.202; x2 = 35.5; y2 = 222.65; z2 = 127.5; e2 = 358.586
x1 = 157.5; y1 = -30.95; z1 = 1.5; e1 = 314.62; x2 = -66.5; y2 = 37.34; z2 = 126.5; e2 = 197.104
x1 = 166.5; y1 = 126.67; z1 = 5.5; e1 = 196.219; x2 = 135.5; y2 = 271.64; z2 = 131.5; e2 = 316.821
*/
}



