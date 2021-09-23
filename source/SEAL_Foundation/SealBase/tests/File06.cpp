#include "SealTest/SealTest.h" 
#include "SealBase/File.h"
#include "SealBase/Filename.h"
#include "SealBase/ShellEnvironment.h"
#include "SealBase/Time.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

static std::string inputFile = "BuildFile";

int seal_test::File06(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    if (ShellEnvironment().has("INPUT_FILE") )
	inputFile = ShellEnvironment().get("INPUT_FILE"); 


    Filename            fileName(inputFile);
    if (!fileName.exists()) { 
      seal_test::out << "Error : input file " << inputFile << " does not exist ! " << std::endl;
      return -1;
    }

    seal_test::out << "Input file   : " << inputFile << std::endl;

    File	f (inputFile);
    Time	ctime, mtime;

    f.times (&ctime, &mtime, 0);

    seal_test::out << "config.log ctime = " << ctime
	      << ", mtime = " << mtime << std::endl;

    return 0;
}
