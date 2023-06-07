#ifndef GmGenerDistPositionDirection2DCorrelGaussian_HH
#define GmGenerDistPositionDirection2DCorrelGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition2D.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#define HISTOGRAMS
#ifdef HISTOGRAMS
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#endif
#include "G4ThreeVector.hh"
#include <vector>

class GmParticleSource;

class GmGenerDistPositionDirection2DCorrelGaussian : public GmVGenerDistPosition2D,
					       public GmVGenerDistDirection
#ifdef HISTOGRAMS
					     ,public GmVHistoBuilder
#endif
{
public:
  GmGenerDistPositionDirection2DCorrelGaussian();
  virtual ~GmGenerDistPositionDirection2DCorrelGaussian(){};

  virtual G4ThreeVector GeneratePosition( GmParticleSource* source );
  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

  G4ThreeVector GetDirection() const {
    return theDirection;
  }

  virtual void SetDirection( G4ThreeVector dir );

private:
  //G4double DecomposeMatrixCholensky(G4double matrix[][100], int n);
  G4double** DecomposeMatrixCholensky( G4double** matrix, G4int dim );
  
  G4double theSigmaX;
  G4double theSigmaXp;
  G4double theCorrelationX;
  G4double theSigmaY;
  G4double theSigmaYp;
  G4double theCorrelationY;
  G4double theXYPhase;
  
  G4double** theMatrixX;
  G4double** theMatrixY;
  G4double** theTriangularMatrixX;
  G4double** theTriangularMatrixY;

  G4ThreeVector thePosition;
  G4ThreeVector theDirection;
  G4ThreeVector theAxisDir;
  G4ThreeVector thePerpDir;
  static bool bHistograms;
  bool bHistogramsHere;
};

#endif
