//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//
/*-------------------------------------------------------

     PET Reconstruction.  
     Gamos Collaboration - CIEMAT 2009-11
                        
     --  lm2pd.cc  --                      

     mario.canadas@ciemat.es

-------------------------------------------------------*/
/*------------------------------------------------------
 	   
	 Description : 
	 
    GAMOS List-mode (Global X Y Z) to projection data (sino 3D). 
    
    Input data: Global X Y Z  (GAMOS format)

    Ciemat (Interfile, .hv .v) and STIR projection data format (Interfile 3.3, .hs .s)

--------------------------------------------------------*/

/*------------------------------------------------------
 Rewritten 2016-01-26 Machiel Kolstein - IFAE, Barcelona, Spain
	- Using ifstream/ofstream for bigger data (not possible with FILE)
	- Corrected way of deleting "sino3D" array 
	- Put "using namespace std" from header to source file
--------------------------------------------------------*/

#include "../include/VIP_lm2pd.hh"
#include <fstream>
#include <cerrno>
#include <string>
#include <cstring>

using namespace std;
// ==============================================================

int main(int argc,char *argv[])
{
	if (argc==1)
	{
		cout << " -------------------------- \n"
		//     " --------------------------  \n"  
		" Description : \n"
		"     List-mode file to projection data (sino 3D). \n"
		"     Input data: Global X Y Z  (Arce format) \n"
		" \n"

		// ARGUMENTS:
		" Arguments convention: \n"
		"     -a Axial FOV (mm), <d_axial=100.0> \n"
		"     -d Diameter Transaxial FOV (mm), <d_FOV=300.0> \n"
		"     -i Type of the input file (by default: 0 = Arce_binary), <typeINfile=0> \n"
		"     -j Offset X direction (mm), <offsetX=0.0> \n"
		" \n"
		"     -m Maximum ring difference (by default: -1 = n_planes), <MRDiff> \n"
		"     -n Name of output file, <fileOUTname> \n"
		"     -p Axial number of planes, <n_planes> \n"
		"     -r Number of bins, \"distancias\", <n_bin> \n"
		// -s Span                                         TO DO:span !!!!!
		"     -t Number of angular views, \"direcciones\", <n_ang> \n"
		"     -v Verbosity (by default: 0=silent, 3=debug), <verbos> \n"
		"     -x Maximum number of coincidences to be stored (by default: -1 = no limit), <Max_Coinci> \n"
		"     -o Output type (by default: 0 = mcc Interfile, 1 = STIR Interfile), <oType> \n"
		
		" \n"
		" GAMOS Collaboration. CIEMAT 2009-11 \n"
		" mario.canadas@ciemat.es \n"
		// " -------------------------- \n"
		" -------------------------- \n";
		exit(1);
	}

	for (int i = 1; i < argc; i++) 
	{
		/* Check for a switch (leading "-"). */

		if (argv[i][0] == '-') 
		{
			/* Use the next character to decide what to do. */
			switch (argv[i][1]) {
	
			case 'a':	d_axial = atof(argv[++i]);
				break;
	
			case 'd': d_FOV = atof(argv[++i]);
				break;
	
			case 'i':	typeINfile = atoi(argv[++i]);
				break;
	
			case 'j':	offsetX = atof(argv[++i]);
				break;
	
			case 'm':   MRDiff = atoi(argv[++i]);
				break;
			
			case 'n':   fileOUTname = argv[++i];
				break;
	
			case 'p':	n_planes = atoi(argv[++i]);  //rings or "direct planes"
				break;
	
			case 'r':	n_bin = atoi(argv[++i]);
				break;
		
			case 't':	n_ang = atoi(argv[++i]);
				break;		
			
			case 'v':	verbos = atoi(argv[++i]);
				break;
	
			case 'x':   MAX_Coinci = (long unsigned int)atof(argv[++i]);
				break;
					
			case 'o':	oType = atoi(argv[++i]);
				break;
			
			default:				
				break;
			}
		}
		else
		{
			fileINname=argv[i];
		}               
	}

	cout << "lm2pd::Input file name: " << fileINname << endl;
	cout << "lm2pd::Parameters: planes = " << n_planes 
		 << "; bins = " << n_bin << "; ang_views = " << n_ang << endl;
	cout << "lm2pd::Dimensions: Transaxial FOV = " << d_FOV 
		 << "; Axial FOV = " << d_axial << " ; Transaxial_pix(mm) = " 
		 << d_FOV/(n_bin-1) <<"; Plane_width(mm) = " << d_axial/(n_planes-1) 
		 << endl; // OLD!! ssrb== 0.5*(d_axial)/n_planes << endl;
	cout << "... " << endl;
 
	if (MRDiff == -1) 
		MRDiff=n_planes-1;
 
	TOTaxial_pl=n_planes*n_planes;
	if (oType==1) 
		TOTaxial_pl = (2*n_planes-1 - MRDiff)*MRDiff + n_planes;  
			// total number of Axial planes (segments*planes) in STIR format
 
	/*--- Initialize sino3D ---*/
	InitializeSino3D();

	// Loop over files
	_PI = 2*asin(1);
	lor theLOR;

	if (typeINfile == FILETYPE_DEFAULT)
	{
		ifstream mylist( fileINname.c_str() );
		if( !mylist.is_open() ) 
		{
			cout << "ERROR with list" << endl;
			exit(1);
		}

		for( ; mylist >> fileINname;) 
		{
			cout << "opening file " << fileINname << endl;

			// open binary file "theFile"
			theINfile.open( fileINname.c_str() , std::ios::binary | std::ios::in ); 

			if ( !theINfile.is_open() )
			{
				cout << "ERROR, file is not open! " << fileINname << endl;
				exit(1);
			}

			theLOR = ReadData();
		
			while ( !theINfile.eof() && !theINfile.fail() && (MAX_Coinci==0 || nCoinciVal<MAX_Coinci) )
			{
				if (verbos >= 3) 
				{
					cout << "lm_to_sino3D::ReadData LoR_p1: " << theLOR.p1.X << " " << theLOR.p1.Y 
						 << " " << theLOR.p1.Z << "; LoR_p2: " << theLOR.p2.X << " " << theLOR.p2.Y 
						 << " " << theLOR.p2.Z << endl;
				}
		
				//theLOR --> sino3D
				Load_sino3D(theLOR);
			
				theLOR = ReadData();
			}
		}
	}

	cout << "Total  Coinci: " << nCoinciTot << endl; 
	cout << "Sino3D Coinci: " << nCoinciVal << endl;  
	
	write_sino3D();
 
	DeleteSino3D();

	theINfile.close();
}

// ==============================================================

void Load_sino3D(lor inpLOR)
{
	int z1_i, z2_i; 
	//for discretization on the crystal: int x1_i, x2_i, y1_i, y2_i;
	
	float z1_abs=inpLOR.p1.Z+d_axial/2;
	float z2_abs=inpLOR.p2.Z+d_axial/2;
	double a, b, phi, dis;
	int phi_i, dis_i;
	int ring_diff;
	
	z1_i=(int)(n_planes* z1_abs/d_axial);  //round --> n_planes+1 ...
	z2_i=(int)(n_planes* z2_abs/d_axial);

	// control; if z_i out of range: return
	if ( (z1_i<0) || (z2_i<0) || (z1_i>= n_planes) || (z2_i>= n_planes) ) return;
	
	// control; if x1, y1 == x2, y2: return
	if ( (inpLOR.p1.X==inpLOR.p2.X) && (inpLOR.p1.Y==inpLOR.p2.Y) ) return;
	
	ring_diff = (int)fabs(z1_i-z2_i);
	
	// max ring difference; control:
	if (ring_diff > MRDiff) return;

	a=(double)(inpLOR.p2.Y- inpLOR.p1.Y);
	b=(double)(inpLOR.p2.X- inpLOR.p1.X);

  	if (a==0.0)
	{
    	phi=_PI*0.5;
  	}
  	else
	{
    	phi=atan(b/a);
  	}

	if (phi<0) phi = phi +_PI;
	
	dis = inpLOR.p1.X*cos(phi) - inpLOR.p1.Y*sin(phi);

	// control; transaxial FOV
	if ( fabs(dis) > d_FOV*0.5 ) return;
	
	dis = dis + d_FOV*0.5;
	
	// discret values:
	
	phi_i=(int)round( (double)(n_ang-1)*phi/_PI );
	dis_i=(int)round( (double)(n_bin-1)*dis/(double)d_FOV );
	
	if ((phi_i>=n_ang) || (dis_i>=n_bin)) return;  
        // only possible "=" because 'round' check it..

	// OLD: (SRRB included) sino3D[dis_i][phi_i][ (z1_i+z2_i)+ring_diff*(n_planes-1) ]++;
  
	int Zpos;

	if (oType==0) 
	{
		Zpos = (z1_i*n_planes + z2_i);
	}
	else
	{
		if (z1_i>=z2_i) 
		{  
			// SIN Max Ring_Diff: Zpos= ( ((n_planes-ring_diff)*(n_planes-1-ring_diff))/2 + z2_i );
			Zpos= ( ((2*n_planes-1 - MRDiff - ring_diff)*(MRDiff - ring_diff))/2 + z2_i);
		} 
		else 
		{
			Zpos= ( (TOTaxial_pl) - ((2*n_planes-1 - MRDiff - ring_diff +1)*(MRDiff - ring_diff +1))/2  + z1_i );
		}
	}
  	
	sino3D[dis_i][phi_i][ Zpos ]++;
	
	nCoinciVal++;
}

// ================================================

lor ReadData()
{
	arce_struct arce_out;
	lor lorOUT;
  
	if ( typeINfile == FILETYPE_DEFAULT )
	{
		theINfile.read((char*) &arce_out, sizeof(arce_struct));

		lorOUT.p1.X = arce_out.x1 + offsetX;
		lorOUT.p1.Y = arce_out.y1;
		lorOUT.p1.Z = arce_out.z1;
		
		lorOUT.p2.X = arce_out.x2 + offsetX;
		lorOUT.p2.Y = arce_out.y2;
		lorOUT.p2.Z = arce_out.z2;	
	}

	if ( !theINfile.eof() && !theINfile.fail() )
	{
    	nCoinciTot++;
  	}

	return lorOUT;
}

// =========================================================

void write_sino3D()
{
	char name_hv[512];
	char name_v[512];

	if (oType==0)
	{
		strcpy(name_hv, fileOUTname.c_str() );
		strcpy(name_v, fileOUTname.c_str() );
		
		strcat(name_hv, ".hv");
		strcat(name_v, ".v");
		
		theOUTfile.open(name_hv, std::ios::out);

		theOUTfile << "!INTERFILE  := " << endl;
		theOUTfile << "name of data file := " << fileOUTname << endl;
		theOUTfile << "!GENERAL DATA := " << endl;
		theOUTfile << "!GENERAL IMAGE DATA := " << endl;
		theOUTfile << "!type of data := tomographic " << endl;
		theOUTfile << "!version of keys := 3.3 " << endl;
		theOUTfile << "!data offset in bytes := 0 " << endl;
		theOUTfile << "imagedata byte order := littleendian " << endl;
		theOUTfile << "!PET STUDY (General) := " << endl;
		theOUTfile << "!PET data type := 3D-Sinogram " << endl;
		theOUTfile << "process status := Reconstructed " << endl;
		theOUTfile << "!number format := unsigned short " << endl;
		theOUTfile << "!number of bytes per pixel := 2 " << endl;
		theOUTfile << "number of dimensions := 3 " << endl;
		theOUTfile << "matrix axis label [1] := x " << endl;
		theOUTfile << "!matrix size [1] := " << n_bin << endl;
		theOUTfile << "scaling factor (mm/pixel) [1] := " << (float)(d_FOV/(n_bin-1)) << endl;
		
		theOUTfile << "matrix axis label [2] := y" << endl;
		theOUTfile << "!matrix size [2] := " << n_ang << endl;
		
		theOUTfile << "scaling factor (degree/pixel) [2] := " << (float)(360./(n_ang-1)) << endl;
		
		
		theOUTfile << "matrix axis label [3] := z" << endl;
		theOUTfile << "!matrix size [3] := " << n_planes*n_planes << endl;
		theOUTfile << "scaling factor (mm/pixel) [3] := " << (float)(d_axial/(n_planes-1)) << endl;
		
		theOUTfile << "number of slices := " << n_planes*n_planes << endl;
		theOUTfile << "number of time frames := 1" << endl;
		theOUTfile << "image scaling factor[1] := 1" << endl;
		theOUTfile << "data offset in bytes[1] := 0" << endl;
		theOUTfile << "quantification units := 1" << endl;
		theOUTfile << "!END OF INTERFILE := " << endl;
		
		theOUTfile.close();
		
		//(size_t)(n_bin*n_ang*n_planes*n_planes);    
	} 
	else 
	{
		strcpy(name_hv, fileOUTname.c_str());
		strcpy(name_v,  fileOUTname.c_str());
		
		strcat(name_hv, ".hs"); // STIR extension: .hs .s
		strcat(name_v, ".s");
		
		theOUTfile.open(name_hv, std::ios::out);
		
		theOUTfile << "!INTERFILE  := " << endl;
		theOUTfile << "name of data file := " << fileOUTname << endl;
		theOUTfile << "!GENERAL DATA := " << endl;
		theOUTfile << "!GENERAL IMAGE DATA :=" << endl;
		theOUTfile << "!type of data := PET" << endl;
		//    theOUTfile << "!version of keys := 3.3\n" << endl;     
		// STIR format is not 3.3 (almost but not completely), ERROR in STIR if it is not removed
		//    theOUTfile << "!data offset in bytes := 0\n" << endl;
		theOUTfile << "imagedata byte order := littleendian" << endl;
		theOUTfile << "!PET STUDY (General) :=" << endl;
		theOUTfile << "!PET data type := Emission" << endl;
		theOUTfile << "applied corrections := {arc correction}" << endl;        // {none}\n" << endl;
		//    theOUTfile << "process status := Reconstructed\n" << endl;
		theOUTfile << "!number format := unsigned integer" << endl;
		theOUTfile << "!number of bytes per pixel := 2" << endl;
		
		theOUTfile << "number of dimensions := 4" << endl;
		theOUTfile << "matrix axis label [4] := segment" << endl;
		theOUTfile << "!matrix size [4] := " << MRDiff*2 + 1 << endl;
		//    theOUTfile << "scaling factor (mm/pixel) [1] := %f\n",(float)(d_FOV/(n_bin-1)) << endl;
		theOUTfile << "matrix axis label [3] := axial coordinate" << endl;
		theOUTfile << "!matrix size [3] := { " << endl;

    	if (MRDiff==0) 
      	{
			theOUTfile << "%i}" << n_planes << endl;
      	} else {
			for(int m=n_planes-MRDiff;m<=n_planes;m++) 
				theOUTfile << m << endl;
			for(int m=n_planes-1;m>n_planes-MRDiff;m--)  
				theOUTfile << m << endl;
			theOUTfile << n_planes-MRDiff << endl;
      	}
		theOUTfile << "matrix axis label [2] := view" << endl;
		theOUTfile << "!matrix size [2] := " << n_ang << endl;
		theOUTfile << "matrix axis label [1] := tangential coordinate" << endl;
		theOUTfile << "!matrix size [1] := " << n_bin << endl;   
		
		theOUTfile << "minimum ring difference per segment := {" << endl;     
			// TO DO :  add SPAN (MRDiff per seg. variable)
		theOUTfile << -MRDiff << endl; 
		for(int m=-MRDiff+1;m<=MRDiff;m++)  
			theOUTfile << m << endl;
		theOUTfile << "}" << endl;
		theOUTfile << "maximum ring difference per segment := {" << endl;     
			// TO DO :  add SPAN (MRDiff per seg. variable)
		theOUTfile << -MRDiff << endl; 
		for(int m=-MRDiff+1;m<=MRDiff;m++)  
			theOUTfile << m << endl;
		theOUTfile << "}" << endl;
		
		theOUTfile << "inner ring diameter (cm) := " << d_FOV/10 << endl;   
			// STIR Required parameter, now assigned to FOV (not detectors)
		theOUTfile << "average depth of interaction (cm) := 0.0001" << endl;
		theOUTfile << "default bin size (cm) := " << 0.1*((float)d_FOV/((float)n_bin-1.0))  << endl;
		theOUTfile << "number of rings := " << n_planes  << endl;    
		theOUTfile << "distance between rings (cm) := " << 0.1*(float)(d_axial/(n_planes-1.0))  << endl;  
			// Axial pixel dimension
   
		theOUTfile << "number of detectors per ring := " << n_ang*2  << endl;  
		//    theOUTfile << "number of slices := %i\n",n_planes*n_planes << endl;
		theOUTfile << "number of time frames := 1" << endl;
		theOUTfile << "image scaling factor[1] := 1" << endl;
		theOUTfile << "data offset in bytes[1] := 0" << endl;
		theOUTfile << "quantification units := 1" << endl;
		theOUTfile << "!END OF INTERFILE := " << endl;
		
		theOUTfile.close();
	}

	long unsigned int cont=0;
	theOUTfile.open( name_v, std::ios::binary | std::ios::out );  

	for(int k=0; k<TOTaxial_pl; k++)
	{
		for(int j=0; j<n_ang; j++)
		{
			for(int i=0; i<n_bin; i++)
			{
				SINO_TYPE sino = sino3D[i][j][k];
				theOUTfile.write((char*) &sino, sizeof(SINO_TYPE));
				cont++;
			}
		}
	}
  
	theOUTfile.close();
}

// ==============================================================

void InitializeSino3D()
{
	sino3D = new SINO_TYPE**[n_bin];
	for (int i=0; i < n_bin; i++)
	{
		sino3D[i] = new SINO_TYPE*[n_ang];
		for (int j=0; j < n_ang; j++)
		{
			sino3D[i][j] = new SINO_TYPE[TOTaxial_pl];
			for(int k=0; k < TOTaxial_pl; k++)
			{
				sino3D[i][j][k] = 0;
			}
		}
	}
}

// ==============================================================

void DeleteSino3D() 
{
	for(int i=0; i<n_bin; i++)
	{
		for(int j=0; j<n_ang; j++)
		{
			delete [] sino3D[i][j];
		}
		delete [] sino3D[i];
	}
	delete [] sino3D;
}


