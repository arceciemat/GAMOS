/*-------------------------------------------------------

     PET Reconstruction.  
     Gamos Collaboration - CIEMAT 2009-11
                        
     --  sumProjdata.cc  --                      

     mario.canadas@ciemat.es

-------------------------------------------------------*/
/*------------------------------------------------------
 
			   
	 Description :
   
	 This program sums a set of projection data files (sino 3D) files. 
	 A file_list must include the name of the input files (without extension: the program searchs for *.s (first) and *.v files).
   
         Type of data: Ciemat projData (Interfile, .hv .v) and STIR projection data format (Interfile 3.3, .hs .s) \n" 

         Input data: FILE_WITH_LIST_OF_PROJDATA_FILES   FILE_OUTPUT. 

    It is useful to merge several simulation output files (under the same conditions but using differet seeds) in a global file which can feed the reconstruction software.
    
--------------------------------------------------------*/

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "sumProjdata.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"



int main(int argc,char *argv[])
{

  /*
for (int i = 1; i < argc; i++) {
  
  // Check for a switch (leading "-"). 
  
  if (argv[i][0] == '-') {
    
    // Use the next character to decide what to do. 
    
    switch (argv[i][1]) {
      
      
    case 'i':	Ni = atoi(argv[++i]);
      break;
      
    case 'e':	Ne = atoi(argv[++i]);
      break;
      
    case 'n':   fileOUTname=argv[++i];
      break;
      
      //case 'o':	oType = atoi(argv[++i]);
      //break;
      
      break;
      
    }
  }
  else{
    fileINname=argv[i];
  }               
}
 
 */


 
 if(argc != 3) { 
   G4Exception("sumProjData",
	       "Wrong argument",
	       FatalErrorInArgument,
	       "YOU MUST SUPPLY TWO ARGUMENTS: FILE_WITH_LIST_OF_PROJDATA_FILES FILE_OUTPUT_(without ext.)");
  }

  G4String fileListName = argv[1];
  fileOUTname = argv[2];
  // FILE* fout  = fopen(fileOUTname,"wb");

  std::vector<G4String> wl;
  GmFileIn finlis = GmFileIn::GetInstance(fileListName);

  
  G4int ii;
  char theFilename[255];
  SINO_TYPE * sino_tmp;
  unsigned long int posNEW, j;
  
  for( ii = 0;; ii++){
    if( ! finlis.GetWordsInLine(wl) ) break;      
    G4cout << "### sumProjdata: reading projection data file " << wl[0] << G4endl;
    if( ii == 0 ) {

      // -- Init File:
      FILE *InitFile;
      sprintf(theFilename,"%s.s",wl[0].c_str());
      InitFile=fopen(theFilename, "rb"); 
      
      if (InitFile==NULL){
	sprintf(theFilename,"%s.v",wl[0].c_str());
	InitFile=fopen(theFilename, "rb");
	oType=0;
	if (InitFile==NULL){
	  G4Exception("sumProjData",
		      "Wrong argument",
		      FatalErrorInArgument, 
		      "!!! FATAL ERROR: INPUT FILE NOT FOUND");
	}
      }
      
      fileINname = wl[0];
      fseek(InitFile, 0, SEEK_END); // Locates end file
      sizeTOT = ftell(InitFile); // dumps file position (bytes) // printf("Size: %d\n", fileSize);
      rewind(InitFile);
      posTOT=sizeTOT/sizeof(SINO_TYPE);
      
      sino_tmp = (SINO_TYPE*) malloc (sizeTOT);
      sino_total = (SINO_TYPE*) malloc (sizeTOT);
      fread(sino_total,1,sizeTOT, InitFile);
      
      fclose(InitFile);
      // -- End Init file (posTOT stored)
    }else{
      
      oType==1 ? sprintf(theFilename,"%s.s",wl[0].c_str()) : sprintf(theFilename,"%s.v",wl[0].c_str());
      
      posNEW=read_data(theFilename, sino_tmp);
      if (posNEW==posTOT){
	for(j=0;j<posTOT;j++){
	  sino_total[j]=sino_total[j]+sino_tmp[j];	
	}
      }else{
	G4Exception("sumProjData",
		    "Wrong argument",
		    FatalErrorInArgument,
		    "FATAL ERROR!!  FILE HAS DIFFERENT SIZE!");
      }
    }
    
  }

 
/*
  char *pch, *last;
  // DEBUG: printf ("Splitting string \"%s\" into tokens:\n",fileINname);
  pch = strtok(fileINname,"_");
  while (pch != NULL)
  {
  //printf("%s\n",pch);
  last=pch;
  pch = strtok(NULL, "_");
  }
*/
 
//for(i=Ni+1;i<=Ne;i++){
// }


 free(sino_tmp);

 write_sino3D(sino_total);

 free(sino_total);
 return 0;
} //end  Main


// ---------------------------------------------------------------
unsigned long int read_data(const char* theName, SINO_TYPE* theSino){
    
  FILE *theINfile;
  size_t newfileSize;

  theINfile=fopen(theName, "rb");
  if (theINfile==NULL)
  {
    cout << "Error opening file:" << theName << "\n";
    exit (-1);
  }
  
    fseek(theINfile, 0, SEEK_END); // Se posiciona al final del archivo
    newfileSize = ftell(theINfile); // Devuelve la posición actual del archivo (en bytes)
                               // printf("Tamaño: %d\n", fileSize);
    rewind(theINfile);
    
    
    fread(theSino,1,newfileSize, theINfile);
    fclose(theINfile);
    
    return ( newfileSize/sizeof(SINO_TYPE) );
    
  //free(sino_line);
  
}

// -----------------------------------------------------------------
void write_sino3D(SINO_TYPE* sino){
  
  char theFilenameOUT[255];
  char OneHeaderFile[255];
  char ext;                  // .s (STIR, oType==1) or .v (Ciemat, oType==0)
  string NEWline;
  FILE *theOUTfile;
  
  oType==1 ? ext='s' : ext='v';
  
  sprintf(theFilenameOUT,"%s.h%c",fileOUTname.c_str(),ext);
  sprintf(OneHeaderFile,"%s.h%c",fileINname.c_str(), ext);
  
  // -- copying header files (and replacing line with the ".s" filename):
  ifstream f1(OneHeaderFile);
  
  if( !f1.is_open())
    {
      cout << "Header file: " << OneHeaderFile << " failed to open\n";
      exit(-1);
    }
  ofstream f2(theFilenameOUT);
  
  while(getline(f1,NEWline)){
    
    if ( "name of data file :=" == NEWline.substr(0,20)){  // 20 chars + 1 (end)
      f2 << "name of data file := " << fileOUTname << "." << ext << "\n";
    }else{
      f2 << NEWline << "\n";     
    }
    
  }
  // -- END, copying header files 
  
  
  // writing ".s" binary file:
  sprintf(theFilenameOUT,"%s.%c",fileOUTname.c_str(), ext);
  
  theOUTfile=fopen(theFilenameOUT, "wb");
  fwrite(sino,1,posTOT*sizeof(SINO_TYPE), theOUTfile);
  
  fclose(theOUTfile);
  
}
