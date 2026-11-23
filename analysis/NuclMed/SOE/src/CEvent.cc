
#include "CEvent.h"
#include "CUserParameters.h"
#include <cmath>
#include <iostream>
#include "constants.h"

using namespace std;


CEvent::CEvent(int in_ID, EVENT_TYPE in_eventType)
	: m_ID(in_ID)
	, m_eventType(in_eventType)
{
}

CEvent::~CEvent()
{
}

// copy constructor
CEvent::CEvent(const CEvent& in_obj)
{
	*this = in_obj;
}

// assignment operator
CEvent&
CEvent::operator= (const CEvent& in_obj)
{
	if (this != &in_obj)
	{
		m_ID = in_obj.GetID();
		m_eventType = in_obj.GetEventType();

		m_hit1 = in_obj.GetHit1();
		m_hit2 = in_obj.GetHit2();
	}
	return *this;
}

void
CEvent::Set(  const C3Vector& in_hitPosition1, const double& in_hitE1
		    , const C3Vector& in_hitPosition2, const double& in_hitE2 )
{
	m_hit1.Set(in_hitPosition1, in_hitE1);
	m_hit2.Set(in_hitPosition2, in_hitE2);
}

bool
CEvent::operator==(const CEvent &in_obj) const
{
	bool result = ( m_ID == in_obj.GetID()
			&& m_hit1 == in_obj.GetHit1()
			&& m_hit2 == in_obj.GetHit2() );

	return result;
}

bool
CEvent::operator!=(const CEvent &in_obj) const
{
    return !(*this == in_obj);
}

// (De)Serialization
void
CEvent::Serialize( std::ostream& io_outstream ) const
{	
	/*
	boost::archive::binary_oarchive oarch(in_outstream);
	oarch << *this;
	*/

	// in_outstream << m_ID << " " << m_eventType << " ";
	io_outstream.write((char*) &m_ID, sizeof(m_ID));
	io_outstream.write((char*) &m_eventType, sizeof(m_eventType));
	m_hit1.Serialize(io_outstream);
	m_hit2.Serialize(io_outstream);
}

void
// binary_iarchive does not work with const std::ifstream
// C3Vector::Deserialize( const std::ifstream& in_instream )
CEvent::Deserialize( std::istream& io_instream )
{

	/*
	boost::archive::binary_iarchive iarch(in_instream);
	iarch >> *this;
	*/

	// in_instream >> m_ID >> m_eventType;
	io_instream.read((char*) &m_ID, sizeof(m_ID));
	io_instream.read((char*) &m_eventType, sizeof(m_eventType));
	m_hit1.Deserialize(io_instream);
	m_hit2.Deserialize(io_instream);
}



