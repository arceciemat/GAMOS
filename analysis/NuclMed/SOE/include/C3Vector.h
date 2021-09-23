
#pragma once

#ifndef _SOE_C3VECTOR___
#define _SOE_C3VECTOR___

// #include "boostincludes.h"

#include <iostream>
#include <fstream>

using namespace std;

class C3Vector
{
public:
				C3Vector();
				C3Vector(const double& in_x, const double& in_y, const double& in_z);
	virtual		~C3Vector();

	// copy and assign constructors
				C3Vector(const C3Vector& in_obj);
	C3Vector& 	operator= (const C3Vector& in_obj);

	// set function
	void			Set (const double& in_x, const double& in_y, const double& in_z);
	inline double	GetX() const { return m_x; }
	inline double	GetY() const { return m_y; }
	inline double	GetZ() const { return m_z; }
	inline void		SetX(const double& in_val) { m_x = in_val; }
	inline void		SetY(const double& in_val) { m_y = in_val; }
	inline void		SetZ(const double& in_val) { m_z = in_val; }

	// useful functions
	double		GetLength() const;
	double		GetScalarProduct(const C3Vector& in_vec) const;       // scalarproduct
	double		GetScalarProductAngle(const C3Vector& in_vec) const;       // scalarproduct angle

	// (De)Serialization
	void		Serialize( std::ostream& outstream ) const;
	void		Deserialize( std::istream& instream );

	// operators
	C3Vector 	operator+(const C3Vector&) const;       		// operator+()
	C3Vector 	operator-(const C3Vector&) const;       		// operator-()

	double 		operator*(const C3Vector& in_vector) const;     // inproduct
	C3Vector 	operator*(const double& in_coeff) const;        // multiplication factor

	bool		operator==(const C3Vector& in_obj) const; 		// ==
	bool		operator!=(const C3Vector& in_obj) const;		// !=

private:
	// data
	double 				m_x, m_y, m_z;

	// define output stream operator
	friend ostream& 	operator<<(ostream& os, const C3Vector& vec);

	// BOOST SERIALIZATION STUFF
	/*
	friend class boost::serialization::access;
	// template functions should always be defined where they are defined (in header file)
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_x & m_y & m_z;
    }
	*/
};

#endif




