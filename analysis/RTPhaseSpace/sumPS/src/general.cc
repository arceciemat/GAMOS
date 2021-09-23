#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>
#include <time.h>
#include <cstring>
using namespace std;
#include "FILE2MEMORY.h"


double get_num(string texto,int n){
/// return the numer n on the line text
	char a[80];
	int inicio=-1;
	int final=-1;
	int index=0;
	int control_E=0;
	string saux;

	for(unsigned int i=0;i<texto.size();i++){
		saux=texto[i];
		strcpy(a,saux.c_str());
           if (inicio!=-1 && (strcmp(a,"e")==0 || strcmp(a,"E")==0) ){
			control_E=1;
//			cout << "******** E *******  " << endl;
	   }else if (control_E==1 && inicio!=-1){
			if(strcmp(a,"+")==0 && strcmp(a,"-")==0){control_E=0;}
	   }else{
		control_E=0;
		if( (strcmp(a,"1")==0) || (strcmp(a,"2")==0) || (strcmp(a,"3")==0) || (strcmp(a,"4")==0) || (strcmp(a,"5")==0) || (strcmp(a,"6")==0) || (strcmp(a,"7")==0) || (strcmp(a,"8")==0) || (strcmp(a,"9")==0) || (strcmp(a,"0")==0) || (strcmp(a,".")==0) || (strcmp(a,"-")==0) ){
			if(inicio==-1){inicio=i;control_E=0;}
		}else{
			if(inicio!=-1){final=i;control_E=0;}
		}
		if( (inicio!=-1) && (final!=-1)){
			control_E=0;
			index++;
			saux="";
			saux=texto.substr(inicio,final-inicio);
			//cout << "Texto seleccionado: " << saux << endl;
			strcpy(a,saux.c_str());
			if (n==index) {
				return atof(a);
			}else{
			inicio=-1;
			final=-1;
			}
		}
	     }
	}
	if (inicio!=-1){
          index++;
          saux="";
		  saux=texto.substr(inicio,texto.size());
		  strcpy(a,saux.c_str());
		  if (n==index) {
				return atof(a);
		  }
    }
//	cout << "Numero no encontrado " << endl;
	
	return 0.0;
}

string get_string(string texto,string sep,int n){
// texto is the text in to analice.
// sep is the separator marc inter text
// n is the order number betwen separator

   //cout << "Texto = '" << texto  << "'" << endl;
   //cout << "Separador = '" << sep << "'" << endl;
   //cout << " n = " << n << endl;

	char a[80];
	char b[80];
	int inicio=-1;
	int final=-1;
	int index=0;
	string saux;
	saux=sep[0];
	// cout << " Aqui el saux = '" << saux << "' " << endl;
    strcpy(b,saux.c_str());
    saux="";
	for(size_t i=0;i<texto.size();i++){
		saux=texto[i];
		strcpy(a,saux.c_str());
		// cout << "Analizando el texto '" << saux << "' " ;
		// cout << " comparo a = '" << a << "' con b = '" << b << "' " << endl;
		if( (strcmp(a,b)!=0) ){
			if(inicio==-1){inicio=i;}
		}else{
			if(inicio!=-1){final=i;}
		}
		if( (inicio!=-1) && (final!=-1)){
        	index++;
			saux="";
			saux=texto.substr(inicio,final-inicio);
			//cout << "Texto seleccionado: " << saux << endl;
			//strcpy(a,saux.c_str());
			if (n==index) {
				//return atof(saux);
				return saux;
			}else{
			inicio=-1;
			final=-1;
			}
		}
	}
	if (inicio!=-1){
          index++;
          saux="";
		  saux=texto.substr(inicio,texto.size());
		  //strcpy(a,saux.c_str());
		  if (n==index) {
				//return atof(saux);
				return saux;
		  }
    }
/*	cout << "Numero no encontrado " << endl;
	cout << " incicio = " << inicio << endl;
	cout << " final   = " << final << endl;
	return "";
*/

	return "";
}

string clear_string_all(string in,string b,string s){// in entrada, b para cambiar por s
	int lo=in.size();
	for (int i=0; i<lo; i++){
		if (in[i]==b[0]){in[i]=s[0];}
	}
	return in;
}
/////////////////////////////////////////////////////////////
void timeofcalc(long p, int& h, int& m, int& s,double referencia){
     //cout << "PARICULA= " << p << endl;
     //REFERENCIA == particulas por segundo. (lo normal pueden ser 8000)
     double aux=referencia*3600.0;
     int auxh=0;
     auxh=int((p/aux));
     h=auxh;
     aux=(p/aux-auxh)*60;
     auxh=int(aux);
     m=auxh;
     aux=(aux-auxh)*60;
     auxh=int(aux);
     s=auxh;
}

string num2string(int i){
       string aux;
       string aux2=".";
       char caux[80]="";
       if (i>999){
	   gcvt(i,10,caux);
	} else {
         gcvt(i,3,caux);
	}
       aux=caux;
       //cout << "123.21345 " << fmod(123.21345,1) << endl;
       /*if ( fmod(i,1) > 0){
           
            int longitud=aux.length();
            aux=aux.substr(longitud,1);
            
            }*/
       int longitud=aux.size();
       if (aux[longitud-1]==aux2[0]){
                           //   cout << "Ahora es " << aux << endl;
                              aux=aux.substr(0,longitud-1);}    
                           //   cout << "Y ahora es " << aux << endl;
       return aux; 
}
string num2string(double d){
       string aux;
       char caux[80]="";
       gcvt(d,3,caux);
       aux=caux;
       if ( fmod(d,1) > 0){
            int longitud=aux.length();
            aux=aux.substr(longitud,1);
            
            }
       if (aux.length()==0){
           
           stringstream s;
           s << d;
           s >> aux;
           }
        
       return aux;
      
}
/*       
string char2string(char caux[80]){
       string aux;
       aux=caux;
       if ( fmod(d,1) > 0){
            int longitud=aux.length();
            aux=aux.substr(longitud,1);
            
            }
       if (aux.length()==0){
           
           stringstream s;
           s << d;
           s >> aux;
           }
        
       return aux;
      
       }
*/

long string2long(string text){
	long num;
	stringstream s;
	s << text;	
	s >> num;
	return num;
}

int string2int(string text){
	int num;
	stringstream s;
	s << text;	
	s >> num;
	return num;
}
double string2double(string text){
	double num;
	stringstream s;
	s << text;	
	s >> num;
	return num;
}




string find_attribute(string line, string word, string end){
  
  int loc;
  int i;
  string elem1;
  string outword;
  loc = line.find(word,0);
  elem1 = line.substr(loc+strlen(word.c_str()));
  i = elem1.find(end);
  outword = elem1.substr(0,i);
  return outword;

}

string get_pwd(){

	int num=int(rand()/10000000);
	//cout << "RAND = " << num << endl;
	string line_out="pwd > tmp.";
	line_out+=num2string(num);
	system(line_out.c_str());
	line_out="tmp."+num2string(num);
	FILE2MEMORY PWD(line_out);
	line_out="rm "+line_out;
	//cout << " LAST = " << line_out << endl;
	system(line_out.c_str());	

	return 	PWD.get_line(0);
	

}
string get_last_field(string texto,string sep){
/// texto is the text in to analice.
/// sep is the separator marc inter text


   //cout << "Texto = '" << texto  << "'" << endl;
   //cout << "Separador = '" << sep << "'" << endl;
   //cout << " n = " << n << endl;
	int n=0;
	char a[256];
	char b[256];
	string saux;
	saux=sep[0];
	strcpy(a,saux.c_str());

	for(size_t i=0; i <texto.size();i++){
		saux=texto[i];
		strcpy(b,saux.c_str());
		if (strcmp(a,b)==0){
			n++;
		}

	}

	//	n=n++;
	n++;

	//cout << "n = " << n << endl;
	string aux;
	aux=get_string(texto,sep,n+1);
	if (aux.size()==0){
		aux=get_string(texto,sep,n);
	}
	return aux;
	
}

int get_num_of_field(string texto,string sep){
/// texto is the text in to analice.
/// sep is the separator marc inter text


   //cout << "Texto = '" << texto  << "'" << endl;
   //cout << "Separador = '" << sep << "'" << endl;
   //cout << " n = " << n << endl;
	int n=0;
	char a[256];
	char b[256];
	string saux;
	saux=sep[0];
	strcpy(a,saux.c_str());

	for(size_t i=0; i <texto.size();i++){
		saux=texto[i];
		strcpy(b,saux.c_str());
		if (strcmp(a,b)==0){
			n++;
		}

	}

	//	n=n++;
	n++;

	//cout << "n = " << n << endl;
	
	return n;
	
}
string get_string_without_last_field(string texto,string sep){
/// texto is the text in to analice.
/// sep is the separator marc inter text


   //cout << "Texto = '" << texto  << "'" << endl;
   //cout << "Separador = '" << sep << "'" << endl;
   //cout << " n = " << n << endl;
	int n=0;
	char a[256];
	char b[256];
	string saux;
	saux=sep[0];
	strcpy(a,saux.c_str());

	for(size_t i=0; i <texto.size();i++){
		saux=texto[i];
		strcpy(b,saux.c_str());
		if (strcmp(a,b)==0){
			n=i;
		}

	}
	
	
	if (texto.size() < size_t(n) ) {return "Error";}
	
	saux=texto.substr(0,n);
	
	return saux;
	
}

string put_string(string texto,string sep,int n, string word){
// texto is the text in to analice.
// sep is the separator marc inter text
// n is the order number betwen separator

   //cout << "Texto = '" << texto  << "'" << endl;
   //cout << "Separador = '" << sep << "'" << endl;
   //cout << " n = " << n << endl;

	char a[80];
	char b[80];
	int inicio=-1;
	int final=-1;
	int index=0;
	string saux;
	saux=sep[0];
	// cout << " Aqui el saux = '" << saux << "' " << endl;
    strcpy(b,saux.c_str());
    saux="";
	for(size_t i=0;i<texto.size();i++){
		saux=texto[i];
		strcpy(a,saux.c_str());
		// cout << "Analizando el texto '" << saux << "' " ;
		// cout << " comparo a = '" << a << "' con b = '" << b << "' " << endl;
		if( (strcmp(a,b)!=0) ){
			if(inicio==-1){inicio=i;}
		}else{
			if(inicio!=-1){final=i;}
		}
		if( (inicio!=-1) && (final!=-1)){
        	index++;
			saux="";
			//saux=texto.substr(inicio,final-inicio);
			saux=texto.substr(0,inicio)+word+texto.substr(final,texto.size()-final);
			//cout << "Texto seleccionado: " << saux << endl;
			//strcpy(a,saux.c_str());
			if (n==index) {
				//return atof(saux);
				return saux;
			}else{
			inicio=-1;
			final=-1;
			}
		}
	}
	if (inicio!=-1){
          index++;
          saux="";
		  //saux=texto.substr(inicio,texto.size());
		  saux=texto.substr(0,inicio)+word;
		  //strcpy(a,saux.c_str());
		  if (n==index) {
				//return atof(saux);
				return saux;
		  }
    }
	cout << "Numero no encontrado " << endl;
	cout << " incicio = " << inicio << endl;
	cout << " final   = " << final << endl;
	return "";
}


int cssh(string username,
			  string servername,
			  string exe_command,
			  vector<string> &output){

	int exec_value;
	string exec_data;
	output.clear();
	exec_data = "ssh "+username+"@"+servername+" '"+exe_command+"' > get_ssh_remote_string_vector_system.tmp";
	cout << exec_data << endl;
	exec_value=system(exec_data.c_str());
	FILE2MEMORY data((char*)"get_ssh_remote_string_vector_system.tmp");
	cout << "FILE2MEMORY line.size()=" << data.line.size() << endl;
	data.clear_empty_at_the_biginning();
	cout << "Just to print " << endl;
	data.print();
	if ( data.line.size()>1){    
		cout << " End Print" << endl << " Size[0] " << data.line[0].size() << endl;
		cout <<  " Size[1] " << data.line[1].size() << endl;
	}
	system("rm -f  get_ssh_remote_string_vector_system.tmp");	
	cout << "EXE_READY" << endl;
	output=data.line;
	return exec_value;

}

int cssh(string username,
			  string servername,
			  string exe_command,
			  string &output){

	int exec_value;
	string exec_data;
	output.clear();
	exec_data = "ssh "+username+"@"+servername+" '"+exe_command+"' > get_ssh_remote_string_vector_system.tmp";
	cout << exec_data << endl;
	exec_value=system(exec_data.c_str());
	FILE2MEMORY data((char*)"get_ssh_remote_string_vector_system.tmp");
	data.clear_empty_at_the_biginning();
	cout << "Just to print " << endl;
	data.print();
	if ( data.line.size()>1){    
		cout << " End Print" << endl << " Size[0] " << data.line[0].size() << endl;
		cout <<  " Size[1] " << data.line[1].size() << endl;
	}
	system(" rm -f  get_ssh_remote_string_vector_system.tmp");	
	cout << "EXE_READY" << endl;
	if (data.line.size()>0){
		output=data.line[0];
	} else {
		output="";
	}
	
	return exec_value;

}


string cssh(string username, string servername, string exe_command ){

	
  //	int exec_value;
	string exec_data;
	//output.clear();
	exec_data = "ssh "+username+"@"+servername+" '"+exe_command+"' > get_ssh_remote_string_vector_system.tmp";
	cout << exec_data << endl;
	//	exec_value=system(exec_data.c_str());
	system(exec_data.c_str());
	
	FILE2MEMORY data((char*)"get_ssh_remote_string_vector_system.tmp");
	data.clear_empty_at_the_biginning();
	cout << "Just to print " << endl;
	data.print();
        if ( data.line.size()>1){    
		cout << " End Print" << endl << " Size[0] " << data.line[0].size() << endl;
		cout <<  " Size[1] " << data.line[1].size() << endl;
	}
	system(" rm -f  get_ssh_remote_string_vector_system.tmp");	
	cout << "EXE_READY" << endl;
	//output=data.line[0];
	//return exec_value;
	if ( data.line.size()>0){   
		return data.line[0];
	    } else{
	        return "";
	    }
}


 ///mscp(servers_data[i].username,servers_data[i].servername,file_list        ,        remote_path);
int mscp(string username         , string servername        , string file_list, string remote_path){

	
	int exec_value;
	string exec_data;

	cout << endl << username
	     << endl << servername
	     << endl << file_list
	     << endl << remote_path << endl << endl;	


	//output.clear();
	exec_data = "scp "+file_list+" "+username+"@"+servername+":"+remote_path;
	cout << exec_data << endl;
	exec_value=system(exec_data.c_str());
	//FILE2MEMORY data("get_ssh_remote_string_vector_system.tmp");
	//data.clear_empty_at_the_biginning();
	//cout << "Just to print " << endl;
	//data.print();
	//cout << " End Print" << endl << " Size[0] " << data.line[0].size() << endl;
	//cout <<  " Size[1] " << data.line[1].size() << endl;
	//system(" rm -f  get_ssh_remote_string_vector_system.tmp");
	if (exec_value==0){	
	cout << "CP_READY" << endl;
	}else{
	cout << "CP Error" << endl;
	}
	//output=data.line[0];
	//return exec_value;
	//return data.line[0];
	return exec_value ;

}

int mscp2h(string username         , string servername        , string file_name, string remote_path){

	
	int exec_value;
	string exec_data;

	cout << endl << username
	     << endl << servername
	     << endl << file_name
	     << endl << remote_path << endl << endl;	


	//output.clear();
	exec_data = "scp "+username+"@"+servername+":"+remote_path+"//"+file_name+" .";
	cout << exec_data << endl;
	exec_value=system(exec_data.c_str());
	//FILE2MEMORY data("get_ssh_remote_string_vector_system.tmp");
	//data.clear_empty_at_the_biginning();
	//cout << "Just to print " << endl;
	//data.print();
	//cout << " End Print" << endl << " Size[0] " << data.line[0].size() << endl;
	//cout <<  " Size[1] " << data.line[1].size() << endl;
	//system(" rm -f  get_ssh_remote_string_vector_system.tmp");
	if (exec_value==0){	
	cout << "CP_READY" << endl;
	}else{
	cout << "CP Error" << endl;
	}
	//output=data.line[0];
	//return exec_value;
	//return data.line[0];
	return exec_value ;

}

string get_remote_env(string username,
				  string servername,
				  string env_data){

	string temp_file="/tmp/temp_exe_";
	int N=time(NULL)/10;
	while (N>100){
	    N=N/10;
	}
	cout <<" N = " << N << endl;
	for (int i=0;i<N;i++){
	     N=rand();
	}	
	while (N>1000){
	    N=N/10;
	}
	temp_file+=num2string(N);

	FILE2MEMORY exec_remote;
	env_data="echo $"+env_data;
	exec_remote.push_back(env_data);
	exec_remote.save_as(temp_file);
	mscp(username,servername,temp_file,"/tmp/");
	cssh(username,servername,"chmod u+x "+temp_file);
	//cout << "EXE remote " << endl;
	
	env_data=cssh(username,servername,temp_file);
	cssh(username,servername,"rm -f "+temp_file);
	temp_file="rm -f "+temp_file;
	system(temp_file.c_str());
	//cout << "Path = " << remote_path << endl;
        //remote_path+=get_string(runs_data[j].exe_command," ",2)+"";
	return env_data;
}

string seconds2timeformat(double seconds){
	double h,ha;
	double m,ma;
	double s;
	string timef;
		
		h=seconds/(3600.0);
		ha=int(h);
		m=60.0*(h-ha);
		ma=int(m);
		s=60.0*(m-ma);
	timef=num2string(ha)+":"+num2string(ma)+":"+num2string(s);
	return timef;
}
