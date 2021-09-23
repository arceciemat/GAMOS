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
     GAMOS Collaboration. CIEMAT 2009-11
     --  ssrb_fbp.cc  --                      
     mario.canadas@ciemat.es

	// Rewritten by Machiel Kolstein 2016 - 05 - 23
	- Using new and delete instead of malloc and free

-------------------------------------------------------*/
/*------------------------------------------------------
 		   
	 Description : 
	 
    SSRB - FBP method, using 3D sinograms. 
    
    Input data: Nbins * Ntang * (Nrings * Nrings), unsigned short 16 bit (.hv .v Interfile format)

Jan 2011: Bug:   Line 160: "filt = new double[P/2]" changed to "filt = new double[P/2+1]"
-------------------------------------------------------*/

#include "../include/VIP_ssrb_fbp.hh"
 
#ifndef PI
	#define PI 3.14159265358979323846
#endif

void PrintArgList()
{
	std::cout << " -------------------------- \n"   
	" Description :  \n"
	"   This program applies a SSRB-FBP2D method to reconstruct PET images. \n"
	"   Input data: Projection data (3D-sinogram) with dimension Nbins * Ntang * Nrings*Nrings, unsigned short 16 bit "
	" (.hv .v Interfile format) \n"
	" \n"

  // ARGUMENTS:
" Arguments convention: \n"

"     -m SSRB: Maximum ring difference (by default: -1 = n_planes-1), <MRDiff=-1) \n" //TO DO, leer desde .hv
"     -r SSRB: Normalization (by default: 1 = yes), <norm=1> \n"
"     -i Image size, number of X and Y pixels (by default: -1 = Number of bins in sinogram), <M>\n"
"     -x Pixel size, <delta_x> (by default: -1 = sinogram bin size) \n"
"     \n"    
"     -f Apodization window (Low pass) for filtering <type_f>:  \n"
"        1, Hamming gener. (by default, it includes Hann window and ramplak) \n"
"        2, Butterworth order <Butt_n>. \n"
"        3, Shepp-Logan. \n"
"     -a Alpha parameter for Hamming gener. window (by default: 1 = Ramp filter), <alpha> \n"
"     -b Order for Butterworth window (by default: 1) <Butt_n>. \n"
  "     -c Cutoff frecuency, relative to Nyquist frec: delta_rho/2 (by default: 0.75), <Nf_cut> \n"
  "     \n"
  "     -t Type of the input file (by default: 0 = Interfile), <typeINfile=0>\n"
  "     -n Name of the output image file (w/o extension: .hdr .img, Interfile), <fileOUTname> \n"
  "      \n"
  "     -h (Help) Printing arguments list \n" 
  "     -v Verbosity (by default:0=silent, 3=debug), <verbos> \n"
  // "     -o Output type   --> TO DO \n"
  
  " \n"
  " PET Reconstruction. \n"
  " CIEMAT - GAMOS Collaboration 2009-11 \n"
  " mario.canadas@ciemat.es \n"
  // " -------------------------- \n"
  " -------------------------- \n";
}

using namespace VIP_ssrb;
using namespace std;

// =============================================== MAIN CODE =================================

int main(int argc,char *argv[])
{
    if (argc==1)
	{
    	PrintArgList();
        exit(1);
    }

    for (int i = 1; i < argc; i++) 
    {
    /* Check for a switch (leading "-"). */
        if (argv[i][0] == '-') 
        {
      		/* Use the next character to decide what to do. */
			switch (argv[i][1]) 
			{
                 case 'a':	alpha = atof(argv[++i]);
     				break;
                 case 'b':	Butt_n = atof(argv[++i]);
     	            break;
                 case 'c':	Nf_cut = atof(argv[++i]);
                     break;
                 case 't':	typeINfile = atoi(argv[++i]);
     	            break;
                 case 'f':	type_f = atoi(argv[++i]);
     	            break;
                 case 'm': MRDiff = atoi(argv[++i]);
     	            break;
                 case 'i': M = atoi(argv[++i]);
     	            break;
                 case 'r': norm = atoi(argv[++i]);
     	            break;
                 case 'n': fileOUTname = argv[++i];
     	            break;
                 case 'v':	verbos = atoi(argv[++i]);
	                break;
                 case 'x':	delta_x = atof(argv[++i]);
	                break;
                 case 'h':
	                PrintArgList();
	                exit(0);
	                break;
                 case 'o':			               
	                break;
		    }
	    }
        else
	    {
    	    fileINname=argv[i];
        }
	}
  
    double elapTime;
    clock_t beginT, beginT_AfterINIT, endT, endT_AFTER_wr;

    beginT = clock();	// some system dependent time measurement

    // HERE WE READ THE HEADER FILES (output from lm2pd): 
    //
    cout << "ssrb_fbp2D::Input file name: " << fileINname << endl;
    ReadSinoDim();

    if (span==0 && MRDiff==-1) 
		MRDiff = n_planes - 1; 

	cout << "ssrb::input parameters: rings = " << n_planes << "; bins = " << n_bin 
		 << "; ang_views = " << n_ang << " .. pixDim: "<< pixDim << " ringDim: " << sliceDim << endl;
	cout << "ssrb::input parameters: Max. Ring Diff. = " << MRDiff << endl; //"; Span = " << span << endl;
	cout << "ssrb::output 2Dsino: slices = " << 2*n_planes-1 << "; sliceDim: " << sliceDim/2 << endl;
	cout << "... " << endl;
  
    // HERE WE READ THE SINOGRAMS (output from lm2pd): 
    //
	read_sino3D();
	cout << "3Dsino read" << endl;
	cout << "... " << endl;

	CreateSino2D();
  
	//Create sino2D, SSRB algorithm
	int plane2D;

	n_slices = 2*n_planes-1;
	planesNorm = new int[n_slices];  // for normalization:

	for(int i=0;i<n_bin;i++)
	{
 		for(int j=0;j<n_ang;j++)
		{
			//inicializa sino2D:
			for(int k=0; k < n_slices; k++)
			{
				sino2D[i][j][k] = 0.0;
				planesNorm[k] = 0;
      		}

      		// if (span==0){   // No axial compresion: rings*rings 3D projection planes
      		for (int k=0; k<(n_planes_file);k++)
			{
				plane2D=(int)(k%n_planes + k/n_planes);  
					// SSRB: p2D = p3D%n_planes + p3D/n_planes --> max. 2*n_planes - 1
				//Max Ring Difference:
				if (abs((k/n_planes - k%n_planes)) <= MRDiff) 
				{  // Z1= p3D / n_planes; Z2= p3D % n_planes
	  				planesNorm[plane2D]++;
	  				sino2D[i][j][plane2D]=(float) sino3D[i][j][k] + sino2D[i][j][plane2D];
				}
      		}
      		// }
      
      		// Normalization:
      		if (norm>0)
			{
				for(int k=0; k<n_slices; k++)
				{
	  				if (planesNorm[k]>1) sino2D[i][j][k]=sino2D[i][j][k]/(float)planesNorm[k];
				}      
      		}
    	}
	}

	VIP_ssrb::DeleteSino3D();

	endT = clock();
	elapTime = ((double)(endT - beginT))/CLOCKS_PER_SEC;

	cout << "ssrb::Process time(s): " << elapTime << endl;
	cout << "... " << endl;

// --- data from SSRB to FBP part:

	beginT = clock();

	delta_rho = pixDim;
	delta_slice = sliceDim*0.5;

// ------------------------------ end SSRB part ----------------------------------- //

	if (delta_x<=0) delta_x=delta_rho;
	if (M<=0) M=n_bin;
  
	cout << "fbp2D::output      image: X and Y dimension = " << M 
		 << "; pixel size = " << delta_x << endl;
	cout << "... " << endl;
	cout << "fbp2D::filter parameters: Window type " << type_f 
		 << "; alpha = " << alpha << "; Butterworth order = " << Butt_n << endl;
	cout << "... " << endl;
  
	// Initialization of the image = M x M x Slices matrix
	CreateImage();	
  
	// Initialization of Sino_FFT[dist][ang] (to store filtered sinogram data, per slice, before backprojection)
	sino_FFT = new double*[n_bin];   //n_bin=R, en primera versión no se usa Rmod, se chequea dentro del bucle
	for(int i=0;i<n_bin;i++)
	{
    	sino_FFT[i] = new double[n_ang];
    	/*for(j=0;j<n_ang;j++){ sino_FFT[i][j]=0.0;}*/
  	}
  
	//Memory allocation of filt[dist]; dist=1..P/2
	//ZERO PADDING: searching for P=2^n with P the first integer that is >=R  (R=n_bin)

	int n;
	n = (int)ceil(log(n_bin)/log(2));
	P = (int)pow(2,n);

	// Mem. allocation of in_data, out_data (temporal data fftw_complex) and plan (fftw_plan), 
	//	to be used for filtering with FFTW libraries (type: fftw_complex)
  
	int Pd2 = P/2+1;

	filt = new double[Pd2];
  
	fftw_plan plan_D, plan_I;

	double* in_data = new double[ P ];

	fftw_complex* out_data = new fftw_complex[ Pd2 ];
  
	plan_D = fftw_plan_dft_r2c_1d(P,in_data,out_data, FFTW_ESTIMATE);
	plan_I = fftw_plan_dft_c2r_1d(P,out_data,in_data, FFTW_ESTIMATE);

	//(*)FFTW NOTE: if input data in[i] are purely real numbers, the DFT output satisfies the "Hermitian" redundancy: 
	//	out[i] is the conjugate of out[n-i].
	//    (for multidimensional: For example, consider the transform of a two-dimensional real array of size n0 by n1. 
	//    The output of the r2c transform is a two-dimensional complex array of size n0 by n1/2+1, 
	//		where the y dimension has been cut nearly in half because of redundancies in the output.)

	// Mem. allocation for other temporal variables:
	costheta = new double[n_ang];
	sintheta = new double[n_ang];
  
	xc = new double*[M];
	for(int i=0;i<M;i++)
	{
    	xc[i] = new double[n_ang];
  	}
  
	ys = new double*[M];
	for(int i=0;i<M;i++)
	{
    	ys[i] = new double[n_ang];
	}  

	// Calculate filter:
	calculate_filter();

	// Initialialization of variables before backprojection:
	double rho_off = -((double)n_bin-1)/2;  // Rho offset (position of the center of first bin, rho_off * delta_rho = s_min)
	double th;
	for(int j=0;j<n_ang;j++)
	{
		th=j*PI/(double)n_ang;                // delta_theta = PI / n_ang
		costheta[j]=cos(th);
		sintheta[j]=sin(th);
	}

	double x_rel;
	for(int i=0;i<M;i++)
	{
    	x_rel=(i-(M-1)/2)*delta_x/delta_rho;   // sinogram position (distance, s) of a x point, normalized by bin size (delta_rho)
    	for(int j=0;j<n_ang;j++)
		{
			xc[i][j]=x_rel*costheta[j];
			ys[i][j]=x_rel*sintheta[j]-rho_off;
		}
	}

	beginT_AfterINIT = clock();

	for(int k=0;k<n_slices;k++)
	{            // FOR every slice
    	// Filtering using DFT (fftw-3 package)
    	for(int j=0;j<n_ang;j++)
		{
      		for(int i=0;i<n_bin;i++)
			{
				in_data[i] = (double)sino2D[i][j][k];   //real part
      		}
      		for(int i=n_bin;i<P;i++)
			{                  //FOR zero padding, using R!=2^n  (and P=2^n)
				in_data[i] = 0.0;           	
      		}
      		// Compute FFT
      		fftw_execute(plan_D);
      
      		// Multiply with filter    !!TO DO: multiply complex numbers directamente
      		out_data[0][0]=0.0;            // Handle zero freq. specially
		    out_data[0][1]=0.0;
		    out_data[P/2][0]=out_data[P/2][0] * filt[P/2]; // Handle half sampling freq.
		    out_data[P/2][1]=out_data[P/2][1] * filt[P/2];

    		for(int i=1;i<P/2;i++)
    		{
    			out_data[i][0]=out_data[i][0] * filt[i];     // Positive freq.
    			out_data[i][1]=out_data[i][1] * filt[i];
         		//out_data[P-i][0]=out_data[P-i][0] * filt[i];     // Not necesary: using real_to_complex FFTW: see above FTTW NOTE(*)
    			//out_data[P-i][1]=out_data[P-i][1] * filt[i];	
          	}
    
          	// Compute Inverse FFT, NOTE: fftw inverse values are unnormalized (scaled by n=P)
          	fftw_execute(plan_I);
    
    		for(int i=0;i<n_bin;i++)
    		{
    			sino_FFT[i][j] = in_data[i];   // For the radial samples: move the real-part
                    // add "../P" to use normalized fft values (affects to the absolute value in the image --> cuantification factor)
    		}
    	}  

	    ///////cout<< in_data[33][0] << endl;
	    ///////cout<< in_data[33][0]/P << endl;
		///////cout<< "..." << endl;

		// BACKPROJECTION per slice
		fastBP(k);
  	}
 
	endT_AFTER_wr=clock(); 
  
	write_INTERFILE(fileOUTname, image, M, M, n_slices, delta_x, delta_slice);

	fftw_destroy_plan(plan_D);
	fftw_destroy_plan(plan_I);

	DeleteData();

	endT = clock();

	elapTime = ((double)(endT - beginT))/CLOCKS_PER_SEC;
	double elapTime_perSlice = ( (double)(beginT_AfterINIT - beginT + endT - endT_AFTER_wr) 
            + (double)(endT_AFTER_wr - beginT_AfterINIT)/n_slices )/CLOCKS_PER_SEC; 

	cout << "fbp2D::Process time(s): " << elapTime << endl;
	cout << "fbp2D::Process time(s) per slice: " << elapTime_perSlice << endl;
}
// end main

// ==============================================================

void VIP_ssrb::ReadSinoDim()
{
	char nameIN_hv[512];
	strcpy(nameIN_hv, fileINname);
	strcat(nameIN_hv, ".hv");
  
	if (typeINfile==0)
	{
    	theINfile_hv = fopen( nameIN_hv , "r" ); // open text file "theFile"
  	}
  	if (theINfile_hv==NULL)
  	{
    	printf ("Error opening file \n");
    	exit (1);
  	}
  
	char cadena[100];
	char *c;
  
    // NOTE: this is reading "projdata.hv"
    //
	c= fgets (cadena, 20, theINfile_hv);
	while (c != NULL) 
	{
    	if ( strcmp("!matrix size [1] :=", cadena) == 0 )
		{
			fscanf (theINfile_hv, "%d",&n_bin);
			c= fgets (cadena, 1000, theINfile_hv);
			c= fgets (cadena, 33, theINfile_hv);
			fscanf (theINfile_hv, "%f",&pixDim);
    	}
		else if  ( strcmp("!matrix size [2] :=", cadena) == 0 ) 
		{
      		fscanf (theINfile_hv, "%d",&n_ang);
    	}
		else if ( strcmp("!matrix size [3] :=", cadena) == 0 ) 
		{
			fscanf (theINfile_hv, "%d",&n_planes_file);
			c= fgets (cadena, 1000, theINfile_hv);
			c= fgets (cadena, 33, theINfile_hv);
			fscanf (theINfile_hv, "%f",&sliceDim);
			c=NULL;
		}
		c= fgets (cadena, 1000, theINfile_hv);
		c= fgets (cadena, 20, theINfile_hv);
	}
	fclose(theINfile_hv);
  
	if (span==0)
	{
    	n_planes=(int)sqrt((float)n_planes_file);  // Total 3D planes = rings * rings
  	}
  	else
	{
    	nSEG= (((2*(MRDiff+1)+1)/span) - 1)/2;     // TO DO: now only for span==3 !!!!!!
    	n_planes= (n_planes_file + 2*nSEG*span*(nSEG+1) - 2*nSEG + 1) / (4*nSEG + 2);
  	}
}

// =================================================================================

void VIP_ssrb::read_sino3D()
{
	int i,j,k;

	char nameIN_v[512];
	strcpy(nameIN_v,fileINname);
	strcat(nameIN_v, ".v");
	
	theINfile_v=fopen(nameIN_v, "rb");
	if (theINfile_v == NULL)
	{
	  printf ("Error opening file (.v) \n");
	  exit (1);
	}
	
	// SINO_TYPE* sino_line; 
	// sino_line = (SINO_TYPE*) malloc( n_bin*n_ang*n_planes_file*sizeof(SINO_TYPE));
	// 		compare: 
	//				int *array = new int[n];
	SINO_TYPE* sino_line = new SINO_TYPE[ n_bin*n_ang*n_planes_file ];
	
	fread(sino_line,1,n_bin*n_ang*n_planes_file*sizeof(SINO_TYPE), theINfile_v);
	fclose(theINfile_v);
	
	//--- Initialize sino3D with data read from file---
	sino3D = new SINO_TYPE**[n_bin];
	for(i=0; i<n_bin; i++)
	{
		sino3D[i] = new SINO_TYPE*[n_ang];
	  	for(j=0;j<n_ang;j++)
		{
	    	sino3D[i][j] = new SINO_TYPE[(n_planes_file)];  
	    	/// !!!! DECIDIR si almacenar MAX_Ring difference o dejar asi con el tamaño 
			///			incluyendo todos los planos, y =0 en caso > MRDiff
	    	for(k=0;k<(n_planes_file);k++)
			{
				sino3D[i][j][k] = sino_line[ k*n_ang*n_bin + j*n_bin + i];
	    	}
	  	}
	}
	
	delete [] sino_line;
}

// =============================================================================

void VIP_ssrb::calculate_filter()
{
  double temp_f;
  double f_cut = Nf_cut / (2*delta_rho);  // Nyquist frecuency = 1 / (2*delta_rho)

  for(int r=1;r<=P/2;r++)
  {
    temp_f=(double)r/(delta_rho*(double)P);
    
    switch (type_f) 
    {
    case 1:                                          // Hamming generalizad
      
      if(temp_f<f_cut)
      {
	     filt[r]=temp_f*(alpha + (1.0-alpha)*cos(PI*temp_f/f_cut) );
      }
      else
      {
	     filt[r]=0.0;
      };
      break;
      
    case 2:                                          // Butterworth order N
      filt[r]=temp_f*( 1.0 / sqrt( 1+pow((temp_f/f_cut),Butt_n) ) );
      break;
      
    case 3:                                          // Shepp Logan
      if(temp_f<f_cut)
      {
	     filt[r]=temp_f*( (2.0*f_cut/(temp_f*PI))*sin(temp_f*PI/(2.0*f_cut)) );
      } 
	  else 
      {
	     filt[r]=0.0;
      };
      break;   
    } 
  }
}

// =============================================================================

void VIP_ssrb::write_INTERFILE(const char* filename, float*** data, int x, int y, int z, double pixD, double sliceD)
{   
    //TO DO: sacar x, y, z directament del array "data"

    char name_hdr[512];
    char name_hv[512]; 
    char name_img[512];

    strcpy(name_hdr, filename);
    strcpy(name_hv, filename);
    strcpy(name_img,filename);

    strcat(name_hdr, ".hdr");
    strcat(name_hv, ".hv");
    strcat(name_img, ".img");

    theOUTfile=fopen(name_hdr, "w");

    fprintf (theOUTfile, "!INTERFILE  := \n");
    fprintf (theOUTfile, "name of data file := %s.img\n",fileOUTname);
    fprintf (theOUTfile, "!GENERAL DATA := \n");
    fprintf (theOUTfile, "!GENERAL IMAGE DATA :=\n");
    fprintf (theOUTfile, "!type of data := tomographic\n");
    fprintf (theOUTfile, "!version of keys := 3.3\n");
    fprintf (theOUTfile, "!data offset in bytes := 0\n");
    fprintf (theOUTfile, "imagedata byte order := littleendian\n");
    fprintf (theOUTfile, "!PET STUDY (General) :=\n");
    fprintf (theOUTfile, "!PET data type := Image\n");
    fprintf (theOUTfile, "process status := Reconstructed\n");
    fprintf (theOUTfile, "!number format := short float\n");
    fprintf (theOUTfile, "!number of bytes per pixel := 4\n");
    fprintf (theOUTfile, "number of dimensions := 3\n");
    fprintf (theOUTfile, "matrix axis label [1] := x\n");
    fprintf (theOUTfile, "!matrix size [1] := %i\n", x);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [1] := %lf\n",pixD);
    fprintf (theOUTfile, "matrix axis label [2] := y\n");
    fprintf (theOUTfile, "!matrix size [2] := %i\n", y);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [2] := %lf\n",pixD);
    fprintf (theOUTfile, "matrix axis label [3] := z\n");
    fprintf (theOUTfile, "!matrix size [3] := %i\n",z);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [3] := %lf\n",sliceD);
    fprintf (theOUTfile, "number of slices := %i\n",z); 
    fprintf (theOUTfile, "number of time frames := 1\n");
    fprintf (theOUTfile, "image scaling factor[1] := 1\n");
    fprintf (theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf (theOUTfile, "quantification units := 1\n");
    fprintf (theOUTfile, "!END OF INTERFILE := \n");

    fclose(theOUTfile);

    theOUTfile=fopen(name_hv, "w");

    fprintf (theOUTfile, "!INTERFILE  := \n");
    fprintf (theOUTfile, "name of data file := %s.img\n",fileOUTname);
    fprintf (theOUTfile, "!GENERAL DATA := \n");
    fprintf (theOUTfile, "!GENERAL IMAGE DATA :=\n");
    fprintf (theOUTfile, "!type of data := PET\n");
    fprintf (theOUTfile, "imagedata byte order := LITTLEENDIAN\n");
    fprintf (theOUTfile, "!PET STUDY (General) :=\n");
    fprintf (theOUTfile, "!PET data type := Image\n");
    fprintf (theOUTfile, "process status := Reconstructed\n");
    fprintf (theOUTfile, "!number format := float\n");
    fprintf (theOUTfile, "!number of bytes per pixel := 4\n");
    fprintf (theOUTfile, "number of dimensions := 3\n");
    fprintf (theOUTfile, "matrix axis label [1] := x\n");
    fprintf (theOUTfile, "!matrix size [1] := %i\n", x);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [1] := %lf\n",pixD);
    fprintf (theOUTfile, "matrix axis label [2] := y\n");
    fprintf (theOUTfile, "!matrix size [2] := %i\n", y);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [2] := %lf\n",pixD);
    fprintf (theOUTfile, "matrix axis label [3] := z\n");
    fprintf (theOUTfile, "!matrix size [3] := %i\n",z);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [3] := %lf\n",sliceD); 
    fprintf (theOUTfile, "number of time frames := 1\n");
    fprintf (theOUTfile, "image scaling factor[1] := 1\n");
    fprintf (theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf (theOUTfile, "quantification units := 1\n");
    fprintf (theOUTfile, "!END OF INTERFILE := \n");

    fclose(theOUTfile);

    float* data_line = new float[ x*y*z ];

    long unsigned int cont=0;
    int i,j,k;

    for(k=0;k<z;k++)
    {
        for(j=0;j<y;j++)
        {
            for(i=0;i<x;i++)
            {
                data_line[cont]=data[i][j][k];
                cont++;
            }
        }
    }

    theOUTfile=fopen(name_img, "wb");
    fwrite(data_line,1, x*y*z*sizeof(float), theOUTfile);
    fclose(theOUTfile);

    // free(data_line);
    delete [] data_line;
}

// =============================================================================

void VIP_ssrb::fastBP(int slice)
{
  int x, y, j, rl;
  double sum, rm,rl_f, w;

  for(x=0;x<M;x++)
  {
    for(y=0;y<M;y++)
    {
      sum=0.0;
      for(j=0;j<n_ang;j++)
      {
	    rm=xc[x][j]+ys[y][j];
    	rl_f=floor(rm);
    	rl=(int)rl_f;	
    	w=rm-rl_f;

    	if (rl>=0 && rl<n_bin-1)    //TO DO New version without this "if" inside the loop
        {
    	  sum=sum+(1-w)*sino_FFT[rl][j]+w*sino_FFT[rl+1][j];   // Using Linear interpolation
    	} 
    	else if (rl==n_bin-1) 
        {
    	  sum=sum+(1-w)*sino_FFT[rl][j];
    	} 
    	else if (rl==-1) 
        {
    	  sum=sum+w*sino_FFT[0][j];
    	}
      }      

      image[x][y][slice] = (float)(sum*PI/(double)n_ang);
    }
  }
}

// =================================================================================

void VIP_ssrb::DeleteData()
{
	VIP_ssrb::DeleteSino2D();
	for(int i=0;i<n_bin;i++)
	{
		delete [] sino_FFT[i];
	}
	delete [] sino_FFT;

	for(int i=0;i<M;i++)
	{
		for(int j=0;j<M;j++)
		{
			delete [] image[i][j];
		}
		delete [] image[i];
		delete [] xc[i];
		delete [] ys[i];
	}
	delete [] image;
	delete [] xc;
	delete [] ys;
}

// ==============================================================

void VIP_ssrb::DeleteSino3D() 
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

// ==============================================================

void VIP_ssrb::DeleteSino2D() 
{
	for(int i=0; i<n_bin; i++)
	{
		delete [] sino2D[i];
	}
	delete [] sino2D;
}

// ==============================================================

void VIP_ssrb::CreateSino2D()
{
	sino2D = new float**[n_bin];
	n_slices = 2*n_planes-1;

	for(int i=0; i<n_bin; i++)
	{
    	sino2D[i] = new float*[n_ang];
 		for(int j=0;j<n_ang;j++)
		{
			sino2D[i][j] = new float[n_slices];   
		}
	}
}

// ==============================================================

void VIP_ssrb::CreateImage()
{
	n_slices = 2*n_planes-1;
	image = new float**[M];
	for(int i=0; i<M; i++)
	{
		image[i] = new float*[M];
		for(int j=0; j<M; j++)
		{
			image[i][j] = new float[n_slices];
			for(int k=0; k<n_slices; k++)
			{
				image[i][j][k] = 0.0;
			}
		}
	}
}

// ==============================================================




