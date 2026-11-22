/*==================================================================
                                                                 
      PLOTPHSP UTILITIES LIBRARY  
 
      Juan Diego Soler Pulido
      email: juandiego.soler@ciemat.es
      Madrid, Spain
      December 2006

 =================================================================*/
 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include "utilities.h"
using namespace std;


// OPEN FILE OPEN FILE OPEN FILE OPEN FILE OPEN FILE OPEN FILE OPEN FILE 

// COMPARE COMPARE COMPARE COMPARE COMPARE COMPARE COMPARE COMPARE COMPARE


float compare(float value, float liminf, float limsup){

  if(value >= liminf && value <= limsup)
    return 1.00;
  else
    return 0.00;

}


//EVALUATE PLOT MODE EVALUATE PLOT MODE EVALUATE PLOT MODE EVALUATE PLOT MODE 


float evaluate_event(int mode, float wtin, float ein, float minweight, float maxweight, float minenergy, float maxenergy){

  float load;
  //particle plot
  if(mode==0){
    load=1.00*compare(wtin,minweight,maxweight)*compare(ein,minenergy,maxenergy);
    return load;
  }
  //weighted particle plot
  else if(mode==1){
    load=1.00*wtin*compare(wtin,minweight,maxweight)*compare(ein,minenergy,maxenergy);
    return load;
  }
  //energy plot
  else if(mode==2){
    load=ein*compare(wtin,minweight,maxweight)*compare(ein,minenergy,maxenergy);
    return load;
  }
  //energy plot
  else if(mode==3){
    load=ein*wtin*compare(wtin,minweight,maxweight)*compare(ein,minenergy,maxenergy);
    return load;
  }
  else{
    cout << "Error in matrix load" << endl;
    return 0;
  }

}

// PRINT HELP PRINT HELP PRINT HELP PRINT HELP PRINT HELP PRINT HELP PRINT HELP


void print_help()
{
    
    cout << "" << endl;
    cout << "**********************************************************************" << endl;
    cout << "*                    EGS PHASE SPACE PLOTTER HELP                    *" << endl;
    cout << "**********************************************************************" << endl;
    cout << "*   Juan Diego Soler Pulido                                          *" << endl;
    cout << "*   juandiego.soler@ciemat.es                                        *" << endl;
    cout << "*   December 2006                                                    *" << endl;
    cout << "**********************************************************************" << endl;
    cout << "" << endl;

 
    cout << "Command line sintax" << endl;
    cout << "===================" << endl;
    cout << "" << endl;
    cout << "plotegsphspvp [option]" << endl;
    cout << "" << endl;
    

    cout << "Input options" << endl;
    cout << "=============" << endl;
    cout << "" << endl;

    cout << "-file" << endl;
    cout << "          -file [filename]" << endl;

    cout << "-n" << endl;
    cout << "          -n [number of events to be considered]" << endl;
    cout << "" << endl;
    cout << "          default values: all events in phsp" << endl;

    cout << "" << endl;
    cout << "Output options" << endl;
    cout << "==============" << endl;
    cout << "" << endl;

    cout << "-x" << endl;
    cout << "          -x [x minimum value] [x maximum value] [x grid spacing]" << endl;
    cout << "" << endl;
    cout << "          default values  -10.00 10.00 1" << endl;

    cout << "-y" << endl;
    cout << "          -y [y minimum value] [y maximum value] [y grid spacing]" << endl;
    cout << "" << endl;
    cout << "          default values  -10.00 10.00 1" << endl;

    cout << "-plot" << endl;
    cout << "          -plot [plot mode]" << endl;
    cout << "" << endl;
    cout << "          -plot 0   Number of particles" << endl;
    cout << "          -plot 1   Weighted number of particles" << endl;
    cout << "          -plot 2   Energy" << endl;
    cout << "          -plot 3   Weighted energy" << endl;
    cout << "" << endl;
    cout << "          default value   0" << endl;

    cout << "-w" << endl;
    cout << "          -w [minimum weight value]  [maximum weight value]" << endl;
    cout << "" << endl;
    cout << "          default values  0.00 1.00" << endl;

    cout << "-e" << endl;
    cout << "          -e [minimum energy value]  [maximum energy value]" << endl;
    cout << "" << endl;
    cout << "          default values  0.0 100.0" << endl;
   
    cout << "-color" << endl;
    cout << "          -color [colormap file name]" << endl;
    cout << "" << endl;
    cout << "          default value   newmap" << endl;

    cout << "" << endl;
    cout << "Histogram options" << endl;
    cout << "=================" << endl;
    cout << "" << endl;


    cout << "-whist" << endl;
    cout << "          enables a weight histogram" << endl;
    cout << "          -whist [minimum weight value]  [maximum weight value] [histogram binsize]" << endl;
    cout << "" << endl;
    cout << "          default values  0.0 1.0 0.1" << endl;

    cout << "-ehist" << endl;
    cout << "          enables an energy histogram" << endl;
    cout << "          -ehist [minimum energy value]  [maximum energy value] [histogram binsize]" << endl;
    cout << "" << endl;
    cout << "          default values  0.0 1.0 0.1" << endl;

    cout << "" << endl;
    cout << "Other options" << endl;
    cout << "=============" << endl;
    cout << "" << endl;


    cout << "-rewrite" << endl;
    cout << "          rewrite the phase space applying the selected cuts" << endl;
    cout << "" << endl;

    cout << "-help" << endl;
    cout << "          display help menu" << endl;
    cout << "" << endl;
}
