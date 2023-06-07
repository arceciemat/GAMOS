#include "GmModuleMLC.hh"
#include "GmModuleMgr.hh"

#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <math.h>
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLinLin.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"

//#include <stdlib.h>
//#include <stdio.h>
//#include <string>

//------------------------------------------------------------------------
GmModuleMLC::GmModuleMLC( const std::vector<G4String>& par ) : GmVModule(par)
{
  //G4tgrUtils::CheckWLsize( par, 310, WLSIZE_EQ,
  //                         "GmModuleMLC::GmModuleMLC");
  
}

//------------------------------------------------------------------------
void GmModuleMLC::BuildObjects()
{
  G4int ip = 0; 
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << "@@@ START TO READ MLC data" << G4endl;
#endif  
  
  //---
  SetWord("NAME",ip++);  
  GmModuleMgr::GetInstance()->RegisterModule(this,theWords["NAME"]);
  theName = theWords["NAME"];
  
  SetWord("TYPE",ip++);          // FOCUSED 
  SetWord("ORIENTATION",ip++);   // X or Y leaf move orientation
  if( theWords["ORIENTATION"] != "X" &&  theWords["ORIENTATION"] != "Y"){
    G4Exception("GmModuleMLC::BuildObjects",
		"Wrong ORIENTATION",
		FatalErrorInArgument,
		G4String("It is: "+theWords["ORIENTATION"]+" , while it should be X or Y").c_str());
  }  
  SetWord("LEAFTIPTYPE",ip++);  // ROUND, ROUND_DISP, ROUND_DISP_POSFILE or STRAIGHT end leaf
  /*  if( theWords["LEAFTIPTYPE"] == "ROUND_DISP_POSFILE" ) {
    theWords["LEAFTIPTYPE"] == "ROUND_POSFILE";
    }*/
  
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintW("NAME") << PrintW("TYPE") << PrintW("ORIENTATION")  << PrintW("LEAFTIPTYPE") << G4endl;
#endif  
  
  SetWord("LONGHDIM",ip++);
  G4double LONGHDIM = G4tgrUtils::GetDouble(theWords["LONGHDIM"]);
  //  theWords["ZHDIM"]      = theParams[ip++);  G4double ZHDIM    = G4tgrUtils::GetDouble(theWords["ZHDIM"]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("LONGHDIM") << G4endl;
#endif  
  
  G4double TIP_RADIUS;
  //  SetWord("TIP_CIRCLE_Z",-1);
  G4double TIP_CIRCLE_Z = 0.;
  if( theWords["LEAFTIPTYPE"] == "ROUND" 
      || theWords["LEAFTIPTYPE"] == "ROUND_DISP" ) {
    SetWord("TIP_RADIUS",ip++);
    TIP_RADIUS  = G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) {
      G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("TIP_RADIUS") << G4endl;
    }
#endif
    if( theWords["LEAFTIPTYPE"] == "ROUND_DISP" ) {
      SetWord("TIP_CIRCLE_Z",ip++);
      TIP_CIRCLE_Z = G4tgrUtils::GetDouble(theWords["TIP_CIRCLE_Z"]);
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) {
	G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("TIP_CIRCLE_Z") << G4endl;
      }
#endif
    }
  } else if( theWords["LEAFTIPTYPE"] == "ROUND_DISP_POSFILE" ) {
    SetWord("TIP_RADIUS",ip++);
    TIP_RADIUS  = G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("TIP_RADIUS") << G4endl;
#endif  
    SetWord("TIP_CIRCLE_Z",ip++);
    TIP_CIRCLE_Z = G4tgrUtils::GetDouble(theWords["TIP_CIRCLE_Z"]);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("TIP_CIRCLE_Z") << G4endl;
#endif  
    SetWord("TIP_POS_FILE",ip++);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << theWords["TIP_POS_FILE"] << G4endl;
#endif
  } else if( theWords["LEAFTIPTYPE"] == "STRAIGHT" ||
	     theWords["LEAFTIPTYPE"] == "STRAIGHT_HORIZ") {
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
  } else {
    G4Exception("GmModuleMLC:BuildObjects",
		"M001",
		FatalException,
		("MLC TYPE HAS TO BE STRAIGHT, STRAIGHT_HORIZ, ROUND, ROUND_DISP or ROUND_DISP_POSFILE, WHILE IT IS "+theWords["LEAFTIPTYPE"]).c_str());
  }
  G4double HVL = 0.;
  if( theWords["LEAFTIPTYPE"] == "ROUND" 
       || theWords["LEAFTIPTYPE"] == "ROUND_DISP" ){
     SetWord("HVL",ip++);
     HVL = G4tgrUtils::GetDouble(theWords["HVL"]);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) {
      G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("HVL") << G4endl;
    }    
#endif
   }
  
  SetWord("Z_FOCUS",ip++);  G4double Z_FOCUS    = G4tgrUtils::GetDouble(theWords["Z_FOCUS"]);
  SetWord("C_FOCUS",ip++); G4double C_FOCUS        = G4tgrUtils::GetDouble(theWords["C_FOCUS"]);
  SetWord("Z_ISOCENTRE",ip++);  G4double Z_ISOCENTRE= G4tgrUtils::GetDouble(theWords["Z_ISOCENTRE"]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("Z_FOCUS") << PrintWVal("C_FOCUS") << PrintWVal("Z_ISOCENTRE") << G4endl;
#endif  
    
  SetWord("Z_TOP",ip++);  G4double Z_TOP      = G4tgrUtils::GetDouble(theWords["Z_TOP"]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("Z_TOP") << G4endl;
#endif  
  
  SetWord("Z_GAP",ip++);  G4double Z_GAP      = G4tgrUtils::GetDouble(theWords["Z_GAP"]);
  SetWord("GAP",ip++);  G4double GAP      = G4tgrUtils::GetDouble(theWords["GAP"]);
  SetWord("CROSS_START_POINT",ip++);  G4double CROSS_START_POINT      = G4tgrUtils::GetDouble(theWords["CROSS_START_POINT"]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("Z_GAP") << PrintWVal("GAP") << PrintWVal("CROSS_START_POINT") << G4endl;
#endif  
  
  //  theWords["Z_PROFILE"]        = theParams[ip++);  G4double Z_PROFILE      = G4tgrUtils::GetDouble(theWords["Z_PROFILE"]);
  //  G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("Z_PROFILE") << G4endl;
  
  SetWord("N_LEAF_CROSS_PROFILES",ip++); G4double N_LEAF_CROSS_PROFILES= G4tgrUtils::GetDouble(theWords["N_LEAF_CROSS_PROFILES"]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("N_LEAF_CROSS_PROFILES") << G4endl;
#endif  
  
  for(int ii=1;ii<int(1+N_LEAF_CROSS_PROFILES);ii++){
    
    SetWord("LeafMode["+GmGenUtils::itoa(ii)+"]",ip++);
    SetWord("NLeafCrossPoints["+GmGenUtils::itoa(ii)+"]",ip++);
    int nLCPoints = int(G4tgrUtils::GetDouble(theWords["NLeafCrossPoints["+GmGenUtils::itoa(ii)+"]"]));
    //  G4cout << " NLeafCrossPoints " << nLCPoints << G4endl;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintW("LeafMode["+GmGenUtils::itoa(ii)+"]") << PrintWVal("NLeafCrossPoints["+GmGenUtils::itoa(ii)+"]") << G4endl;
#endif  
    for(int jj=1; jj<nLCPoints+1; jj++){	
      //      G4cout << " ***** READING PARAMETER ********* " ;
      SetWord("z["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]",ip++);
      SetWord("c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]",ip++);
      
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("z["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]") << PrintWVal("c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]") << G4endl;
#endif  
      
    };
  };
  
  ///--- input lines to Leaf numbers 
  SetWord("N_LEAF_PAIRS",ip++); G4double N_LEAF_PAIRS = G4tgrUtils::GetDouble(theWords["N_LEAF_PAIRS"]);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintWVal("N_LEAF_PAIRS") << G4endl;
#endif  
  
  for(int ii=1;ii<int(1+N_LEAF_PAIRS);ii++){
    if( GmGenUtils::GetValue(theParams[ip]) > N_LEAF_CROSS_PROFILES ) {
      G4Exception("GmModuleMLC::BuildObjects",
		  "",
		  FatalException,
		  ("LEAF TYPE NUMBER "+theParams[ip]
		   +" DOES NOT EXIST. THERE ARE ONLY "+GmGenUtils::itoa(N_LEAF_CROSS_PROFILES)+" TYPES").c_str());
    }
    ///--- input lines of leaf type and opening position 
    SetWord("LEAF_TYPE["+GmGenUtils::itoa(ii)+"]",ip++);		
    SetWord("OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]",ip++); 
    SetWord("OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]",ip++);
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintW("LEAF_TYPE["+GmGenUtils::itoa(ii)+"]") << PrintWVal("OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]") << PrintWVal("OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]") << G4endl;
#endif  
  };
  
  SetWord("MATE",ip++);
  SetWord("MOTHER_VOLUME",ip++);
#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " GmModuleMLC::BuildObjects " << PrintW("MATE") << PrintW("MOTHER_VOLUME") << G4endl;
  if( GeomVerb(debugVerb) ) G4cout << "FULL MODULE READ OK" << G4endl << "*********____________*********" << G4endl << G4endl;
#endif  
  
  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONTRUCT GEOMETRY

  std::ostringstream fout;
  
  G4double cg_min=0; // min cross coordinate proyected in Z_GAP
  G4double cg_max=0; // max cross coordinate proyected in Z_GAP
  G4double cg_min_prev=0;
  G4double cg_max_prev=0;	
  
  G4double c_min=0;
  G4double c_max=0;
  G4double z_min=0;	 
  G4double z_max=0;	 
  
  G4double c1=0;
  //  G4double c2=0;
  G4double z1=0;
  //  G4double z2=0;
  G4double C_MIN_POS = CROSS_START_POINT;
  
  G4String text_aux0;
  G4String text_aux="";
  G4String output_line_text="";
  
  //@@@@@  Build rotation matrices
  G4String R_90x = "RM90X"+theName;
  G4String R_90x_90y = "RM90X90Y"+theName;
  G4String R_90y = "RM90Y"+theName;
  G4String R_270y_180z = "RM270Y180Z"+theName;
  G4String R_270y = "RM270Y"+theName;
  G4String R00   = "RM0"+theName;
  //  G4String R_270x_90z = "RM270X90Z"+theName;
  G4String R_90x_90z = "RM90X90Z"+theName;
  G4String R_aux;
  
  fout  << ":ROTM " << R_90x << " 90 0 0 ";  BuildObject( fout );
  fout  << ":ROTM " << R_90y << " 0 90 0 ";  BuildObject( fout );
  fout  << ":ROTM " << R_270y_180z << " 0 270 180 ";  BuildObject( fout );
  fout  << ":ROTM " << R_270y << " 0 270 0 ";  BuildObject( fout );
  fout  << ":ROTM " << R_90x_90y << " 90 90 0 ";  BuildObject( fout );
  //  fout  << ":ROTM  "<<R_270x_90z  <<"  0 0 -1 1  0 0 0 -1 0 ";  BuildObject( fout );
  //fout  << ":ROTM  "<<R_90x_90z  << " 0 0 0 ";  BuildObject( fout );
  //   fout  << ":ROTM  "<<R_90x_90z  << " 0 1 0 0 0 1 1 0 0 ";  BuildObject( fout );
  //   fout  << ":ROTM  "<<R_90x_90z  << " 0 0 1 1 0 0 0 1 0 ";  BuildObject( fout );
  //  fout  << ":ROTM  "<<R_90x_90z  << " 0 -1 0 0 0 -1 1 0 0 ";  BuildObject( fout ); 
  fout  << ":ROTM  "<<R_90x_90z  << " 90 0 90";  BuildObject( fout ); //8
  fout  << ":ROTM  "<< R00  << " 0 0 0";  BuildObject( fout ); //8

  //    fout  << ":ROTM  "<<R_90x_90z  << " 90 0 -90";  BuildObject( fout ); //10
  //  fout  << ":ROTM  "<<R_90x_90z  << " -90 0 90";  BuildObject( fout ); //12
  //fout  << ":ROTM  "<<R_90x_90z  << " -90 0 -90";  BuildObject( fout ); //14
  //fout  << ":ROTM  "<<R_90x_90z  << " 90 0 0";  BuildObject( fout ); //18
  // fout  << ":ROTM  "<<R_90x_90z  << " 90 0 0";  BuildObject( fout ); //
  
  Z_GAP += Z_TOP;
  //@@@@@ Loop to leaves
  GmNumericDistributionLinLin* posLeafDist = 0;
  if( theWords["LEAFTIPTYPE"]=="ROUND_DISP_POSFILE"){
    std::vector<G4String> paramDist;
    GmParameterMgr::GetInstance()->AddParam("leafPosMLCDist:FileName "+ theWords["TIP_POS_FILE"], PTstring);
    GmParameterMgr::GetInstance()->AddParam("leafPosMLCDist:Data InitialPosX", PTstring); // dummy data
    paramDist.push_back("leafPosMLCDist");
    paramDist.push_back("GmNumericDistributionLinLin");
    posLeafDist = (GmNumericDistributionLinLin*)(GmDistributionMgr::GetInstance()->FindOrBuildDistribution(paramDist));
    posLeafDist->Initialize();
  }

  for(int ii=1;ii<int(1+N_LEAF_PAIRS);ii++){
    
    cg_min = DBL_MAX; // min/max leaf cross coordinates projected in Z_GAP
    cg_max = -DBL_MAX;
    c_min = DBL_MAX; // min/max leaf cross coordinates
    c_max = -DBL_MAX;
    z_min = DBL_MAX; // min/max leaf Z coordinates projected in Z_GAP
    z_max = -DBL_MAX;
    G4String leaf_type = theWords["LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"];
    int nLCPoints = int(G4tgrUtils::GetDouble(theWords["NLeafCrossPoints["+leaf_type+"]"] ));
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) ) G4cout << "***** Start to process leaf " << ii << " out of " << atoi(theWords["N_LEAF_PAIRS"])  << " Leaves " << " , LEAF_TYPE = " << leaf_type << G4endl;
    if( GeomVerb(debugVerb) ) G4cout << " Number of Leaf Cross Points = " << nLCPoints << G4endl;
#endif
    
    text_aux0="NLeafCrossPoints["+leaf_type+"]";
    output_line_text.clear();

    //@@@ FIND intersection of Z_GAP plane with leaf profile lines
    for(int jj = 1;jj <= nLCPoints; jj++){
      c1 = G4tgrUtils::GetDouble(theWords["c["+leaf_type+","+GmGenUtils::itoa(jj)+"]"]);
      /*      int jj2 = jj+1;
      if( jj == nLCPoints ) jj2 = 1;
      //    for(int jj=1;jj<int(-1+1+G4tgrUtils::GetDouble(theWords[text_aux0]));jj++){ 
      c1 = G4tgrUtils::GetDouble(theWords["c["+leaf_type+","+GmGenUtils::itoa(jj)+"]"]);
      c2 = G4tgrUtils::GetDouble(theWords["c["+leaf_type+","+GmGenUtils::itoa(jj2)+"]"]);
      z1 = G4tgrUtils::GetDouble(theWords["z["+leaf_type+","+GmGenUtils::itoa(jj)+"]"]);
      z2 = G4tgrUtils::GetDouble(theWords["z["+leaf_type+","+GmGenUtils::itoa(jj2)+"]"]);
      z1 = z1+Z_TOP;
      z2 = z2+Z_TOP;
#ifndef GAMOS_NO_VERBOSE
      //      if( GeomVerb(debugVerb) ) G4cout << jj << " points " << z1 << " " << c1 << " AND " << z2 << " " << c2 << G4endl; 
#endif
      
      //      if(  ((Z_PROFILE >= z1) && (Z_PROFILE < z2 ))  ||  ((Z_PROFILE <= z1) && (Z_PROFILE > z2 )) ){ 
      if( z1 != z2 ) {
	//-      c1 = (C_LEAF+c1-cg_min-C_FOCUS)*(z1-Z_FOCUS)/(Z_ISOCENTRE-Z_FOCUS);
	
	G4double inters = c1+(c2-c1)/(z2-z1)*(Z_GAP-z1); // intersection with Z_GAP plane
	cg_min = std::min(cg_min,inters);
	cg_max = std::max(cg_max,inters);
	//		v[kk]=c+((c_pi-c)/(z_pi-z))*(Z_PROFILE-z);
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << jj << "@ MIN/MAX INTERSECTION WITH Z_GAP PLANE  cg_min " << cg_min << " cg_max " << cg_max << " c1 " << c1 << " c2 " << c2 << " z1 " << z1 << " z2 " << z2 << G4endl;
#endif
} */
      cg_min = std::min(cg_min,c1);
      cg_max = std::max(cg_max,c1);
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << jj << "@ MIN/MAX INTERSECTION WITH Z_GAP PLANE  cg_min " << cg_min << " cg_max " << cg_max << G4endl;
#endif
    }
    
    if( ii != 1 ) C_MIN_POS += GAP + cg_max_prev-cg_min_prev; 
    
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) )  {
      G4cout <<  ii << "@ START TO CALCULATE THE FINAL C POSITION: MIN_POS " << C_MIN_POS;
       if( ii != 1 ) G4cout << " , added +GAP " << GAP << " +cg_max_prev " << cg_max_prev << " -cg_min_prev " << cg_min_prev;
       G4cout << G4endl;
    }
#endif

    for(int jj = 1;jj <= nLCPoints; jj++){
      c1 = G4tgrUtils::GetDouble(theWords["c["+leaf_type+","+GmGenUtils::itoa(jj)+"]"]);
      z1 = G4tgrUtils::GetDouble(theWords["z["+leaf_type+","+GmGenUtils::itoa(jj)+"]"]);
      //-      z1 += Z_TOP;
      c1 += C_MIN_POS - cg_min; // position in the minimal
      G4double c1t = c1;
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(testVerb) ) G4cout << jj << " points moved to minimal C " << z1 << " " << c1 << G4endl; 
#endif
      
      c1 = C_FOCUS+(z1+Z_TOP-Z_FOCUS)*(c1-C_FOCUS)/(Z_GAP-Z_FOCUS); // project from Z_GAP to z1+Z_TOP
       
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(testVerb) ) G4cout << jj << " c1 " << c1 << " = " << C_FOCUS << " + " <<  (z1+Z_TOP-Z_FOCUS) << " * " << (c1t-C_FOCUS) << " / " << (Z_GAP-Z_FOCUS) << G4endl;
#endif
      
      //@@@ Recalculate cross points projecting the profile so that the lines start at point (Z_FOCUS,C_FOCUS), and points are displaced by C_LEAF      
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(testVerb) ) G4cout << ii << " " << jj << " FINAL LEAF POS " << z1 << " , " << c1 << " FROM " << G4tgrUtils::GetDouble(theWords["z["+leaf_type+","+GmGenUtils::itoa(jj)+"]"]) << " , " << G4tgrUtils::GetDouble(theWords["c["+leaf_type+","+GmGenUtils::itoa(jj)+"]"]) << G4endl;
#endif
      
      if( theWords["LEAFTIPTYPE"]=="ROUND" || theWords["LEAFTIPTYPE"]=="ROUND_DISP"
	  || theWords["LEAFTIPTYPE"]=="ROUND_DISP_POSFILE"){
	output_line_text+=" "+GmGenUtils::ftoa(z1)+" "+GmGenUtils::ftoa(c1)+" ";
      } else if (theWords["LEAFTIPTYPE"].substr(0,8)=="STRAIGHT"){
	output_line_text+=" "+GmGenUtils::ftoa(z1)+" "+GmGenUtils::ftoa(c1)+" ";		
      };
      
      //      G4cout << "output_line_text == " << output_line_text  << G4endl;
      //fout 	<<  GmGenUtils::itoa(z) << " "
      //	<<  GmGenUtils::itoa(c) << " ";
      c_min = std::min( c_min, c1 );
      c_max = std::max( c_max, c1 );
      z_min = std::min( z_min, z1 );
      z_max = std::max( z_max, z1 );
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(debugVerb) )  G4cout << " FINAL z[" << ii << "," << jj << "]= " << z1 << "  c[" << ii << "," << jj << "]= " << c1 << G4endl;
#endif

    };
    
    cg_max_prev=cg_max;
    cg_min_prev=cg_min;
#ifndef GAMOS_NO_VERBOSE
    if( GeomVerb(testVerb) ) {
      G4cout << ii << " C_MIN/MAX IN GLOBAL COORD " << c_min << " " << c_max << " Z_MIN/MAX " << z_min << " " << z_max << G4endl;
      //      G4cout  << "@@@ BUILDING SOLID FOR LEAF " << ii << G4endl;
    }
#endif
    ///---- ROUNDER BORDER    
    if( theWords["LEAFTIPTYPE"]=="ROUND" || theWords["LEAFTIPTYPE"]=="ROUND_DISP"
	|| theWords["LEAFTIPTYPE"]=="ROUND_DISP_POSFILE"){    
      //@@@ UNION OF LEAF PROFILE AND TUBE FOR ROUNDED EDGE
      //@@ PROFILE:
      text_aux=":SOLID "+theWords["NAME"]+"_Leaf_"+GmGenUtils::itoa(ii)+"_BASE_POS  EXTRUDED "+theWords[text_aux0]+" "+output_line_text +" 2 "
	+"-1*"+theWords["LONGHDIM"]+" 0  0  1 "
	+theWords["LONGHDIM"]+" 0  0  1 ";
      fout << text_aux;
      BuildObject( fout );
      
      text_aux=":SOLID "+theWords["NAME"]+"_Leaf_"+GmGenUtils::itoa(ii)+"_BASE_NEG  EXTRUDED "+theWords[text_aux0]+" "+output_line_text+" 2 "
	+"-1*"+theWords["LONGHDIM"]+" 0  0  1 "
	+theWords["LONGHDIM"]+" 0  0  1 ";
      fout   << text_aux  ;
      BuildObject( fout );
      // G4cout << " EXTRUDED B LEAF END" << G4endl;

      //@@ TUBE:
      fout << ":SOLID "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_TUBE  TUBE 0 "<<theWords["TIP_RADIUS"]<<"  "<<GmGenUtils::ftoa((c_max-c_min)/2+0.); 
      BuildObject( fout );

      G4String tubeSolidPos = "_TUBE";
      G4String tubeSolidNeg = "_TUBE";
      if( LONGHDIM*2 > TIP_RADIUS ) {  // IF TUBE RADIUS IS SMALLER THAN LEAF LENGTH, IT NEEDS TO BE ADDED A BOX SO THAT THE INTERSECTION COVERS THE LEAF PROFILE AND THE OTHER TIP IS NOT MADE ROUND
	//	fout << ":SOLID "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_BOX   BOX    "<<" "<<theWords["TIP_RADIUS"]<<" "<<GmGenUtils::ftoa(LONGHDIM-TIP_RADIUS/2)<<" "<<GmGenUtils::ftoa((c_max-c_min)/2+10);
	fout << ":SOLID "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_BOX   BOX    "
	     <<" "<<theWords["TIP_RADIUS"]<<" "
	     <<GmGenUtils::ftoa(LONGHDIM-TIP_RADIUS/2)<<" "
	     <<GmGenUtils::ftoa((c_max-c_min)/2);
	BuildObject( fout );  
      
	fout << ":SOLID "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_TUBEBOX_POS UNION "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_TUBE "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_BOX "<< R00 <<" 0 "<<GmGenUtils::ftoa((LONGHDIM-TIP_RADIUS/2))<<" 0";
	BuildObject( fout );  
	
	fout << ":SOLID "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_TUBEBOX_NEG UNION "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_TUBE "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_BOX "<< R00 <<" 0 -1*("<<GmGenUtils::ftoa((LONGHDIM-TIP_RADIUS/2))<<") 0";
	BuildObject( fout );  
	tubeSolidPos = "_TUBEBOX_POS";
	tubeSolidNeg = "_TUBEBOX_NEG";
      }
    
      //@@@ FINAL INTERSECTION
      fout << ":SOLID "  << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_POS INTERSECTION  " 
	   << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_BASE_POS " 
 	   << " "
	   << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<< tubeSolidPos << " "
	//<< R00 << " 0 0 0 "; 
	//	   << R_90x << " 0. " << (c_max+c_min)/2 << " " << GmGenUtils::ftoa(TIP_RADIUS-LONGHDIM) << " ";
	//t	   << R_90x << " " << -TIP_CIRCLE_Z << " "  << (c_max+c_min)/2 << " " << GmGenUtils::ftoa(-LONGHDIM+TIP_RADIUS) << " ";
#ifndef BEAMZPOS
	   << R_90x << " " << -TIP_CIRCLE_Z << " "  << (c_max+c_min)/2 << " " << GmGenUtils::ftoa(LONGHDIM-TIP_RADIUS) << " ";
#else
           << R_90x << " " << TIP_CIRCLE_Z << " "  << (c_max+c_min)/2 << " " << GmGenUtils::ftoa(LONGHDIM-TIP_RADIUS) << " ";
#endif
      //-	   << R_90x << " " << z_min+TIP_CIRCLE_Z << " " << (c_max+c_min)/2 << " " << GmGenUtils::ftoa(-TIP_RADIUS+LONGHDIM) << " ";
      //	   << R_90x << " " << z_max/2 << " " << (c_max+c_min)/2+z_max/2.-TIP_CIRCLE_Z << " " << GmGenUtils::ftoa(-TIP_RADIUS+LONGHDIM) << " ";
      //	   << rotmLeaf << " " << GmGenUtils::ftoa(Z_TOP+(z_max-Z_TOP)/2.0) << " " << (cg_max+cg_min)/2 << " " << GmGenUtils::ftoa(LONGHDIM+TIP_RADIUS/2) << " ";
      BuildObject( fout );

      fout << ":SOLID "  << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_NEG INTERSECTION  " 
	   << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_BASE_NEG " 
	   << " "
	   << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<< tubeSolidNeg << " "
	//<< R00 << " 0 0 0 "; BuildObject( fout ); 
#ifndef BEAMZPOS
	   << R_90x << " " << -TIP_CIRCLE_Z << " " << (c_max+c_min)/2 <<" "<<GmGenUtils::ftoa(TIP_RADIUS-LONGHDIM) << " ";
#else 
	   << R_90x << " " << z_min+TIP_CIRCLE_Z << " " << (c_max+c_min)/2 <<" "<<GmGenUtils::ftoa(TIP_RADIUS-LONGHDIM) << " ";
#endif
      //	   << R_90x << " " << z_max/2 << " " << (c_max+c_min)/2+z_max/2.-TIP_CIRCLE_Z <<" "<<GmGenUtils::ftoa(TIP_RADIUS-LONGHDIM) << " ";
      BuildObject( fout );

      //@@@  FINAL VOLUME DEFINITION
      fout << ":VOLU  "  << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_POS "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_POS "<<theWords["MATE"];
      BuildObject( fout );
      
      fout << ":VOLU  "  << theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_NEG "<<theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_NEG "<<theWords["MATE"];
      BuildObject( fout );
      
#ifndef GAMOS_NO_VERBOSE
      if( GeomVerb(debugVerb) ) G4cout << "Making the final volume" << G4endl;
#endif
      if( theWords["ORIENTATION"]=="X"){
	//-	R_aux=R_270y_180z;
#ifndef BEAMZPOS
	R_aux=R_90y;
#else
	R_aux=R_90y;
#endif
	//-	R_aux=R00;
      } else {
	R_aux=R_90x_90z;
      }
      //-      R_aux = "RM0";
      G4double openingPos = G4tgrUtils::GetDouble(theWords["OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]"]);
      G4double openingNeg =G4tgrUtils::GetDouble(theWords["OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]"]);
#ifndef BEAMZPOS
      G4double posCircleZ = Z_TOP-TIP_CIRCLE_Z;
#else 
      G4double posCircleZ = Z_TOP+TIP_CIRCLE_Z;
#endif
      
      if( theWords["LEAFTIPTYPE"]=="ROUND"
	  || theWords["LEAFTIPTYPE"]=="ROUND_DISP"){
      
	G4double profR = G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]);
	//	G4double hvlXP = HVL*sin(atan(1./fabs(slopeP)));
	//	G4double posCircleZ2 = posCircleZ*posCircleZ;
	
#ifndef BEAMZPOS
	G4double slopeP = openingPos / (Z_ISOCENTRE-Z_FOCUS);
	G4double slopeP2 = slopeP*slopeP;
	G4double hvlXP = HVL*(slopeP2+1)/2/cos(atan(fabs(slopeP)));
#else 
	G4double slopeP = (Z_ISOCENTRE-Z_FOCUS) / openingPos;
	G4double slopeP2 = slopeP*slopeP;
	G4double hvlXP = HVL*sin(atan(1./fabs(slopeP)));
#endif
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << " hvlXP " <<  G4tgrUtils::GetDouble(theWords["HVL"]) << G4endl;
#endif
	G4double posCP = GetPosRound( posCircleZ, slopeP, profR, hvlXP, 1 );

	//    G4cout << " CCP " << CCP << " < posCircleZ2 " << posCircleZ2 << " :: " << slopeP2*posCircleZ2 << " - " << (1.+slopeP2)*(posCircleZ2-profR*profR) << " - " << hvlXP*hvlXP/4.*(1+slopeP2)*(1+slopeP2) << " ::: " << slopeP2 << " * " << posCircleZ2 << " -(1+ " << slopeP2 << " )*( " << posCircleZ2 << " - " <<profR << " * " << profR << " )- " << hvlXP << " * " << hvlXP << " /4.*(1+ " << slopeP2 << " )*(1+ " << slopeP2 << G4endl; //GDEB
	posCP += G4tgrUtils::GetDouble(theWords["LONGHDIM"])-profR;
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << " POS_CIRCLE_CENTRE " << posCP << " slopeP " << slopeP << " < posCircleZ " << posCircleZ  <<  G4endl;  
#endif

#ifndef BEAMZPOS
	G4double slopeN = openingNeg / (Z_ISOCENTRE-Z_FOCUS);
 	G4double slopeN2 = slopeN*slopeN;
	G4double hvlXN = HVL*(slopeN2+1)/2/cos(atan(fabs(slopeN)));
#else 
	G4double slopeN = (Z_ISOCENTRE-Z_FOCUS) / openingNeg;
	G4double slopeN2 = slopeN*slopeN;
	G4double hvlXN = HVL*sin(atan(1./fabs(slopeN)));
#endif
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << " hvlXN " <<  G4tgrUtils::GetDouble(theWords["HVL"]) << G4endl;
#endif
	G4double posCN = GetPosRound( posCircleZ, slopeN, profR, hvlXN, -1 );
	//     G4cout << " CCN " << CCN << " < posCircleZ2 " << posCircleZ2 << " :: " << slopeN2*posCircleZ2 << " - " << (1.+slopeN2)*(posCircleZ2-profR*profR) << " - " << hvlXN*hvlXN/4.*(1+slopeN2)*(1+slopeN2) << " ::: " << slopeN2 << " * " << posCircleZ2 << " -(1+ " << slopeN2 << " )*( " << posCircleZ2 << " - " <<profR << " * " << profR << " )- " << hvlXN << " * " << hvlXN << " /4.*(1+ " << slopeN2 << " )*(1+ " << slopeN2 << G4endl; //GDEB
	posCN -= G4tgrUtils::GetDouble(theWords["LONGHDIM"])-profR;
	
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << " POS_CIRCLE_CENTRE " << posCN << " slopeN " << slopeN << " < posCircleZ " << posCircleZ  <<  G4endl;  
#endif
	
	fout << ":PLACE " << theWords["NAME"] << "_Leaf_" <<GmGenUtils::itoa(ii) << "_NEG 1 "
	     << theWords["MOTHER_VOLUME"] << " "
	     << R_aux;
	if( theWords["ORIENTATION"]=="X"){
	//t	  fout << " " << -LONGHDIM-(-posCN-profR)
	  fout << " " << posCN
	       << " 0";
	} else if( theWords["ORIENTATION"]=="Y"){
	  fout << " 0"
	       << " " << posCN;
	}
	//	   << GmGenUtils::ftoa(1.0*(-1.0*TIP_RADIUS+(B/Z_ISOCENTRE)*( (Z_TOP+(z_max-Z_TOP)/2.0)  +  TIP_RADIUS*B/( sqrt(pow(Z_ISOCENTRE,2)+pow(B,2)) ) ) + sqrt(  pow(TIP_RADIUS,2) -  pow(B*TIP_RADIUS,2)/( (pow(TIP_RADIUS,2)+pow(B,2)) ) ) ))
	fout << " " << Z_TOP;
	BuildObject(fout );
      
	fout << ":PLACE " << theWords["NAME"] << "_Leaf_"<< GmGenUtils::itoa(ii) << "_POS 1 "
	     << theWords["MOTHER_VOLUME"] << " "
	     << R_aux ;
	if( theWords["ORIENTATION"]=="X"){
	  //	fout << " " << LONGHDIM+openingPos*(Z_CENTRE-Z_FOCUS)/(Z_ISOCENTRE-Z_FOCUS)
	  fout << " " << posCP
	       << " 0";
	} else	if( theWords["ORIENTATION"]=="Y"){
	  fout << " 0 "
	       << " " << posCP;
	}
	//<< GmGenUtils::ftoa(1.0*( TIP_RADIUS+((-1.0)*A/Z_ISOCENTRE)*( (Z_TOP+(z_max-Z_TOP)/2.0) - TIP_RADIUS*(-1.0)*A/( sqrt(pow(Z_ISOCENTRE,2)+pow(A,2)) ) )  -  sqrt(  pow(TIP_RADIUS,2) -  pow(A*TIP_RADIUS,2)/( (pow(TIP_RADIUS,2)+pow(A,2)) ) ) ))
	fout << " " << Z_TOP;
	BuildObject(fout );
	//      G4cout << " Z_TOP " << Z_TOP << " z_max " << z_max << " " <<  ((Z_TOP+z_max)/2.) << " " << GmGenUtils::ftoa((Z_TOP+z_max)/2.) <<G4endl;
	//      B= G4tgrUtils::GetDouble(theWords["OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]"]);
	
	//		system("sleep 2");
	//	G4cout << "DONE" << G4endl;
      
      } else if( theWords["LEAFTIPTYPE"]=="ROUND_DISP_POSFILE"){
	//-	G4double profR = G4tgrUtils::GetDouble(theWords["TIP_RADIUS"]);
	
	G4double z_centre = posCircleZ;
	//	G4cout << " z_centre " << z_centre << " = " << posCircleZ << G4endl; //GDEB
	G4double posCN = -posLeafDist->GetNumericValueFromIndex(-openingNeg);
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) ) G4cout << " GmModuleMLC POS_CIRCLE_CENTRE posCN " << posCN << " from opening " << openingNeg << G4endl;
#endif
	posCN *= z_centre/Z_ISOCENTRE;
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) ) G4cout << " GmModuleMLC POS_CIRCLE_CENTRE posCN corrected by Z " << posCN << " *= " << z_centre << " / " << Z_ISOCENTRE << G4endl;
#endif
	
	G4double posCP = posLeafDist->GetNumericValueFromIndex(openingPos);
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) ) G4cout << " GmModuleMLCPOS_CIRCLE_CENTRE posCP " << posCP << " from opening " << openingPos << G4endl;
#endif
	posCP *= z_centre/Z_ISOCENTRE;
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) ) G4cout << " GmModuleMLC POS_CIRCLE_CENTRE posCP corrected by Z " << posCP << " *= " << z_centre << " / " << Z_ISOCENTRE << G4endl;
#endif
	fout << ":PLACE " << theWords["NAME"] << "_Leaf_" <<GmGenUtils::itoa(ii) << "_NEG 1 "
	     << theWords["MOTHER_VOLUME"] << " "
	     << R_aux;
	// Z_GAP READ IS TAKEN AS LEAF WIDTH
	//	G4cout << " angle  " << ( fabs(-G4tgrUtils::GetDouble(theWords["Z_GAP"])/2.-TIP_CIRCLE_Z)/TIP_RADIUS) << " " << -G4tgrUtils::GetDouble(theWords["Z_GAP"])/2. << " - " << TIP_CIRCLE_Z << " / " << TIP_RADIUS << G4endl; //GDEB
	G4double angle = asin( fabs(-G4tgrUtils::GetDouble(theWords["Z_GAP"])/2.-TIP_CIRCLE_Z)/TIP_RADIUS);
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(testVerb) )  G4cout << " ZHDIM-TIP_CIRCLE_Z ANGLE " << angle << " " << cos(angle) << " : " << LONGHDIM*cos(angle) << G4endl;
#endif
	if( theWords["ORIENTATION"]=="X"){
	  //	posCN -= G4tgrUtils::GetDouble(theWords["XHDIM"]);
	  fout << " " << posCN-LONGHDIM*cos(angle)
	       << " 0";
	} else if( theWords["ORIENTATION"]=="Y"){
	  fout << " 0"
	       << " " << posCN-LONGHDIM*cos(angle);
	}
	//	   << GmGenUtils::ftoa(1.0*(-1.0*TIP_RADIUS+(B/Z_ISOCENTRE)*( (Z_TOP+(z_max-Z_TOP)/2.0)  +  TIP_RADIUS*B/( sqrt(pow(Z_ISOCENTRE,2)+pow(B,2)) ) ) + sqrt(  pow(TIP_RADIUS,2) -  pow(B*TIP_RADIUS,2)/( (pow(TIP_RADIUS,2)+pow(B,2)) ) ) ))
	fout << " " << Z_TOP;
	BuildObject(fout );
      
	fout << ":PLACE " << theWords["NAME"] << "_Leaf_"<< GmGenUtils::itoa(ii) << "_POS 1 "
	     << theWords["MOTHER_VOLUME"] << " "
	     << R_aux << " ";

	if( theWords["ORIENTATION"]=="X"){
	  fout << " " << LONGHDIM*cos(angle)+posCP
	       << " 0";
	} else	if( theWords["ORIENTATION"]=="Y"){
	  fout << " 0 "
	       << " " << LONGHDIM*cos(angle)+posCP;
	}
	fout << " " << Z_TOP;
	BuildObject(fout );
	
	//		system("sleep 2");
	//	G4cout << "DONE" << G4endl;
    
      }

      ///---- STRAIGHT or STRAIGHT_HORIZ
    } else if (theWords["LEAFTIPTYPE"].substr(0,8) =="STRAIGHT"){    

      /*      if (theWords["LEAFTIPTYPE"]=="STRAIGHT"){  
	if(theWords["ORIENTATION"]=="X") {
	  R_aux=R_270y_180z;
	} else {
	  R_aux=R_90x_90z;
	}
      } else if (theWords["LEAFTIPTYPE"]=="STRAIGHT_HORIZ"){
	R_aux= R00;
	}*/
      if(theWords["ORIENTATION"]=="X") {
	R_aux=R_270y_180z;
      } else {
	R_aux=R_90x_90z;
      }
      
      ///--- LEAVES POS
      text_aux=":VOLU "+theWords["NAME"]+"_Leaf_"+GmGenUtils::itoa(ii)+"_POS  EXTRUDED "+theWords[text_aux0]+" "+output_line_text +" 2 "
	+"-1*"+theWords["LONGHDIM"]+" 0  0  1 "
	+theWords["LONGHDIM"]+" 0  0  1 "
	+theWords["MATE"];

      fout << text_aux;
      BuildObject( fout );
      
      ///--- LEAVES NEG
      text_aux=":VOLU "+theWords["NAME"]+"_Leaf_"+GmGenUtils::itoa(ii)+"_NEG  EXTRUDED "+theWords[text_aux0]+" "+output_line_text+" 2 "
	+"-1*"+theWords["LONGHDIM"]+" 0  0  1 "
	+theWords["LONGHDIM"]+" 0  0  1 "
	+theWords["MATE"];
      fout   << text_aux  ;
      BuildObject( fout );
      
      G4double openingPos = G4tgrUtils::GetDouble(theWords["OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]"]);
      G4double openingNeg =G4tgrUtils::GetDouble(theWords["OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]"]);
      
      if( theWords["LEAFTIPTYPE"].substr(0,8) == "STRAIGHT" ) {
	G4double angleN = atan(openingNeg / 
			       (Z_ISOCENTRE - Z_FOCUS));
	//    G4cout << "CHECK ANGLEN " << angleN/CLHEP::deg << " = " << openingNeg / (Z_ISOCENTRE - Z_FOCUS) << " " << openingNeg << "/" << (Z_ISOCENTRE - Z_FOCUS) << G4endl; //GDEB
	G4double angleP = atan(openingPos /
			       (Z_ISOCENTRE - Z_FOCUS));
	
	G4double RADIUS = G4tgrUtils::GetDouble(theWords["RADIUS"]);
	
#ifndef GAMOS_NO_VERBOSE
	if( GeomVerb(debugVerb) ) G4cout << " RADIUS " << RADIUS << " angleN " << angleN << " angleP " << angleP << " " << RADIUS*sin(angleN) << " " << RADIUS*cos(angleN) << " " << RADIUS*sin(angleP) << " " << RADIUS*cos(angleP) <<  G4endl;  
#endif
	G4String rotname = "RMJ_" + theWords["NAME"] ;
	
	fout << ":PLACE " <<  theWords["NAME"]<<"_Leaf_"<<GmGenUtils::itoa(ii)<<"_NEG 1 "
	     <<  theWords["MOTHER_VOLUME"] << " "
	     << R_aux << " ";
	  
	G4double angleI = (180.*CLHEP::deg-angleN)/2.; // angle of isosceles triangle formed by rotation origin, border point at Y=0,Z=RADIUS if no rotation and border point after rotating N
	G4double lenI = 2*RADIUS*sin(angleN/2.);
	G4double borderT;
	G4double borderZ;
	if( theWords["LEAFTIPTYPE"] == "STRAIGHT" ) {
	  borderT = lenI*sin(angleI);
	  borderZ = Z_TOP + lenI*cos(angleI);
	  G4double angleBorder = atan(borderT/borderZ);
#ifndef GAMOS_NO_VERBOSE
	  if( GeomVerb(debugVerb) ) G4cout << " MLC CHECK BORDER " << angleBorder << " = " << angleN << " borderT " << borderT << " borderZ " << borderZ << G4endl; 
#endif
	} else if( theWords["LEAFTIPTYPE"] == "STRAIGHT_HORIZ" ) {
	  borderT = RADIUS*tan(angleN);
	  borderZ = -RADIUS;
	  G4double angleBorder = atan(borderT/borderZ);
#ifndef GAMOS_NO_VERBOSE
	  if( GeomVerb(debugVerb) ) G4cout << " MLC CHECK BORDER " << angleBorder << " = " << angleN << " borderT " << borderT << " borderZ " << borderZ << G4endl;
#endif
	}
	
	if( theWords["ORIENTATION"] == "X" ) {
	  G4double XHDIM = G4tgrUtils::GetDouble(theWords["LONGHDIM"]);
	  
	  G4double centreX = borderT + XHDIM*cos(angleN);
	  G4double centreZ = borderZ + XHDIM*sin(angleN);
	  G4double angleCentre = atan(centreX/centreZ);
	  G4double angle0 = atan(XHDIM/Z_TOP);
#ifndef GAMOS_NO_VERBOSE
	  if( GeomVerb(debugVerb) ) G4cout << " MLC CHECK CENTER " << angleCentre << " = " << angleN-angle0 << " centreX " << centreX << " centreZ " << centreZ << G4endl;
#endif
	  fout << centreX << " " 
	       << "0. "
	       << centreZ;
	  BuildObject( fout );
      
	  //--- Build second MLC

	  fout << ":PLACE " << theWords["NAME"] << "_Leaf_" <<GmGenUtils::itoa(ii) << "_POS 1 "
	       << theWords["MOTHER_VOLUME"] << " "
	       << R_aux << " " 
	       << -centreX << " " 
	       << " 0. " 
	       << centreZ;
	  BuildObject( fout );
	} else if( theWords["ORIENTATION"] == "Y" ) {
	  G4double YHDIM = G4tgrUtils::GetDouble(theWords["LONGHDIM"]);
	  
	  G4double centreY = borderT + YHDIM*cos(angleN);
	  G4double centreZ = borderZ + YHDIM*sin(angleN);
	  G4double angleCentre = atan(centreY/centreZ);
	  G4double angle0 = atan(YHDIM/Z_TOP);
#ifndef GAMOS_NO_VERBOSE
	  if( GeomVerb(debugVerb) ) G4cout << " MLC CHECK CENTER " << angleCentre << " = " << angleN-angle0 << " centreY " << centreY << " centreZ " << centreZ << G4endl;
#endif
	  fout << " 0. " 
	       << centreY << " " 
	       << centreZ;
	  BuildObject( fout );
	  
	  fout << ":PLACE " << theWords["NAME"] << "_Leaf_" <<GmGenUtils::itoa(ii) << "_POS 1 "
	       << theWords["MOTHER_VOLUME"] << " "
	       << R_aux << " " 
	       << " 0. " 
	       << -centreY << " " 
	       << centreZ;
	  
	  BuildObject( fout );
	  
	} else {
	  G4Exception("GmModuleLeaf::BuildObjects",
		      "Wrong orientation type of MLC",
		      FatalErrorInArgument,
		      G4String("It is: "+theWords["ORIENTATION"]+" , while it should be X or Y").c_str());
	}
	
      }

    }
    
  };

  return;

}

//------------------------------------------------------------------------
GmModuleMLC::~GmModuleMLC()
{

}


/*
G4String num2string(std::int i){
      G4String aux;
      G4String aux2=".";
      std::char caux[80]="";
       if (i>999){
           gcvt(i,10,caux);
        } else {
         gcvt(i,3,caux);
        };
       aux=caux;
            std::int longitud=aux.length();
            aux=aux.substr(longitud,1);
       std::int longitud=aux.size();
       if (aux[longitud-1]==aux2[0]){
                              aux=aux.substr(0,longitud-1);};
       return aux;
};
*/

