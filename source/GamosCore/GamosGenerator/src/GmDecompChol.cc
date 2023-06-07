

/** \class GmDecompChol
    \ingroup Matrix

 Cholesky Decomposition class

 Decompose a symmetric, positive definite matrix A = U^T * U

 where U is a upper triangular matrix

 The decomposition fails if a diagonal element of fU is <= 0, the
 matrix is not positive negative . The matrix fU is made invalid .

 fU has the same index range as A .
*/

#include "globals.hh"
#include "GmDecompChol.hh"
#include "TMath.h"

//ClassImp(GmDecompChol);

////////////////////////////////////////////////////////////////////////////////
/// Constructor for ([row_lwb..row_upb] x [row_lwb..row_upb]) matrix

GmDecompChol::GmDecompChol(Int_t row_lwb,Int_t row_upb)
{
   const Int_t nrows = row_upb-row_lwb+1;
   fU.ResizeTo(row_lwb,row_lwb+nrows-1,row_lwb,row_lwb+nrows-1);
   fU[0][0] = 0.9;
  G4cout << " FU " << fU[0][0] <<" " << fU[0][1] <<" " << fU[1][0] <<" " << fU[1][1] <<  G4endl; 
   fU[1][1] = 0.8;
  G4cout << " FU " << fU[0][0] <<" " << fU[0][1] <<" " << fU[1][0] <<" " << fU[1][1] <<  G4endl; 
   fU[0][1] = sqrt(fU[0][1]*fU[1][0]*0.9);
  G4cout << " FU " << fU[0][0] <<" " << fU[0][1] <<" " << fU[1][0] <<" " << fU[1][1] <<  G4endl; 
   fU[1][0] = sqrt(fU[0][1]*fU[1][0]*0.9);
  G4cout << " FU " << fU[0][0] <<" " << fU[0][1] <<" " << fU[1][0] <<" " << fU[1][1] <<  G4endl; 
   Decompose();		   
}


////////////////////////////////////////////////////////////////////////////////
/// Matrix A is decomposed in component U so that A = U^T * U
/// If the decomposition succeeds, bit kDecomposed is set , otherwise kSingular

Bool_t GmDecompChol::Decompose()
{
  G4cout << " FU " << fU[0][0] <<" " << fU[0][1] <<" " << fU[1][0] <<" " << fU[1][1] <<  G4endl; 
   Int_t i,j,icol,irow;
   const Int_t     n  = fU.GetNrows();
         Double_t *pU = fU.GetMatrixArray();
	 pU[0] = 0.9;
	 pU[3] = 0.8;
	 pU[1] =  sqrt(pU[0]*pU[3]*0.9);
	 pU[2] =  sqrt(pU[0]*pU[3]*0.9);
	 G4cout << " PU " << pU[0]<<" " << pU[1] <<" " << pU[2] <<" " << pU[3] <<  G4endl; 
   for (icol = 0; icol < n; icol++) {
      const Int_t rowOff = icol*n;

      //Compute fU(j,j) and test for non-positive-definiteness.
      Double_t ujj = pU[rowOff+icol];
      G4cout << icol << " UJJ " << ujj << " " <<rowOff << " " << icol<<G4endl; 
      for (irow = 0; irow < icol; irow++) {
         const Int_t pos_ij = irow*n+icol;
         ujj -= pU[pos_ij]*pU[pos_ij];
	 G4cout << icol<<":"<<irow<<" -UJJ " << ujj << " " << pos_ij << " " << icol<<G4endl; 
      }
      if (ujj <= 0) {
         Error("Decompose()","matrix not positive definite");
         return kFALSE;
      }
      ujj = TMath::Sqrt(ujj);
      pU[rowOff+icol] = ujj;
      G4cout << " pU[ " << rowOff+icol << " " << ujj << icol<< G4endl; 

      if (icol < n-1) {
         for (j = icol+1; j < n; j++) {
            for (i = 0; i < icol; i++) {
               const Int_t rowOff2 = i*n;
	       G4cout << icol<<":"<<i<<":"<<j<< "=" << rowOff2+j <<" "<< pU[rowOff2+j] << "*" <<rowOff2+icol << pU[rowOff2+icol] << G4endl;
               pU[rowOff+j] -= pU[rowOff2+j]*pU[rowOff2+icol];
	       G4cout << icol<<":"<<i<<":"<<j<<" 2pU[ " << rowOff<<"+"<<j << "="<<pU[rowOff+j] << G4endl;
            }
         }
         for (j = icol+1; j < n; j++)
            pU[rowOff+j] /= ujj;
	 G4cout << icol<<":"<<j<<" 3pU[ " << rowOff<<"+"<<j << "=" <<pU[rowOff+j] << " / " << ujj << G4endl;
      }
   }

   for (irow = 0; irow < n; irow++) {
      const Int_t rowOff = irow*n;
      for (icol = 0; icol < irow; icol++)
         pU[rowOff+icol] = 0.;
   }

   G4cout << " FINAL PU " << pU[0]<<" " << pU[1] <<" " << pU[2] <<" " << pU[3] <<  G4endl; 

   return TRUE;
}

