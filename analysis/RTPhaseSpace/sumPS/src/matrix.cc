/*==================================================================
                                                                 
      PLOTPHSP MATRIX LIBRARY  
 
      Juan Diego Soler Pulido
      email: juandiego.soler@ciemat.es
      Madrid, Spain
      December 2006

 =================================================================*/

#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>
#include "utilities.h"
using namespace std;

#include "matrix.h"

matrix::matrix(int p, int q){

  int i;
  rowSize = p;
  colSize = q;
  mat = new float*[p];
  
  for (i = 0; i < p; i++)            
	mat[i] = new float[q];
  
  for(int ii = 0; ii < rowSize; ii++)
    {
      for (int jj = 0; jj < colSize; jj++)
	{
	  mat[ii][jj]=0;
	}
    }
  
}

matrix::~matrix(){
}


void matrix::print_matrix(){

  printf("\n");
  for(int i = 0; i < rowSize; i++)
    {
      for (int j = 0; j < colSize; j++)
	{
	  printf("%f ", mat[i][j]);
        }
      printf("\n");
    }

}


void matrix::get_grid(float dx, float dy, float x1, float y1){

  xspacing = dx;
  yspacing = dy;
  xstart = x1;
  ystart = y1;

}


void matrix::asign_matrix(int k, int l, float value){

  mat[k][l] = value;

}


void matrix::addto_matrix(int k, int l, float value){

  if((k > 0 && l > 0) && (k < rowSize && l < colSize))
    {
      mat[k][l] = mat[k][l] + value;
    }

}


void matrix::save_matrix(string filename){

  FILE* file;
  float xaux;
  float yaux;
 

  file = fopen(filename.c_str(),"w");

  fprintf (file,"#  PHSP DATA FROM %s \n",filename.c_str());
  for (int i = 0; i < rowSize; i++)
    {
      for (int j = 0; j < colSize; j++)
	{
	  fprintf(file,"%f ",mat[i][j]);
        }
      fprintf (file,"\n"); 
    }

  fprintf (file,"#  GRID DATA \n");

  fprintf (file,"# x\n"); 

  for (int i = 0; i < colSize; i++)
    {
      xaux = xstart + i*xspacing;
      fprintf (file,"# %f\n", xaux ); 
    }

  fprintf (file,"# y\n");

  for (int j = 0; j < rowSize; j++)
    {
      yaux = -ystart + j*yspacing;
      fprintf (file,"# %f\n", yaux ); 
    }
  

  fclose(file);
  
}


void matrix::colorscale(string filename){

  float RGBMap[256][3];
  int i;
  FILE* file;

  for (i=0;i<32;i++)
    {
      RGBMap[i][0]=0.0;
      RGBMap[i][1]=0.0;
      RGBMap[i][2]=(33+i)/64.0;
    }
  for (i=32;i<96;i++)
    {
      RGBMap[i][0]=0.0;
      RGBMap[i][1]=1.0;
      RGBMap[i][2]=(i-31)/64.0;
    }
  for (i=96;i<160;i++)
    {
      RGBMap[i][0]=(i-95)/64.0;
      RGBMap[i][1]=1.0;
      RGBMap[i][2]=(1-(i-95)/64.0);      
    }
  for (i=160;i<224;i++)
    {
      RGBMap[i][0]=1.0;
      RGBMap[i][1]=(223-i)/64.0;
      RGBMap[i][2]=0.0;      
    }
  for (i=224;i<256;i++)
    {
      RGBMap[i][0]=(1-(i-223)/64.0);
      RGBMap[i][1]=0.0;
      RGBMap[i][2]=0.0;      
    }

  file = fopen(filename.c_str(),"w");

  fprintf (file,"# COLORSCALE \n");
  for (int k = 0; k < 256; k++)
    {
      for (int j = 0; j < 3; j++)
	{
	  fprintf(file,"%f ",RGBMap[k][j]);
        }
      fprintf (file,"\n"); 
    }

  fclose(file);

}


void matrix::plot_matrix(string filename, string colorscale){

#ifndef WIN32
  FILE* octaveplotfile;
  FILE* gnuplotfile;

  octaveplotfile = popen("octave -q","w");
  fprintf(octaveplotfile,"figure(1);");
  
  fprintf(octaveplotfile,"load %s \n",filename.c_str());
//  fprintf(octaveplotfile,"gset terminal x11;");
  fprintf(octaveplotfile,"load %s; ",colorscale.c_str());
  fprintf(octaveplotfile,"colormap(%s);",colorscale.c_str());
  fprintf(octaveplotfile,"imagesc(%s,1) \n",filename.c_str());
  fflush(octaveplotfile);
  fclose(octaveplotfile);

  gnuplotfile = popen("gnuplot -persist","w");
  fprintf(gnuplotfile,"set contour base %s \n",filename.c_str()); 
  fprintf(gnuplotfile,"set title 'Phase space %s' \n", filename.c_str());
  fprintf(gnuplotfile,"set xlabel 'x'\n");
  fprintf(gnuplotfile,"set ylabel 'y'\n");

  // Calculate tics

  int posx[5]; 
  int posy[5];
  float tagsx[5]; 
  float tagsy[5];
 

  for(int l=0;l<5;l++){
    
    posx[l] = (int)(l*rowSize/4);
    tagsx[l] = xstart + xspacing*posx[l];
    posy[l] = (int)(l*colSize/4);
    tagsy[l] = -ystart + yspacing*posy[l];
  }

  fprintf(gnuplotfile,"set xtics ('%f' %d,'%f' %d, '%f' %d,'%f' %d, '%f' %d)\n",tagsx[0], posx[0] ,tagsx[1], posx[1], tagsx[2], posx[2], tagsx[3], posx[3], tagsx[4], posx[4]);

  fprintf(gnuplotfile,"set ytics ('%f' %d,'%f' %d, '%f' %d,'%f' %d, '%f' %d)\n",tagsy[0], posy[0] ,tagsy[1], posy[1], tagsy[2], posy[2], tagsy[3], posy[3], tagsy[4], posy[4]);

  fprintf(gnuplotfile,"splot '%s' matrix with lines\n",filename.c_str());

  fprintf(gnuplotfile,"set terminal postscript\n");
  fprintf(gnuplotfile,"set output '%s.ps'\n",filename.c_str());
  fflush(gnuplotfile);
  fclose(gnuplotfile);
#endif
}




  
