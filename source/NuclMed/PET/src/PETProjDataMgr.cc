#define _USE_MATH_DEFINES
#include "PETProjDataMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "PETVerbosity.hh"
#include <iomanip>
#include <iostream>
#include <math.h>
#include <cmath>

//----------------------------------------------------------------------
PETProjDataMgr* PETProjDataMgr::theInstance = 0;

//----------------------------------------------------------------------
PETProjDataMgr* PETProjDataMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new PETProjDataMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
PETProjDataMgr::PETProjDataMgr()
{

  /*
// ARGUMENTS:
" cout << " -------------------------- \n"
" Arguments convention: \n"
"     -a Axial FOV (mm), <theDist_axial=100.0> \n"
"     -d Diameter Transaxial FOV (mm), <theD_FOV=300.0> \n"
"     -i Type of the input file (by default: 0 = Arce_binary), <typeINfile=0> \n"
"     -j Offset X direction (mm), <theOffsetX=0.0> \n"
" \n"
"     -m Maximum ring difference (by default: -1 = n_planes), <theMRDiff> \n"
"     -n Name of output file, <theFileOUTname> \n"
"     -p Axial number of planes, <theN_planes> \n"
"     -r Number of bins, \"distancias\", <theN_bin> \n"
  // -s Span                                         TO DO:span !!!!!
"     -t Number of angular views, \"direcciones\", <theN_ang> \n"
"     -v Verbosity (by default: 0=silent, 3=debug), <verbos> \n"
"     -x Maximum number of coincidences to be stored (by default: -1 = no limit), <theMax_Coinci> \n"
"     -o Output type (by default: 0 = mcc Interfile, 1 = STIR Interfile), <theOutType> \n"

" \n"
" PET Reconstruction. CIEMAT 2009-11 \n"
" mario.canadas@ciemat.es \n"
 " -------------------------- \n";
  */

  theDistAxial = GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:DistAxial",100.0);
  theD_FOV = GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:DiameterTranFOV",300.0);
  theN_planes = G4int(GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:Nplanes",48));
  theN_bin = G4int(GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:Nbin",128));
  theN_ang = G4int(GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:NangViews",128));
  theMRD = G4int(GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:MaxRingDiff",-1));
  // toDo:  theSpan = G4int(GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:Span",1));

  theOffsetX = GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:OffsetX",0.0);
  theMaxCoinci = G4int(GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:MaxCoinci",-1));

  theOutFormat = G4int(GmParameterMgr::GetInstance()->GetNumericValue("PET:ProjData:OutFormat", 1));
  theFileOUTname = GmParameterMgr::GetInstance()->GetStringValue("PET:ProjData:Filename","default_sino3D");
  
  G4int i,j,k;
  if (theMRD==-1) theMRD=theN_planes-1;
  

  theTOTaxial_pl=theN_planes*theN_planes;
  if (theOutFormat==1) theTOTaxial_pl= (2*theN_planes-1 - theMRD)*theMRD + theN_planes;  // total number of Axial planes (segments*planes) in STIR format
  
  /*--- Initialize sino3D ---*/
  theSino3D = new SINO_TYPE**[theN_bin];
  for(i=0;i<theN_bin;i++){
    theSino3D[i] = new SINO_TYPE*[theN_ang];
    for(j=0;j<theN_ang;j++){
      theSino3D[i][j] = new SINO_TYPE[theTOTaxial_pl];  /// ! If theOutFormat==1 (STIR output):Matrix size depends on the MAX_Ring_Difference
      for(k=0;k<theTOTaxial_pl;k++){
	theSino3D[i][j][k]=0;
      }
    }
  }
 
  theN_CoinciVal=0;
  theN_CoinciTot=0;
  //theOutputType = "pet";
}

//-----------------------------------------------------------------------
void PETProjDataMgr::AddEvent( const G4ThreeVector& pos1 , const G4ThreeVector& pos2)
{
  G4int z1_i, z2_i; 
    //for discretization on the crystal: int x1_i, x2_i, y1_i, y2_i;
    
  G4double z1_abs=pos1.z()+theDistAxial/2;
  G4double z2_abs=pos2.z()+theDistAxial/2;
  G4double a, b, phi, dis;
  G4int phi_i, dis_i;
  G4int ring_diff;

  G4double _PI=2*asin(double(1));

  theN_CoinciTot++;

  z1_i=(G4int)(theN_planes* z1_abs/theDistAxial);  //round --> n_planes+1 ...
  z2_i=(G4int)(theN_planes* z2_abs/theDistAxial);

  // control; if z_i out of range: return

  if ( (pos1.x()==pos2.x()) && (pos1.y()==pos2.y()) ) {
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) {
      G4cout << "PETProjDataMgr::AddEvent:WARNING! Event_1 == Event_2  ; x= " << pos2.x() << ", y= " << pos2.y() << ", z= " << pos2.z() << G4endl;
    }
#endif 
    return;
  }

  if ( (z1_i<0) || (z2_i<0) || (z1_i>= theN_planes) || (z2_i>= theN_planes) ) {
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) {
      G4cout << "PETProjDataMgr::AddEvent:WARNING! Event out of bounds (Axial): x1= " << pos1.x() << ", y1= " << pos1.y() << ", z1= " << pos1.z() << " ; x2= " << pos2.x() << ", y2= " << pos2.y() << ", z2= " << pos2.z() << G4endl;
    }
#endif 
    return;
  }
  
  ring_diff = (G4int)fabs(double(z1_i-z2_i));

  // max ring difference; control:
  if (ring_diff > theMRD) {
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) {
      G4cout << "PETProjDataMgr::AddEvent:WARNING! Event out of bounds (Max. Ring Diff.): x1= " << pos1.x() << ", y1= " << pos1.y() << ", z1= " << pos1.z() << " ; x2= " << pos2.x() << ", y2= " << pos2.y() << ", z2= " << pos2.z() << G4endl;
    }
#endif 
    return;
  }

  a=(G4double)(pos2.y()- pos1.y());
  b=(G4double)(pos2.x()- pos1.x());

  if (a==0.0){
    phi=_PI*0.5;
  }
  else{
    phi=atan(b/a);
  }

  if (phi<0) phi = phi +_PI;

  dis=pos1.x()*cos(phi) - pos1.y()*sin(phi);

  // control; transaxial FOV
  if ( fabs(dis) > theD_FOV*0.5 ) {
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) {
      G4cout << "PETProjDataMgr::AddEvent:WARNING! Event out of bounds (Transaxial): x1= " << pos1.x() << ", y1= " << pos1.y() << ", z1= " << pos1.z() << " ; x2= " << pos2.x() << ", y2= " << pos2.y() << ", z2= " << pos2.z() << G4endl;
    }
#endif 
    return;
  }

  dis = dis + theD_FOV*0.5;

  // discret values:
  phi_i=(G4int)round( (G4double)(theN_ang-1)*phi/_PI );
  dis_i=(G4int)round( (G4double)(theN_bin-1)*dis/(G4double)theD_FOV );

  if ((phi_i>=theN_ang) || (dis_i>=theN_bin))  return;  // only possible "=" because 'round' check it..
  
  // OLD: (SRRB included) sino3D[dis_i][phi_i][ (z1_i+z2_i)+ring_diff*(n_planes-1) ]++;
  
  G4int Zpos;

  if (theOutFormat==0) {
    Zpos = (z1_i*theN_planes + z2_i);
  }
  else{
    
    if (z1_i>=z2_i) {  // SIN Max Ring_Diff: Zpos= ( ((n_planes-ring_diff)*(n_planes-1-ring_diff))/2 + z2_i );
      
      Zpos= ( ((2*theN_planes-1 - theMRD - ring_diff)*(theMRD - ring_diff))/2 + z2_i);
      
    }else{
      Zpos= ( (theTOTaxial_pl) - ((2*theN_planes-1 - theMRD - ring_diff +1)*(theMRD - ring_diff +1))/2  + z1_i );	
      
    }
  }
 
  theSino3D[dis_i][phi_i][ Zpos ]++; 
  theN_CoinciVal++;

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) {
    G4cout << "PETProjDataMgr::AddEvent: x1= " << pos1.x() << ", y1= " << pos1.y() << ", z1= " << pos1.z() << " ; x2= " << pos2.x() << ", y2= " << pos2.y() << ", z2= " << pos2.z() << G4endl;
    G4cout << "PETProjDataMgr::AddEvent: Sinogram pos.: distance(s)= " << dis_i << ", angular view(phi)= " << phi_i << " ; Zpos=" << Zpos <<"; Segment (Ring diff.) = " << ring_diff << G4endl;
  }
#endif


}

//-----------------------------------------------------------------------
PETProjDataMgr::~PETProjDataMgr()
{
 G4int i,j;
		
	for(i=0;i<theN_bin;i++){
		for(j=0;j<theN_ang;j++){
			free(theSino3D[i][j]);
		
		}
		free(theSino3D[i]);
	
	}
	free(theSino3D);

}

/*  TO DO: call lm_to_sino3D program
//-----------------------------------------------------------------------
void PETIOMgr::ReadFile()
{
  if( !theFileIn ) OpenFileIn();

  PETOutput po;
  G4bool bEof;
  for(;;) {
    po = ReadEvent( bEof );
    // theFileIn->read(reinterpret_cast<char *>(&po),sizeof(PetOutput)); 
    if(bDumpCout) PrintEvent(" PETOutput: ", po, bCartesian);
    if( bEof ) break;
  }
}
//-----------------------------------------------------------------------
PETOutput PETProjDataMgr::ReadEvent( G4bool& bEof )
{
  if( theFileIn == 0 ){
    G4Exception("PETIOMgr::ReadEvent",
		"Wrong argument",
		FatalErrorInArgument,
		"File not opened, call OpenFileIn() first ");
  }

  PETOutput po;
  fread (&po, sizeof(struct PETOutput),1,theFileIn);
  if ( feof( theFileIn ) ) {
    bEof = TRUE;
  } else {
    bEof = FALSE;
  }

  return po; 

}
*/


//-----------------------------------------------------------------------
void PETProjDataMgr::WriteInterfile()
{
  
  char name_hv[512];
  char name_v[512];
  
  if (theOutFormat==0){
   
    strcpy(name_hv, theFileOUTname);
    strcpy(name_v,theFileOUTname);
    strcat(name_hv, ".hv");
    strcat(name_v, ".v");
    
    theOUTfile=fopen(name_hv, "w");
    
    fprintf (theOUTfile, "!INTERFILE  := \n");
    fprintf (theOUTfile, "name of data file := %s\n", name_v);
    fprintf (theOUTfile, "!GENERAL DATA := \n");
    fprintf (theOUTfile, "!GENERAL IMAGE DATA :=\n");
    fprintf (theOUTfile, "!type of data := tomographic\n");
    fprintf (theOUTfile, "!version of keys := 3.3\n");
    fprintf (theOUTfile, "!data offset in bytes := 0\n");
    fprintf (theOUTfile, "imagedata byte order := littleendian\n");
    fprintf (theOUTfile, "!PET STUDY (General) :=\n");
    fprintf (theOUTfile, "!PET data type := 3D-Sinogram\n");
    fprintf (theOUTfile, "process status := Reconstructed\n");
    fprintf (theOUTfile, "!number format := unsigned short\n");
    fprintf (theOUTfile, "!number of bytes per pixel := 2\n");
    fprintf (theOUTfile, "number of dimensions := 3\n");
    fprintf (theOUTfile, "matrix axis label [1] := x\n");
    fprintf (theOUTfile, "!matrix size [1] := %i\n",theN_bin);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [1] := %f\n",(float)(theD_FOV/(theN_bin-1.0)));
    
    fprintf (theOUTfile, "matrix axis label [2] := y\n");
    fprintf (theOUTfile, "!matrix size [2] := %i\n",theN_ang);
    
    fprintf (theOUTfile, "scaling factor (CLHEP::degree/pixel) [2] := %f\n",(float)(360./theN_ang));
    
    fprintf (theOUTfile, "matrix axis label [3] := z\n");
    fprintf (theOUTfile, "!matrix size [3] := %i\n",theN_planes*theN_planes);
    fprintf (theOUTfile, "scaling factor (mm/pixel) [3] := %f\n",(float)(theDistAxial/(theN_planes-1.0)));
    
    fprintf (theOUTfile, "number of slices := %i\n",theN_planes*theN_planes);
    fprintf (theOUTfile, "number of time frames := 1\n");
    fprintf (theOUTfile, "image scaling factor[1] := 1\n");
    fprintf (theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf (theOUTfile, "quantification units := 1\n");
    fprintf (theOUTfile, "!END OF INTERFILE := \n");
    
    fclose(theOUTfile);
    //(size_t)(n_bin*n_ang*n_planes*n_planes);
    
  }else{
    
    strcpy(name_hv, theFileOUTname);
    strcpy(name_v,theFileOUTname);
    
    strcat(name_hv, ".hs"); // STIR extension: .hs .s
    strcat(name_v, ".s");
    
    theOUTfile=fopen(name_hv, "w");
    
    fprintf (theOUTfile, "!INTERFILE  := \n");
    fprintf (theOUTfile, "name of data file := %s\n",name_v);
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
    fprintf (theOUTfile, "!matrix size [4] := %i\n",theMRD*2 + 1);
    //    fprintf (theOUTfile, "scaling factor (mm/pixel) [1] := %f\n",(float)(d_FOV/(n_bin-1)));
    fprintf (theOUTfile, "matrix axis label [3] := axial coordinate\n");
    fprintf (theOUTfile, "!matrix size [3] := { ");
    if (theMRD==0) 
      {
	fprintf (theOUTfile, "%i}\n", theN_planes);
      }else{
	for(int m=theN_planes-theMRD;m<=theN_planes;m++)  fprintf (theOUTfile, "%i,", m);
	for(int m=theN_planes-1;m>theN_planes-theMRD;m--)  fprintf (theOUTfile, "%i,", m);
	fprintf (theOUTfile, "%i}\n", theN_planes-theMRD);
      }
    fprintf (theOUTfile, "matrix axis label [2] := view\n");
    fprintf (theOUTfile, "!matrix size [2] := %i\n",theN_ang);
    fprintf (theOUTfile, "matrix axis label [1] := tangential coordinate\n");
    fprintf (theOUTfile, "!matrix size [1] := %i\n",theN_bin);   

    fprintf (theOUTfile, "minimum ring difference per segment := {");     // TO DO :  add SPAN (MRDiff per seg. variable)
    fprintf (theOUTfile, "%i", -theMRD); 
    for(int m=-theMRD+1;m<=theMRD;m++)  fprintf (theOUTfile, ",%i", m);
    fprintf (theOUTfile, "}\n");
    fprintf (theOUTfile, "maximum ring difference per segment := {");     // TO DO :  add SPAN (MRDiff per seg. variable)
    fprintf (theOUTfile, "%i", -theMRD); 
    for(int m=-theMRD+1;m<=theMRD;m++)  fprintf (theOUTfile, ",%i", m);
    fprintf (theOUTfile, "}\n");

    fprintf (theOUTfile, "inner ring diameter (CLHEP::cm) := %f\n", theD_FOV/10);   // STIR Required parameter, now assigned to FOV (not detectors)
    fprintf (theOUTfile, "average depth of interaction (CLHEP::cm) := 0.0001\n");
    fprintf (theOUTfile, "default bin size (CLHEP::cm) := %f\n",0.1*((float)theD_FOV/((float)theN_bin-1.0)) );
    fprintf (theOUTfile, "number of rings := %i\n",theN_planes );    
    fprintf (theOUTfile, "distance between rings (CLHEP::cm) := %f\n", 0.1*((float)theDistAxial/(float)(theN_planes-1)) );  // Axial pixel dimension
   
    fprintf (theOUTfile, "number of detectors per ring := %i\n",theN_ang*2 );  
   //    fprintf (theOUTfile, "number of slices := %i\n",n_planes*n_planes);
    fprintf (theOUTfile, "number of time frames := 1\n");
    fprintf (theOUTfile, "image scaling factor[1] := 1\n");
    fprintf (theOUTfile, "data offset in bytes[1] := 0\n");
    fprintf (theOUTfile, "quantification units := 1\n");
    fprintf (theOUTfile, "!END OF INTERFILE := \n");
    
    fclose(theOUTfile);

  }
  theSino_line = (SINO_TYPE*) malloc( theN_bin*theN_ang*theTOTaxial_pl*sizeof(SINO_TYPE));
  
  long unsigned int cont=0;
  G4int i,j,k;
  
  for(k=0;k<theTOTaxial_pl;k++){
    for(j=0;j<theN_ang;j++){
      for(i=0;i<theN_bin;i++){
	theSino_line[cont]=theSino3D[i][j][k];
	cont++;
      }}}
  
  theOUTfile=fopen(name_v, "wb");
  
  //cout << 4096*sizeof(SINO_TYPE) << endl;
  fwrite(theSino_line,1,theN_bin*theN_ang*theTOTaxial_pl*sizeof(SINO_TYPE), theOUTfile);
  fclose(theOUTfile);

#ifndef GAMOS_NO_VERBOSE
  G4cout << "PETProjDataMgr::WriteInerfile: File name: " << theFileOUTname << G4endl;
  G4cout << "PETProjDataMgr::WriteInterfile: Planes = " << theN_planes << "; bins = " << theN_bin << "; ang_views = " << theN_ang << G4endl;
  G4cout << "PETProjDataMgr::WriteInterfile: Dimensions (mm): Transaxial FOV = " << theD_FOV << "; Axial FOV = " << theDistAxial << " ; Transaxial_pix = " << theD_FOV/(theN_bin-1) <<"; Plane width = " << theDistAxial/(theN_planes-1) << G4endl; // Image Axial Pixel(ssrb) == 0.5*(Plane_Width);
  G4cout << "... " << G4endl;

 G4cout << "PETProjDataMgr::WriteInterfile: Total  Coinci: " << theN_CoinciTot << G4endl; 
 G4cout << "PETProjDataMgr::WriteInterfile: Sino3D Coinci: " << theN_CoinciVal << G4endl;  
#endif
 
}
