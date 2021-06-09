#include "GmModuleRangeModulator.hh"
#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"

//------------------------------------------------------------------------
GmModuleRangeModulator::GmModuleRangeModulator( const std::vector<G4String>& par ) : GmVModule(par)
{
//  G4tgrUtils::CheckWLsize( par, 9, WLSIZE_EQ,
//                           "GmModuleRangeModulator::GmModuleRangeModulator");

}

//------------------------------------------------------------------------
void GmModuleRangeModulator::BuildObjects()
{
  G4int ip = 0;
  theWords["NAME"] = theParams[ip++];
  theWords["RMIN"] = theParams[ip++];
  theWords["RIN"] = theParams[ip++];
  theWords["ROUT"] = theParams[ip++];
  theWords["ZSIZE"] = theParams[ip++];
  theWords["NBLADE"] = theParams[ip++];
  theWords["STEPS"] = theParams[ip++];
  theWords["ANGLE"] = theParams[ip++];
  theWords["DANGLE_START"] = theParams[ip++];
  theWords["DANGLE_DECREASE"] = theParams[ip++];



        G4double rmin = G4tgrUtils::GetDouble(theWords["RMIN"]);
        G4double rin = G4tgrUtils::GetDouble(theWords["RIN"]);
        G4double rout = G4tgrUtils::GetDouble(theWords["ROUT"]);
        G4double zsize = G4tgrUtils::GetDouble(theWords["ZSIZE"]);
        G4double nblade = G4tgrUtils::GetDouble(theWords["NBLADE"]);
        G4double steps = G4tgrUtils::GetDouble(theWords["STEPS"]);
        G4double angle = G4tgrUtils::GetDouble(theWords["ANGLE"]);
        G4double dangle_start = G4tgrUtils::GetDouble(theWords["DANGLE_START"]);
        G4double dangle_decrease = G4tgrUtils::GetDouble(theWords["DANGLE_DECREASE"]);
	
	G4double zstep = zsize/steps;




  std::ostringstream fout;
  
  G4int solid_counter = 0;	

  fout << ":ROTM " << theWords["NAME"] << "_R00 0 0 0 "; 
  BuildObject( fout ); 


  fout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter 
       << " TUBE " << " " 
       << rmin     << " " 
       << rin      << " " 
       << zsize/2    ;
  BuildObject( fout );
  G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " CONTROL LL " << G4endl;
  solid_counter++;


 G4double dangle_aux = 0;
 for (int b=0; b<nblade; b++){
	dangle_aux=angle;
  	for (int s=0;s<steps;s++){
		dangle_aux=dangle_aux-dangle_start-s*dangle_decrease;
		fout    << ":SOLID " << theWords["NAME"] << "_S" << solid_counter
			<< " TUBS " 
			<< rin   << " "
			<< rout  << " "
			<< zstep/2 << " "
			<< (360/nblade)*(1+b)+dangle_aux/2  << " "
			<< angle-dangle_aux  ;

			//<< (360/nblade)*(1+b)+dangle_start*s/2-dangle_decrease*s/2 << " "
			//<< angle-(dangle_start-s*dangle_decrease)*s ;
			
			// << 360/nblade*(1+b)-360/(2*nblade)+(dangle_start-s*dangle_decrease)*s << " "

		BuildObject( fout );
		G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " CONTROL LL " << G4endl;
		solid_counter++;
		  
		fout    << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " UNION " 
				     << theWords["NAME"] << "_S" << solid_counter-2 << " "
				     << theWords["NAME"] << "_S" << solid_counter-1   << " "
				     << theWords["NAME"] << "_R00 0 0 "  
				     << -zsize/2+zstep/2+(zstep*s) ; 			
		BuildObject( fout );
		G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " CONTROL LL " << G4endl;
		solid_counter++;
  	};
 };
  
	fout    << ":SOLID " << theWords["NAME"] << " UNION " 
                                     << theWords["NAME"] << "_S" << solid_counter-1   << " "
                                     << theWords["NAME"] << "_S" << solid_counter-1   << " "
                                     << theWords["NAME"] << "_R00 "
                                     << " 0 0 0 ";

	BuildObject( fout );


//// DOWNLINEs is a example to program this tool /////

// std::ostringstream fout;
//  fout << ":VOLU " << theWords["NAME"] << " " << theWords["NAME"] <<   
//       << theWords["XDIM"] << " "
//       << theWords["YDIM"] << " "
//       << theWords["ZDIM"] << " "
//       << theWords["MATE"];
//  BuildObject( fout );


//#ifndef GAMOS_NO_VERBOSE

//  if( GeomVerb(debugVerb) ) G4cout << " RADIUS " << radius << " angle_1 " << angle_1 << " angle_2 " << angle_2 << " " << radius*sin(angle_1) << " " << radius*cos(angle_1) << " " << radius*sin(angle_2) << " " << radius*cos(angle_2) <<  G4endl;  

//#endif 


//  if( theWords["TYPE"] == "Y" ) {//
//  } else {
//    G4Exception("GmModuleJaws::BuildObjects",
//		"Wrong type of JAWS",
//		FatalErrorInArgument,
//		G4String("It is: "+theWords["TYPE"]+" , while it should be X or Y").c_str());
//  }

}



//------------------------------------------------------------------------


GmModuleRangeModulator::~GmModuleRangeModulator()
{

}
