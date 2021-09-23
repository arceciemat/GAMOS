#pragma once

#ifndef _SOE_C3MATRIX___
#define _SOE_C3MATRIX___

#include "C3Vector.h"
// #include "boostincludes.h"

#include <iostream>
#include <fstream>

using namespace std;

class C3Matrix
{
public:

	enum AXISTYPE
	{
		Axis_X = 0,
		Axis_Y, 
		Axis_Z
	};

	// ctors and dtor
				C3Matrix();
				C3Matrix(const C3Vector& in_col1, const C3Vector& in_col2, const C3Vector& in_col3);
				C3Matrix( AXISTYPE in_axis, const double& in_angle);		// angles in radians

	virtual		~C3Matrix();


	// copy and assign constructors
				C3Matrix(const C3Matrix& in_obj);
	C3Matrix& 	operator= (const C3Matrix& in_obj);

	// set function
	void		Set (const C3Vector& in_col1, const C3Vector& in_col2, const C3Vector& in_col3);
	double		GetElement(int col, int row) const;
	void		SetElement(int col, int row, double value);

	void 		GetColumn(int col, C3Vector& io_vec) const;
	void 		GetRow(int row, C3Vector& io_vec) const;

	// useful functions
	// double		GetInverse() const;		// not implemented yet

	// serialization
	void		Serialize( std::ostream& outstream ) const;
	void		Deserialize( std::istream& instream );

	// operators
	C3Matrix 	operator*(const C3Matrix&) const;       
	C3Vector 	operator*(const C3Vector&) const;    
   
	bool		operator==(const C3Matrix& in_obj) const; 		// ==
	bool		operator!=(const C3Matrix& in_obj) const;		// !=

private:

	// data
	double** 	m_a;

	// initialize data
	void 		InitMatrixElements();

	// define output stream operator
	friend ostream& operator<<(ostream& os, const C3Matrix& vec);

	// BOOST SERIALIZATION STUFF
	/*
	friend class boost::serialization::access;
	// template functions should always be defined where they are defined (in header file)
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
    	for(int j = 0; j < 3; j++)
		{
    		for(int i = 0; i < 3; i++)
			{
				ar & m_a[i][j];
			}
		}
    }
	*/
};

#endif
