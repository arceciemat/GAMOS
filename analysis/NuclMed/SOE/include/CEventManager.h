#pragma once

#ifndef _SOE_CEVENTMANAGER__
#define _SOE_CEVENTMANAGER__

#include "CEvent.h"

#include "CDataSetManager.h"

class CEventManager
{
public:
							CEventManager();
	virtual					~CEventManager();

	int						GetMaxEventID() const 
							{		
								return m_dataSetManager.m_maxEventID; 
							}
	void					AddEvent(const CEvent& in_event);
	CEvent*					GetEvent(int in_eventID);


private:
	// friend test function has access to private methods...
	friend void TestEventManager();

	// disabled copy and assign constructors
	// not implemented, to avoid copying this object
							CEventManager(const CEventManager& in_obj);
	CEventManager& 			operator= (const CEventManager& in_obj);

	// data
	CDataSetManager<CEvent>	m_dataSetManager;	// Friend class. Takes care of data (set) management 

};

#endif

