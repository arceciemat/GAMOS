#include "GmModuleJaws.hh"
#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"

//------------------------------------------------------------------------
GmModuleJaws::GmModuleJaws( const std::vector<G4String>& par ) : GmVModule(par)
{
  G4tgrUtils::CheckWLsize( par, 14, WLSIZE_EQ,
                           "GmModuleJaws::GmModuleJaws");

}

//------------------------------------------------------------------------
void GmModuleJaws::BuildObjects()
{
  G4int ip = 0;
  theWords["NAME"] = theParams[ip++];
  theWords["TYPE"] = theParams[ip++];
  theWords["XDIM"] = theParams[ip++];
  theWords["YDIM"] = theParams[ip++];
  theWords["ZDIM"] = theParams[ip++];
  theWords["Z_FOCUS"] = theParams[ip++];
  theWords["Z_TOP"] = theParams[ip++];
  theWords["Z_PATIENT"] = theParams[ip++];
  theWords["PROJ_1"] = theParams[ip++];
  theWords["PROJ_2"] = theParams[ip++];
  theWords["MATE"] = theParams[ip++];
  theWords["WORLD"] = theParams[ip++];

  std::ostringstream fout;
  fout << ":VOLU " << theWords["NAME"] << " BOX " 
       << theWords["XDIM"] << " " 
       << theWords["YDIM"] << " " 
       << theWords["ZDIM"] << " " 
       << theWords["MATE"];
  BuildObject( fout );

  G4double angle_1 = atan(G4tgrUtils::GetDouble(theWords["PROJ_1"]) / 
			(G4tgrUtils::GetDouble(theWords["Z_PATIENT"]) - 
			 G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) ) );
  G4double angle_2 = -atan(G4tgrUtils::GetDouble(theWords["PROJ_2"]) / 
			(G4tgrUtils::GetDouble(theWords["Z_PATIENT"]) - 
			 G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) ) );
  G4double radius = G4tgrUtils::GetDouble(theWords["Z_TOP"]) - 
    G4tgrUtils::GetDouble(theWords["Z_FOCUS"]);

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " RADIUS " << radius << " angle_1 " << angle_1 << " angle_2 " << angle_2 << " " << radius*sin(angle_1) << " " << radius*cos(angle_1) << " " << radius*sin(angle_2) << " " << radius*cos(angle_2) <<  G4endl;  
#endif
  G4String rotname = "RM_" + theWords["NAME"] ;
  
  if( theWords["TYPE"] == "X" ) {
    G4double XDIM = G4tgrUtils::GetDouble(theWords["XDIM"]);

    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << " 0. " << -angle_1/CLHEP::deg << " 0. ";
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << theWords["WORLD"] << " " 
 	 << rotname << "_1 "
	 << radius*sin(angle_1) + XDIM*cos(angle_1)
	 << " 0. " 
	 << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius*cos(angle_1) - XDIM*sin(angle_1); 

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) )  G4cout << " RADIUS POS Z " << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius*cos(angle_1) - XDIM*sin(angle_1) << " " 
				    << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) << " " << radius*cos(angle_1) << " " << XDIM*sin(angle_1) << G4endl;
#endif

    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << " 0. " << -angle_2/CLHEP::deg << " 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << theWords["WORLD"] << " " 
	 << rotname << "_2 "
	 << radius*sin(angle_2) - XDIM*cos(angle_2)
	 << " 0. " 
	 << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius*cos(angle_2) + XDIM*sin(angle_2);
    BuildObject( fout );
  } else 
  if( theWords["TYPE"] == "Y" ) {
    G4double YDIM = G4tgrUtils::GetDouble(theWords["YDIM"]);

    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << angle_1/CLHEP::deg << " 0. 0. " ;
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << theWords["WORLD"] << " " 
	 << rotname << "_1 "
	 << " 0. " 
	 << radius*sin(angle_1) + YDIM*cos(angle_1) << " " 
	 << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius*cos(angle_1) - YDIM*sin(angle_1); 
    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << angle_2/CLHEP::deg << " 0. 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << theWords["WORLD"] << " " 
	 << rotname << "_2 "
	 << " 0. " 
	 << radius*sin(angle_2) - YDIM*cos(angle_2) << " " 	
	 << G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + radius*cos(angle_2) + YDIM*sin(angle_2); 
    BuildObject( fout );

  } else {
    G4Exception("GmModuleJaws::BuildObjects",
		"Wrong type of JAWS",
		FatalErrorInArgument,
		G4String("It is: "+theWords["TYPE"]+" , while it should be X or Y").c_str());
  }

}

//------------------------------------------------------------------------
GmModuleJaws::~GmModuleJaws()
{
}
