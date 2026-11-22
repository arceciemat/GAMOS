#include "SealTest/SealTest.h" 
#include "SealBase/DebugAids.h"
#include "SealBase/Signal.h"
#include "SealBase/XTypeInfo.h"
#include <iostream>
#include <utility>

using namespace seal;

namespace test_XtypeInfo { 

class X					{ public: virtual ~X (); };
class Y : public X			{ };
class Z : public virtual X		{ };
class W : public virtual X, public Z	{ };
class V : public W			{ };

}

using namespace test_XtypeInfo; 

X::~X (void) {}

#define API
XTYPEINFO_DECLARE (API, X)
XTYPEINFO_DECLARE (API, Y)
XTYPEINFO_DECLARE (API, Z)
XTYPEINFO_DECLARE (API, W)
XTYPEINFO_DECLARE (API, V)

XTYPEINFO_DEF_0(X);
XTYPEINFO_DEF_1(Y, X,false);
XTYPEINFO_DEF_1(Z, X,true);
XTYPEINFO_DEF_2(W, X,true, Z,false);
XTYPEINFO_DEF_1(V, W,false);

//////////////////////////////////////////////////////////////////////

// FIXME: This needs to be outside functions due to mutually incompatible
// bugs in GCC 3.1 and Solaris CC 5.3.  GCC can't grok the first form in
// a function-local scope but does accept a "using namespace std::rel_ops;".
// However, Solaris CC doesn't find the operator with the latter.
#if HAVE_NAMESPACE_STD_REL_OPS
  using std::rel_ops::operator!=;
#elif HAVE_REL_OPS_IN_STD
  using std::operator!=;
#endif

int seal_test::XTypeInfo01(int, char **argv)
{
    Signal::handleFatal (argv[0]);

    // define objects 
    X x; 
    Y y; 
    W w;
    Z z;
    V v;


    XTypeInfo::TypeIterator	type = XTypeInfo::beginTypes ();
    XTypeInfo::TypeIterator	last = XTypeInfo::endTypes ();

    for ( ; type != last; ++type)
    {
	XTypeInfo::BaseIterator	base = (*type)->beginBases ();
	XTypeInfo::BaseIterator	lastBase = (*type)->endBases ();

	XTypeInfo::DerivedIterator derived	= (*type)->beginDerived ();
	XTypeInfo::DerivedIterator lastDerived = (*type)->endDerived ();

	seal_test::out << "type `" << (*type)->standard ().name () << "'"
		  << " (#bases: " << (lastBase - base)
		  << ", #derived: " << (lastDerived - derived) << ")\n"
		  << "  bases: {\n";


	// check with expectations
	if ((*type)->standard () == typeid(x)) { 
	  CPPUNIT_ASSERT( (lastBase-base) == 0); 
	  CPPUNIT_ASSERT( (lastDerived - derived) == 3); 
	}
	if ((*type)->standard () == typeid(y)) { 
	  CPPUNIT_ASSERT( (lastBase-base) == 1); 
	  CPPUNIT_ASSERT( (lastDerived - derived) == 0); 
	}
	if ((*type)->standard () == typeid(w)) { 
	  CPPUNIT_ASSERT( (lastBase-base) == 2); 
	  CPPUNIT_ASSERT( (lastDerived - derived) == 1); 
	}
	if ((*type)->standard () == typeid(z)) { 
	  CPPUNIT_ASSERT( (lastBase-base) == 1); 
	  CPPUNIT_ASSERT( (lastDerived - derived) == 1); 
	}
	if ((*type)->standard () == typeid(v)) { 
	  CPPUNIT_ASSERT( (lastBase-base) == 3); 
	  CPPUNIT_ASSERT( (lastDerived - derived) == 0); 
	}

	for ( ; base != lastBase; ++base) {

	  seal_test::out << "    { d=" << base->distance ()
			 << " public " << (base->isVirtual () ? "virtual " : "")
			 << '`' << base->base ()->standard ().name () << '\''
			 << "}\n";
	  if  ((*type)->standard () == typeid(y) ) { 
	    CPPUNIT_ASSERT( base->base ()->standard () == typeid(x) );
	    CPPUNIT_ASSERT( base->distance () == 1 );
	    CPPUNIT_ASSERT( ! base->isVirtual ()  );
	  }
	  if  ((*type)->standard () == typeid(z) ) { 
	    CPPUNIT_ASSERT( base->base ()->standard () == typeid(x) );
	    CPPUNIT_ASSERT( base->distance () == 1 );
	    CPPUNIT_ASSERT( base->isVirtual ()  );
	  }
	  if  ((*type)->standard () == typeid(w) ) { 
	    if (  base->base ()->standard () == typeid(x) ) { 
	      CPPUNIT_ASSERT( base->distance () == 1 );
	      CPPUNIT_ASSERT( base->isVirtual ()  );
	    }
	    if (  base->base ()->standard () == typeid(z) ) { 
	      CPPUNIT_ASSERT( base->distance () == 1 );
	      CPPUNIT_ASSERT( !base->isVirtual ()  );
	    }
	  }
	  if  ((*type)->standard () == typeid(v) ) { 
	    if (  base->base ()->standard () == typeid(x) ) { 
	      CPPUNIT_ASSERT( base->distance () == 2 );
	      CPPUNIT_ASSERT( base->isVirtual ()  );
	    }
	    if (  base->base ()->standard () == typeid(z) ) { 
	      CPPUNIT_ASSERT( base->distance () == 2 );
	      CPPUNIT_ASSERT( !base->isVirtual ()  );
	    }
	    if (  base->base ()->standard () == typeid(w) ) { 
	      CPPUNIT_ASSERT( base->distance () == 1 );
	      CPPUNIT_ASSERT( !base->isVirtual ()  );
	    }
	  }

	}

	seal_test::out << "  }\n  derived: {";
	for ( ; derived != lastDerived; ++derived) { 
	  seal_test::out << ' ' << (*derived)->standard ().name ();

	    if  ((*type)->standard () == typeid(y) ) 
	      CPPUNIT_ASSERT( (*derived)->standard () == typeid(x)  );
	    if  ((*type)->standard () == typeid(z) ) 
	      CPPUNIT_ASSERT( (*derived)->standard () == typeid(w)  );
	    if  ((*type)->standard () == typeid(w) ) 
	      CPPUNIT_ASSERT( (*derived)->standard () == typeid(v)  );

	}

	seal_test::out << " }\n\n";


    }

    return 0;
}
