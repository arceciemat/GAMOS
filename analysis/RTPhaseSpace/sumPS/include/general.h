#ifndef __GENERAL_H__
#define __GENERAL_H__ 

#include <vector>
#include <string.h>

	double get_num(string texto,int n);
	string get_string(string texto,string sep,int n);
	string clear_string_all(string in,string b,string s);
	void timeofcalc(long p, int& h, int& m, int& s,double referencia);
		

	string num2string(int i);
	string num2string(double d);

	string find_attribute(string line, string word, string end);
	
	long string2long(string text);
	int string2int(string text);
	//double string2double(string text);
	double string2double(string text);

	string get_last_field(string text,string sep);
	int get_num_of_field(string text,string sep);
	string get_string_without_last_field(string texto,string sep);
	string get_pwd();

	string put_string(string texto,string sep,int n, string word);
        int cssh(string username,
				  string servername,
				  string exe_command,
				  vector<string> & );
	int cssh(string username,
				  string servername,
				  string exe_command,
				  string & );
	string cssh(string username,
				  string servername,
				  string exe_command );

	//int cssh(string username, string servername,  string exe_command );
/*	void cssh(string username,
				  string servername,
				  string exe_command );
*/

	string seconds2timeformat(double);
	int mscp(string username, string servername, string file_list, string remote_path );
	int mscp2h(string username         , string servername        , string file, string remote_path);
	string get_remote_env(string username,
				  string servername,
				  string env_data);
       
#endif
