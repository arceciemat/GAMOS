#ifndef GmGenerDistPositionDirection2DCorrelDoubleGaussian_HH
#define GmGenerDistPositionDirection2DCorrelDoubleGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition2D.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#define HISTOGRAMS
#ifdef HISTOGRAMS
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#endif
#include "G4ThreeVector.hh"
#include <vector>

class GmParticleSource;
class GmGenerDistPositionDirection2DCorrelGaussian;

class GmGenerDistPositionDirection2DCorrelDoubleGaussian : public GmVGenerDistPosition2D,
							   public GmVGenerDistDirection
#ifdef HISTOGRAMS
							 ,public GmVHistoBuilder
#endif
{
public:
  GmGenerDistPositionDirection2DCorrelDoubleGaussian();
  virtual ~GmGenerDistPositionDirection2DCorrelDoubleGaussian(){};

  virtual G4ThreeVector GeneratePosition( GmParticleSource* source );
  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetParamsEnergy( G4String energy );

  virtual void SetDirection( G4ThreeVector dir );
  virtual void SetCentre( G4ThreeVector pos );

private:
  
  G4double theSigmaX1;
  G4double theSigmaXp1;
  G4double theCorrelationX1;
  G4double theSigmaY1;
  G4double theSigmaYp1;
  G4double theCorrelationY1;
  G4double theXYPhase1;

  G4double theSigmaX2;
  G4double theSigmaXp2;
  G4double theCorrelationX2;
  G4double theSigmaY2;
  G4double theSigmaYp2;
  G4double theCorrelationY2;
  G4ThreeVector theCentre2;
  G4double theXYPhase2;

  G4double theProbabilityX12;
  G4double theProbabilityY12;
  
  G4ThreeVector thePosition;
  G4ThreeVector theDirection;
  static bool bHistograms;
  bool bHistogramsHere;

  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX1Y1;
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX1Y2;
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX2Y1;
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX2Y2;
  G4String theConfFileNamePrefix;
  
};

#endif
