#pragma once

#ifndef _SOE_CHIT___
#define _SOE_CHIT___

#include "C3Vector.h"
#include <string>

class CHit
{
public:
						CHit();
						CHit(const C3Vector& in_position, const double& in_e);
	virtual				~CHit();

	// copy and assign constructors
						CHit(const CHit& in_obj);
	CHit& 				operator= (const CHit& in_obj);

	void 				Set(const C3Vector& in_position, const double& in_e);

	inline double				GetE() const 				{ return m_e; }
	inline const C3Vector&		GetPosition() const			{ return m_position; }

	bool			operator==(const CHit& in_obj) const; 	// ==
	bool			operator!=(const CHit& in_obj) const;	// !=

	// (De)Serialization
	void			Serialize( std::ostream& outstream ) const;
	void			Deserialize( std::istream& instream );

private:

	double 				m_e;
	C3Vector			m_position;	
};

#endif
