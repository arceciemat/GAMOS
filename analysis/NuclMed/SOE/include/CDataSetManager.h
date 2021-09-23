
#pragma once

#ifndef _SOE_CDATASETMANAGER___
#define _SOE_CDATASETMANAGER___

//C/C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <cstdlib>

#include <map>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include "CUserParameters.h"

template <class TType>
class CDataSetManager
{
public:
							CDataSetManager();
	virtual					~CDataSetManager();

	void					LoadCorrectSet( long in_eventID );
	void					AddToMap( long in_eventID, const TType& in_dataElement );
	TType*					GetDataElement( long in_eventID );

	void					CleanCurrentMap();

private:
	// friends
	friend class CEventManager;
	friend class CCurrentState;
	friend void TestCCurrentState();
	friend void TestEventManager();

	// methods
	int						GetSetNumber(int in_eventID) const;
	void					GetSetFileName(std::string& io_fname, int in_nr) const;
	void					GetSetBaseFileName(std::string& io_fbasename) const;	
	void					GetCurrentFirstAndLastEventID(int& out_firstEventID, int& out_lastEventID) const;
	void					DoAddToMap( long in_eventID, const TType& in_dataElement );

	// (De)serialize
	void					Serialize();
	void					Deserialize( int in_setNumber );

	// disabled copy and assign constructors
	// not implemented, to avoid copying this object
								CDataSetManager(const CDataSetManager& in_obj);
	CDataSetManager& 			operator= (const CDataSetManager& in_obj);

	// Data
	std::map<long, TType*>		m_dataSetMap;
	std::map<int, std::string>	m_setFileNames;
	int							m_currentSetNumber;
	int							m_maxEventID;
};

// ------------ IMPLEMENTATION --------------------------------

template <class TType>
CDataSetManager<TType>::CDataSetManager()
	: m_currentSetNumber(0)
	, m_maxEventID(0)
{
}

template <class TType>
CDataSetManager<TType>::~CDataSetManager()
{
	CleanCurrentMap();
}

template <class TType>
int
CDataSetManager<TType>::GetSetNumber(int in_eventID) const
{
	int setSize = CUserParameters::Instance()->GetEventSetSize();
	int setNumber = (setSize == 0) ? 0 : in_eventID / setSize;
	return setNumber;
}

template <class TType>
void					
CDataSetManager<TType>::LoadCorrectSet( long in_eventID )
{
	// get correct set
	int	setNumber = GetSetNumber(in_eventID);
	if (setNumber != m_currentSetNumber)
	{
		// 1. Serialize current set
		Serialize();

		// 2. Clean current map
		CleanCurrentMap();

		// 3. deserialize new vector map
		Deserialize( setNumber );

		// 4. we can suppose that the next requested vector will also be from this set so change "current set number"
		m_currentSetNumber = setNumber;
	}
}

template <class TType>
void					
CDataSetManager<TType>::GetSetFileName(std::string& io_fname, int in_nr) const
{
	GetSetBaseFileName( io_fname );
	std::stringstream setNrStr;
	setNrStr << in_nr;
	io_fname += setNrStr.str(); 
	io_fname += ".dat";
}

template <class TType>
void
CDataSetManager<TType>::Serialize()
{
	std::string fname;
	GetSetFileName(fname, m_currentSetNumber);

	std::map<int, std::string>::iterator mapiter = m_setFileNames.find(m_currentSetNumber);
	if (mapiter == m_setFileNames.end() )
	{
		m_setFileNames[m_currentSetNumber ] = fname;
	}

	std::ofstream outfile(fname.c_str(), ios::binary | ios::out );
	assert (outfile.is_open());

	// For serialization, we need a const map to the boost::archive function
	// const std::map<long, TType*>& tmpMap = m_dataSetMap;
	/*
	boost::archive::text_oarchive oarch(outfile);
	oarch << tmpMap;
	*/

	int firstEventID, lastEventID;
	GetCurrentFirstAndLastEventID(firstEventID, lastEventID);

	// outfile << m_dataSetMap.size() << " ";
	int size = m_dataSetMap.size();
	outfile.write((char*) &size, sizeof(size));
	TType* dataEl = NULL;
	for (int eventID = firstEventID; eventID <= lastEventID; eventID++)
	{
		dataEl = m_dataSetMap[eventID];
		assert (dataEl != NULL);

		// outfile << eventID << " ";
		outfile.write((char*) &eventID, sizeof(eventID));

		dataEl->Serialize( outfile );
	}
}

template <class TType>
void
CDataSetManager<TType>::Deserialize( int in_setNumber )
{
	std::map<int, std::string>::iterator mapiter = m_setFileNames.find(in_setNumber);
	if (mapiter == m_setFileNames.end())
	{
		// no previous data available
		return;
	}

	std::string fname =	((*mapiter).second);
	std::ifstream infile(fname.c_str(), ios::binary | ios::in );
	if (!infile.is_open())
	{
		// no previous data available
		return;
	}

	/*
	boost::archive::text_iarchive iarch(infile);
	iarch >> m_dataSetMap;
	*/

	int size, eventID;
	// infile >> size;
	infile.read((char*) &size, sizeof(size));
	TType dataEl;
	for (int iloop = 0; iloop < size; iloop++)
	{
		// infile >> eventID;
		infile.read((char*) &eventID, sizeof(eventID));

		dataEl.Deserialize( infile );

		DoAddToMap( eventID, dataEl );
	}
}

template <class TType>
void
CDataSetManager<TType>::AddToMap( long in_eventID, const TType& in_dataElement )
{
	// get correct set
	LoadCorrectSet( in_eventID );

	// Add to map
	DoAddToMap( in_eventID, in_dataElement );
}

template <class TType>
void
CDataSetManager<TType>::DoAddToMap( long in_eventID, const TType& in_dataElement )
{	
	// Check that this eventID is not in map already
	assert ( m_dataSetMap.end() == m_dataSetMap.find(in_eventID) );
	assert ( m_dataSetMap[in_eventID] == NULL );

	TType* newElement = new TType( in_dataElement );	// Make a copy to own... (deleted in destructor)

	// add to map
	m_dataSetMap[in_eventID] = newElement;

	// increment counter
	if (in_eventID > m_maxEventID) 
	{
		m_maxEventID = in_eventID;
	}
}

template <class TType>
TType*
CDataSetManager<TType>::GetDataElement( long in_eventID )
{
	if ( in_eventID < 0 || in_eventID > m_maxEventID)
	{
		return NULL;
	}

	// get correct set
	LoadCorrectSet( in_eventID );

	TType* dataEl = m_dataSetMap[in_eventID];
	return dataEl;
}


#endif
