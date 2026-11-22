#include "SealTest/SealTest.h" 
#include "SealBase/Argz.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

int seal_test::Argz01(int, char **argv)
{
    Signal::handleFatal (argv[0]);


    static const char *tests[] = {
	"a simple command line",
	"arg 'foo' is single quoted",
	"arg \"bar\" is double quoted",
	"arg \"foo bar\" has embedded whitespace",
	"arg \"foo \\\"bar\" has embedded quoted double quote",
	"arg \"foo \\\\bar\" has embedded quoted backslash",
	"arg \"foo \\'bar\" has inactive backslash",
	"arg 'foo bar' has embedded whitespace",
	"arg 'foo \\'bar has embedded backslash",
	"arg 'Jack said \\'hi'' has backslash and single quotes",
	"arg 'Jack said \\\"hi\\\"' has backslashed double quotes",
	"a b c d e f g h i j k l m n o p q r s t u v w x y z 1 2 3 4 5 6 7 8 9",
	"trailing-whitespace ",
	"",
	"arg 'Jack said \\'hi\\'' has unbalanced single quotes",
	NULL
    };

    for (const char **test = tests; *test; ++test)
    {
	Argz t (*test);
	seal_test::out << "Argz (" << *test << ") = quoted(" << t.quote () << ")\n";
	for (const char **argz = t.argz (); *argz; ++argz)
	    seal_test::out << "\t<<" << *argz << ">>\n";
    }

    return 0;
}
