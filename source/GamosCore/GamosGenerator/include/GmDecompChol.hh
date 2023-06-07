// @(#)root/matrix:$Id$
// Authors: Fons Rademakers, Eddy Offermann   Dec 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_GmDecompChol
#define ROOT_GmDecompChol

///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Cholesky Decomposition class                                          //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "TMatrixDSym.h"
#include "TMatrixD.h"

class GmDecompChol 
{
protected :

   TMatrixD fU; // decomposed matrix fU so that a = fU^T fU

public :

   GmDecompChol() : fU() {}
   GmDecompChol(Int_t row_lwb,Int_t row_upb);
   virtual Bool_t   Decompose  ();

};

#endif
