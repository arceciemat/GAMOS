#define _USE_MATH_DEFINES
#include <cmath>
#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistPositionDirection2DCorrelTripleGaussian.hh"
#include "GmGenerDistPositionDirection2DCorrelGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GmGeneratorMgr.hh"

#include "G4ThreeVector.hh"
#include "CLHEP/Random/RandFlat.h"

#ifdef HISTOGRAMS
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#endif
G4bool GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms = false;

//---------------------------------------------------------------------
GmGenerDistPositionDirection2DCorrelTripleGaussian::GmGenerDistPositionDirection2DCorrelTripleGaussian()
{
  theCentre = G4ThreeVector(0., 0., 0.);

#ifdef HISTOGRAMS
  bHistogramsHere = true;
  if( !GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms ) {
    bHistogramsHere = false;
  }
  if( GmParameterMgr::GetInstance()->GetNumericValue("GmGenerDistPositionDirection2DCorrelTripleGaussian:Histograms",1) == 0 ) {
    bHistogramsHere = false; 
    bHistograms = false;
  }
  //  G4cout << this << " CREATE bHistogramsHere " << bHistogramsHere << " " << GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms << G4endl; //GDEB
#endif

  theConfFileNamePrefix = GmParameterMgr::GetInstance()->GetStringValue("GmGenerDistPositionDirection2DCorrelTripleGaussian:ConfFileNamePrefix","conf3E");

  //GDEBTG
  theGenerMgr = GmGeneratorMgr::GetInstance();
	
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirection2DCorrelTripleGaussian::GeneratePosition( GmParticleSource* partSource )
{
  //  int iX = theProbabilitiesAccumX.lower_bound(CLHEP::RandFlat::shoot()) - theProbabilitiesAccumX.begin() + 1;
  // int iY = theProbabilitiesAccumY.lower_bound(CLHEP::RandFlat::shoot()) - theProbabilitiesAccumY.begin() + 1;
  G4double ranX = CLHEP::RandFlat::shoot();
  int iX = std::lower_bound(theProbabilitiesAccumX.begin(),theProbabilitiesAccumX.end(),ranX) - theProbabilitiesAccumX.begin() + 1;
  //  G4cout << "GmGenerDistPositionDirection2DCorrelTripleGaussian::GeneratePosition " << iX << " ranX " << ranX << "  theProbabilitiesAccumX " << theProbabilitiesAccumX[0] << " " << theProbabilitiesAccumX[1] << " " << theProbabilitiesAccumX[2] << G4endl; //GDEB
  G4double ranY = CLHEP::RandFlat::shoot();
  int iY = std::lower_bound(theProbabilitiesAccumY.begin(),theProbabilitiesAccumY.end(),ranY) - theProbabilitiesAccumY.begin() + 1; 
  //  G4cout << "GmGenerDistPositionDirection2DCorrelTripleGaussian::GeneratePosition " << iY << " ranY " << ranY << "  theProbabilitiesAccumY " << theProbabilitiesAccumY[0] << " " << theProbabilitiesAccumY[1] << " " << theProbabilitiesAccumY[2] << G4endl; //GDEB
  //  G4cout << " SAMPLED GAUSSIAN " << iX << iY << G4endl; //GDEB
  G4cout << this << " GAUSSIANran " << iX << iY << " " << ranX << " " << ranY << "PROB " << theProbabilitiesAccumX[iX-1] << " " << theProbabilitiesAccumY[iY-1] << G4endl; //GDEB
  if( iX == 1 && iY == 1 ) {
    thePosition = theCorrelGaussianX1Y1->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX1Y1->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(11);
  } else if( iX == 1 && iY == 2 ) {
    thePosition = theCorrelGaussianX1Y2->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX1Y2->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(12);
  } else if( iX == 1 && iY == 3 ) {
    thePosition = theCorrelGaussianX1Y3->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX1Y3->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(13);
  } else if( iX == 2 && iY == 1 ) {
    thePosition = theCorrelGaussianX2Y1->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX2Y1->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(21);
  } else if( iX == 2 && iY == 2 ) {
    thePosition = theCorrelGaussianX2Y2->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX2Y2->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(22);
  } else if( iX == 2 && iY == 3 ) {
    thePosition = theCorrelGaussianX2Y3->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX2Y3->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(23);
  } else if( iX == 3 && iY == 1 ) {
    thePosition = theCorrelGaussianX3Y1->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX3Y1->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(31);
  } else if( iX == 3 && iY == 2 ) {
    thePosition = theCorrelGaussianX3Y2->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX3Y2->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(32);
  } else if( iX == 3 && iY == 3 ) {
    thePosition = theCorrelGaussianX3Y3->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX3Y3->GetDirection();
    theGenerMgr->SetIdXYTripleGaussian(33);
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::GeneratePosition POS= " << thePosition << " DIR= " << theDirection << " " << iX << " " << iY << G4endl;
#endif
  thePosition = theRotation * thePosition;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::GeneratePosition POS_afterROT= " << thePosition << G4endl;
#endif

#ifdef HISTOGRAMS
  //  G4cout << this <<" " << bHistogramsHere <<" FILL GmGenerDistPositionDirecGmGenerDistPositionDirection2DCorrelTripleGaussiantion2DCorrelTripleGaussian::bHistograms = " << GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms << G4endl; //GDEB
  if( bHistogramsHere ) {
    G4double posX = thePosition.x();
    G4double posY = thePosition.y();
    G4double dirX = theDirection.x();
    G4double dirY = theDirection.y();
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
G4ThreeVector GmGenerDistPositionDirection2DCorrelTripleGaussian::GenerateDirection( GmParticleSource* partSource )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << partSource << " GmGenerDistPositionDirection2DCorrelTripleGaussian::GenerateDirection " << partSource->GetDirection() << G4endl;
#endif
  
  return partSource->GetDirection();

}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams( const std::vector<G4String>& params )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(-debugVerb) ) {
    G4cout << this << " GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams ";
    for( int ii = 0 ; ii < params.size(); ii++ ) {
      G4cout << ii << " PARAM=" << params[ii] << G4endl;

    }
  }
#endif
  
  if( params.size() != 25 && params.size() != 28 ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("N parameters "+GmGenUtils::itoa(params.size())+"  You have to set the following 25 (28) parameters: SIGMA_X1 SIGMA_DIR_X1 CORRELATION_X1 SIGMA_Y1 SIGMA_DIR_Y1 CORRELATION_Y1 PHASE_XY1  SIGMA_X2 SIGMA_DIR_X2 CORRELATION_X2 SIGMA_Y2 SIGMA_DIR_Y2 CORRELATION_Y2 PHASE_XY2  SIGMA_X3 SIGMA_DIR_X3 CORRELATION_X3 SIGMA_Y3 SIGMA_DIR_Y3 CORRELATION_Y3 PHASE_XY3  CORRELATION_X_1 CORRELATION_Y_1 CORRELATION_X_2 CORRELATION_Y_2 (CENTRE_X CENTRE_Y CENTRE_Z)  (DIR_X DIR_Y DIR_Z)").c_str());
  }
  theSigmaX1 = GmGenUtils::GetValue(params[0]);
  theSigmaXp1 = GmGenUtils::GetValue(params[1]);
  theCorrelationX1 = GmGenUtils::GetValue(params[2]);
  theSigmaY1 = GmGenUtils::GetValue(params[3]);
  theSigmaYp1 = GmGenUtils::GetValue(params[4]);
  theCorrelationY1 = GmGenUtils::GetValue(params[5]);
  if( fabs(theCorrelationX1) > 1. || fabs(theCorrelationY1) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Correlations cannot be bigger than 1, you are using: correlationX1= "+GmGenUtils::ftoa(theCorrelationX1)+" correlationY1= "+GmGenUtils::ftoa(theCorrelationY1)).c_str());
  }
  if( fabs(theSigmaXp1) > 1. || fabs(theSigmaYp1) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Direction sigmas cannot be bigger than 1, you are using: sigma_directionX= "+GmGenUtils::ftoa(theSigmaXp1)+" sigma_directionY= "+GmGenUtils::ftoa(theSigmaYp1)).c_str());
  }
  theXYPhase1 = GmGenUtils::GetValue(params[6]);
  
  theSigmaX2 = GmGenUtils::GetValue(params[7]);
  theSigmaXp2 = GmGenUtils::GetValue(params[8]);
  theCorrelationX2 = GmGenUtils::GetValue(params[9]);
  theSigmaY2 = GmGenUtils::GetValue(params[10]);
  theSigmaYp2 = GmGenUtils::GetValue(params[11]);
  theCorrelationY2 = GmGenUtils::GetValue(params[12]);
  if( fabs(theCorrelationX2) > 1. || fabs(theCorrelationY2) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Correlations cannot be bigger than 1, you are using: correlationX2= "+GmGenUtils::ftoa(theCorrelationX2)+" correlationY2= "+GmGenUtils::ftoa(theCorrelationY2)).c_str());
  }
  if( fabs(theSigmaXp2) > 1. || fabs(theSigmaYp2) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Direction sigmas cannot be bigger than 1, you are using: sigma_directionX= "+GmGenUtils::ftoa(theSigmaXp2)+" sigma_directionY= "+GmGenUtils::ftoa(theSigmaYp2)).c_str());
  }
  theXYPhase2 = GmGenUtils::GetValue(params[13]);

  theSigmaX3 = GmGenUtils::GetValue(params[14]);
  theSigmaXp3 = GmGenUtils::GetValue(params[15]);
  theCorrelationX3 = GmGenUtils::GetValue(params[16]);
  theSigmaY3 = GmGenUtils::GetValue(params[17]);
  theSigmaYp3 = GmGenUtils::GetValue(params[18]);
  theCorrelationY3 = GmGenUtils::GetValue(params[19]);
  if( fabs(theCorrelationX3) > 1. || fabs(theCorrelationY3) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Correlations cannot be bigger than 1, you are using: correlationX3= "+GmGenUtils::ftoa(theCorrelationX3)+" correlationY3= "+GmGenUtils::ftoa(theCorrelationY3)).c_str());
  }
  if( fabs(theSigmaXp3) > 1. || fabs(theSigmaYp3) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Direction sigmas cannot be bigger than 1, you are using: sigma_directionX= "+GmGenUtils::ftoa(theSigmaXp3)+" sigma_directionY= "+GmGenUtils::ftoa(theSigmaYp3)).c_str());
  }
  theXYPhase3 = GmGenUtils::GetValue(params[20]);

  theProbabilityX1 = GmGenUtils::GetValue(params[21]);
  theProbabilityY1 = GmGenUtils::GetValue(params[22]);
  theProbabilityX2 = GmGenUtils::GetValue(params[23]);
  theProbabilityY2 = GmGenUtils::GetValue(params[24]);
  theProbabilityX3 = 1.-theProbabilityX1-theProbabilityX2;
  theProbabilityY3 = 1.-theProbabilityY1-theProbabilityY2;
  theProbabilitiesAccumX.clear();
  theProbabilitiesAccumY.clear();
  theProbabilitiesAccumX.push_back(theProbabilityX1);
  for( size_t ii = 0 ; ii < theProbabilitiesAccumX.size(); ii++ ) {
    G4cout << ii << " " << theProbabilityX1 << "  1theProbabilitiesAccumX " << theProbabilitiesAccumX[ii] << G4endl;
  }
  theProbabilitiesAccumX.push_back(theProbabilityX1 + theProbabilityX2);
  theProbabilitiesAccumX.push_back(theProbabilityX1 + theProbabilityX2 + theProbabilityX3);
  theProbabilitiesAccumY.push_back(theProbabilityY1);
  theProbabilitiesAccumY.push_back(theProbabilityY1 + theProbabilityY2);
  theProbabilitiesAccumY.push_back(theProbabilityY1 + theProbabilityY2 + theProbabilityY3);
  G4cout << "PROBS X " << theProbabilityX1 << " " << theProbabilityX2 << " " << theProbabilityX3 << G4endl; 
  for( size_t ii = 0 ; ii < theProbabilitiesAccumX.size(); ii++ ) {
    G4cout << ii << " " << " theProbabilitiesAccumX " << theProbabilitiesAccumX[ii] << G4endl;
  }
  for( double prob : theProbabilitiesAccumX ) {
    G4cout << " PROBACCUM X " << prob << G4endl;
  }
    G4cout << "PROBS Y " << theProbabilityY1 << " " << theProbabilityY2 << " " << theProbabilityY3 << G4endl; 
  for( double prob : theProbabilitiesAccumY ) {
    G4cout << " PROBACCUM Y " << prob << G4endl;
  }
  if( params.size() >= 28 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue(params[25]), GmGenUtils::GetValue(params[26]), GmGenUtils::GetValue(params[27]) );
  } else {
    theCentre = G4ThreeVector();
  }

  if( params.size() == 31 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[28] ), GmGenUtils::GetValue( params[29] ), GmGenUtils::GetValue( params[30] ) );
    theDirection = dir;
  } else {
    theDirection = G4ThreeVector(0.,0.,-1.);
  }

  std::vector<G4String> paramsIJ;

  theCorrelGaussianX1Y1 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp1));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp1));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY1));
  paramsIJ.push_back(GmGenUtils::ftoa(theXYPhase1)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX1Y1->SetParams(paramsIJ);
  paramsIJ.clear();
  
  theCorrelGaussianX1Y2 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp1));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp2));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY2));
  paramsIJ.push_back(GmGenUtils::ftoa((theXYPhase1+theXYPhase2)/2.)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX1Y2->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX1Y3 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp1));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp3));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY3));
  paramsIJ.push_back(GmGenUtils::ftoa((theXYPhase1+theXYPhase3)/2.)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX1Y3->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX2Y1 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp2));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp1));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY1));
  paramsIJ.push_back(GmGenUtils::ftoa((theXYPhase2+theXYPhase1)/2.)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX2Y1->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX2Y2 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp2));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp2));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY2));
  paramsIJ.push_back(GmGenUtils::ftoa(theXYPhase2)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX2Y2->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX2Y3 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp2));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp3));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY3));
  paramsIJ.push_back(GmGenUtils::ftoa((theXYPhase2+theXYPhase3)/2.)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX2Y3->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX3Y1 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp3));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY1));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp1));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY1));
  paramsIJ.push_back(GmGenUtils::ftoa((theXYPhase3+theXYPhase1)/2.)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX3Y1->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX3Y2 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp3));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY2));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp2));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY2));
  paramsIJ.push_back(GmGenUtils::ftoa((theXYPhase3+theXYPhase2)/2.)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX3Y2->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX3Y3 = new GmGenerDistPositionDirection2DCorrelGaussian();
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaX3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaXp3));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationX3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaY3));
  paramsIJ.push_back(GmGenUtils::ftoa(theSigmaYp3));
  paramsIJ.push_back(GmGenUtils::ftoa(theCorrelationY3));
  paramsIJ.push_back(GmGenUtils::ftoa((theXYPhase3+theXYPhase3)/2.)); // use XY phase averaged
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX3Y3->SetParams(paramsIJ);
  paramsIJ.clear();

  //  G4cout << " theCorrelGaussianXY " << theCorrelGaussianX1Y1 << " "  << theCorrelGaussianX1Y2 << " "  << theCorrelGaussianX1Y3 << " "  << theCorrelGaussianX2Y1 << " "  << theCorrelGaussianX2Y2 << " "  << theCorrelGaussianX2Y3 << " "  << theCorrelGaussianX3Y1 << " "  << theCorrelGaussianX3Y2 << " "  << theCorrelGaussianX3Y3 << G4endl; //GDEB
#ifdef HISTOGRAMS
  if( GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms ) {
    std::vector<GmVFilter*> filters;
    SetHistoNameAndNumber("PosDirGauss2D", filters, nullptr);
    G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
    // Create histogram for position and direction
    G4double nSigmas = 5.;
    G4double sigmaX = std::max(theSigmaX1,theSigmaX2);
    G4double sigmaXp = std::max(theSigmaXp1,theSigmaXp2);
    G4double sigmaY = std::max(theSigmaY1,theSigmaY2);
    G4double sigmaYp = std::max(theSigmaYp1,theSigmaYp2);
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Position X",100,-sigmaX*nSigmas,sigmaX*nSigmas,theHistoNumber+10);
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Direction X",100,-sigmaXp*nSigmas,sigmaXp*nSigmas,theHistoNumber+11);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position vs Direction X",100,-sigmaX*nSigmas,sigmaX*nSigmas,100,-sigmaXp*nSigmas,sigmaXp*nSigmas,theHistoNumber+12);
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Position Y",100,-sigmaY*nSigmas,sigmaY*nSigmas,theHistoNumber+20);
    theAnaMgr->CreateHisto1D(theHistoName+""+sepa+" Direction Y",100,-sigmaYp*nSigmas,sigmaYp*nSigmas,theHistoNumber+21);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position vs Direction Y",100,-sigmaY*nSigmas,sigmaY*nSigmas,100,-sigmaYp*nSigmas,sigmaYp*nSigmas,theHistoNumber+22);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Position XY",100,-sigmaX*nSigmas,sigmaX*nSigmas,100,-sigmaY*nSigmas,sigmaY*nSigmas,theHistoNumber+101);
    theAnaMgr->CreateHisto2D(theHistoName+""+sepa+" Direction XY",100,-sigmaXp*nSigmas,sigmaXp*nSigmas,100,-sigmaYp*nSigmas,sigmaYp*nSigmas,theHistoNumber+102);
  }
  //  G4cout << this << " GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms = " << GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms << G4endl; //GDEB
  GmGenerDistPositionDirection2DCorrelTripleGaussian::bHistograms = false;
#endif
  

}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParamsEnergy( G4String energy )
{
  // try all options conf3E90 conf3E090 conf3E90.0 conf3E090.0
  G4bool bFileOK = true;
  G4String energyFinal = energy;
  if(FILE *file = fopen((theConfFileNamePrefix+energy).c_str(), "r")) {
    fclose(file);
    bFileOK = true;
  } else {
    G4cout << " FILE NOT FOUND " << theConfFileNamePrefix+energy << " trying with " << theConfFileNamePrefix+"0"+energy << G4endl; //GDEB
    bFileOK = false;    
  }
  if (!bFileOK ) {
    if (FILE *file = fopen((theConfFileNamePrefix+"0"+energy).c_str(), "r")) {
      fclose(file);
      bFileOK = true;
      energyFinal = "0"+energy; // conf3E99.9 -> conf3E099.9
    } else {
      G4cout << " FILE NOT FOUND " << theConfFileNamePrefix+"0"+energy << " trying with " << theConfFileNamePrefix+energy+".0" << G4endl; //GDEB
      bFileOK = false;
    }
  }
  if (!bFileOK ) {
    if (FILE *file = fopen((theConfFileNamePrefix+energy+".0").c_str(), "r")) {
      fclose(file);
      bFileOK = true;
      energyFinal = energy+".0";  // conf3E148. -> conf3E148.0
    } else {
      G4cout << " FILE NOT FOUND " << theConfFileNamePrefix+"0"+energy << " trying with " << theConfFileNamePrefix+"0"+energy+".0" << G4endl; //GDEB
      bFileOK = false;
    }
  }
  if (!bFileOK ) {
    energyFinal = "0"+energy+".0";  // conf3E74. -> conf3E074.0
  } 
  GmFileIn fin = GmFileIn::GetInstance(theConfFileNamePrefix+energyFinal,true);
  G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParamsEnergy " << theConfFileNamePrefix+energy << G4endl; //GDEB
  
  std::vector<G4String> params;
  fin.GetWordsInLine(params);
  fin.Close();
  //  G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParamsEnergy NPARAM " << params.size() << G4endl; //GDEB

  SetParams(params);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) {
    G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::SetParamsEnergy " << G4endl;
    for(size_t ii = 0; ii < params.size(); ii++ ) {
      G4cout << params[ii] << " ";
    }
    G4cout << G4endl;
  }
#endif
}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelTripleGaussian::SetDirection( G4ThreeVector dir )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::SetDirection " << dir << G4endl;
#endif
  theCorrelGaussianX1Y1->SetDirection(dir);
  theCorrelGaussianX1Y2->SetDirection(dir);
  theCorrelGaussianX2Y1->SetDirection(dir);
  theCorrelGaussianX2Y2->SetDirection(dir);
  theCorrelGaussianX1Y3->SetDirection(dir);
  theCorrelGaussianX2Y3->SetDirection(dir);
  theCorrelGaussianX3Y1->SetDirection(dir);
  theCorrelGaussianX3Y2->SetDirection(dir);
  theCorrelGaussianX3Y3->SetDirection(dir);

  //  G4cout << " theCorrelGaussianX1Y1->SetDirection(dir); " << theCorrelGaussianX1Y1 << " " << theCorrelGaussianX1Y1->GetDirection() << G4endl; //GDEB
  //--- SetDirection of Position Dist (the one that calculates direction)
  GmVGenerDistDirection* dirDist = dynamic_cast<GmVGenerDistDirection*>(this);
  if( dirDist && dirDist->GetParticleSource() ) {
    GmVGenerDistPosition* posDist = dirDist->GetParticleSource()->GetPositionDistribution();
    GmGenerDistPositionDirection2DCorrelTripleGaussian* posDistDG = dynamic_cast<GmGenerDistPositionDirection2DCorrelTripleGaussian*>(posDist);
    posDistDG->SetDirection(dir);
  }

}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelTripleGaussian::SetRotation( G4RotationMatrix& rotm )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::SetRotation " << rotm << G4endl;
#endif
  theCorrelGaussianX1Y1->SetRotation(rotm);
  theCorrelGaussianX1Y2->SetRotation(rotm);
  theCorrelGaussianX2Y1->SetRotation(rotm);
  theCorrelGaussianX2Y2->SetRotation(rotm);
  theCorrelGaussianX1Y3->SetRotation(rotm);
  theCorrelGaussianX2Y3->SetRotation(rotm);
  theCorrelGaussianX3Y1->SetRotation(rotm);
  theCorrelGaussianX3Y2->SetRotation(rotm);
  theCorrelGaussianX3Y3->SetRotation(rotm);

  //  G4cout << " theCorrelGaussianX1Y1->SetRotation(rotm); " << theCorrelGaussianX1Y1 << " " << theCorrelGaussianX1Y1->GetRotation() << G4endl; //GDEB
  //--- SetDirection of Position Dist (the one that calculates direction)
  GmVGenerDistDirection* dirDist = dynamic_cast<GmVGenerDistDirection*>(this);
  if( dirDist && dirDist->GetParticleSource() ) {
    GmVGenerDistPosition* posDist = dirDist->GetParticleSource()->GetPositionDistribution();
    GmGenerDistPositionDirection2DCorrelTripleGaussian* posDistDG = dynamic_cast<GmGenerDistPositionDirection2DCorrelTripleGaussian*>(posDist);
    posDistDG->SetRotation(rotm);
  }

}



//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelTripleGaussian::SetCentre( G4ThreeVector pos )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelTripleGaussian::SetCentre " << pos << G4endl;
#endif
  theCorrelGaussianX1Y1->SetCentre(pos);
  theCorrelGaussianX1Y2->SetCentre(pos);
  theCorrelGaussianX2Y1->SetCentre(pos);
  theCorrelGaussianX2Y2->SetCentre(pos);
  theCorrelGaussianX1Y3->SetCentre(pos);
  theCorrelGaussianX2Y3->SetCentre(pos);
  theCorrelGaussianX3Y1->SetCentre(pos);
  theCorrelGaussianX3Y2->SetCentre(pos);
  theCorrelGaussianX3Y3->SetCentre(pos);
}
