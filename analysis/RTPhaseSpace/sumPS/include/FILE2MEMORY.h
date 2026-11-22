#ifndef __FILE2MEMORY_H__
#define __FILE2MEMORY_H__

#include <iostream>
#include <string>
#include <vector>

class FILE2MEMORY
{
       public:
	      FILE2MEMORY();
              FILE2MEMORY(char file_in[80]);
              FILE2MEMORY(string file_in);
              ~FILE2MEMORY();
            //  int numero[500]

              int longitud_total;   // Obsolete
              string find_texto(string ref,int increase, int time); // obsolete
              vector<int> longitud;  // Obsolete
              
              int Number_Of_Lines;
              vector<int> length;
              vector<string> line;
	      vector<string>::iterator line_iterator;
              
              string get_line(int p);
	      string get_stringFromLine(int p,string sep,int n);
	      

	
	          
	          string find_text(string ref,int increase, int time);
	          int find_line(string ref,int increase, int time);
		  int find_line(string ref,int increase, int time, int star_line);
		  int save_as(string file_name);
		  void print();
		  void clear_empty_at_the_biginning();
		  void push_back(string text);
		  void load_file(string text);
		  void clear(int i);
		  
        private:
};
#endif
