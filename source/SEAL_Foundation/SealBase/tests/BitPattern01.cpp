#include "SealTest/SealTest.h" 
#include "SealBase/BitPattern.h"
#include "SealBase/BitTraits.h"
#include "SealBase/BitIterator.h"
#include "SealBase/Signal.h"
#include <iostream>
#include <iomanip>

using namespace seal;

inline unsigned int
maxbit (unsigned int n, unsigned int mod)
{
    static unsigned const int size = BitTraits<unsigned char>::Bits;
    return (n * size) / mod + (size % mod ? 1 : 0);
}

static void
dump_bits (const unsigned char *data, unsigned int n)
{
    static const char *const bits1 [] = { "0", "1" };
    static const char *const bits2 [] = { "00", "01", "10", "11" };
    static const char *const bits3 [] = { "000", "001", "010", "011",
					  "100", "101", "110", "111" };

    seal_test::out << " 1 (0.." << maxbit (n, 1) << "): ";
    BitIterator<unsigned char,1> start1 (data, 0);
    BitIterator<unsigned char,1> end1   (data, maxbit (n, 1));
    for (; start1 != end1; ++start1)
	seal_test::out << bits1[*start1];
    seal_test::out << std::endl;

    seal_test::out << " 2 (0.." << maxbit (n, 2) << "): ";
    BitIterator<unsigned char,2> start2 (data, 0);
    BitIterator<unsigned char,2> end2   (data, maxbit (n, 2));
    for (; start2 != end2; ++start2)
	seal_test::out << bits2[*start2] << ' ';
    seal_test::out << std::endl;

    seal_test::out << " 3 (0.." << maxbit (n, 3) << "): ";
    BitIterator<unsigned char,3> start3 (data, 0);
    BitIterator<unsigned char,3> end3   (data, maxbit (n, 3));
    for (; start3 != end3; ++start3)
	seal_test::out << bits3[*start3] << ' ';
    seal_test::out << std::endl;

    seal_test::out << " 4 (0.." << maxbit (n, 4) << "): ";
    BitIterator<unsigned char,4> start4 (data, 0);
    BitIterator<unsigned char,4> end4   (data, maxbit (n, 4));
    for (; start4 != end4; ++start4)
	seal_test::out << bits2[*start4 >> 2] << bits2[*start4 & 0x3] << ' ';
    seal_test::out << std::endl;
}

static void
dump_pattern (unsigned int pattern)
{
    unsigned int pat [2] = { pattern, 0 };
    seal_test::out << "pat = (0x" << std::hex << pattern << std::dec << ")\n";
    dump_bits ((unsigned char *) pat, sizeof (pat[0]));

    pat[0] = BitOps<unsigned int>::reverse (pattern);
    seal_test::out << "rev = (0x" << std::hex << pat[0] << std::dec << ")\n";
    dump_bits ((unsigned char *) pat, sizeof (pat[0]));
    
    seal_test::out << "count = " << BitOps<unsigned int>::count (pattern) << "\n";
}

int seal_test::BitPattern01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    dump_pattern (BitPattern<unsigned,0x1,2,16>::Pattern);

    seal_test::out << std::endl;
    dump_pattern (BitPattern<unsigned,0xd0,8,4>::Pattern);

    seal_test::out << std::endl;
    dump_pattern (BitPattern<unsigned,0x0f,8,3>::Pattern);
    return 0;
}

