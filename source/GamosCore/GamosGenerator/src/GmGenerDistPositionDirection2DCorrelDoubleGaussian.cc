#define _USE_MATH_DEFINES
#include <cmath>
#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistPositionDirection2DCorrelDoubleGaussian.hh"
#include "GmGenerDistPositionDirection2DCorrelGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#include "G4ThreeVector.hh"
#include "CLHEP/Random/RandFlat.h"

#ifdef HISTOGRAMS
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#endif
G4bool GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms = true;

//---------------------------------------------------------------------
GmGenerDistPositionDirection2DCorrelDoubleGaussian::GmGenerDistPositionDirection2DCorrelDoubleGaussian()
{
  theCentre = G4ThreeVector(0., 0., 0.);

#ifdef HISTOGRAMS
  bHistogramsHere = true;
  if( !GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms ) {
    bHistogramsHere = false;
  }
  if( GmParameterMgr::GetInstance()->GetNumericValue("GmGenerDistPositionDirection2DCorrelDoubleGaussian:Histos",0) == 0 ) {
    bHistogramsHere = false; //GDEB
    bHistograms = false; //GDEB
  }

  //  G4cout << this << " CREATE bHistogramsHere " << bHistogramsHere << " " << GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms << G4endl; //GDEB
#endif

  theConfFileNamePrefix = GmParameterMgr::GetInstance()->GetStringValue("GmGenerDistPositionDirection2DCorrelDoubleGaussian:ConfFileNamePrefix","confE");
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirection2DCorrelDoubleGaussian::GeneratePosition( GmParticleSource* partSource )
{
  G4bool bX1 = CLHEP::RandFlat::shoot() <= theProbabilityX12;
  G4bool bY1 = CLHEP::RandFlat::shoot() <= theProbabilityY12;
  if( bX1 && bY1 ) {
    thePosition = theCorrelGaussianX1Y1->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX1Y1->GetDirection();
  } else if( bX1 && !bY1 ) {
    thePosition = theCorrelGaussianX1Y2->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX1Y2->GetDirection();
  } else if( !bX1 && bY1 ) {
    thePosition = theCorrelGaussianX2Y1->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX2Y1->GetDirection();
  } else if( !bX1 && !bY1 ) {
    thePosition = theCorrelGaussianX2Y2->GeneratePosition(partSource);
    theDirection = theCorrelGaussianX2Y2->GetDirection();
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::GeneratePosition POS= " << thePosition << " DIR= " << theDirection << " " << bX1 << " " << bY1 << G4endl;
#endif
  //  ???  thePosition = theRotation * thePosition;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::GeneratePosition POS_afterROT= " << thePosition << G4endl;
#endif

#ifdef HISTOGRAMS
  //  G4cout << this <<" " << bHistogramsHere <<" FILL GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms = " << GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms << G4endl; //GDEB
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
G4ThreeVector GmGenerDistPositionDirection2DCorrelDoubleGaussian::GenerateDirection( GmParticleSource* partSource )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << partSource << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::GenerateDirection " << partSource->GetDirection() << G4endl;
#endif
  
  return partSource->GetDirection();

}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParams( const std::vector<G4String>& params )
{
  
  if( params.size() != 16 && params.size() != 19 && params.size() != 22 ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("N parameters "+GmGenUtils::itoa(params.size())+"  You have to set the following 16 (19) parameters: SIGMA_X1 SIGMA_DIR_X1 CORRELATION_X1 SIGMA_Y1 SIGMA_DIR_Y1 CORRELATION_Y1 PHASE_XY1  SIGMA_X2 SIGMA_DIR_X2 CORRELATION_X2 SIGMA_Y2 SIGMA_DIR_Y2 CORRELATION_Y2 PHASE_XY2 CORRELATION_X_12 CORRELATION_Y_12 (CENTRE_X CENTRE_Y CENTRE_Z)  (DIR_X DIR_Y DIR_Z)").c_str());
  }
  theSigmaX1 = GmGenUtils::GetValue(params[0]);
  theSigmaXp1 = GmGenUtils::GetValue(params[1]);
  theCorrelationX1 = GmGenUtils::GetValue(params[2]);
  theSigmaY1 = GmGenUtils::GetValue(params[3]);
  theSigmaYp1 = GmGenUtils::GetValue(params[4]);
  theCorrelationY1 = GmGenUtils::GetValue(params[5]);
  if( fabs(theCorrelationX1) > 1. || fabs(theCorrelationY1) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Correlations cannot be bigger than 1, you are using: correlationX1= "+GmGenUtils::ftoa(theCorrelationX1)+" correlationY1= "+GmGenUtils::ftoa(theCorrelationY1)).c_str());
  }
  if( fabs(theSigmaXp1) > 1. || fabs(theSigmaYp1) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParams",
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
    G4Exception(" GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Correlations cannot be bigger than 1, you are using: correlationX2= "+GmGenUtils::ftoa(theCorrelationX2)+" correlationY2= "+GmGenUtils::ftoa(theCorrelationY2)).c_str());
  }
  if( fabs(theSigmaXp2) > 1. || fabs(theSigmaYp2) > 1. ) {
    G4Exception(" GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		("Direction sigmas cannot be bigger than 1, you are using: sigma_directionX= "+GmGenUtils::ftoa(theSigmaXp2)+" sigma_directionY= "+GmGenUtils::ftoa(theSigmaYp2)).c_str());
  }

  theXYPhase2 = GmGenUtils::GetValue(params[13]);

  theProbabilityX12 = GmGenUtils::GetValue(params[14]);
  theProbabilityY12 = GmGenUtils::GetValue(params[15]);

  if( params.size() >= 19 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue(params[16]), GmGenUtils::GetValue(params[17]), GmGenUtils::GetValue(params[18]) );
  } else {
    theCentre = G4ThreeVector();
  }

  if( params.size() == 22 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[19] ), GmGenUtils::GetValue( params[20] ), GmGenUtils::GetValue( params[21] ) );
    theDirection = dir;
  } else {
    theDirection = G4ThreeVector(0.,0.,-1.);
  }

  std::vector<G4String> paramsIJ;

  theCorrelGaussianX1Y1 = new GmGenerDistPositionDirection2DCorrelGaussian();
  for( int ii = 0; ii < 7; ii++ ) {
    paramsIJ.push_back(params[ii]);
  }
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX1Y1->SetParams(paramsIJ);
  paramsIJ.clear();
  
  theCorrelGaussianX1Y2 = new GmGenerDistPositionDirection2DCorrelGaussian();
  for( int ii = 0; ii < 7; ii++ ) {
    int iip = ii;
    if( ii >= 3 ) {
      iip += 7;
    } 
    paramsIJ.push_back(params[iip]);
  }
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX1Y2->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX2Y1 = new GmGenerDistPositionDirection2DCorrelGaussian();
  for( int ii = 0; ii < 7; ii++ ) {
    int iip = ii;
    if( ii < 3 ) {
      iip += 7;
    } 
    paramsIJ.push_back(params[iip]);
  }
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX2Y1->SetParams(paramsIJ);
  paramsIJ.clear();

  theCorrelGaussianX2Y2 = new GmGenerDistPositionDirection2DCorrelGaussian();
  for( int ii = 0; ii < 7; ii++ ) {
    int iip = ii;
    iip += 7;
    paramsIJ.push_back(params[iip]);
  }
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theCentre.z()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.x()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.y()));
  paramsIJ.push_back(GmGenUtils::ftoa(theDirection.z()));
  theCorrelGaussianX2Y2->SetParams(paramsIJ);
  paramsIJ.clear();

  
#ifdef HISTOGRAMS
  if( GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms ) {
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
  //  G4cout << this << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms = " << GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms << G4endl; //GDEB
  GmGenerDistPositionDirection2DCorrelDoubleGaussian::bHistograms = false;
#endif
  

}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParamsEnergy( G4String energy )
{
  if (FILE *file = fopen((theConfFileNamePrefix+energy).c_str(), "r")) {
    fclose(file);
  } else {
    energy = "0"+energy;
  }
  GmFileIn fin = GmFileIn::GetInstance(theConfFileNamePrefix+energy,true);
  //  G4cout << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParamsEnergy " << theConfFileNamePrefix+energy << G4endl; //GDEB
  
  std::vector<G4String> params;
  fin.GetWordsInLine(params);
  fin.Close();
  // G4cout << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParamsEnergy NPARAM " << params.size() << G4endl; //GDEB

  SetParams(params);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) {
    G4cout << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetParamsEnergy " << G4endl;
    for(size_t ii = 0; ii < params.size(); ii++ ) {
      G4cout << params[ii] << " ";
    }
    G4cout << G4endl;
  }
#endif
}

//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetDirection( G4ThreeVector dir )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetDirection " << dir << G4endl;
#endif
  theCorrelGaussianX1Y1->SetDirection(dir);
  theCorrelGaussianX1Y2->SetDirection(dir);
  theCorrelGaussianX2Y1->SetDirection(dir);
  theCorrelGaussianX2Y2->SetDirection(dir);

  //--- SetDirection of Position Dist (the one that calculates direction)
  GmVGenerDistDirection* dirDist = dynamic_cast<GmVGenerDistDirection*>(this);
  if( dirDist && dirDist->GetParticleSource() ) {
    GmVGenerDistPosition* posDist = dirDist->GetParticleSource()->GetPositionDistribution();
    GmGenerDistPositionDirection2DCorrelDoubleGaussian* posDistDG = dynamic_cast<GmGenerDistPositionDirection2DCorrelDoubleGaussian*>(posDist);
    posDistDG->SetDirection(dir);
  }

}


//---------------------------------------------------------------------
void GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetCentre( G4ThreeVector pos )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << theCorrelGaussianX1Y1 << " GmGenerDistPositionDirection2DCorrelDoubleGaussian::SetCentre " << pos << G4endl;
#endif
  theCorrelGaussianX1Y1->SetCentre(pos);
  theCorrelGaussianX1Y2->SetCentre(pos);
  theCorrelGaussianX2Y1->SetCentre(pos);
  theCorrelGaussianX2Y2->SetCentre(pos);
}
