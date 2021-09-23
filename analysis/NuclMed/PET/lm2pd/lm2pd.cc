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

#include "include/lm2pd.hh" 


int main(int argc,char *argv[])
{
  if (argc==1){

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

for (int i = 1; i < argc; i++) {

	/* Check for a switch (leading "-"). */

	if (argv[i][0] == '-') {

	    /* Use the next character to decide what to do. */

	    switch (argv[i][1]) {

		case 'a':	d_axial = atof(argv[++i]);
				break;

	        case 'd':       d_FOV = atof(argv[++i]);
		                break;

        	case 'i':	typeINfile = atoi(argv[++i]);
				break;

        	case 'j':	offsetX = atof(argv[++i]);
				break;

	        case 'm':       MRDiff = atoi(argv[++i]);
				break;
		
	        case 'n':       fileOUTname = argv[++i];
				break;

	        case 'p':	n_planes = atoi(argv[++i]);  //rings or "direct planes"
				break;

		case 'r':	n_bin = atoi(argv[++i]);
				break;
	
		case 't':	n_ang = atoi(argv[++i]);
				break;		
		
		case 'v':	verbos = atoi(argv[++i]);
				break;

	        case 'x':       MAX_Coinci = (long unsigned int)atof(argv[++i]);
				break;
			      
		case 'o':	oType = atoi(argv[++i]);
				break;
			               
				break;

	    }
	}
	else{
	      fileINname=argv[i];
	}               
}

 cout << "lm2pd::Input file name: " << fileINname << endl;
 cout << "lm2pd::Parameters: planes = " << n_planes << "; bins = " << n_bin << "; ang_views = " << n_ang << endl;
 cout << "lm2pd::Dimensions: Transaxial FOV = " << d_FOV << "; Axial FOV = " << d_axial << " ; Transaxial_pix(mm) = " << d_FOV/(n_bin-1) <<"; Plane_width(mm) = " << d_axial/(n_planes-1) << endl; // OLD!! ssrb== 0.5*(d_axial)/n_planes << endl;
 cout << "... " << endl;
 
 int i,j,k;
 if (MRDiff==-1) MRDiff=n_planes-1;
 
 TOTaxial_pl=n_planes*n_planes;
 if (oType==1) TOTaxial_pl= (2*n_planes-1 - MRDiff)*MRDiff + n_planes;  // total number of Axial planes (segments*planes) in STIR format
 
 /*--- Initialize sino3D ---*/
 sino3D = new SINO_TYPE**[n_bin];
 for(i=0;i<n_bin;i++){
   sino3D[i] = new SINO_TYPE*[n_ang];
   for(j=0;j<n_ang;j++){
     sino3D[i][j] = new SINO_TYPE[TOTaxial_pl];  /// ! If oType==1 (STIR output):Matrix size depends on the MAX_Ring
     for(k=0;k<TOTaxial_pl;k++){
       sino3D[i][j][k]=0;
     }
   }
 }

 _PI=2*asin(1);
 lor theLOR;

if (typeINfile==0){
 theINfile = fopen( fileINname , "rb" ); // open binary file "theFile"
  }
  if (theINfile==NULL)
  {
    printf ("Error opening file \n");
    exit (1);
  }

 theLOR=ReadData();
 
 while (!feof(theINfile) && !ferror(theINfile) && (MAX_Coinci==0 || nCoinciVal<MAX_Coinci) ){
    if (verbos >= 3) {
  cout << "lm_to_sino3D::ReadData LoR_p1: " << theLOR.p1.X << " " << theLOR.p1.Y << " " << theLOR.p1.Z << "; LoR_p2: " << theLOR.p2.X << " " << theLOR.p2.Y << " " << theLOR.p2.Z << endl;
    }

   //theLOR --> sino3D
   Load_sino3D(theLOR);
   
   theLOR=ReadData();
 }

 cout << "Total  Coinci: " << nCoinciTot << endl; 
 cout << "Sino3D Coinci: " << nCoinciVal << endl;  
 //cout << asin(1) << endl;

 write_sino3D();
 
 // free_sino3D();  error segm fault ?????????

fclose (theINfile); // close file "theFile"
 
}

void Load_sino3D(lor inpLOR){

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

  if (a==0.0){
    phi=_PI*0.5;
  }
  else{
    phi=atan(b/a);
  }

  if (phi<0) phi = phi +_PI;

  dis=inpLOR.p1.X*cos(phi) - inpLOR.p1.Y*sin(phi);

  // control; transaxial FOV
  if ( fabs(dis) > d_FOV*0.5 ) return;
 
  dis = dis + d_FOV*0.5;

  // discret values:

  phi_i=(int)round( (double)(n_ang-1)*phi/_PI );
  dis_i=(int)round( (double)(n_bin-1)*dis/(double)d_FOV );

  if ((phi_i>=n_ang) || (dis_i>=n_bin)) return;  // only possible "=" because 'round' check it..

  // OLD: (SRRB included) sino3D[dis_i][phi_i][ (z1_i+z2_i)+ring_diff*(n_planes-1) ]++;
  
int Zpos;

  if (oType==0) {
    Zpos = (z1_i*n_planes + z2_i);
  }
  else{
    
    if (z1_i>=z2_i) {  // SIN Max Ring_Diff: Zpos= ( ((n_planes-ring_diff)*(n_planes-1-ring_diff))/2 + z2_i );
      
      Zpos= ( ((2*n_planes-1 - MRDiff - ring_diff)*(MRDiff - ring_diff))/2 + z2_i);
      
    }else{
      Zpos= ( (TOTaxial_pl) - ((2*n_planes-1 - MRDiff - ring_diff +1)*(MRDiff - ring_diff +1))/2  + z1_i );	
      
    }
  }
  
  
  sino3D[dis_i][phi_i][ Zpos ]++;
  
  nCoinciVal++;
  
}


lor ReadData(){
  arce_str arce_out;
  lor lorOUT;
  
  if (typeINfile==0){
    
    fread (&arce_out, sizeof(struct arce_str),1,theINfile);
    lorOUT.p1.X=arce_out.x1 + offsetX;
    lorOUT.p1.Y=arce_out.y1;
    lorOUT.p1.Z=arce_out.z1;
    
    lorOUT.p2.X=arce_out.x2 + offsetX;
    lorOUT.p2.Y=arce_out.y2;
    lorOUT.p2.Z=arce_out.z2;

  }
  
  if (!feof(theINfile) && !ferror(theINfile) ){
    nCoinciTot++;
  }
  
  return lorOUT;
  
}


void write_sino3D(){

  char name_hv[512];
  char name_v[512];



  if (oType==0){
    
    
    strcpy(name_hv, fileOUTname);
    strcpy(name_v,fileOUTname);
    
    strcat(name_hv, ".hv");
    strcat(name_v, ".v");
    
    theOUTfile=fopen(name_hv, "w");
    
    fprintf (theOUTfile, "!INTERFILE  := \n");
    fprintf (theOUTfile, "name of data file := %s.v\n",fileOUTname);
    fprintf (theOUTfile, "!GENERAL DATA := \n");
    fprintf (theOUTfile, "!GENERAL IMAGE DATA :=\n");
    fprintf (theOUTfile, "!type of data := tomographic\n");
    fprintf (theOUTfile, "!version of keys := 3.3\n");
    fprintf (theOUTfile, "!data offset in bytes := 0\n");
    fprintf (theOUTfile, "imagedata byte order := littleendian\n");
    fprintf (theOUTfile, "!PET STUDY (General) :=\n");
    fprintf (theOUTfile, "!PET data type := 3D-Sinogram\n");
    fprintf (theOUTfile, "process status := Reconstructed\n");
    //t    fprintf (theOUTfile, "!number format := unsigned short\n");
    fprintf (theOUTfile, "!number format := float\n");
    fprintf (theOUTfile, "!number of bytes per pixel := 2\n");
    fprintf (theOUTfile, "number of dimensions := 3\n");
    fprintf (theOUTfile, "matrix axis label [1] := x\n");
    fprintf (theOUTfile, "!matrix size [1] := %i\n",n_bin);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [1] := %f\n",(float)(d_FOV/(n_bin-1)));
    
    fprintf (theOUTfile, "matrix axis label [2] := y\n");
    fprintf (theOUTfile, "!matrix size [2] := %i\n",n_ang);
    
    fprintf (theOUTfile, "scaling factor (degree/pixel) [2] := %f\n",(float)(360./(n_ang-1)));
    
    
    fprintf (theOUTfile, "matrix axis label [3] := z\n");
    fprintf (theOUTfile, "!matrix size [3] := %i\n",n_planes*n_planes);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [3] := %f\n",(float)(d_axial/(n_planes-1)));
    
    fprintf (theOUTfile, "number of slices := %i\n",n_planes*n_planes);
    fprintf (theOUTfile, "number of time frames := 1\n");
    fprintf (theOUTfile, "image scaling factor[1] := 1\n");
    fprintf (theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf (theOUTfile, "quantification units := 1\n");
    fprintf (theOUTfile, "!END OF INTERFILE := \n");
    
    fclose(theOUTfile);
    
    //(size_t)(n_bin*n_ang*n_planes*n_planes);
    
  }else{
    
    strcpy(name_hv, fileOUTname);
    strcpy(name_v,fileOUTname);
    
    strcat(name_hv, ".hs"); // STIR extension: .hs .s
    strcat(name_v, ".s");
    
    theOUTfile=fopen(name_hv, "w");
    
    fprintf (theOUTfile, "!INTERFILE  := \n");
    fprintf (theOUTfile, "name of data file := %s.s\n",fileOUTname);
    fprintf (theOUTfile, "!GENERAL DATA := \n");
    fprintf (theOUTfile, "!GENERAL IMAGE DATA :=\n");
    fprintf (theOUTfile, "!type of data := PET\n");
    //    fprintf (theOUTfile, "!version of keys := 3.3\n");     STIR format is not 3.3 (almost but not completely), ERROR in STIR if it is not removed
    //    fprintf (theOUTfile, "!data offset in bytes := 0\n");
    fprintf (theOUTfile, "imagedata byte order := littleendian\n");
    fprintf (theOUTfile, "!PET STUDY (General) :=\n");
    fprintf (theOUTfile, "!PET data type := Emission\n");
    fprintf (theOUTfile, "applied corrections := {arc correction}\n");        // {none}\n");
    //    fprintf (theOUTfile, "process status := Reconstructed\n");
    fprintf (theOUTfile, "!number format := unsigned integer\n");
    fprintf (theOUTfile, "!number of bytes per pixel := 2\n");

    fprintf (theOUTfile, "number of dimensions := 4\n");
    fprintf (theOUTfile, "matrix axis label [4] := segment\n");
    fprintf (theOUTfile, "!matrix size [4] := %i\n",MRDiff*2 + 1);
    //    fprintf (theOUTfile, "scaling factor (mm/pixel) [1] := %f\n",(float)(d_FOV/(n_bin-1)));
    fprintf (theOUTfile, "matrix axis label [3] := axial coordinate\n");
    fprintf (theOUTfile, "!matrix size [3] := { ");
    if (MRDiff==0) 
      {
	fprintf (theOUTfile, "%i}\n", n_planes);
      }else{
	for(int m=n_planes-MRDiff;m<=n_planes;m++)  fprintf (theOUTfile, "%i,", m);
	for(int m=n_planes-1;m>n_planes-MRDiff;m--)  fprintf (theOUTfile, "%i,", m);
	fprintf (theOUTfile, "%i}\n", n_planes-MRDiff);
      }
    fprintf (theOUTfile, "matrix axis label [2] := view\n");
    fprintf (theOUTfile, "!matrix size [2] := %i\n",n_ang);
    fprintf (theOUTfile, "matrix axis label [1] := tangential coordinate\n");
    fprintf (theOUTfile, "!matrix size [1] := %i\n",n_bin);   

    fprintf (theOUTfile, "minimum ring difference per segment := {");     // TO DO :  add SPAN (MRDiff per seg. variable)
    fprintf (theOUTfile, "%i", -MRDiff); 
    for(int m=-MRDiff+1;m<=MRDiff;m++)  fprintf (theOUTfile, ",%i", m);
    fprintf (theOUTfile, "}\n");
    fprintf (theOUTfile, "maximum ring difference per segment := {");     // TO DO :  add SPAN (MRDiff per seg. variable)
    fprintf (theOUTfile, "%i", -MRDiff); 
    for(int m=-MRDiff+1;m<=MRDiff;m++)  fprintf (theOUTfile, ",%i", m);
    fprintf (theOUTfile, "}\n");

    fprintf (theOUTfile, "inner ring diameter (cm) := %f\n", d_FOV/10);   // STIR Required parameter, now assigned to FOV (not detectors)
    fprintf (theOUTfile, "average depth of interaction (cm) := 0.0001\n");
    fprintf (theOUTfile, "default bin size (cm) := %f\n",0.1*((float)d_FOV/((float)n_bin-1.0)) );
    fprintf (theOUTfile, "number of rings := %i\n",n_planes );    
    fprintf (theOUTfile, "distance between rings (cm) := %f\n", 0.1*(float)(d_axial/(n_planes-1.0)) );  // Axial pixel dimension
   
    fprintf (theOUTfile, "number of detectors per ring := %i\n",n_ang*2 );  
   //    fprintf (theOUTfile, "number of slices := %i\n",n_planes*n_planes);
    fprintf (theOUTfile, "number of time frames := 1\n");
    fprintf (theOUTfile, "image scaling factor[1] := 1\n");
    fprintf (theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf (theOUTfile, "quantification units := 1\n");
    fprintf (theOUTfile, "!END OF INTERFILE := \n");
    
    fclose(theOUTfile);

  }
  sino_line = (SINO_TYPE*) malloc( n_bin*n_ang*TOTaxial_pl*sizeof(SINO_TYPE));
  
  long unsigned int cont=0;
  int i,j,k;
  
  for(k=0;k<TOTaxial_pl;k++){
    for(j=0;j<n_ang;j++){
      for(i=0;i<n_bin;i++){
	sino_line[cont]=sino3D[i][j][k];
	cont++;
      }}}
  
  theOUTfile=fopen(name_v, "wb");
  
  //cout << 4096*sizeof(SINO_TYPE) << endl;
  fwrite(sino_line,1,n_bin*n_ang*TOTaxial_pl*sizeof(SINO_TYPE), theOUTfile);
  fclose(theOUTfile);
  
  free(sino_line);
  
}




void free_sino3D() {       // liberar memoria
 int i,j;
		
	for(i=0;i<n_bin;i++){
		for(j=0;j<n_ang;j++){
			free(sino3D[i][j]);
		
		}
		free(sino3D[i]);
	
	}
	free(sino3D);

}


