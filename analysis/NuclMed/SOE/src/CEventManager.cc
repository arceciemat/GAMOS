#include "CEventManager.h"

#include "CUserParameters.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <cstdlib>

CEventManager::CEventManager()
{
}

CEventManager::~CEventManager() 
{
}

void
CEventManager::AddEvent(const CEvent& in_event)
{
	int eventID = in_event.GetID();

	assert (    (eventID == 0 && m_dataSetManager.m_maxEventID == 0) 
             || (eventID == m_dataSetManager.m_maxEventID + 1));	
		// N.B. eventIDs should be added in correct order without gaps in between consecutive IDs !

	// Add event to eventCurrentMap 
	m_dataSetManager.AddToMap(eventID, in_event); 
}

CEvent*
CEventManager::GetEvent(int in_eventID)
{
	return m_dataSetManager.GetDataElement(in_eventID);
}



