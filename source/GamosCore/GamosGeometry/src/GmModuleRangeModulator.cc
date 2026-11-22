#include "GmModuleRangeModulator.hh"
#include "G4tgrUtils.hh"
#include "G4tgrVolumeMgr.hh"
#include "G4UIcommand.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GmGeomVerbosity.hh"
#include "math.h"

//------------------------------------------------------------------------
GmModuleRangeModulator::GmModuleRangeModulator( const std::vector<G4String>& par ) : GmVModule(par)
{
//  G4tgrUtils::CheckWLsize( par, 9, WLSIZE_EQ,
//                           "GmModuleRangeModulator::GmModuleRangeModulator");

}

//------------------------------------------------------------------------
void GmModuleRangeModulator::BuildObjects()
{

// *************  LEERMOS LAS VARIABLAES  ****************** ///

  G4cout << " *************** START MODULE RANGE GEOMETRY DEFINITION **************** " <<  G4endl <<  G4endl   ;
  
  G4int ip = 0;
  theWords["NAME"] = theParams[ip++];
  theWords["RMIN"] = theParams[ip++];
  theWords["RIN"] = theParams[ip++];
  theWords["ROUT"] = theParams[ip++];
  theWords["NBLADE"] = theParams[ip++];
  theWords["STEPS"] = theParams[ip++];
  
  G4double rmin = G4tgrUtils::GetDouble(theWords["RMIN"]);
  G4double rin = G4tgrUtils::GetDouble(theWords["RIN"]);
  G4double rout = G4tgrUtils::GetDouble(theWords["ROUT"]);
  G4double nblade = G4tgrUtils::GetDouble(theWords["NBLADE"]);
  G4double steps = G4tgrUtils::GetDouble(theWords["STEPS"]);
  
  std::vector<G4double> thickness;
  std::vector<G4double> weight;
  std::vector<G4double> sweight;
  std::vector<G4double> diff_thickness;
  G4double aux=0;
  
  diff_thickness.clear();
  sweight.clear();
  
  G4double Zsize=0;
  
  G4cout << " rmin =    " << rmin    << G4endl  ;  
  G4cout << " rin =     " << rin     << G4endl  ;  
  G4cout << " rout =    " << rout    << G4endl  ;  
  G4cout << " nblade =  " << nblade  << G4endl  ;  
  G4cout << " steps =   " << steps   << G4endl  ;  
  
  
  for(int i=0;i<steps;i++){
    
    theWords["AUX"]=theParams[ip++];
    aux=G4tgrUtils::GetDouble(theWords["AUX"]);
    thickness.push_back(aux);
    if(aux>Zsize){Zsize=aux;};
    
    theWords["AUX"]=theParams[ip++];
    aux=G4tgrUtils::GetDouble(theWords["AUX"]);
    weight.push_back(aux);
    
    G4cout << "i=" << i << G4endl;	
    if(i==0){
      diff_thickness.push_back(thickness[0]);
      G4cout << " thickness [i] = " << thickness[i] << G4endl;
    } else {
      
      G4cout << " thickness [i] = " << thickness[i] << G4endl;
      aux=thickness[i]-thickness[i-1];
      if(aux<0){aux=-1*aux;}; 
      G4cout << " aux  = " << aux << G4endl;
      diff_thickness.push_back(aux);
    };
    
  };
  
  
  // *********** plots sweigth and weight  ************ ////
  
  for(int i=0;i<steps;i++){
    aux=0;
    for(int j=i;j<steps;j++){
      G4cout << "  weight[" << j << "]="<< weight[j] << G4endl;
      
      aux=aux+weight[j];
    };
    G4cout << " sweight[" << i << "]="<< aux       << G4endl;
    G4cout << "  weight[" << i << "]="<< weight[i] << G4endl;
    sweight.push_back(aux);
  };
  
  G4cout <<  G4endl <<  " ****  VARIABLE READS *** "<< G4endl << G4endl  ;
  
  
  // *********** FIN DE LECTURA DE LAS VARIABLES ************ ////

  
  std::ostringstream fout;
  
  G4int solid_counter = 0;	
  
  // *********** CENTRAL SOLID TUBE **************//
  
  fout << ":ROTM " << theWords["NAME"] << "_R00 0 0 0 "; 
  BuildObject( fout ); 
  
  fout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter
       << " TUBE " << " "
       << rmin     << " "
       << rin      << " "
       << Zsize/2    ;
  BuildObject( fout );
  
  G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter
       << " TUBE " << " "
       << rmin     << " "
       << rin      << " "
       << Zsize/2  << G4endl  ;


  //G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " CONTROL LL " << G4endl;
  
  // ********************************************* ///
 
  solid_counter++;
  // *********** Solid wich describe the nBlades ******** //
  
  for (int b=0; b<nblade; b++){
    for (int s=0;s<steps;s++){
      
      //for (int b=0; b<1; b++){
      //     for (int s=0;s<2;s++){
      G4cout << "  "  << G4endl;
      G4cout << "  "  << G4endl;
      G4cout << "  "  << G4endl;
      G4cout << "  "  << G4endl;
      
      G4cout << " ***************** START CASE **************************** "  << G4endl;
      G4cout << " s = " << s  << G4endl;
      G4cout << " diff_thickness[s] = " << diff_thickness[s] << G4endl;
      G4cout << " thickness[s] = " << thickness[s] << G4endl;
      G4cout << " Start A = " <<    (360/nblade)*(1+b)+(360*sweight[s]/nblade)/2   
	     << " Delta A = " <<    abs(360*sweight[s])/nblade                    << G4endl;
      G4cout << " sweight[s+1] = " << sweight[s+1] << G4endl;
      G4cout << " nblade     = " << nblade     << G4endl;
      G4cout << " ********************************************************" <<  G4endl << G4endl << G4endl << G4endl << G4endl ;
      
      fout    << ":SOLID " << theWords["NAME"] << "_S" << solid_counter
	      << " TUBS "
	      << rin   << " "
	      << rout  << " "
	      <<  diff_thickness[s]/2 << " "
	      << (360/nblade)*(1+b)+(360*(sweight[1]-sweight[s])/nblade)/2  << " "
	      << 360*sweight[s]/nblade  ;
      
      G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter
	     << " TUBS "
	     << rin   << " "
	     << rout  << " "
	     <<  diff_thickness[s]/2 << " "
	     << (360/nblade)*(1+b)+(360*(sweight[1]-sweight[s])/nblade)/2  << " "
	     << 360*sweight[s]/nblade << G4endl; ; 
      
      
      BuildObject( fout );
      //G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " CONTROL LL " << G4endl;
      solid_counter++;
      
      fout    << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " UNION "
	      << theWords["NAME"] << "_S" << solid_counter-2 << " "
	      << theWords["NAME"] << "_S" << solid_counter-1   << " "
	      << theWords["NAME"] << "_R00 0 0 "
	      << Zsize/2 ;
      for( int ss=0;ss<s;ss++){
	fout  << "-"<<diff_thickness[ss];
      };	
      fout    <<"-"<<diff_thickness[s]<<"/2.0";
      
      G4cout    << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " UNION "
		<< theWords["NAME"] << "_S" << solid_counter-2 << " "
		<< theWords["NAME"] << "_S" << solid_counter-1   << " "
		<< theWords["NAME"] << "_R00 0 0 "
		<< Zsize/2 ;
      for( int ss=0;ss<s;ss++){
	G4cout  << "-"<<diff_thickness[ss];
      };
      G4cout    <<"-"<<diff_thickness[s]<<"/2.0"  << G4endl ;
      
      
      BuildObject( fout );
      //G4cout << ":SOLID " << theWords["NAME"] << "_S" << solid_counter << " CONTROL LL " << G4endl;
      solid_counter++;
      
    }
  }
  
  fout    << ":SOLID " << theWords["NAME"] << " UNION "
	  << theWords["NAME"] << "_S" << solid_counter-1   << " "
	  << theWords["NAME"] << "_S" << solid_counter-1   << " "
	  << theWords["NAME"] << "_R00 "
	  << " 0 0 0 ";
  
  G4cout    << ":SOLID " << theWords["NAME"] << " UNION "
	    << theWords["NAME"] << "_S" << solid_counter-1   << " "
	    << theWords["NAME"] << "_S" << solid_counter-1   << " "
	    << theWords["NAME"] << "_R00 "
	    << " 0 0 0 " << G4endl ;
  
  G4cout << " ************* MODULATE RANGE GEOMETRY COMPLETED **************** "  << G4endl;
  G4cout << "  "  << G4endl;
  G4cout << "  "  << G4endl;
  BuildObject( fout );
  
}


//------------------------------------------------------------------------
GmModuleRangeModulator::~GmModuleRangeModulator()
{


}
