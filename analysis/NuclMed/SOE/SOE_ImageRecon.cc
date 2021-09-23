
#include "CUserParameters.h"
#include "CEventManager.h"
#include "CCurrentState.h"
#include "C3Vector.h"
#include "CEvent.h"
#include "CCone.h"

#include "constants.h"

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandFlat.h"

//C/C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <cstdlib>

#include <map>
#include <vector>
#include <cmath>

#define IS_GAMOS_DISTRIBUTION 0

using namespace std;

// auxiliary functions.

void GetGeomResolutionRecoveryPosition(const double& in_gDeltaX, const double& in_gDeltaY, const double& in_gDeltaZ
            , C3Vector& io_position);

void GetEnergyResolutionRecoveryEnergy(const double& in_eResolution, double& io_e);

#if IS_GAMOS_DISTRIBUTION == 0
void DebugNearestPositions(const C3Vector& in_hitPosition1, const double& in_hitE1,
                            const C3Vector& in_hitPosition2, const double& in_hitE2,
                            CCone& io_cone, std::ofstream& in_ofile,
                            C3Vector& io_position, double& thetaReal);
#endif

void parseInputFlags(int argc, char *argv[]);


// Program flags
EVENT_TYPE m_eventType = COMPTON_EVENT;
bool m_doExtraIterInfo = false;
bool m_doReadDensityMatrix = false;
bool m_doReadCurrentState = false;
bool m_doReadRandomEngineStatus = false;

bool m_doRandomEventSelection = false;

bool m_doLimitRange = false;

#if IS_GAMOS_DISTRIBUTION == 0
bool m_doDebugCalcNearestPoint = false;
bool m_doOutputConvergence = false;
#endif

bool m_doPrintFlagsAndQuit = false;

long m_seed;
bool m_isSeedDone = false;

CUserParameters* m_userParameters = NULL;

// =========================================

int main(int argc, char *argv[])
{
    // 0.   Program flags
    parseInputFlags(argc, argv);

	// 0.	- Read CUserParameters
	m_userParameters = CUserParameters::Instance();
	if (!m_userParameters)
	{
		cout << "Error in instantiating CUserParameters. Exit" << endl;
		exit(1);
	}

	// fill user parameters with some of the flags
	if (m_doLimitRange)
	{
		cout << "LIMIT RANGE MODE" << endl;
		m_userParameters->SetDoLimitRange( m_doLimitRange );
	}
	else
	{
		cout << "ALL RANGE MODE" << endl;
	}

	// 0. 	- RANDOM GENERATOR
	//  	HEP SEEDS... always random.... TODO: right?
	if (!m_isSeedDone)
	{
		time_t systime = time(NULL);
	    m_seed = (long) systime;
	}

	cout << "CLHEP ENGINE: name: " << CLHEP::HepRandom::getTheEngine()->name() << endl;
	cout << "CLHEP RANDOM SEED: " << m_seed << endl;
	CLHEP::HepRandom::setTheSeed(m_seed);

	if (m_doReadRandomEngineStatus)
	{
		CLHEP::HepRandom::restoreEngineStatus("in_RandomEngineStatus.rndm");
	}

	double test_uniformVal;
	cout << "first 5 random phi values: ";
	for (int i = 0; i < 5; i++)
	{
		test_uniformVal = CLHEP::RandFlat::shoot(2.0 * kPI);
		cout << " " << test_uniformVal << " ";
	}
	cout << endl;

	// 0.	Energy, Spatial and Doppler resolution recovery parameters
	double eResolution = m_userParameters->GetEnergyResolutionFactorSigma();
	double spatialDeltaX = m_userParameters->GetSpatialDeltaX();
	double spatialDeltaY = m_userParameters->GetSpatialDeltaY();
	double spatialDeltaZ = m_userParameters->GetSpatialDeltaZ();
	double dopplerRes = m_userParameters->GetDopplerEffectSigmaTheta();

    bool doResolutionRecovery = (   eResolution > 0
				|| spatialDeltaX > 0 || spatialDeltaY > 0 || spatialDeltaZ > 0
				|| dopplerRes > 0 );

	// 1.
    // 		- Read all events -> CEvent -> CEventList
    //      - Calculate corresponding CCones -> m_coneDataSet
 	//		- Create CCurrentState for iteration 0 with random values for vector and a DensityMatric

	std::string datafilename = m_userParameters->GetDataFileName();
	std::ifstream datafile(datafilename.c_str(), ios::in);
	if (!datafile.is_open())
	{
		cout << "Input Data file not open: " << datafilename << endl;
		exit(1);
	}

	// 		initialise current state object
	CCurrentState currentState;
	if (m_doReadDensityMatrix) currentState.ReadDensityMatrix();


#if IS_GAMOS_DISTRIBUTION == 0
	std::ofstream beginNearestFile;
	std::ofstream beginaftersetNearestFile;
    if ( m_doDebugCalcNearestPoint )
    {
        beginNearestFile.open("nearestpoints_begin.dat");
    }
#endif

	// 		event reading loop
	int maxNum = m_userParameters->GetMaxNumberOfEvents();

	CEventManager evtMger;
	CDataSetManager<CCone>	m_coneDataSetManager;
    CCone cone;

    C3Vector hitPosition1, hitPosition2;
	int eventID = 0;
	double z1, y1, x1, e1, z2, y2, x2, e2;
	int linecnt = 0;
	while (!datafile.eof() && (maxNum == 0 || eventID < maxNum) )
    {
		datafile >> z1 >> y1 >> x1 >> e1 >> z2 >> y2 >> x2 >> e2;
		if (!datafile.eof())
		{
			if (datafile.fail())
			{
				cout << "Reading data file failed, with eventID: " << eventID << endl;
				cout << "badbit: " << datafile.bad() << ", failbit: " << datafile.fail() << endl;
				cout << "Last read values: " << z1 << " " << y1 << " " << x1 << " " << e1 << " " << z2 << " " << y2 << " " << x2 << " " << e2 << endl;
				exit(2);
			}
			CEvent event(eventID, m_eventType);

            hitPosition1.Set(x1, y1, z1);
            hitPosition2.Set(x2, y2, z2);

			event.Set(hitPosition1, e1, hitPosition2, e2);

            C3Vector position;
            if (m_eventType == COMPTON_EVENT)			// Compton Camera
            {
				// cout << "linecnt: " << linecnt << endl;
                int error = cone.Calculate(hitPosition1, e1, hitPosition2, e2);
				if (!error)
				{
					error = cone.CalculatePhiRanges();
				}
                if (!error)
                {
#if IS_GAMOS_DISTRIBUTION == 0
					if ( m_doDebugCalcNearestPoint )
					{
                        C3Vector nearestPos;
						double thetaReal;
						DebugNearestPositions(hitPosition1, e1, hitPosition2, e2,
                                            cone, beginNearestFile, nearestPos, thetaReal );
					}
#endif

					if (!m_doReadCurrentState)
					{
                        cone.GetRandomPositionOnCone( position );
                    	currentState.AddToStateVector( eventID, position );
					}

                    evtMger.AddEvent( event );
	                m_coneDataSetManager.AddToMap(eventID, cone);	// copy of "cone" is stored
                    eventID++;
                }
            }
            else			// PET DATA
            {
                evtMger.AddEvent( event );

				if (!m_doReadCurrentState)
				{
                	CPETLine::GetRandomPositionOnLine( hitPosition1, hitPosition2, position );
                	currentState.AddToStateVector( eventID, position );
				}

                eventID++;
            }
			linecnt++;
		}
	}

	if (m_doReadCurrentState)
	{
		bool res = currentState.ReadCurrentState( "currentstate.dat" );
		if (!res) exit(2);
	}

	cout << "max event ID: " << evtMger.GetMaxEventID() << endl;

	// testing initial state
	currentState.DumpCurrentState( "data_beginstate_xy.dat" );

	// In case of Random order of events: make a list of all event IDs
	std::vector<int> evtIdxList_store;
	std::vector<int> evtIdxList;
	if (m_doRandomEventSelection)
	{
		for (int eventID = 0; eventID <= evtMger.GetMaxEventID(); eventID++)
		{
			evtIdxList_store.push_back(eventID);
		}
	}

	// 2. - Main Iteration loop
#if IS_GAMOS_DISTRIBUTION == 0
	std::ofstream convergencefile;
	if (m_doOutputConvergence)
	{
		convergencefile.open("convergence.dat");
	}
#endif

	int niter = 0;
	int numberOfIters = CUserParameters::Instance()->GetNumberOfIterations();
	bool doNextIteration = (niter < numberOfIters);

	std::stringstream coneDump;
    CCone* coneptr = NULL;

	// 		iteration loop
	while ( doNextIteration )
	{
		double avediff = 0.0;
		int diffevts = 0;
		// 3.   - Loop over all events
		bool loopingEvents = true;
		int eventID = 0;
		if (m_doRandomEventSelection)
		{
			evtIdxList = evtIdxList_store;
		}

		// 		event loop
		while (loopingEvents && eventID <= evtMger.GetMaxEventID())
		{
			// random selection
			if (m_doRandomEventSelection)
			{
				int listidx = (int) CLHEP::RandFlat::shoot(evtIdxList.size());
				eventID = evtIdxList[listidx];
				evtIdxList.erase (evtIdxList.begin() + listidx);
				loopingEvents = ( evtIdxList.size() > 0 );
			}

			const CEvent* event = evtMger.GetEvent(eventID);
			assert( event );

			C3Vector old_position;
			currentState.GetPosition( eventID, old_position );

			// 4. 	- Get random point "new_position" on Ellips or Cone
			C3Vector new_position( old_position );

            // Geometry resolution recovery, if necessary
            C3Vector position1 = event->GetHitPosition1();
            C3Vector position2 = event->GetHitPosition2();
            if ( spatialDeltaX > 0 || spatialDeltaY > 0 || spatialDeltaZ > 0 )
            {
                GetGeomResolutionRecoveryPosition(spatialDeltaX, spatialDeltaY, spatialDeltaZ, position1);
                GetGeomResolutionRecoveryPosition(spatialDeltaX, spatialDeltaY, spatialDeltaZ, position2);
            }

            if (m_eventType == COMPTON_EVENT) 	// Compton Camera
            {
				// Resolution recovery <<<<<<<<
				int againerr = 0;
				// we keep "cone" in a list, because parts of it do not have to be re-calculated again
				coneptr = m_coneDataSetManager.GetDataElement(eventID);
				assert( coneptr );

				if (doResolutionRecovery)
				{
					coneDump.str("");
					coneptr->Serialize(coneDump);
				}

                // For all cones, all basic variables are already calculated
                // (Compton angle, axis, axis-origin and rotation matrix)
                // In case of resolution recovery we re-calculate some (but not all) of these
				e1 = event->GetHitE1();
				e2 = event->GetHitE2();
				if ( eResolution > 0 || dopplerRes > 0 )
                {
                    // In case of E resolution recovery, vary energies randomly
                    // and, after that, re-calculate compton angle
                    if (eResolution > 0 )
                    {
                        GetEnergyResolutionRecoveryEnergy(eResolution, e1);
                        GetEnergyResolutionRecoveryEnergy(eResolution, e2);
                    }
                    // In case of Doppler or Energy resolution recovery: re-calculate angle for each iteration from scratch
					againerr = coneptr->CalculateComptonAngle( e1, e2 );
					if (dopplerRes > 0)
					{
					    // In case of Doppler res. recv. vary the angle randomly
					    coneptr->VaryComptonAngle( dopplerRes );
					}
				}	// E or Doppler res. recv.

				// In case of Geom res. recv: re-calculate axis and rotation matrix
				// or, in case of limiting range, we have to re-calculate the phi range in case of res recv.
                if (!againerr)
                {
					//	if (    ( m_userParameters->GetDoLimitRange() && doResolutionRecovery )
					//		|| ( spatialDeltaX > 0 || spatialDeltaY > 0 || spatialDeltaZ > 0 ) )
					if ( spatialDeltaX > 0 || spatialDeltaY > 0 || spatialDeltaZ > 0 )
                    {
                        againerr = coneptr->CalculateComptonGeometrics(position1, position2);
                    }
                }
				// >>>>>>>>>>

                if (!againerr)
                {
                    coneptr->GetRandomPositionOnCone( new_position );
                }
            }
            else if (m_eventType == PET_EVENT) 	// PET
            {
                CPETLine::GetRandomPositionOnLine( position1, position2, new_position );
            }

			bool doAccept = false;
			if (new_position != old_position)
			{
				bool insideBounds = CUserParameters::Instance()->IsInsideFOVBounds( new_position );
				if (insideBounds)
				{
					// 5. 	Calculate Acceptance probability using Density at new voxel and Density of previous voxel
					int old_density = currentState.GetDensity( old_position );
					int new_density = currentState.GetDensity( new_position ) + 1;

					double accProb = old_density > 0 ? new_density / old_density : 1;
					if ( accProb >= 1)
					{
						doAccept = true;	// accept new position
					}
					else
					{
						// if AccProb < 1: accept new position with throwing die hit/miss

						// double tmp = CUniformRandomGenerator::Instance()->GetUniformRandomValue(1.0);
						double uniformVal = CLHEP::RandFlat::shoot(1.0);
						double tmp = uniformVal;

						if (tmp < accProb) doAccept = true;
					}
				}
			}

			// 6. 	If new position is accepted, update DensityMatrix and CCurrentState
			if (doAccept)
			{
				currentState.Update(event->GetID(), new_position);

#if IS_GAMOS_DISTRIBUTION == 0
				if (m_doOutputConvergence)
				{
					C3Vector tmp = new_position - old_position;
					{
						avediff += std::sqrt(tmp * tmp);
						diffevts++;
					}
				}
#endif
			}
			else	// if not succesful, do not keep the new angles, energies and positions of the cone...
			{
				if (doResolutionRecovery && m_eventType == COMPTON_EVENT)
				{
					assert( coneptr );
					C3Vector axisDir = coneptr->GetComptonAxisDirection();
					C3Vector axisOrg = coneptr->GetComptonAxisOrigin();

					coneptr->Deserialize(coneDump);
				}
			}

			if (!m_doRandomEventSelection)
			{
				eventID++;
			}
		}	// End of loop over all events

#if IS_GAMOS_DISTRIBUTION == 0
		if (m_doOutputConvergence)
		{
			if (diffevts > 0)
				avediff = avediff / diffevts;
			convergencefile << niter << " " << avediff << " " << diffevts << endl;
		}
#endif
		niter++;

		if (   	(niter <= 500    && niter%100 == 0)
            || (niter <= 5000    && niter%1000 == 0)
            || (niter <= 50000   && niter%10000 == 0)
            || (niter <= 500000  && niter%100000 == 0)
            || (niter > (numberOfIters - 500) && niter%50 == 0)
		   )
		{
			cout << "iteration: " << niter << endl;

			if (m_doExtraIterInfo)
			{
				std::string fnfn = "data_state_xy_iter";
				std::stringstream iterStr;
				iterStr << niter;
				fnfn += iterStr.str();
				fnfn += ".dat";

				currentState.DumpCurrentState( fnfn );
			}

#if IS_GAMOS_DISTRIBUTION == 0
			if (m_doExtraIterInfo && m_doDebugCalcNearestPoint && (niter == 10000 || niter == 100000) )
			{
				std::string fnfn = "nearestpoints_iter";
				std::stringstream iterStr;
				iterStr << niter;
				fnfn += iterStr.str();
				fnfn += ".dat";

				std::ofstream aFile( fnfn.c_str() );
				for (int eventID = 0; eventID <= evtMger.GetMaxEventID(); eventID++)
				{
					const CEvent* event = evtMger.GetEvent(eventID);
					assert( event );
					coneptr = m_coneDataSetManager.GetDataElement(eventID);
					assert( coneptr );

					hitPosition1 = event->GetHitPosition1();
					hitPosition2 = event->GetHitPosition2();
					e1 = event->GetHitE1();
					e2 = event->GetHitE2();

					C3Vector notUsed1;
					double notUsed2;
					DebugNearestPositions(hitPosition1, e1, hitPosition2, e2,
							*coneptr, aFile, notUsed1, notUsed2);
				}
			}
#endif
		}

		if (niter >= numberOfIters) doNextIteration = false;

	}	// End of loop over all iterations

	// testing final state
	currentState.DumpCurrentState( "data_endstate_xy.dat" );

	// save density matrix in a "DICOM/AMIDE" ".img" file (plus header ".hdr" file)
	currentState.DumpDensityMatrix();

#if IS_GAMOS_DISTRIBUTION == 0
	// for debugging, loop over all cones
	if (m_doDebugCalcNearestPoint)
	{
		std::ofstream endNearestFile("nearestpoints_end.dat");
		for (int eventID = 0; eventID <= evtMger.GetMaxEventID(); eventID++)
		{
			const CEvent* event = evtMger.GetEvent(eventID);
			assert( event );
			coneptr = m_coneDataSetManager.GetDataElement(eventID);
			assert( coneptr );

            hitPosition1 = event->GetHitPosition1();
            hitPosition2 = event->GetHitPosition2();
			e1 = event->GetHitE1();
			e2 = event->GetHitE2();

            C3Vector notUsed1;
			double notUsed2;
            DebugNearestPositions(hitPosition1, e1, hitPosition2, e2,
                    *coneptr, endNearestFile, notUsed1, notUsed2);
		}
	}
#endif

	// CLHEP::HepRandom::saveEngineStatus("RandomEngineStatus.rndm");

	// Destroy singletons
	CUserParameters::Destroy();
}

// ===========================================================================================================

void parseInputFlags(int argc, char *argv[])
{
    std::string tmpStr;
    for (int i=0; i<argc; i++)
	{
		tmpStr = argv[i];
		cout << tmpStr << " ";
	}
	cout << endl;

    for (int i=1; i<argc; i++)
	{
		tmpStr = argv[i];
		if (tmpStr == "-PET")			// PET
		{
			m_eventType = PET_EVENT;
		}
		else if (tmpStr == "-Compton")			// Compton Camera
		{
			m_eventType = COMPTON_EVENT;
		}
		else if (tmpStr == "-extraIterInfo")
		{
			m_doExtraIterInfo = true;
		}
		else if (tmpStr == "-seed")
		{
			if ( (i+1) >= argc)
			{
				cout << "Wrong number of arguments. Usage: " << endl;
				cout << "SOE_ImageRecon -seed <seedNumber>" << endl;
				exit(1);
			}
			i++;
			m_seed = atoi( argv[i] );
			m_isSeedDone = true;
		}
		else if (tmpStr == "-randomEventLoop")
		{
			m_doRandomEventSelection = true;
		}
		else if (tmpStr == "-readDensityMatrix")
		{
			m_doReadDensityMatrix = true;
		}
		else if (tmpStr == "-readCurrentState")
		{
			m_doReadCurrentState = true;
		}
		else if (tmpStr == "-readRandomEngineStatus")
		{
			m_doReadRandomEngineStatus = true;
		}
		else if (tmpStr == "-limitRange")
		{
			m_doLimitRange = true;
		}
#if IS_GAMOS_DISTRIBUTION == 0
		else if (tmpStr == "-debugCalcNearestPoint")
		{
			m_doDebugCalcNearestPoint = true;
		}
		else if (tmpStr == "-debugConvergence")
		{
			m_doOutputConvergence = true;
		}
#endif
		else if (  tmpStr == "-help")
		{
			m_doPrintFlagsAndQuit = true;
		}
		else if (tmpStr.size() > 0)
		{
			cout << "Error! Wrong flag: " << tmpStr << endl;
			cout << endl;
			m_doPrintFlagsAndQuit = true;
		}
		else if (m_eventType == EVENTTYPE_NONE)
		{
			cout << "Error! Select -PET or -Compton" << endl;
			cout << endl;
			m_doPrintFlagsAndQuit = true;
		}

		if (m_doPrintFlagsAndQuit)
		{
			cout << "Possible flags: " << endl;
			cout << "SOE_ImageRecon " << endl;
			cout << "SOE_ImageRecon -PET (for PET data)" << endl;
			cout << "SOE_ImageRecon -Compton (for Compton Camera data)" << endl;
			cout << "SOE_ImageRecon -seed <seedNumber>  (selecting specific seed)" << endl;
			cout << "SOE_ImageRecon -randomEventLoop (looping over events in random order)" << endl;
			cout << "SOE_ImageRecon -extraIterInfo (additional output files for intermediate iterations)" << endl;
			cout << "SOE_ImageRecon -limitRange (only select random positions inside FOV)" << endl;
			cout << "SOE_ImageRecon -readDensityMatrix (read density matrix 'in_density_matrix.img' at initialisation of program)" << endl;
			cout << "SOE_ImageRecon -readCurrentState (read current state file 'currentstate.dat' at initialisation of program)" << endl;
//			cout << "SOE_ImageRecon -readRandomEngineStatus (read initial random engine status from 'in_RandomEngineStatus.rndm')" << endl;
#if IS_GAMOS_DISTRIBUTION == 0
			cout << "SOE_ImageRecon -debugCalcNearestPoint" << endl;
			cout << "SOE_ImageRecon -debugConvergence" << endl;
#endif
			cout << "SOE_ImageRecon -help (for this help output)" << endl;
			exit(1);
		}
	}

	if (m_eventType == COMPTON_EVENT)
        cout << "Running mode: Compton camera" << endl;
    else
        cout << "Running mode: PET" << endl;


	if (m_doRandomEventSelection)
		cout << "RANDOM EVENT ORDER" << endl;
	else
		cout << "CONSECUTIVE EVENT ORDER" << endl;

	cout << endl;
}

// ===========================================================================================================

void
GetGeomResolutionRecoveryPosition(const double& in_gDeltaX, const double& in_gDeltaY, const double& in_gDeltaZ
            , C3Vector& io_position)
{
	double x = io_position.GetX();
	double y = io_position.GetY();
	double z = io_position.GetZ();

	if (in_gDeltaX > 0.0)
	{
		double randval = CLHEP::RandFlat::shoot(2 * in_gDeltaX);
		x += randval - in_gDeltaX;
	}
	if (in_gDeltaY > 0.0)
	{
		double randval = CLHEP::RandFlat::shoot(2 * in_gDeltaY);
		y += randval - in_gDeltaY;
	}
	if (in_gDeltaZ > 0.0)
	{
		double randval = CLHEP::RandFlat::shoot(2 * in_gDeltaZ);
		z += randval - in_gDeltaZ;
	}

	io_position.Set(x, y, z);
}

// ===========================================================================================================

void
GetEnergyResolutionRecoveryEnergy(const double& in_eResolution, double& io_e)
{
  	// Energy resolution
	double gaussVal = CLHEP::RandGauss::shoot(1.0, in_eResolution);
	io_e = io_e * gaussVal;
}

// ===========================================================================================================

#if IS_GAMOS_DISTRIBUTION == 0
void
DebugNearestPositions(const C3Vector& in_hitPosition1, const double& in_hitE1,
                        const C3Vector& in_hitPosition2, const double& in_hitE2,
                        CCone& io_cone, std::ofstream& in_ofile,
                        C3Vector& io_nearestPos, double& thetaReal)
{
// TODO: make this a user-given value...
C3Vector m_targetPoint(0.5, 0.5, -100);

    double delTh_PointMinOnCone = 0.0;

    io_cone.GetNearestPointOnCone(m_targetPoint, io_nearestPos, delTh_PointMinOnCone);

    double comptonAngle = io_cone.GetComptonAngle();
    thetaReal = comptonAngle + delTh_PointMinOnCone;

    C3Vector voxelReal;
    m_userParameters->CalculateVoxel( m_targetPoint, voxelReal );
    C3Vector voxelNearest;
    m_userParameters->CalculateVoxel( io_nearestPos, voxelNearest );

    if (in_ofile.is_open())
	{
		in_ofile << in_hitPosition1 << " " << in_hitE1 << " "
				<< in_hitPosition2 << " " << in_hitE2 << " "
				<< (comptonAngle/kPI)*180.0 << " " << (thetaReal/kPI)*180.0
				<< " " << m_targetPoint << " " << voxelReal
				<< " " << io_nearestPos << " " << voxelNearest << " "
				<< endl;
	}
}

// =============================================================================================

#endif





