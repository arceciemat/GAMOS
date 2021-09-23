#pragma once

#ifndef _SOE_CPETLINE___
#define _SOE_CPETLINE___

#include "C3Vector.h"
#include "constants.h"

class CPETLine 
{
public:
						CPETLine();
	virtual				~CPETLine();

	// copy and assign constructors
						CPETLine(const CPETLine& in_obj);
	CPETLine& 			operator= (const CPETLine& in_obj);

	// (De)Serialization
	void				Serialize( std::ostream& outstream ) const;
	void				Deserialize( std::istream& instream );

	// Methods to get random position on the PET line
	// static, because CPETLine does not have member data, so we don't need to instantiate an object 
	static void			GetRandomPositionOnLine( const C3Vector& in_hitPosition1, 
				   				  				 const C3Vector& in_hitPosition2,
												 C3Vector& io_positionOnLine); // const;

	bool				operator==(const CPETLine& in_obj) const; 		// ==
	bool				operator!=(const CPETLine& in_obj) const;		// !=

private:
	// private data (NONE)

	// define output stream operator
	friend ostream& 		operator<<(ostream& os, const CPETLine& in_pETLine);

	// BOOST SERIALIZATION STUFF
	/*
	friend class boost::serialization::access;
	// template functions should always be defined where they are declared (in header file)
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
    }
	*/
};

#endif
