
#include <fstream>
#include <iostream>
#include <sstream>
#include "cstring"

#include <string>
#include <vector>
#include <limits>

using namespace std;

void GetFileNames ( const string& in_string, vector<string>& out_fileNames );

struct PETOutput
{
	char name[8];
	float xVtx, yVtx, zVtx, x1, y1, z1, x2, y2, z2;
};

int main(int argc, char *argv[])
{
	if (argc<2) 
	{
		cout << "ERROR! Usage: VIP_TXT2FBP_outfile <PET_img.out filenames>" << endl;
		return 1;
	}
	
	std::string nameString = argv[1];
	std::vector<std::string> fileNames;
	GetFileNames ( nameString, fileNames );
	if (fileNames.size() < 1)
	{
		cout << "ERROR! Not enough filenames: " << nameString << " " << fileNames.size() << endl;
		return 1;
	}
	for (int i = 0; i < fileNames.size(); i++)
    {
        cout << fileNames[i] << endl;
    }

	bool isPEM( false );
	if (argc > 2)
	{
		string tmpStr = argv[2];
		cout << "tmpStr: " << tmpStr << endl;
		if ( tmpStr == "-PEM" )
		{
			isPEM = true;
			cout << "making is PEM is true" << endl;
		}
	}

	PETOutput po;	
	po.xVtx = po.yVtx = po.zVtx = 0;
	strcpy(po.name,"PET");		
	char theFileNameOut[100];
	std::string fileName = fileNames[0];
	sprintf(theFileNameOut,"%s.fbp", fileName.c_str());
	std::ofstream theFileOut( theFileNameOut, std::ios::binary | std::ios::out );
		
	ifstream infile;
	int icount(0);
	double dummyE;
	// Loop over all files
	for (int iloop = 0; iloop < fileNames.size(); iloop++)
	{	
		fileName = fileNames[iloop];
		infile.open( fileName.c_str() );
		if (!infile.is_open()) 
		{
			cout << "ERROR! File not open: " << fileName << endl;
			return 2;
		}
		cout << "file: " << fileName << endl;
		int thisfilecount( 0 );
			
		if ( !isPEM )
		{
			for(; infile >> po.z1 >> po.y1 >> po.x1 >> dummyE 
                         >> po.z2 >> po.y2 >> po.x2 >> dummyE;) 
			{
				infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	// ignore additional fields on this line
				theFileOut.write((char*) &po, sizeof(struct PETOutput));
				icount++;
				thisfilecount++;
			}
		} else {	// In case of PEM z = y, y = z
			for(; infile >> po.y1 >> po.z1 >> po.x1 >> dummyE 
                         >> po.y2 >> po.z2 >> po.x2 >> dummyE;) 
			{
				infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');	// ignore additional fields on this line
				theFileOut.write((char*) &po, sizeof(struct PETOutput));
				icount++;
				thisfilecount++;
			}
		}

		infile.close();
		cout << "#events for this file: " << thisfilecount << endl;
	} // Loop over all files

	cout << "Number of events: " << icount << endl;
	
	return 0;
}

// ======================================================================

void
GetFileNames ( const string& in_string, vector<string>& out_fileNames ) 
// COMMA separated substrings
{
	/* // No spaces possible in string"
	string tmpStr( in_string );
 	std::string::iterator end_pos = std::remove(tmpStr.begin(), tmpStr.end(), ' ');
	tmpStr.erase(end_pos, tmpStr.end());
	*/

	stringstream ss( in_string );
	out_fileNames.clear();
	while( ss.good() )
	{
   		string substr;
   		getline( ss, substr, ',' );
   		out_fileNames.push_back( substr );
	}
}
