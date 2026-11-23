#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H_
#endif

class histogram
{
      public:
              histogram(float min, float max, float size, int tp);
              ~histogram();
              float v1min;
              float v1max;
              float bin_size;
              int bins;
              float* vect1;
	      float* vect2;
	      int type;
              void fill_histogram();
              void putin_histogram(float value);
              void print_histogram();
              void plot_histogram(char filename[80]);
              void save_histogram(char filename[80]);

      private:
}; 
