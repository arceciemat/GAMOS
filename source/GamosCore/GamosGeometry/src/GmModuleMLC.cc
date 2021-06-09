#include "GmModuleMLC.hh"
#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <math.h>


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
  G4cout << "Starting to process MLC" <<  G4endl;

//---- input line 1
  theWords["WORLD"]= theParams[ip++];
  theWords["NAME"] = theParams[ip++];
  
//--- input line 2
  theWords["TYPE"]         = theParams[ip++];          // FOCUSED 
  theWords["ORIENTATION"]  = theParams[ip++];   // X or Y leave move orientation 
  theWords["ENDLEAVETYPE"] = theParams[ip++];  // ROUND or STRAIGHT end leave
  
//--- input line 3
  theWords["Z_FOCUS"]      = theParams[ip++];  G4double Z_FOCUS    = G4tgrUtils::GetDouble(theWords["Z_FOCUS"]);
  theWords["Z_TOP"]        = theParams[ip++];  G4double Z_TOP      = G4tgrUtils::GetDouble(theWords["Z_TOP"]);
  theWords["Z_ISOCENTER"]  = theParams[ip++];  G4double Z_ISOCENTER= G4tgrUtils::GetDouble(theWords["Z_ISOCENTER"]);

//--- input line 4
  theWords["LONGDIM"]      = theParams[ip++];  G4double LONGDIM    = G4tgrUtils::GetDouble(theWords["LONGDIM"]);
  theWords["RDIM"]         = theParams[ip++];  G4double RDIM       = G4tgrUtils::GetDouble(theWords["RDIM"]);
  		//if( theWords["ENDLEAFETYPE"] == "ROUND" ){
  		//	theWords["RDIM"] = theParams[ip++];
  		//};

//--- input line 5
  theWords["GAP"]          = theParams[ip++];  G4double GAP        = G4tgrUtils::GetDouble(theWords["GAP"]);
  theWords["Z_GAP"]        = theParams[ip++];  G4double Z_GAP      = G4tgrUtils::GetDouble(theWords["Z_GAP"]);

   G4cout << "The GAP is " << GAP <<  G4endl;  

//--- input line 6
  theWords["CrossLeavesFocus"]       = theParams[ip++]; G4double CrossLeavesFocus        = G4tgrUtils::GetDouble(theWords["CrossLeavesFocus"]);

//--- input line 7
  theWords["CrossLeavesStartPoint"] = theParams[ip++];  G4double CrossLeavesStartPoint   = G4tgrUtils::GetDouble(theWords["CrossLeavesStartPoint"]);
 // theWords["CrossBlockEndWidth"]    = theParams[ip++];  G4double CrossBlockEndWidth      = G4tgrUtils::GetDouble(theWords["CrossBlockEndWidth"]);

	G4String aux_text;
	G4String aux_text_c;
	G4String aux_text_z; 

//--- input line 8 
  theWords["N_LEAVES_CROSS_PROFILES"] = theParams[ip++]; G4double N_LEAVES_CROSS_PROFILES= G4tgrUtils::GetDouble(theWords["N_LEAVES_CROSS_PROFILES"]);
	
	for(int i=1;i<int(1+N_LEAVES_CROSS_PROFILES);i++){
                
///--- input line 9 
		theWords["NLeavesCrossPoints["+GmGenUtils::itoa(i)+"]"] = theParams[ip++];
		theWords["LeaveMode["+GmGenUtils::itoa(i)+"]"] = theParams[ip++];
		for(int j=1;j<int(1+G4tgrUtils::GetDouble(theWords["NLeavesCrossPoints["+GmGenUtils::itoa(i)+"]"]));j++){	
			G4cout << " ***** READING PARAMETER ********* " ;
///--- input line 10 and nexts		 	
			theWords["z["+GmGenUtils::itoa(i)+","+GmGenUtils::itoa(j)+"]"] = theParams[ip++];
		 	theWords["c["+GmGenUtils::itoa(i)+","+GmGenUtils::itoa(j)+"]"] = theParams[ip++];
			
			G4cout << "z["+GmGenUtils::itoa(i)+","+GmGenUtils::itoa(j)+"]="  <<  theWords["z["+GmGenUtils::itoa(i)+","+GmGenUtils::itoa(j)+"]"] << " " << G4endl;
			G4cout << "c["+GmGenUtils::itoa(i)+","+GmGenUtils::itoa(j)+"]="  <<  theWords["c["+GmGenUtils::itoa(i)+","+GmGenUtils::itoa(j)+"]"] << " " << G4endl;
   		};
	};

///--- input lines to Leaves numbers 
   theWords["N_LEAVES"] = theParams[ip++]; G4double N_LEAVES = G4tgrUtils::GetDouble(theWords["N_LEAVES"]);

   for(int i=1;i<int(1+N_LEAVES);i++){

///--- input lines to leave type and open position 
	 theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]          = theParams[ip++];		
	 theWords["OPEN_POSITON_LEAF["+GmGenUtils::itoa(i)+"]"]   = theParams[ip++]; 
	 theWords["OPEN_POSITON_RIGHT["+GmGenUtils::itoa(i)+"]"]  = theParams[ip++];
   };

  theWords["MATE"] = theParams[ip++];

  G4cout << "ALL MATERIAL READ OK" << G4endl << "*********____________*********" <<  G4endl;
  G4cout <<  G4endl << G4endl<< G4endl<< G4endl;

  std::ostringstream fout;


	
	G4double cg_min=0;
	G4double cg_max=0;
	G4double cg_min_b=0;
	G4double cg_max_b=0;	
	G4double v[2]={0,0};

	
	G4double c_min=0;
	G4double c_max=0;
	G4double z_max=0;	 

	G4int k=0;
	G4float c=0;
	G4float c_pi=0;
	G4float z=0;
	G4float z_pi=0;
	G4float Z_DEF=Z_GAP+Z_TOP;
	G4float FASE=CrossLeavesStartPoint;
	G4float A=0;
	G4float B=0;

	std::string  text_aux="";
	std::string  output_line_text="";
	
	std::string R_90x = theWords["NAME"]+"_R_90x";
	std::string R_90y = theWords["NAME"]+"_R_90y";
	std::string R00   = theWords["NAME"]+"_R00";
	std::string R_270x_90z = theWords["NAME"]+"_R_270x_90z";
	std::string R_90y_180z = theWords["NAME"]+"_R_90y_180z";
	std::string R_aux;

   fout  << ":ROTM  "<<R_90x<<" 90  0 0 ";  BuildObject( fout );
   fout  << ":ROTM  "<<R_90y<<"  0 90 0 ";  BuildObject( fout );
   fout  << ":ROTM  "<<R00  <<"  0  0 0 ";  BuildObject( fout );
   fout  << ":ROTM  "<<R_270x_90z  <<"  0 0 -1 1  0 0 0 -1 0 ";  BuildObject( fout );
   fout  << ":ROTM  "<<R_90y_180z  <<"  0 0 -1 0 -1 0 -1 0 0 ";  BuildObject( fout );
	
   //	G4int counts=0;

  for(int i=1;i<int(1+N_LEAVES);i++){
 
	G4cout  << G4endl << " *********************************************************************************************" << G4endl;
	G4cout 	<< G4endl << "Start to process the leave " << i << " of " << theWords["N_LEAVES"]  << " Leaves ************ " << G4endl;	
	G4cout  << "   The Leave Cross Points is " << theWords["NLeavesCrossPoints["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+"]"] << G4endl;
	G4cout << "    LEAVE TYPE is " << theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]  <<   G4endl ;	
  	G4cout << "    It's going to use LEAVE_TYPE["+GmGenUtils::itoa(i)+"] " <<  G4endl <<  G4endl ;

	G4cout  << G4endl << " *********************************************************************************************" << G4endl << G4endl ;
  	
	aux_text="NLeavesCrossPoints["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+"]";
	output_line_text.clear();
	G4cout << "output_line_text == \"" << output_line_text  << "\""<<  G4endl <<  G4endl;
			
			//---- FIND Z_GAP intersection points 
			for(int j=1;j<int(-1+1+G4tgrUtils::GetDouble(theWords[aux_text]));j++){ 
				c    = G4tgrUtils::GetDouble(theWords["c["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+","+GmGenUtils::itoa(j)+"]"]);
				c_pi = G4tgrUtils::GetDouble(theWords["c["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+","+GmGenUtils::itoa(j+1)+"]"]);
				z    = G4tgrUtils::GetDouble(theWords["z["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+","+GmGenUtils::itoa(j)+"]"]);
				z_pi = G4tgrUtils::GetDouble(theWords["z["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+","+GmGenUtils::itoa(j+1)+"]"]);
				z=z+Z_TOP;
				z_pi=z_pi+Z_TOP;
								
				if(  ((Z_DEF >= z) & (Z_DEF < z_pi ))  |  ((Z_DEF <= z) & (Z_DEF > z_pi )) ){ 

					v[k]=c+((c_pi-c)/(z_pi-z))*(Z_DEF-z);
					G4cout << " v[k] = " << k << "  " << v[k] << G4endl;
					k++;
		
				}
			};
			if (v[0]>v[1]){	cg_max=v[0]; cg_min=v[1];} else { cg_max=v[1]; cg_min=v[0]; }; k=0;
			if(i==1){
				//FASE=FASE+cg_min-
			};

			FASE=FASE+GAP+cg_max_b-cg_min_b;

			G4cout << G4endl << " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  << G4endl;  
			G4cout <<           " +++++  STARTING TO CALCULATE DE FINAL C POSITION +++++++++++++++ " << G4endl << G4endl; 
			
			for(int j=1;j<int(1+G4tgrUtils::GetDouble(theWords[aux_text]));j++){ 
				G4cout << G4endl << " ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"  << G4endl;  
				G4cout <<           " +++++  LOOP " << j << " coordinate               +++++++++++++++ " << G4endl << G4endl; 
				// Start operations
		  		c    = G4tgrUtils::GetDouble(theWords["c["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+","+GmGenUtils::itoa(j)+"]"]);
				z    = G4tgrUtils::GetDouble(theWords["z["+theWords["LEAVE_TYPE["+GmGenUtils::itoa(i)+"]"]+","+GmGenUtils::itoa(j)+"]"]);
				z=z+Z_TOP;
				
				// Displacemente operations acounts (FASE)
				
				c=c+FASE-cg_min;
				
				
				G4cout << "  --> FOCUSED FUNCTION " << G4endl;

				//G4cout << "output_line_text == " << output_line_text  << G4endl;

				c=(c-CrossLeavesFocus)*(z-Z_FOCUS)/(Z_TOP-Z_FOCUS)+CrossLeavesFocus;
				if( theWords["ENDLEAVETYPE"]=="ROUND"){
					output_line_text+=" "+GmGenUtils::ftoa(z)+" "+GmGenUtils::ftoa(c)+" ";
				} else if (theWords["ENDLEAVETYPE"]=="STRAIGHT"){
					output_line_text+=" "+GmGenUtils::ftoa(z)+" "+GmGenUtils::ftoa(c)+" ";		
				};

				
				G4cout << "output_line_text == " << output_line_text  << G4endl;
  				//fout 	<<  GmGenUtils::itoa(z) << " "
       			 	//	<<  GmGenUtils::itoa(c) << " ";
				if(j==1){c_min=c;c_max=c;z_max=z;};
				if(c_min>c){c_min=c;};
				if(c_max<c){c_max=c;};
				if(z_max<z){z_max=z;};
			};
	
	cg_max_b=cg_max;
	cg_min_b=cg_min;
	
	G4cout << G4endl << " ++++++********* COMPLITING THE LEAVE PROFILE *******++++++ "  << G4endl << G4endl;

        
///---- ROUNDER BORDER

	G4cout  << " PLACING " << i << " LEAVE " << G4endl;		


	if( theWords["ENDLEAVETYPE"]=="ROUND"){	

		text_aux=":SOLID "+theWords["NAME"]+"_LeaveNumber_"+GmGenUtils::itoa(i)+"_BASE_A  EXTRUDED "+theWords[aux_text]+" "+output_line_text +" 2 "
		+"0  0  0  1 "
		+theWords["LONGDIM"]+"/2 0  0  1 ";
		fout << text_aux;
		BuildObject( fout );
		
		G4cout << " EXTRUDED B LEAVE " << G4endl;
		text_aux=":SOLID "+theWords["NAME"]+"_LeaveNumber_"+GmGenUtils::itoa(i)+"_BASE_B  EXTRUDED "+theWords[aux_text]+" "+output_line_text+" 2 "
		+"-1*("+theWords["LONGDIM"]+")/2 0  0  1 "
		+" 0  0  0  1 ";
		G4cout << text_aux << G4endl;
		fout   << text_aux  ;
		BuildObject( fout );
		G4cout << " EXTRUDED B LEAVE END" << G4endl;



///---- BASE VOLU ----------------------------------------------------------------------------------------------------------
        	fout <<":VOLU "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_BASE_A "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_BASE_A "<<theWords["MATE"];BuildObject( fout );

 		//fout  	<< ":PLACE " <<  theWords["NAME"]+"_LeaveNumber_"+GmGenUtils::itoa(i)+"_BASE "<< " 1 " <<  theWords["WORLD"] <<  " " << R00 << " 0 0 0 "; BuildObject( fout );

///---  SOLIDS ----------------------------------------------------------------------------------------------------------
		G4cout  << G4endl << " Leave " << i << " Procesed  ++++++++++++++++++++ " << G4endl << G4endl;

		fout << ":SOLID "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_TUBE  TUBE 0 "<<theWords["RDIM"]<<"  "<<GmGenUtils::ftoa((c_max-c_min)/2+20); 
		BuildObject( fout );
	
		G4cout  << G4endl << " SOLID_TUBE " << i << " Procesed  ++++++++++++++++++++ " << G4endl << G4endl;
	

		fout << ":SOLID "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_BOX   BOX    "<<" "<<theWords["RDIM"]<<" "<<GmGenUtils::ftoa(LONGDIM/2-RDIM/2)<<" "<<GmGenUtils::ftoa((c_max-c_min)/2+10);BuildObject( fout );  
		G4cout  << G4endl << " SOLID_BOX " << i << " Procesed  ++++++++++++++++++++ " << G4endl << G4endl;

	
		fout << ":SOLID "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_COVER_A UNION "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_BOX "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_TUBE "<< R00 <<" 0 "<<GmGenUtils::ftoa((LONGDIM/2-RDIM/2))<<" 0";   BuildObject( fout );  

		fout << ":SOLID "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_COVER_B UNION "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_BOX "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_TUBE "<< R00 <<" 0 -1*("<<GmGenUtils::ftoa((LONGDIM/2-RDIM/2))<<") 0";   BuildObject( fout );  


		G4cout  << G4endl << " SOLID_COVER " << i << " Procesed  ++++++++++++++++++++ " << G4endl << G4endl;

///---- COVER_VOLU----------------------------------------------------------------------------------------------------------
		//fout << ":VOLU "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_COVER_UNION "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_COVER   G4_W"  ;BuildObject( fout );  

		//fout <<":PLACE "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_COVER_UNION "<< " 1 " <<  theWords["WORLD"]<<  " " << R_90x <<" "<<GmGenUtils::ftoa(Z_TOP+(z_max-Z_TOP)/2.0)<<" "<< (c_max+c_min)/2 <<" "<<GmGenUtils::ftoa(LONGDIM/2+RDIM/2) ;BuildObject( fout ); 



///---- FINAL UNION----------------------------------------------------------------------------------------------------------

		fout << ":SOLID "  << theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_A INTERSECTION  " 
		 	<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_BASE_A " 
		 	<<" "
		 	<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_COVER_A "
			//<< R00 << " 0 0 0 "; BuildObject( fout ); 
		 	<< R_90x <<" "<<GmGenUtils::ftoa(Z_TOP+(z_max-Z_TOP)/2.0)<<" "<< (c_max+c_min)/2 <<" "<<GmGenUtils::ftoa(LONGDIM/2+RDIM/2)<<" " ;BuildObject( fout ); 


		fout << ":SOLID "  << theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_B INTERSECTION  " 
		 	<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_BASE_B " 
		 	<<" "
		 	<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_COVER_B "
			//<< R00 << " 0 0 0 "; BuildObject( fout ); 
		 	<< R_90x <<" "<<GmGenUtils::ftoa(Z_TOP+(z_max-Z_TOP)/2.0)<<" "<< (c_max+c_min)/2 <<" "<<GmGenUtils::ftoa(-1*(LONGDIM/2+RDIM/2))<<" " ;BuildObject( fout );



///---- FINAL VOLUMEN DEFINITION

		fout << ":VOLU  "  << theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"__A "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_A "<<theWords["MATE"];
		BuildObject( fout );

		fout << ":VOLU  "  << theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"__B "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_B "<<theWords["MATE"];
		BuildObject( fout );

	
		G4cout << "Making the final volume" << G4endl;
		if(theWords["ORIENTATION"]=="x" || theWords["ORIENTATION"]=="X"){R_aux=R_90y;} else { R_aux=R_270x_90z;};
			 
			 A= G4tgrUtils::GetDouble(theWords["OPEN_POSITON_LEAF["+GmGenUtils::itoa(i)+"]"]);

			 fout << ":PLACE " <<  theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"__A  1 " <<  theWords["WORLD"]<<" "
			      << R_aux << " " << GmGenUtils::ftoa(1.0*(
				 RDIM+((-1.0)*A/Z_ISOCENTER)*( 
							      (Z_TOP+(z_max-Z_TOP)/2.0)  -  RDIM*(-1.0)*A/( 
													    sqrt(pow(Z_ISOCENTER,2)+pow(A,2))
							                                                   )
							      )  -  sqrt(  pow(RDIM,2) -  pow(A*RDIM,2)/(
												         (pow(RDIM,2)+pow(A,2))
												         )
							                )
				       )) <<" 0 0 "; BuildObject(fout );

			    //<< R_aux  << " ("<<theWords["OPEN_POSITON_LEAF["+GmGenUtils::itoa(i)+"]"]<< ")*"<<GmGenUtils::ftoa(Z_TOP/Z_ISOCENTER) <<" 0 0 "; BuildObject(fout );
			 B= G4tgrUtils::GetDouble(theWords["OPEN_POSITON_RIGHT["+GmGenUtils::itoa(i)+"]"]);

			 fout << ":PLACE " <<  theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"__B  2 " <<  theWords["WORLD"]<<" "
			      <<  R_aux << " "   <<GmGenUtils::ftoa(1.0*(
			           -1.0*RDIM+(B/Z_ISOCENTER)*( 
							      (Z_TOP+(z_max-Z_TOP)/2.0)  +  RDIM*B/( 
													    sqrt(pow(Z_ISOCENTER,2)+pow(B,2))
							                                                   )
							      )  +  sqrt(  pow(RDIM,2) -  pow(B*RDIM,2)/(
												         (pow(RDIM,2)+pow(B,2))
												         )
							                )
				       ))  <<" 0 0 "; BuildObject(fout );

			    //<< R_aux  << " ("<<theWords["OPEN_POSITON_RIGHT["+GmGenUtils::itoa(i)+"]"]<< ")*"<<GmGenUtils::ftoa(Z_TOP/Z_ISOCENTER)<<" 0 0 "; BuildObject(fout );

		system("sleep 2");
		G4cout << "DONE" << G4endl;

	
///---- ROUNDER STRAIGHT
	} else if (theWords["ENDLEAVETYPE"]=="STRAIGHT"){
		

		if(theWords["ORIENTATION"]=="x" || theWords["ORIENTATION"]=="X"){R_aux=R_90x;} else { R_aux=R_270x_90z;};


	///--- LEAVES A
		fout<<":VOLU "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_A  POLYCONE "
			      <<" 270"<<"+("<<theWords["OPEN_POSITON_RIGHT["+GmGenUtils::itoa(i)+"]"]<< ")*"<<GmGenUtils::ftoa(Z_TOP/Z_ISOCENTER)  <<" "     <<GmGenUtils::ftoa((180/3.1416)*LONGDIM/(Z_TOP-Z_FOCUS))<<" "<<theWords[aux_text]<<" "
			      <<output_line_text<<" "<<theWords["MATE"]; 
		BuildObject( fout );
		fout << ":PLACE " <<  theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_A  1 " <<  theWords["WORLD"]<<" "<<R_aux 
		     << " 0 0 0 "; BuildObject(fout );

	

	///--- LEAVES B
		fout<<":VOLU "<<theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_B  POLYCONE "
			      <<" 270"<<"+("<<theWords["OPEN_POSITON_LEAF["+GmGenUtils::itoa(i)+"]"]<< ")*"<<GmGenUtils::ftoa(Z_TOP/Z_ISOCENTER)<<"-"<<GmGenUtils::ftoa(1*(180/3.1416)*LONGDIM/(Z_TOP-Z_FOCUS))  <<" "      <<GmGenUtils::ftoa(1*(180/3.1416)*LONGDIM/(Z_TOP-Z_FOCUS))<<" "<<theWords[aux_text]<<" "
			      <<output_line_text<<" "<<theWords["MATE"]; 
		BuildObject( fout );
		fout << ":PLACE " <<  theWords["NAME"]<<"_LeaveNumber_"<<GmGenUtils::itoa(i)<<"_B 1 " <<  theWords["WORLD"]<<" "<<R_aux 
		     << " 0 0 0 "; BuildObject(fout );


	}

	
  };




 


return;


/*
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
    G4tgrUtils::GetDouble(theWords["Z_FOCUS"]) + 
    G4tgrUtils::GetDouble(theWords["ZDIM"]);

#ifndef GAMOS_NO_VERBOSE
  if( GeomVerb(debugVerb) ) G4cout << " RADIUS " << radius << " angle_1 " << angle_1 << " angle_2 " << angle_2 << " " << radius*sin(angle_1) << " " << radius*cos(angle_1) << " " << radius*sin(angle_2) << " " << radius*cos(angle_2) <<  G4endl;  
#endif
  G4String world = G4tgrVolumeMgr::GetInstance()->GetTopVolume()->GetName();
  G4String rotname = "RM_" + theWords["NAME"] ;
  
  if( theWords["TYPE"] == "X" ) {
    G4double XDIM = G4tgrUtils::GetDouble(theWords["XDIM"]);

    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << " 0. " << -angle_1/deg << " 0. ";
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << world << " " 
 	 << rotname << "_1 "
	 << radius*sin(angle_1) + XDIM*cos(angle_1)
	 << " 0. " 
	 << radius*cos(angle_1) - XDIM*sin(angle_1); 
    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << " 0. " << -angle_2/deg << " 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << world << " " 
	 << rotname << "_2 "
	 << radius*sin(angle_2) - XDIM*cos(angle_2)
	 << " 0. " 
	 << radius*cos(angle_2) + XDIM*sin(angle_2);
    BuildObject( fout );
  } else 
  if( theWords["TYPE"] == "Y" ) {
    G4double YDIM = G4tgrUtils::GetDouble(theWords["YDIM"]);

    //--- Build first jaw    
    fout << ":ROTM " << rotname << "_1 " 
	 << angle_1/deg << " 0. 0. " ;
    BuildObject( fout );
    
    fout << ":PLACE " << theWords["NAME"] << " 1 " 
	 << world << " " 
	 << rotname << "_1 "
	 << " 0. " 
	 << radius*sin(angle_1) + YDIM*cos(angle_1) << " " 
	 << radius*cos(angle_1) - YDIM*sin(angle_1); 
    BuildObject( fout );

    //--- Build second jaw    
    fout << ":ROTM " << rotname << "_2 " 
	 << angle_2/deg << " 0. 0. " ;
    BuildObject( fout );

    fout << ":PLACE " << theWords["NAME"] << " 2 " 
	 << world << " " 
	 << rotname << "_2 "
	 << " 0. " 
	 << radius*sin(angle_2) - YDIM*cos(angle_2) << " " 
	 << radius*cos(angle_2) + YDIM*sin(angle_2); 
    BuildObject( fout );

  } else {
    G4Exception("GmModuleMLC::BuildObjects",
		"Wrong type of JAWS",
		FatalErrorInArgument,
		G4String("It is: "+theWords["TYPE"]+" , while it should be X or Y").c_str());
  }

*/


}

//------------------------------------------------------------------------
GmModuleMLC::~GmModuleMLC()
{


}


/*
std::string num2string(std::int i){
      std::string aux;
      std::string aux2=".";
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
