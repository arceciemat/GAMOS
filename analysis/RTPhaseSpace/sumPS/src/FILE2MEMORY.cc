#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

#include "general.h"
#include "FILE2MEMORY.h"

// #include "general.h"

FILE2MEMORY::FILE2MEMORY(){
       Number_Of_Lines=-1;
       longitud_total=-1;

}

FILE2MEMORY::FILE2MEMORY(char file_in[80]){
                              
       Number_Of_Lines=-1;
       longitud_total=-1;
       string line_in;
       line_in.clear();
       ifstream infile (file_in);
       while (getline(infile,line_in,'\n'))
       {
         line.push_back(line_in);
         Number_Of_Lines++;
         longitud_total++;
         //longitud[Number_Of_Lines]=line_in.size();
         //length[Number_Of_Lines]=line_in.size();
         //longitud[longitud_total]=line.size();
         //length[Number_Of_Lines]=line_in.size();
         longitud.push_back(line.size());
         length.push_back(line_in.size());
       }
       infile.close();
     
}
FILE2MEMORY::FILE2MEMORY(string file_in_name){
      
       const char *file = file_in_name.c_str();
       Number_Of_Lines=-1;
       longitud_total=-1;
       string line_in;
       line.clear();
       //ifstream infile (file_in);
       ifstream infile (file);
       while (getline(infile,line_in,'\n'))
       {
         line.push_back(line_in);
         longitud_total++;
         Number_Of_Lines++;
         //longitud[longitud_total]=line.size();
         //length[Number_Of_Lines]=line_in.size();
         longitud.push_back(line.size());
         length.push_back(line_in.size());
       }
       
       infile.close();
      
}

FILE2MEMORY::~FILE2MEMORY(){}

void FILE2MEMORY::load_file(string file_in_name){
      
       const char *file = file_in_name.c_str();
       Number_Of_Lines=-1;
       longitud_total=-1;
       string line_in;
       line.clear();
       //ifstream infile (file_in);
       ifstream infile (file);
       while (getline(infile,line_in,'\n'))
       {
         line.push_back(line_in);
         longitud_total++;
         Number_Of_Lines++;
         //longitud[longitud_total]=line.size();
         //length[Number_Of_Lines]=line_in.size();
         longitud.push_back(line.size());
         length.push_back(line_in.size());
       }
       
       infile.close();
      
}

string FILE2MEMORY::get_line(int p){
       if (p<=Number_Of_Lines){
           return line[p];
       }else{
           cout << "Index more big than possible" << endl;
           return "1";
       }
}


string FILE2MEMORY::get_stringFromLine(int p,string sep, int n){
	return get_string(line[p],sep,n);
}


string FILE2MEMORY::find_texto(string ref,int increase,int time){
	//cout << "Buscando " << ref << " para increase = " << increase << " en la ocasion " << time << endl;
  //	int control=0;
	int k=0;
	int pos=0;
	for(int i=0;i<=Number_Of_Lines;i++){
		pos=line[i].find(ref);
		if (pos>-1){
			k++;
			if (k==time){
				//cout << "El valor de find es : " << pos << endl;
				if (i+increase > Number_Of_Lines){
					cout << "line out of file" << endl;
					return "1";
				} else {
					return line[i+increase];
				}
			}
		}
	}
	//	cout << "No results" <<  endl;
	return "1";
}

string FILE2MEMORY::find_text(string ref,int increase,int time){
	//cout << "Buscando " << ref << " para increase = " << increase << " en la ocasion " << time << endl;
  //	int control=0;
	int k=0;
	int pos=0;
	for(int i=0;i<=Number_Of_Lines;i++){
		pos=line[i].find(ref);
		if (pos>-1){
			k++;
			if (k==time){
				//cout << "El valor de find es : " << pos << endl;
				if (i+increase > Number_Of_Lines){
					cout << "line out of file" << endl;
					return "1";
				} else {
					return line[i+increase];
				}
			}
		}
	}
	cout << "No Results" <<  endl;
	return "1";
}







//**********************************//*********************//
int FILE2MEMORY::find_line(string ref,int increase,int time){
	//cout << "Buscando " << ref << " para increase = " << increase << " en la ocasion " << time << endl;
  //	int control=0;
	int k=0;
	int pos=0;
	for(int i=0;i<=Number_Of_Lines;i++){
		pos=line[i].find(ref);
		if (pos>-1){
			k++;
			if (k==time){
				//cout << "El valor de find es : " << pos << endl;
				if (i+increase > Number_Of_Lines){
					cout << "line out of file" << endl;
					return -1;
				} else {
					return i+increase;
				}
			}
		}
	}
	cout << "No Results" <<  endl;
	return -1;
}

int FILE2MEMORY::find_line(string ref,int increase,int time, int start_line){
	//cout << "Buscando " << ref << " para increase = " << increase << " en la ocasion " << time << endl;
  //	int control=0;
	int k=0;
	int pos=0;
	for(int i=start_line;i<=Number_Of_Lines;i++){
		pos=line[i].find(ref);
		if (pos>-1){
			k++;
			if (k==time){
				//cout << "El valor de find es : " << pos << endl;
				if (i+increase > Number_Of_Lines){
					cout << "line out of file" << endl;
					return -1;
				} else {
					return i+increase;
				}
			}
		}
	}
	cout << "No Results" <<  endl;
	return -1;
}

int FILE2MEMORY::save_as(string file_name){

        const char *file = file_name.c_str();
        std::ofstream fout(file,ios::out|ios::out);

       	for(int i=0;i<=Number_Of_Lines;i++){
		fout << line[i] << endl;		

	}
	fout.close();
return 0;

}

void FILE2MEMORY::print(){
	if (Number_Of_Lines>0){
	for(int i=0;i<=Number_Of_Lines;i++){
		cout << line[i] << endl;		

	}}
}

void FILE2MEMORY::clear_empty_at_the_biginning(){
	//cout << "Line.size()="<<line.size()<< endl;
	for(int i=0;i<=Number_Of_Lines;i++){
		//cout << "Line["<<i<<"] size = " << line[i].size() << " lince =\"" << line[i] << "\"" << endl;
		if (line[i].size()==0&&line.size()>1){
			//line_iterator=line.begin();
			line.erase(line.begin());
			//cout << "erase" << endl;
			length.erase(length.begin());
			//cout << "erase 2" << endl;
			//cout << "length.size()="<<length.size()<< endl;
			longitud.erase(longitud.begin());
			//cout << "erase 3" << endl;
			longitud_total--;
			//cout << "-- 1" << endl;
			Number_Of_Lines--;
			//cout << "-- 2" << endl;
		} else {
			return;
		}		

	}
}

void FILE2MEMORY::push_back(string text){
	line.push_back(text);
	longitud_total++;
	Number_Of_Lines++;
	length.push_back(text.size());
	longitud.push_back(text.size());


}

void FILE2MEMORY::clear(int i){
  if(int(line.size())>=i){
		line.erase(line.begin()+i);
		length.erase(length.begin()+i);
		longitud.erase(longitud.begin()+i);
		longitud_total--;
		Number_Of_Lines--;
	}
}





/////////////////////////////////////////////////////////////
