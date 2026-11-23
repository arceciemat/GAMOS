#pragma once

#ifndef _SOE_CEVENT__
#define _SOE_CEVENT__

#include "CHit.h"
#include "CCone.h"
#include "CPETLine.h"
#include <map>

// #include "boostincludes.h"
#include "constants.h"

enum EVENT_TYPE
{
	EVENTTYPE_NONE = 0,
	COMPTON_EVENT,
	PET_EVENT
};

class CEvent
{
public:
							CEvent(int in_ID = 0, EVENT_TYPE in_eventType = COMPTON_EVENT);
	virtual					~CEvent();

	// copy and assign constructors
							CEvent(const CEvent& in_obj);
	CEvent& 				operator= (const CEvent& in_obj);

	// Get/Set methods
	void					Set(  const C3Vector& in_hitPosition1, const double& in_hitE1
				   				, const C3Vector& in_hitPosition2, const double& in_hitE2 );

	int				        GetID() const 			{ return m_ID; }
	int						GetEventType() const	{ return m_eventType; }

	inline const CHit&		GetHit1() const { return m_hit1; }
	inline const CHit&		GetHit2() const { return m_hit2; }

	inline double			GetHitE1() const 	{ return m_hit1.GetE(); }
	inline double			GetHitE2() const 	{ return m_hit2.GetE(); }

	const C3Vector&	        GetHitPosition1() const	{ return m_hit1.GetPosition(); }
	const C3Vector&	        GetHitPosition2() const	{ return m_hit2.GetPosition(); }

    // virtual functions
	virtual bool			operator==(const CEvent& in_obj) const; 	// ==
	virtual bool			operator!=(const CEvent& in_obj) const;		// !=

	// (De)Serialization
	virtual void			Serialize( std::ostream& outstream ) const;
	virtual void			Deserialize( std::istream& instream );

private:

	// data
	int				m_ID;
	int				m_eventType;

	CHit			m_hit1;
	CHit			m_hit2;
};

typedef std::map<long, CEvent*> CEventMap;

#endif

