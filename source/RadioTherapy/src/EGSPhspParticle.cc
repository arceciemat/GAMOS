/*==================================================================

      PLOTPHSP UTILITIES LIBRARY

      Juan Diego Soler Pulido
      email: juandiego.soler@ciemat.es
      Madrid, Spain
      December 2006
      Daniel P. A.
      05/2008
      Pedro Arce 
      11/2009
 =================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>

#include "EGSPhspParticle.hh"
#include "G4PhysicalConstants.hh"
#include "G4ios.hh"

EGSPhspParticle::EGSPhspParticle(FILE* , std::string mode ) 
{
  //--- header and records have the same length: 7 4-byte words (MODE0) or 8 (MODE2, ZLAST added)
  theRecordLength = ( mode.size() > 4 && mode[4] == '2' ) ? 32 : 28;
  theFilePointer = theRecordLength; // = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float)+3 for MODE0
  theCharge = 0;
  //  theFilePointer = 5*sizeof(char)+2*sizeof(int)+3*sizeof(float);
}


EGSPhspParticle::~EGSPhspParticle()
{
}


//-------------------------------------------------------------------
int EGSPhspParticle::RefreshParticle(FILE* file, long number)
{
  float aux;
  int reset_phsp;
  int a1;

  /*  fread(&theLatch,sizeof(unsigned int),1,file);
  fread(&theKineticEnergy,sizeof(float),1,file);
  G4cout << " EGSPhspParticle READ theLatch " << theLatch << " theKineticEnergy " << theKineticEnergy << std::endl; //GDEB
  */  
  theParticleNumber = number;
  (void) fseek(file,theFilePointer,SEEK_SET);
  a1 = fread(&theLatch,sizeof(unsigned int),1,file);
  //    G4cout << " EGSPhspParticle READ theLatch " << theLatch << std::endl; //GDEB
  a1 &= fread(&theKineticEnergy,sizeof(float),1,file);
  a1 &= fread(&thePosX,sizeof(float),1,file);
  thePosX *= 10.;
  a1 &= fread(&thePosY,sizeof(float),1,file);
  thePosY *= 10.;
  a1 &= fread(&theDirU,sizeof(float),1,file);
  a1 &= fread(&theDirV,sizeof(float),1,file);
  a1 &= fread(&theWeight,sizeof(float),1,file);
  
  if(a1){
    reset_phsp = false;
    //--- next record (theFilePointer was never advanced: the first record was always read)
    theFilePointer += theRecordLength;
    // obtain npass from theLatch
    
    if ((theLatch & 0x80000000) != 0){
      // printf("Multiple crosser\n");
      theNPass=1;
      theLatch &= 0x7FFFFFFF;
    } else
      theNPass=0;
    
    // obtain q from theLatch
    if((theLatch & 0x40000000) != 0){
      theCharge = -1;
      theLatch &= 0xBFFFFFFF;
    } else if ((theLatch & 0x20000000) != 0){
      theCharge = +1;
      theLatch &= 0xDFFFFFFF;
    } else {
      theCharge = 0;
    }
    //--- charge of this record must be known before subtracting the electron mass (it was taken from the previous record)
    theKineticEnergy = fabs(theKineticEnergy);
    if( theCharge != 0 ) {
      theKineticEnergy -= CLHEP::electron_mass_c2;
    }
    if(!CheckIntegrity(theParticleNumber)){
      printf("Phase space problem on record %d!\n",int(theParticleNumber));
      return true;
    }
    // calculate theDirW
    aux = theDirU*theDirU+theDirV*theDirV;
    if (aux<=1.0)
      theDirW = (float) sqrt((float)(1.0-aux));
    else{
      aux= (float)sqrt((float)aux);
      theDirU/=aux;
      theDirV/=aux;
      theDirW=0.0f;
    }
    if(theDirW < 0.0f)
      theDirW *= -1.0f;
    //long step = sizeof(unsigned int) + 6*sizeof(float);
    //    theFilePointer += step;
    
    int MAX_PRINT = 10;
    //-    if( theKineticEnergy > 0.510 && theKineticEnergy < 0.512 ) G4cout << " ENERGYPEAK " << theKineticEnergy << std::endl; //GDEB
    if(theParticleNumber <= MAX_PRINT){
      G4cout << "# " <<theParticleNumber;
      G4cout << " Q " << theCharge;
      G4cout << " E " << theKineticEnergy;
      G4cout << " x " << thePosX;
      G4cout << " y " << thePosY << std::endl;
      G4cout << " u " << theDirU;
      G4cout << " v " << theDirV;
      G4cout << " w " << theDirW;
      G4cout << " W " << theWeight;
      G4cout << " latch " << theLatch;
      G4cout << " NPass " << theNPass <<  std::endl;
    }
  }else{
    //    G4cerr << "!!! EGSPhspParticle::RefreshParticle: unsuccesful fread at record " << theFilePointer/step << std::endl;
    G4cerr << "End of file" << std::endl;
    
    //     if(fseek(file,(long)0,SEEK_SET) != 0){
    //       printf("In phsp_rw: phsp_read_particle() file could not be reset\n");
    //       exit(-1);
    //     }
    //     eof = eof + 1; //increase the number of restarts
    reset_phsp = true;
    //     theFilePointer += sizeof(long int)+6*sizeof(float);
    //    return reset_phsp;
  }
  
  return reset_phsp;
}


void EGSPhspParticle::write_EGSPhspParticle(FILE* outphspfile)
{
  //char muse;
  
  //  (void) fseek(outphspfile,theFilePointer_out,SEEK_SET);
  /*  
  long int aux_theLatch;
  if (q == -1)
    aux_theLatch |= 0x40000000; // set bit 30
  else if (q == 0)
    aux_theLatch &= 0x9FFFFFFF; // clear bit 30,29
  else {
    aux_theLatch &= 0xBFFFFFFF; 
    aux_theLatch |= 0x20000000; // set 29, clr 30
  }
  
  if (npass == 0)
    aux_theLatch &= 0x7FFFFFFF;  // clear bit 31
  else
    aux_theLatch |= 0x80000000;  // set bit 31
  */
  
  fwrite(&theLatch,sizeof(unsigned int),1,outphspfile);
  //  G4cout << " EGSPhspParticle WRITE theLatch " << theLatch << std::endl; //GDEB
  fwrite(&theKineticEnergy,sizeof(float),1,outphspfile);
  fwrite(&thePosX,sizeof(float),1,outphspfile);
  fwrite(&thePosY,sizeof(float),1,outphspfile);
  fwrite(&theDirU,sizeof(float),1,outphspfile);
  fwrite(&theDirV,sizeof(float),1,outphspfile);
  fwrite(&theWeight,sizeof(float),1,outphspfile);
  //  theFilePointer_out += sizeof(long int) + 6*sizeof(float);
}


int EGSPhspParticle::CheckIntegrity(int i)
{
  char str[50];

  sprintf(str, "%e", thePosX);
  if (strcmp(str,"nan")==0) 
    {
      printf("x coord in record number %d\n",i);
      return(false);
    }
  sprintf(str,"%e",thePosY);
  if (std::strcmp(str,"nan")==0) 
    {
      printf("y coord in record number %d\n",i);
      return(false);
    }
  sprintf(str,"%e",theDirU);
  if (std::strcmp(str,"nan")==0) 
    {
      printf("u direction in record number %d\n",i);
      return(false);
    }
  sprintf(str,"%e",theDirV);
  if (std::strcmp(str,"nan")==0) 
    {
      printf("v direction in record number %d\n",i);
      return(false);
    }
  sprintf(str,"%i",int(theLatch));
  if (std::strcmp(str,"nan")==0) 
    {
      printf("theLatch variable in record number %d\n",i);
      return(false);
    }
  sprintf(str,"%e",theDirW);
  if (std::strcmp(str,"nan")==0) 
    {
      printf("weight in record number %d\n",i);
      return(false);
    }
  sprintf(str,"%e",theKineticEnergy);
  if (std::strcmp(str,"nan")==0) 
    {
      printf("energy in record number %d\n",i);
      return(false);
    }

  return true;
}
