#define _USE_MATH_DEFINES
#include <cmath>
#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistPositionDirection2DCorrelGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmPlane.hh"

#include "G4ThreeVector.hh"
#include "CLHEP/Random/RandGauss.h"

#ifdef HISTOGRAMS
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#endif
G4bool GmGenerDistPositionDirection2DCorrelGaussian::bHistograms = true;

//---------------------------------------------------------------------
GmGenerDistPositionDirection2DCorrelGaussian::GmGenerDistPositionDirection2DCorrelGaussian()
{
  theCentre = G4ThreeVector(0., 0., 0.);
  theDirection = G4ThreeVector(0., 0., 1.);
  theAxisDir = G4ThreeVector(0., 0., 1.);
  thePerpDir = G4ThreeVector(0., 1., 0.);
  
  bHistogramsHere = true;
  if( !GmGenerDistPositionDirection2DCorrelGaussian::bHistograms ) {
    bHistogramsHere = false;
  } 
  //  G4cout << this << " CREATE bHistogramsHere " << bHistogramsHere << " " << GmGenerDistPositionDirection2DCorrelGaussian::bHistograms << G4endl; //GDEB
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirection2DCorrelGaussian::GeneratePosition( GmParticleSource* partSource )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << this << " GmGenerDistPositionDirection2DCorrelGaussian::GeneratePosition Centre=" << theCentre << G4endl;
#endif
  G4double randX = CLHEP::RandGauss::shoot(0.,1.);
  G4double randXp = CLHEP::RandGauss::shoot(0.,1.); 
  //  randX = 2.;
  //  randXp = 1.;
  G4double posX = theTriangularMatrixX[0][0]*randX+theTriangularMatrixX[0][1]*randXp;
  G4double dirX = theTriangularMatrixX[1][0]*randX+theTriangularMatrixX[1][1]*randXp;
  G4double randY = CLHEP::RandGauss::shoot(0.,1.);;
  G4double randYp = CLHEP::RandGauss::shoot(0.,1.);
  G4double posY = theTriangularMatrixY[0][0]*randY+theTriangularMatrixY[0][1]*randYp;
  G4double dirY = theTriangularMatrixY[1][0]*randY+theTriangularMatrixY[1][1]*randYp;
  // G4cout << " POSITION " << thePosition << "  DIRECTION " << theDirection << G4endl;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << this << " GmGenerDistPositionDirection2DCorrelGaussian::GeneratePosition pos=" << posX << " " << posY << "   " <<theXYPhase/CLHEP::deg  << " TM= " << theTriangularMatrixX[0][0] << G4endl;
  if( GenerVerb(debugVerb) ) G4cout << this << " GmGenerDistPositionDirection2DCorrelGaussian::GenerateDirection dir=" << dirX << " " << dirY << " TM= " << theTriangularMatrixX[1][1] << G4endl;
#endif
  if( theXYPhase != 0. ) {
    G4double posXR = posX*cos(theXYPhase)+posY*sin(theXYPhase);
    G4double posYR = -posX*sin(theXYPhase)+posY*cos(theXYPhase);
    posX = posXR;
    posY = posYR;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelGaussian::GeneratePosition pos=" << posX << " " << posY << "   " << cos(theXYPhase) << " " << sin(theXYPhase)  << G4endl;
#endif
    G4double dirXR = dirX*cos(theXYPhase)+dirY*sin(theXYPhase);
    G4double dirYR = -dirX*sin(theXYPhase)+dirY*cos(theXYPhase);
    dirX = dirXR;
    dirY = dirYR;
  }
  thePosition = G4ThreeVector(posX,posY,0.)+theCentre;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelGaussian::GeneratePosition final pos=" << thePosition << " -center " << thePosition-theCentre << G4endl;
#endif

  //---- Rotate around theAxisDir
  G4ThreeVector newDir = theAxisDir; 
  G4double theta = asin(sqrt(dirX*dirX+dirY*dirY));
  newDir.rotate( theta, thePerpDir );
  G4double phi = atan2(dirY,dirX);
  newDir.rotate( phi , theAxisDir );    
  partSource->SetDirection(newDir);
  //  G4cout << this << "theAxisDir="<< theAxisDir << " " << theta << " " << phi << " GmGenerDistPositionDirection2DCorrelGaussian partSource direction " << partSource->GetDirection() << G4endl; //GDEB
  theDirection = newDir;
  
#ifdef HISTOGRAMS
  //  G4cout << this <<" " << bHistogramsHere <<" FILL GmGenerDistPositionDirection2DCorrelGaussian::bHistograms = " << GmGenerDistPositionDirection2DCorrelGaussian::bHistograms << G4endl; //GDEB
  if( bHistogramsHere ) {
    theAnaMgr->GetHisto1(theHistoNumber+10)->Fill( posX );
    theAnaMgr->GetHisto1(theHistoNumber+11)->Fill( dirX );
    theAnaMgr->GetHisto2(theHistoNumber+12)->Fill( posX, dirX );
    theAnaMgr->GetHisto1(theHistoNumber+20)->Fill( posY );
    theAnaMgr->GetHisto1(theHistoNumber+21)->Fill( dirY );
    theAnaMgr->GetHisto2(theHistoNumber+22)->Fill( posY, dirY );
    theAnaMgr->GetHisto2(theHistoNumber+101)->Fill( posX, posY );
    theAnaMgr->GetHisto2(theHistoNumber+102)->Fill( dirX, dirY );
  }
#endif
  
  return thePosition;
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirection2DCorrelGaussian::GenerateDirection( GmParticleSource* partSource )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelGaussian::GenerateDirection " << partSource->GetDirection() << G4endl;
#endif
  // Set in GeneratePosition()
  return partSource->GetDirection();

}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelGaussian::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 7 && params.size() != 10 && params.size() != 13 ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"You have to set the following parameters: SIGMA_X SIGMA_DIR_X CORRELATION_X SIGMA_Y SIGMA_DIR_Y CORRELATION_Y PHASE_XY (CENTRE_X CENTRE_Y CENTRE_Z) (DIR_X DIR_Y DIR_Z)");
  }
  theSigmaX = GmGenUtils::GetValue(params[0]);
  theSigmaXp = GmGenUtils::GetValue(params[1]);
  theCorrelationX = GmGenUtils::GetValue(params[2]);
  theSigmaY = GmGenUtils::GetValue(params[3]);
  theSigmaYp = GmGenUtils::GetValue(params[4]);
  theCorrelationY = GmGenUtils::GetValue(params[5]);
  if( fabs(theCorrelationX) > 1. || fabs(theCorrelationY) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Correlations cannot be bigger than 1, you are using: correlationX= "+GmGenUtils::ftoa(theCorrelationX)+" correlationY= "+GmGenUtils::ftoa(theCorrelationY)).c_str());
  }
  if( fabs(theSigmaXp) > 1. || fabs(theSigmaYp) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Direction sigmas cannot be bigger than 1, you are using: sigma_directionX= "+GmGenUtils::ftoa(theSigmaXp)+" sigma_directionY= "+GmGenUtils::ftoa(theSigmaYp)).c_str());
  }
  
  theXYPhase = GmGenUtils::GetValue(params[6]);
  
  if( params.size() >= 10 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue(params[7]), GmGenUtils::GetValue(params[8]), GmGenUtils::GetValue(params[9]) );
  } else {
    theCentre = G4ThreeVector();
  }
  
  if( params.size() == 13 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[10] ), GmGenUtils::GetValue( params[11] ), GmGenUtils::GetValue( params[12] ) );
    SetRotation( dir );
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << this << " GmGenerDistPositionDirection2DCorrelGaussian::SetParams X: "
				     << theSigmaX << " " << theSigmaXp << " " << theCorrelationX << " Y: " 
				     << theSigmaY << " " << theSigmaYp << " " << theCorrelationY << " PHASE: E"
				     << theXYPhase << G4endl;
#endif

  theMatrixX = new double *[2];
  theMatrixY = new double *[2];
  for(int ii = 0;ii < 2; ii++){
    theMatrixX[ii] = new double[2];
    theMatrixY[ii] = new double[2];
  }

  theMatrixX[0][0] = theSigmaX*theSigmaX;
  theMatrixX[0][1] = theSigmaX*theSigmaXp*theCorrelationX;
  theMatrixX[1][0] = theMatrixX[0][1];
  theMatrixX[1][1] = theSigmaXp*theSigmaXp;
  theMatrixY[0][0] = theSigmaY*theSigmaY;
  theMatrixY[0][1] = theSigmaY*theSigmaYp*theCorrelationY;
  theMatrixY[1][0] = theMatrixY[0][1];
  theMatrixY[1][1] = theSigmaYp*theSigmaYp;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelGaussian::SetParams theMatrixX: "
				     <<  theMatrixX[0][0] << " " << theMatrixX[0][1] << " | " <<  theMatrixX[1][0] << " " << theMatrixX[1][1] << " " 
				     <<  theMatrixY[0][0] << " " << theMatrixY[0][1] << " | " <<  theMatrixY[1][0] << " " << theMatrixY[1][1] << G4endl;
#endif
  theTriangularMatrixX = DecomposeMatrixCholensky( theMatrixX, 2 );
  theTriangularMatrixY = DecomposeMatrixCholensky( theMatrixY, 2 );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelGaussian::SetParams theTriangularMatrixX: "
				     <<  theTriangularMatrixX[0][0] << " " << theTriangularMatrixX[0][1] << " | " <<  theTriangularMatrixX[1][0] << " " << theTriangularMatrixX[1][1] << " " 
				     <<  theTriangularMatrixY[0][0] << " " << theTriangularMatrixY[0][1] << " | " <<  theTriangularMatrixY[1][0] << " " << theTriangularMatrixY[1][1] << G4endl;
#endif
  
#ifdef HISTOGRAMS
  if( GmGenerDistPositionDirection2DCorrelGaussian::bHistograms ) {
    std::vector<GmVFilter*> filters;
    SetHistoNameAndNumber("PosDirGauss2D", filters, nullptr);
    G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
    // Create histogram for position and direction
    G4double nSigmas = 5.;
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Position X",100,-theSigmaX*nSigmas,theSigmaX*nSigmas,theHistoNumber+10);
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Direction X",100,-theSigmaXp*nSigmas,theSigmaXp*nSigmas,theHistoNumber+11);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position vs Direction X",100,-theSigmaX*nSigmas,theSigmaX*nSigmas,100,-theSigmaXp*nSigmas,theSigmaXp*nSigmas,theHistoNumber+12);
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Position Y",100,-theSigmaY*nSigmas,theSigmaY*nSigmas,theHistoNumber+20);
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Direction Y",100,-theSigmaYp*nSigmas,theSigmaYp*nSigmas,theHistoNumber+21);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position vs Direction Y",100,-theSigmaY*nSigmas,theSigmaY*nSigmas,100,-theSigmaYp*nSigmas,theSigmaYp*nSigmas,theHistoNumber+22);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position XY",100,-theSigmaX*nSigmas,theSigmaX*nSigmas,100,-theSigmaY*nSigmas,theSigmaY*nSigmas,theHistoNumber+101);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Direction XY",100,-theSigmaXp*nSigmas,theSigmaXp*nSigmas,100,-theSigmaYp*nSigmas,theSigmaYp*nSigmas,theHistoNumber+102);
  }
  //  G4cout << this << " GmGenerDistPositionDirection2DCorrelGaussian::bHistograms = " << GmGenerDistPositionDirection2DCorrelGaussian::bHistograms << G4endl; //GDEB
  GmGenerDistPositionDirection2DCorrelGaussian::bHistograms = false;
#endif
  

}

////////////////////////////////////////////////////////////////////////////////
/// Matrix A is decomposed in component U so that A = U^T * U
/// If the decomposition succeeds, bit kDecomposed is set , otherwise kSingular
//---------------------------------------------------------------------
G4double** GmGenerDistPositionDirection2DCorrelGaussian::DecomposeMatrixCholensky( G4double** matrix, G4int dim )
// G4double GmGenerDistPositionDirection2DCorrelGaussian::DecomposeMatrixCholensky(G4double matrix[][100], int n)
{
  G4double** lowerMat = new double *[dim];
  for(int ii = 0;ii < dim; ii++){
    lowerMat[ii] = new double[dim];
  }
  

  // Decomposing a matrix into Lower Triangular
  for (int ii = 0; ii < dim; ii++) {
    for (int jj = 0; jj <= dim; jj++) {
      lowerMat[ii][jj] = 0.;
    }
  }

  for (int ii = 0; ii < dim; ii++) {
    for (int jj = 0; jj <= ii; jj++) {
      double sum = 0;
      
      if (jj == ii) { // summation for diagonals
	for (int kk = 0; kk < jj; kk++)
	  sum += pow(lowerMat[jj][kk], dim);
	lowerMat[jj][jj] = sqrt(matrix[jj][jj] - sum);
      } else {
	// Evaluating L(ii, jj) using L(jj, jj)
	for (int kk = 0; kk < jj; kk++)
	  sum += (lowerMat[ii][kk] * lowerMat[jj][kk]);
	lowerMat[ii][jj] = (matrix[ii][jj] - sum) /
	  lowerMat[jj][jj];
      }
    } 
  }
  
  // Displaying Lower Triangular and its Transpose
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) {
    G4cout << std::setw(6) << " Lower Triangular" 
	   << std::setw(30) << "Transpose" << G4endl;
    for (int ii = 0; ii < dim; ii++) {
      
      // Lower Triangular
      for (int jj = 0; jj < dim; jj++) {
	G4cout << std::setw(6) << lowerMat[ii][jj] << "\t";
      }
      G4cout << "\t";
      
      // Transpose of Lower Triangular
      for (int jj = 0; jj < dim; jj++) {
	G4cout << std::setw(6) << lowerMat[jj][ii] << "\t";
      }
      G4cout << G4endl;
    }
  }
#endif

  //CHECK
  G4double** checkMat = new double *[dim];
  for(int ii = 0;ii < dim; ii++) {
    checkMat[ii] = new double[dim];
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) {
    G4cout << " ORIGINAL MATRIX " << G4endl;
    for (int ii = 0; ii < dim; ii++) {
      for (int jj = 0; jj < dim; jj++) {
	G4cout << std::setw(6) << matrix[ii][jj] << "\t";
      }
      G4cout << G4endl;
    }
  }
#endif
  //  G4cout << " CHECK MATRIX " << G4endl;  
  for (int ii = 0; ii < dim; ii++) {
    for (int jj = 0; jj < dim; jj++) {
      checkMat[ii][jj] = 0.;
      for (int kk = 0; kk < dim; kk++) {
	checkMat[ii][jj] += lowerMat[ii][kk]*lowerMat[jj][kk];
      }
      //      G4cout << std::setw(6) << checkMat[ii][jj] << "\t";
    }
    //    G4cout << G4endl;
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) {
    G4cout << " ORIGINAL MATRIX - CHECK MATRIX" << G4endl;
    for (int ii = 0; ii < dim; ii++) {
      for (int jj = 0; jj < dim; jj++) {
	G4cout << std::setw(6) << matrix[ii][jj]-checkMat[ii][jj] << "\t";
      }
      G4cout << G4endl;
    }
  }
#endif

  for (int ii = 0; ii < dim; ii++) {
    for (int jj = 0; jj <= ii; jj++) {
      if( fabs(checkMat[ii][jj] - matrix[ii][jj]) > 1.e-6 ) {
	G4Exception(" GmGenerDistPositionDirection2DCorrelGaussian::DecomposeMatrixCholensky",
		    "",
		    FatalException,
		    "Cholensky decomposition not possible, check the positive-definiteness of your matrix");
      }
    }
  }
	
  return lowerMat;
}
 

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelGaussian::SetDirection( G4ThreeVector dir )
{
  theAxisDir = dir;
  G4ThreeVector dirPP(1.,0.,0.);
  if( fabs(fabs(theAxisDir*dirPP) - 1.) < 1.E-9 ){
    dirPP = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theAxisDir.cross(dirPP);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << this << " GmGenerDistPositionDirection2DCorrelGaussian::SetDirection theAxisDir " << theAxisDir << " thePerpDir " << thePerpDir << G4endl;
#endif

}
