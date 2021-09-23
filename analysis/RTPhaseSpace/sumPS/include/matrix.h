#ifndef __MATRIX_H_
#define __MATRIX_H_
#endif

class matrix
{
      public:
              matrix(int p,int q);
	      ~matrix();
              float** mat;
	      int rowSize;
	      int colSize;
              float xspacing;
              float yspacing;
	      float xstart;
	      float ystart;
              void print_matrix();
	      void get_grid(float dx, float dy, float x1, float y1);
              void asign_matrix(int k, int l, float value);
	      void addto_matrix(int k, int l, float value);
              void save_matrix(string filename);
	      void colorscale(string filename);
	      void plot_matrix(string filename, string colorscale);
      private:
};
