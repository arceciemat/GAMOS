#ifndef GmVNumericDistribution_h
#define GmVNumericDistribution_h 1

#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"

class GmVNumericDistribution : public GmVDistribution
{
  
public: 
  GmVNumericDistribution(G4String name);
  virtual ~GmVNumericDistribution(){};

  virtual void BuildData();
  virtual void ReadFile();

  virtual G4double GetValueFromStep(const G4Step* aStep);
  virtual G4double GetValueFromTrack(const G4Track* aTrack);
  virtual G4double GetNumericValueFromIndex(const G4double indexVal);

  virtual G4double InterpolateValue(const G4double& indexVal, const G4double& indexDown, const G4double& indexUp, const G4double& valDown, const G4double& valUp ) = 0;

  virtual void ReadFileUnknown( G4String& fileName );

  virtual void Normalize();

  G4int GetFileType( G4String& fileName );
  std::map<G4double,G4double> GetValues() const {
    return theValues;
  }

protected:
  void CheckOtherFileNames( G4String& fileNameROOT, G4String& fileNameCSV, G4String& fileNameText, G4String& fileName, G4int fNumber );
  void ReadFileName( G4String& fileName );
#ifndef GAMOS_NO_ROOT
  void ReadFileROOT( G4String& fileName );
#endif
  void ReadFileCSV( G4String& fileName );
  void ReadFileText( G4String& fileName );

protected:
  
  std::map<G4double,G4double> theValues;

  G4double theMinimum;
  G4double theMaximum;

  G4bool bAllowOutOfLimits;

};

#endif
