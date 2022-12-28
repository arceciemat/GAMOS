#include "GmModuleJaws.hh"
#include "GmModuleMgr.hh"

#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLinLin.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"

#include <sstream>

//------------------------------------------------------------------------
GmModuleJaws::GmModuleJaws( const std::vector<G4String>& par ) : GmVModule(par)
{
#ifdef BEAMZPOS
  G4Exception(" GmModuleJaws::GmModuleJaws",
	      "",
	      FatalException,
	      "BEAMZPOS option is no longer supported, please contact GAMOS authors if you really need it");	      
#endif
}

//------------------------------------------------------------------------
void GmModuleJaws::BuildObjects()
{
  G4int ip = 0;

  SetWord("NAME",ip++);
  GmModuleMgr::GetInstance()->RegisterModule(this,theWords["NAME"]);
  theName = theWords["NAME"];

  SetWord("ORIENTATION",ip++);
  if( theWords["ORIENTATION"]!="X" &&  theWords["ORIENTATION"]!="Y"){
    G4Exception("GmModuleJaws::BuildObjects",
		"Wrong ORIENTATION",
		FatalErrorInArgument,
		G4String("It is: "+theWords["ORIENTATION"]+" , while it should be X or Y").c_str());
  }  
  SetWord("JAWSTIPTYPE",ip++);
  /*if( theWords["LEAFTIPTYPE"] == "ROUND_DISP_POSFILE" ) {
    theWords["LEAFTIPTYPE"] == "ROUND_POSFILE";
    }*/
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintW("NAME") << PrintW("ORIENTATION") << PrintW("JAWSTIPTYPE") << G4endl;
#endif
  SetWord("XHDIM",ip++);
  SetWord("YHDIM",ip++);
  SetWord("ZHDIM",ip++);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) )  G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("XHDIM") << PrintWVal("YHDIM") << PrintWVal("ZHDIM") << G4endl;
#endif
  G4double posCircleZ = 0.;
  if( theWords["JAWSTIPTYPE"] == "ROUND" ) {
    SetWord("TIP_RADIUS",ip++);
    theWords["TIP_CIRCLE_Z"] = theWords["ZHDIM"];
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("TIP_RADIUS") << G4endl;
#endif
  } else if( theWords["JAWSTIPTYPE"] == "ROUND_DISP" ) {
    SetWord("TIP_RADIUS",ip++);
    SetWord("TIP_CIRCLE_Z",ip++);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("TIP_RADIUS") << PrintWVal("TIP_CIRCLE_Z") << theWords["TIP_POS_FILE"] << G4endl;
#endif
  } else if( theWords["JAWSTIPTYPE"] == "ROUND_DISP_POSFILE" ) {
    SetWord("TIP_RADIUS",ip++);
    SetWord("TIP_CIRCLE_Z",ip++);
    SetWord("TIP_POS_FILE",ip++);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("TIP_RADIUS") << PrintWVal("TIP_CIRCLE_Z") << PrintWVal("TIP_POS_FILE") << G4endl;
#endif
  } else if( theWords["JAWSTIPTYPE"].substr(0,8) == "STRAIGHT" ) {
  } else {
    G4Exception("GmModuleJaws:BuildObjects",
		"MJ001",
		FatalException,
		("JAWS TYPE HAS TO BE STRAIGHT, STRAIGHT_HORIZ, ROUND, ROUND_DISP OR ROUND_DISP_POSFILE, WHILE IT IS "+theWords["JAWSTIPTYPE"]).c_str());
  }
  G4double HVL = 0.;
  if( theWords["JAWSTIPTYPE"] == "ROUND" 
      || theWords["JAWSTIPTYPE"] == "ROUND_DISP" ){
    SetWord("HVL",ip++);
    HVL = G4tgrUtils::GetDouble(theWords["HVL"]);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("HVL") << G4endl;
  #endif
  }
  
  SetWord("Z_FOCUS",ip++); G4double Z_FOCUS    = G4tgrUtils::GetDouble(theWords["Z_FOCUS"]);
  if( theWords["JAWSTIPTYPE"].substr(0,8) == "STRAIGHT" ) {
    SetWord("RADIUS",ip++);
    if( G4tgrUtils::GetDouble(theWords["RADIUS"]) <= 0. )  {
      G4Exception("GmModuleJaws::BuildObjects",
		  "",
		  FatalErrorInArgument,
		  G4String("RADIUS of movement is " + theWords["RADIUS"]+" , while it should be positive").c_str());
    }
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("RADIUS") << G4endl;
#endif
  }
  SetWord("Z_CENTRE",ip++); 
  G4double posZ = G4tgrUtils::GetDouble(theWords["Z_CENTRE"]);
  SetWord("Z_ISOCENTRE",ip++); G4double Z_ISOCENTRE= G4tgrUtils::GetDouble(theWords["Z_ISOCENTRE"]);
  
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("Z_FOCUS") << PrintWVal("Z_CENTRE") << PrintWVal("Z_ISOCENTRE") << G4endl;
#endif
  SetWord("FIELD_NEG",ip++);
  SetWord("FIELD_POS",ip++);
  G4double openingNeg = G4tgrUtils::GetDouble(theWords["FIELD_NEG"]);
  G4double openingPos = G4tgrUtils::GetDouble(theWords["FIELD_POS"]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintWVal("FIELD_NEG") << PrintWVal("FIELD_POS") << G4endl;
#endif
  SetWord("MATE",ip++);
  SetWord("MOTHER_VOLUME",ip++);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleJaws::BuildObjects " << PrintW("MATE") << PrintW("MOTHER_VOLUME") << G4endl;
#endif
  
  std::ostringstream fout;
  G4String RM0Name = "RM0_"+theName;
  fout << ":ROTM " << RM0Name << " 0. 0. 0."; 
  BuildObject( fout );
  if( theWords["JAWSTIPTYPE"].substr(0,8) == "STRAIGHT" ) {
    fout << ":VOLU " << theWords["NAME"] << " BOX " 
       << theWords["XHDIM"] << " " 
	 << theWords["YHDIM"] << " " 
	 << theWords["ZHDIM"] << " " 
	 << theWords["MATE"];
    BuildObject( fout );
  } else if( theWords["JAWSTIPTYPE"] == "ROUND" 
	    || theWords["JAWSTIPTYPE"] == "ROUND_DISP_POSFILE" ) {
    
    fout << ":SOLID " << theWords["NAME"]+"_a" << " BOX " 
	 << theWords["XHDIM"] << " " 
	 << theWords["YHDIM"] << " " 
	 << theWords["ZHDIM"];
    BuildObject( fout );
    
    if( theWords["ORIENTATION"] == "X" ) {
      fout << ":SOLID " << theWords["NAME"]+"_b" << " TUBE "
	   << "0. " << theWords["TIP_RADIUS"]
	   << " " << theWords["YHDIM"];
      BuildObject( fout );
      fout << ":ROTM RM90X_JAWS 90. 0. 0. ";
      BuildObject( fout );
      fout << ":SOLID " << theWords["NAME"] << " INTERSECTION "
	   << theWords["NAME"]+"_a" << " "
	   << theWords["NAME"]+"_b" << " "
	   << "RM90X_JAWS " << theWords["TIP_RADIUS"] << "-" << theWords["XHDIM"]
	   << " 0. "
	   << theWords["ZHDIM"] << "-" << theWords["TIP_CIRCLE_Z"];
      BuildObject( fout );
    } else if( theWords["ORIENTATION"] == "Y" ) {
      fout << ":SOLID " << theWords["NAME"]+"_b" << " TUBE "
	   << "0. " << theWords["TIP_RADIUS"]
	   << " " << theWords["YHDIM"];
      BuildObject( fout );
      fout << ":ROTM RM90Y_JAWS 0. 90. 0. ";
      BuildObject( fout );
      fout << ":SOLID " << theWords["NAME"] << " INTERSECTION "
	   << theWords["NAME"]+"_a" << " "
	   << theWords["NAME"]+"_b" << " "
	   << "RM90Y_JAWS 0. "
	   << theWords["TIP_RADIUS"] << "-" << theWords["YHDIM"] << " "
	   << theWords["ZHDIM"] << "-" << theWords["TIP_CIRCLE_Z"];
      BuildObject( fout );
    }
    
    fout << ":VOLU " << theWords["NAME"]
	 << " "  << theWords["NAME"]
	 << " "  << theWords["MATE"];
    BuildObject( fout );

// define common things for ROUND and ROUND_DISP_POSFILE
    posCircleZ = posZ + G4tgrUtils::GetDouble(theWords["ZHDIM"]) - G4tgrUtils::GetDouble(theWords["TIP_CIRCLE_Z"]);
    //    G4cout << " posCircleZ " << posCircleZ << " = " << posZ << " + " << G4tgrUtils::GetDouble(theWords["ZHDIM"]) << " - " << G4tgrUtils::GetDouble(theWords["TIP_CIRCLE_Z"]) << G4endl; //GDEB

    if( theWords["JAWSTIPTYPE"] == "ROUND" ||
	theWords["JAWSTIPTYPE"] == "ROUND_DISP" ) {
      G4double slopeN = openingNeg / (Z_ISOCENTRE-Z_FOCUS);
      G4double slopeN2 = slopeN*slopeN;
      G4double hvlXN = HVL*(slopeN2+1)/2/cos(atan(fabs(slopeN)));
      G4double profR = G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]);

  //    G4double CCN = slopeN2*posZ2-(1.+slopeN2)*(posZ2-profR*profR)-hvlXN*hvlXN/4.*(1.+slopeN2)*(1.+slopeN2);
      
      //    G4double CCN = profR*(1.+slopeN2)-hvlXN*hvlXN;
      //    G4cout << " CCN " << CCN << " posZ " << posZ << " slopeN " << slopeN << " < posZ2 " << posZ2 << " :: " << slopeN2*posZ2 << " - " << (1.+slopeN2)*(posZ2-profR*profR) << " - " << hvlXN*hvlXN/4.*(1+slopeN2)*(1+slopeN2) << " ::: " << slopeN2 << " * " << posZ2 << " -(1+ " << slopeN2 << " )*( " << posZ2 << " - " <<profR << " * " << profR << " )- " << hvlXN << " * " << hvlXN << " /4.*(1+ " << slopeN2 << " )*(1+ " << slopeN2 << G4endl; //GDEB
      G4double posCN = GetPosRound( posCircleZ, slopeN, profR, hvlXN, -1 );
      //    G4double posCN = posCircleZ*slopeN-sqrt(profR*profR*(1.+slopeN2)-hvlXN*hvlXN);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " POS_CIRCLE_CENTRE " << posCN << " slopeN " << slopeN << " posCircleZ " << posCircleZ  <<  G4endl;  
#endif
      
      G4double slopeP = openingPos / (Z_ISOCENTRE-Z_FOCUS);
      G4double slopeP2 = slopeP*slopeP;
      G4double hvlXP = HVL*(slopeP2+1)/2/cos(atan(fabs(slopeP)));

      G4double posCP = GetPosRound( posCircleZ, slopeP, profR, hvlXP, 1 );
      //    G4double posCP = posCircleZ*slopeP+sqrt(profR*profR*(1.+slopeP2)-hvlXP*hvlXP);
      
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " POS_CIRCLE_CENTRE " << posCP << " slopeP " << slopeP << " posCircleZ " << posCircleZ  <<  G4endl;  
#endif
      
      fout << ":ROTM RM180Z_JAWS 0. 0. 180. ";
      BuildObject( fout );
      
      if( theWords["ORIENTATION"] == "X" ) {
	posCN -= G4tgrUtils::GetDouble(theWords["XHDIM"])-profR;
	posCP += G4tgrUtils::GetDouble(theWords["XHDIM"])-profR;
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << " posCP FINAL " << posCP << " posCN FINAL " << posCN << " XHDIM "<<  G4tgrUtils::GetDouble(theWords["XHDIM"]) << " profR " << profR << G4endl;  
#endif
	//--- Build first jaw    
	fout << ":PLACE " << theWords["NAME"] << " 2 " 
	     << theWords["MOTHER_VOLUME"] << " " 
	     << "RM180Z_JAWS "
	     << posCN << "  "
	     << " 0. "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );
	//--- Build second jaw    
	fout << ":PLACE " << theWords["NAME"] << " 1 " 
	     << theWords["MOTHER_VOLUME"] << " " 
	     << RM0Name << " "
	     << posCP << " "
	     << " 0. "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );
	
      } else if( theWords["ORIENTATION"] == "Y" ) {
	posCN -= G4tgrUtils::GetDouble(theWords["YHDIM"])-profR;
	posCP += G4tgrUtils::GetDouble(theWords["YHDIM"])-profR;
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << " posCP FINAL " << posCP << " posCN FINAL " << posCN << " XHDIM "<<  G4tgrUtils::GetDouble(theWords["XHDIM"]) << " profR " << profR << G4endl;  
#endif
	//--- Build first jaw    
	fout << ":PLACE " << theWords["NAME"] << " 2 " 
	     << theWords["MOTHER_VOLUME"] << " "
	     << "RM180Z_JAWS "	
	     << " 0. "
	     << posCN << " "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );
	
	//--- Build second jaw    
	fout << ":PLACE " << theWords["NAME"] << " 1 " 
	     << theWords["MOTHER_VOLUME"] << " "
	     << RM0Name 
	     << " 0. "
	     << posCP << " "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );
	
      } else {
	G4Exception("GmModuleJaws::BuildObjects",
		    "Wrong orientation type of JAWS",
		    FatalErrorInArgument,
		    G4String("It is: "+theWords["ORIENTATION"]+" , while it should be X or Y").c_str());
      }
     
    } else if( theWords["JAWSTIPTYPE"] == "ROUND_DISP_POSFILE" ) {
      //    G4double profR = G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]);
      
      GmParameterMgr::GetInstance()->AddParam("leafPosJawsDist:FileName " + theWords["TIP_POS_FILE"]);
      GmParameterMgr::GetInstance()->AddParam("leafPosJawsDist:Data InitialPosX"); // dummy data
      std::vector<G4String> paramDist;
      paramDist.push_back("leafPosJawsDist");
      paramDist.push_back("GmNumericDistributionLinLin");
      GmNumericDistributionLinLin* posLeafDist = (GmNumericDistributionLinLin*)(GmDistributionMgr::GetInstance()->FindOrBuildDistribution(paramDist));
      posLeafDist->Initialize();
      
      G4double posCN = -posLeafDist->GetNumericValueFromIndex(-G4tgrUtils::GetDouble(theWords["FIELD_NEG"]));
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " posCN " << posCN << " = ( " << G4tgrUtils::GetDouble(theWords["FIELD_NEG"]) << G4endl;
#endif
      posCN *= posCircleZ/G4tgrUtils::GetDouble(theWords["Z_ISOCENTRE"]);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " posCN rescaled " << posCN << " *= " << posCircleZ << " / " << G4tgrUtils::GetDouble(theWords["Z_ISOCENTRE"]) << G4endl;
#endif
      
      G4double posCP = posLeafDist->GetNumericValueFromIndex(G4tgrUtils::GetDouble(theWords["FIELD_POS"]));
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " posCP " << posCP << " = ( " << G4tgrUtils::GetDouble(theWords["FIELD_POS"]) << G4endl;
#endif
      
      posCP *= posCircleZ/G4tgrUtils::GetDouble(theWords["Z_ISOCENTRE"]);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " posCP rescaled " << posCP << " *= " << posCircleZ << " / " << G4tgrUtils::GetDouble(theWords["Z_ISOCENTRE"]) << G4endl; //GDEB
#endif
      
      fout << ":ROTM RM180Z_JAWS 0. 0. 180. ";
      BuildObject( fout );
      
      if( theWords["ORIENTATION"] == "X" ) {
	G4double angle = asin( fabs(G4tgrUtils::GetDouble(theWords["ZHDIM"])-G4tgrUtils::GetDouble(theWords["TIP_CIRCLE_Z"]))/G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]));
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) )  G4cout << " ZHDIM-TIP_CIRCLE_Z ANGLE " << angle << " " << cos(angle) << G4endl;
#endif
	G4double xlen = G4tgrUtils::GetDouble(theWords["XHDIM"])*cos(angle);
	//	posCN -= G4tgrUtils::GetDouble(theWords["XHDIM"]);
	posCN -= xlen;
	posCP += xlen;
	//      G4cout << "FINAL posCN " << posCN << " -= " << G4tgrUtils::GetDouble(theWords["XHDIM"]) << " - " << profR << G4endl; //GDEB
	//--- Build first jaw    
	fout << ":PLACE " << theWords["NAME"] << " 2 " 
	     << theWords["MOTHER_VOLUME"] << " " 
	     << "RM180Z_JAWS "
	     << posCN << "  "
	     << " 0. "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );

	//--- Build second jaw    
	fout << ":PLACE " << theWords["NAME"] << " 1 " 
	     << theWords["MOTHER_VOLUME"] << " " 
	     << RM0Name
	     << posCP << " "
	     << " 0. "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );
	
      } else if( theWords["ORIENTATION"] == "Y" ) {
	G4double angle = asin( fabs(G4tgrUtils::GetDouble(theWords["ZHDIM"])-G4tgrUtils::GetDouble(theWords["TIP_CIRCLE_Z"]))/G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]));
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) )  G4cout << " ZHDIM-TIP_CIRCLE_Z ANGLE " << angle << " " << cos(angle) << G4endl;
#endif
	G4double xlen = G4tgrUtils::GetDouble(theWords["YHDIM"])*cos(angle);
	posCN -= xlen;
	posCP += xlen;
	//-	posCN -= G4tgrUtils::GetDouble(theWords["YHDIM"]);

	//--- Build first jaw    
	fout << ":PLACE " << theWords["NAME"] << " 2 " 
	     << theWords["MOTHER_VOLUME"] << " "
	     << "RM180Z_JAWS "	
	     << " 0. "
	     << posCN << " "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );
      
	//--- Build second jaw    
	fout << ":PLACE " << theWords["NAME"] << " 1 " 
	     << theWords["MOTHER_VOLUME"] << " "
	     << RM0Name
	     << " 0. "
	     << posCP << " "
	     << theWords["Z_CENTRE"];
	BuildObject( fout );
	
      } else {
	G4Exception("GmModuleJaws::BuildObjects",
		    "Wrong orientation type of JAWS",
		    FatalErrorInArgument,
		    G4String("It is: "+theWords["ORIENTATION"]+" , while it should be X or Y").c_str());
      }
    }
  }

  //@@@@@ PLACEMENT "STRAIGHT" : rotate JAWS with FOCUS as centre: make that border closer to vertical has a position corresponding to the rotation angle, so that it is face is parallel to rays
  if( theWords["JAWSTIPTYPE"].substr(0,8) == "STRAIGHT" ) {
    G4double angleN = atan(openingNeg / 
			   (Z_ISOCENTRE - Z_FOCUS));
    //    G4cout << "CHECK ANGLEN " << angleN/CLHEP::deg << " = " << openingNeg / (Z_ISOCENTRE - Z_FOCUS) << " " << openingNeg << "/" << (Z_ISOCENTRE - Z_FOCUS) << G4endl; //GDEB
    G4double angleP = atan(openingPos /
      (Z_ISOCENTRE - Z_FOCUS));
    
    G4double RADIUS = G4tgrUtils::GetDouble(theWords["RADIUS"]);
    
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " RADIUS " << RADIUS << " angleN " << angleN << " angleP " << angleP << " " << RADIUS*sin(angleN) << " " << RADIUS*cos(angleN) << " " << RADIUS*sin(angleP) << " " << RADIUS*cos(angleP) <<  G4endl;  
#endif

    G4double angleI = (180.*CLHEP::deg-angleN)/2.; // angle of isosceles triangle formed by rotation origin, border point at Y=0,Z=RADIUS if no rotation and border point after rotating N
    G4double lenI = 2*RADIUS*sin(angleN/2.);
    G4double borderT;
    G4double borderZ;
    if( theWords["JAWSTIPTYPE"] == "STRAIGHT" ) {
      //	   << RADIUS*sin(angleN) - YHDIM*cos(angleN) << " " 
      borderT = lenI*sin(angleI);
      borderZ = posZ + lenI*cos(angleI);
      G4double angleBorder = atan(borderT/borderZ);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " JAWS CHECK BORDER " << angleBorder << " = " << angleN << " borderT " << borderT << " borderZ " << borderZ << G4endl; 
#endif
    } else if( theWords["JAWSTIPTYPE"] == "STRAIGHT_HORIZ" ) {
      borderT = RADIUS*tan(angleN);
      borderZ = -RADIUS;
      G4double angleBorder = atan(borderT/borderZ);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " JAWS CHECK BORDER " << angleBorder << " = " << angleN << " borderT " << borderT << " borderZ " << borderZ << G4endl;
#endif
    }

    if( theWords["ORIENTATION"] == "X" ) {
      G4double XHDIM = G4tgrUtils::GetDouble(theWords["XHDIM"]);
      
      G4double centreX = borderT + XHDIM*cos(angleN);
      G4double centreZ = borderZ + XHDIM*sin(angleN);
      G4double angleCentre = atan(centreX/centreZ);
      G4double angle0 = atan(XHDIM/posZ);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " JAWS CHECK CENTER " << angleCentre << " = " << angleN-angle0 << " centreX " << centreX << " centreZ " << centreZ << G4endl;
#endif
      fout << centreX << " " 
	   << "0. "
	   << centreZ;
      BuildObject( fout );
      
      //--- Build first jaw    
      G4String rotname = "RMJ_" + theWords["NAME"];
      fout << ":ROTM " << rotname << "_1 " 
	   << " 0. " << -angleN/CLHEP::deg << " 0. ";
      BuildObject( fout );
      
      fout << ":PLACE " << theWords["NAME"] << " 1 " 
	   << theWords["MOTHER_VOLUME"] << " " 
	   << rotname << "_1 "
	   << -centreX << " " 
	   << " 0. " 
	   << centreZ;
      BuildObject( fout );
      
      //--- Build second jaw    
      fout << ":ROTM " << rotname << "_2 " 
	   << " 0. " << -angleP/CLHEP::deg << " 0. ";
      BuildObject( fout );

      fout << ":PLACE " << theWords["NAME"] << " 2 " 
	   << theWords["MOTHER_VOLUME"] << " " 
	   << rotname << "_2 "
	   << centreX << " " 
	   << " 0. " 
	   << centreZ;
      BuildObject( fout );
      
    } else if( theWords["ORIENTATION"] == "Y" ) {
      G4double YHDIM = G4tgrUtils::GetDouble(theWords["YHDIM"]);

      G4double centreY = borderT + YHDIM*cos(angleN);
      G4double centreZ = borderZ + YHDIM*sin(angleN);
      G4double angleCentre = atan(centreY/centreZ);
      G4double angle0 = atan(YHDIM/posZ);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " JAWS CHECK CENTER " << angleCentre << " = " << angleN-angle0 << " centreY " << centreY << " centreZ " << centreZ << G4endl;
#endif
      //--- Build first jaw    
      G4String rotname = "RMJ_" + theWords["NAME"] ;
      fout << ":ROTM " << rotname << "_1 " 
	   << angleN/CLHEP::deg-180. << " 0. 0. " ;
      BuildObject( fout );
      fout << ":PLACE " << theWords["NAME"] << " 1 " 
	   << theWords["MOTHER_VOLUME"] << " " 
	   << rotname << "_1 "
	   << " 0. " 
	   << -centreY << " " 
	   << centreZ;
      BuildObject( fout );
      
      //--- Build second jaw    
      fout << ":ROTM " << rotname << "_2 " 
	   << angleP/CLHEP::deg-180. << " 0. 0. " ;
      BuildObject( fout );
      
      fout << ":PLACE " << theWords["NAME"] << " 2 " 
	   << theWords["MOTHER_VOLUME"] << " " 
	   << rotname << "_2 "
	   << " 0. " 
	   << centreY << " " 
	   << centreZ;
      
      BuildObject( fout );
      
    } else {
      G4Exception("GmModuleJaws::BuildObjects",
		  "Wrong orientation type of JAWS",
		  FatalErrorInArgument,
		  G4String("It is: "+theWords["ORIENTATION"]+" , while it should be X or Y").c_str());
    }
   
  }
}

//------------------------------------------------------------------------
GmModuleJaws::~GmModuleJaws()
{
}
