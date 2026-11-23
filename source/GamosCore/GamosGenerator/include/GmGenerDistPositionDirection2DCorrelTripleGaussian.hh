#ifndef GmGenerDistPositionDirection2DCorrelTripleGaussian_HH
#define GmGenerDistPositionDirection2DCorrelTripleGaussian_HH

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
class GmGeneratorMgr;

class GmGenerDistPositionDirection2DCorrelTripleGaussian : public GmVGenerDistPosition2D,
							   public GmVGenerDistDirection
#ifdef HISTOGRAMS
							 ,public GmVHistoBuilder
#endif
{
public:
  GmGenerDistPositionDirection2DCorrelTripleGaussian();
  virtual ~GmGenerDistPositionDirection2DCorrelTripleGaussian(){};

  virtual G4ThreeVector GeneratePosition( GmParticleSource* source );
  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetParamsEnergy( G4String energy );

  virtual void SetDirection( G4ThreeVector dir );
  virtual void SetRotation( G4RotationMatrix& rotm );
  virtual void SetRotation( G4ThreeVector dir ){
    GmVGenerDistPosition2D::SetRotation(dir);
  }
 
  virtual void SetCentre( G4ThreeVector pos );

  std::vector<G4double> GetProbabilitiesAccumX() const {
    return theProbabilitiesAccumX;
  }

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

  G4double theSigmaX3;
  G4double theSigmaXp3;
  G4double theCorrelationX3;
  G4double theSigmaY3;
  G4double theSigmaYp3;
  G4double theCorrelationY3;
  G4ThreeVector theCentre3;
  G4double theXYPhase3;

  G4double theProbabilityX1;
  G4double theProbabilityY1;
  G4double theProbabilityX2;
  G4double theProbabilityY2;
  G4double theProbabilityX3;
  G4double theProbabilityY3;
  std::vector<G4double> theProbabilitiesAccumX;
  std::vector<G4double> theProbabilitiesAccumY;
  
  G4ThreeVector thePosition;
  G4ThreeVector theDirection;
  static bool bHistograms;
  bool bHistogramsHere;

  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX1Y1;
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX1Y2;
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX2Y1;
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX2Y2;
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX1Y3; 
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX2Y3; 
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX3Y1; 
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX3Y2; 
  GmGenerDistPositionDirection2DCorrelGaussian* theCorrelGaussianX3Y3; 

  G4String theConfFileNamePrefix;

  GmGeneratorMgr* theGenerMgr;
};

#endif
