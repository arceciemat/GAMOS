
#include "C3Vector.h"
#include "constants.h"

#include <cmath>

ostream& operator<<(ostream& os, const C3Vector& vec)
{
    os << vec.GetX() << ' ' << vec.GetY() << ' ' << vec.GetZ();
    return os;
}

C3Vector::C3Vector()
	: m_x(0.0)
	, m_y(0.0)
	, m_z(0.0)
{
}

C3Vector::C3Vector(const double& in_x, const double& in_y, const double& in_z)
	: m_x(in_x)
	, m_y(in_y)
	, m_z(in_z)
{
}

C3Vector::~C3Vector()
{
}

// copy constructor
C3Vector::C3Vector(const C3Vector& in_obj)
{
	*this = in_obj;
}

// assignment operator
C3Vector&
C3Vector::operator= (const C3Vector& in_obj)
{
	if (this != &in_obj)
	{
		m_x = in_obj.GetX();
		m_y = in_obj.GetY();
		m_z = in_obj.GetZ();
	}
	return *this;
}

bool
C3Vector::operator==(const C3Vector &in_obj) const
{
	return (   doubleEquals( m_x, in_obj.GetX(), 0.0001)
			&& doubleEquals( m_y, in_obj.GetY(), 0.0001)
			&& doubleEquals( m_z, in_obj.GetZ(), 0.0001) );
}

bool
C3Vector::operator!=(const C3Vector &in_obj) const
{
    return !(*this == in_obj);
}

void
C3Vector::Set (const double& in_x, const double& in_y, const double& in_z)
{
	m_x = in_x;
	m_y = in_y;
	m_z = in_z;
}

double
C3Vector::GetLength() const
{
	double len = sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
	return len;
}

double
C3Vector::GetScalarProduct(const C3Vector& in_vec) const
{
    double inp = (*this) * in_vec;
	double len1 = GetLength();
	double len2 = in_vec.GetLength();

	inp = ((len1 * len2) != 0) ? (inp / (len1 * len2)) : 0;
    return inp;
}

double
C3Vector::GetScalarProductAngle(const C3Vector& in_vec) const
{
	double cosangle = GetScalarProduct(in_vec);

	double angle = acos(cosangle);
	return angle;
}

// (De)Serialization
void
C3Vector::Serialize( std::ostream& io_outstream ) const
{
	io_outstream.write((char*) &m_x, sizeof(m_x));
	io_outstream.write((char*) &m_y, sizeof(m_y));
	io_outstream.write((char*) &m_z, sizeof(m_z));
}

void
C3Vector::Deserialize( std::istream& io_instream )
{
	io_instream.read((char*) &m_x, sizeof(m_x));
	io_instream.read((char*) &m_y, sizeof(m_y));
	io_instream.read((char*) &m_z, sizeof(m_z));
}

// operators

// + (plus) operator
C3Vector
C3Vector::operator+ (const C3Vector& in_v) const
{
      C3Vector result;
      result.m_x = (m_x + in_v.GetX());
      result.m_y = (m_y + in_v.GetY());
      result.m_z = (m_z + in_v.GetZ());
      return result;
}

// - (minus) operator
C3Vector
C3Vector::operator- (const C3Vector& in_v) const
{
      C3Vector result;
      result.m_x = (m_x - in_v.GetX());
      result.m_y = (m_y - in_v.GetY());
      result.m_z = (m_z - in_v.GetZ());
      return result;
}

// inproduct
double
C3Vector::operator*(const C3Vector& in_v) const
{
	double result = m_x * in_v.GetX() + m_y * in_v.GetY() + m_z * in_v.GetZ();
	return result;
}

C3Vector
C3Vector::operator*(double const& in_coeff) const
{
	C3Vector result;
	result.m_x = (m_x * in_coeff);
	result.m_y = (m_y * in_coeff);
	result.m_z = (m_z * in_coeff);
	return result;
}












