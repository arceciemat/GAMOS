#ifndef GmVNumericNDimDistribution_h
#define GmVNumericNDimDistribution_h 1
#include <set>

#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"

class GmVNumericNDimDistribution : public GmVDistribution
{
  
public: 
  GmVNumericNDimDistribution(G4String name);
  virtual ~GmVNumericNDimDistribution(){};
  virtual void Initialize();
  virtual void BuildData();
  virtual void ReadFile();
  virtual void ReadFileUnknown( G4String& fileName );

  virtual G4double GetValueFromStep(const G4Step* aStep);
  virtual G4double GetValueFromTrack(const G4Track* aTrack);
  virtual G4double GetNumericValueFromIndex(const std::vector<G4double> indexVals);

  virtual G4double InterpolateValue( const std::vector<std::vector<G4int> >& indices ) = 0;

  virtual void Normalize();
  G4int GetFileType( G4String& fileName );

protected:
  void CheckOtherFileNames( G4String& fileNameROOT, G4String& fileNameCSV, G4String& fileNameText, G4String& fileName, G4int fNumber );
#ifndef GAMOS_NO_ROOT
  virtual void ReadFileROOT( G4String& fileName );
#endif
  virtual void ReadFileCSV( G4String& fileName );
  virtual void ReadFileText( G4String& fileName );

protected:
  
  std::map<std::vector<G4int>,G4double> theValuesNDim;

  std::vector<G4double> theMinimumNDim;
  std::vector<G4double> theMaximumNDim;

  G4bool bAllowOutOfLimits;

  //----- New for NDim
  G4int theNDim;
  std::vector<GmVData*> theDataNDim;
  std::vector< std::set<G4double>* > theIndices; // keeps the values of the indices (each index appears only once)
 };

#endif
