
#include "CDataSetManager.h"

#include "CUserParameters.h"
#include "C3Vector.h"
#include "CEvent.h"
#include "CCone.h"

template <>
void
CDataSetManager<C3Vector>::CleanCurrentMap()
{
	std::map<long, C3Vector*>::iterator mapiter;
	for (mapiter = m_dataSetMap.begin(); mapiter != m_dataSetMap.end(); mapiter++)
	{
		C3Vector* dataElement = ((*mapiter).second);
		delete dataElement;
	}
	m_dataSetMap.clear();
}

template <>
void
CDataSetManager<CEvent>::CleanCurrentMap()
{
	std::map<long, CEvent*>::iterator mapiter;
	for (mapiter = m_dataSetMap.begin(); mapiter != m_dataSetMap.end(); mapiter++)
	{
		CEvent* dataElement = ((*mapiter).second);
		delete dataElement;
	}
	m_dataSetMap.clear();
}

template <>
void
CDataSetManager<CCone>::CleanCurrentMap()
{
	std::map<long, CCone*>::iterator mapiter;
	for (mapiter = m_dataSetMap.begin(); mapiter != m_dataSetMap.end(); mapiter++)
	{
		CCone* dataElement = ((*mapiter).second);
		delete dataElement;
	}
	m_dataSetMap.clear();
}

template <>
void
CDataSetManager<C3Vector>::GetSetBaseFileName(std::string& io_fbasename) const
{
	io_fbasename = "CStateVector_";
}

template <>
void
CDataSetManager<CEvent>::GetSetBaseFileName(std::string& io_fbasename) const
{
	io_fbasename = "CEventMap_";
}

template <>
void
CDataSetManager<CCone>::GetSetBaseFileName(std::string& io_fbasename) const
{
	io_fbasename = "CConeMap_";
}

template <>
void
CDataSetManager<C3Vector>::GetCurrentFirstAndLastEventID(int& out_firstEventID, int& out_lastEventID) const
{
	std::map<long, C3Vector*>::const_iterator mapiter;
	mapiter = m_dataSetMap.begin();
	out_firstEventID = ((*mapiter).first);

	mapiter = m_dataSetMap.end();
  	mapiter--;
	out_lastEventID = ((*mapiter).first);

	assert(out_lastEventID - out_firstEventID + 1 > 0);
	assert(out_lastEventID - out_firstEventID + 1 == m_dataSetMap.size());
}

template <>
void
CDataSetManager<CEvent>::GetCurrentFirstAndLastEventID(int& out_firstEventID, int& out_lastEventID) const
{
	std::map<long, CEvent*>::const_iterator mapiter;
	mapiter = m_dataSetMap.begin();
	out_firstEventID = ((*mapiter).first);

	mapiter = m_dataSetMap.end();
  	mapiter--;
	out_lastEventID = ((*mapiter).first);

	assert(out_lastEventID - out_firstEventID + 1 > 0);
	assert(out_lastEventID - out_firstEventID + 1 == m_dataSetMap.size());
}

template <>
void
CDataSetManager<CCone>::GetCurrentFirstAndLastEventID(int& out_firstEventID, int& out_lastEventID) const
{
	std::map<long, CCone*>::const_iterator mapiter;
	mapiter = m_dataSetMap.begin();
	out_firstEventID = ((*mapiter).first);

	mapiter = m_dataSetMap.end();
  	mapiter--;
	out_lastEventID = ((*mapiter).first);

	assert(out_lastEventID - out_firstEventID + 1 > 0);
	assert(out_lastEventID - out_firstEventID + 1 == m_dataSetMap.size());
}

