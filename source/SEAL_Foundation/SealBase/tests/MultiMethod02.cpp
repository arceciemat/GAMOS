#include "SealTest/SealTest.h" 
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include "SealBase/MultiMethod.h"
#include "SealBase/Log.h"
#include <iostream>

using namespace seal;

class X { public: virtual ~X (); };
class Y : public X { };
class Z : public Y { };

class U { public: virtual ~U (); };
class V : public U { };
class W : public V { };

X::~X (void) {}
U::~U (void) {}

//////////////////////////////////////////////////////////////////////
#define API
XTYPEINFO_DECLARE (API, X)
XTYPEINFO_DECLARE (API, Y)
XTYPEINFO_DECLARE (API, Z)
XTYPEINFO_DECLARE (API, U)
XTYPEINFO_DECLARE (API, V)
XTYPEINFO_DECLARE (API, W)

XTYPEINFO_DEF_0(X);
XTYPEINFO_DEF_1(Y, X,false);
XTYPEINFO_DEF_1(Z, Y,false);

XTYPEINFO_DEF_0(U);
XTYPEINFO_DEF_1(V, U,false);
XTYPEINFO_DEF_1(W, V,false);

// MSVC++ doesn't like empty macro arguments and using colon causes
// scope parsing difficulties for various compilers.
#define BLANK

MULTIMETHOD_DECLARE(extern,int,cross,2,0,(X,U));
MULTIMETHOD_DEFINE (BLANK, cross);
MMM_DEFUN_FUNC(int,::,cross,(X *, U *)) { seal_test::out << "[cross (X *, U *)]"; return 0; }
MMM_DEFUN_FUNC(int,::,cross,(X *, W *)) { seal_test::out << "[cross (X *, W *)]"; return 1; }
MMM_DEFUN_FUNC(int,::,cross,(Y *, W *)) { seal_test::out << "[cross (Y *, W *)]"; return 2; }
MMM_DEFUN_FUNC(int,::,cross,(Z *, W *)) { seal_test::out << "[cross (Z *, W *)]"; return 3; }
MMM_DEFUN_FUNC(int,::,cross,(Z *, U *)) { seal_test::out << "[cross (Z *, U *)]"; return 4; }
MMM_DEFUN_FUNC(int,::,cross,(Y *, V *)) { seal_test::out << "[cross (Y *, V *)]"; return 5; }

//////////////////////////////////////////////////////////////////////

int seal_test::MultiMethod02(int argc, char **argv)
{
    Signal::handleFatal (argv[0]);

    X x; Y y; Z z;
    U u; V v; W w;

    seal_test::out << "(x, u) -> " << cross (&x, &u) << std::endl;
    seal_test::out << "(x, w) -> " << cross (&x, &w) << std::endl;
    seal_test::out << "(z, v) -> " << cross (&z, &v) << std::endl;

    return 0;
}
