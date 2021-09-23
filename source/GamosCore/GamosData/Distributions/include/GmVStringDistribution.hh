#ifndef GmVStringDistribution_h
#define GmVStringDistribution_h 1

#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"

class GmVStringDistribution : public GmVDistribution
{
  
public:
  GmVStringDistribution();
  GmVStringDistribution(G4String name);
  virtual ~GmVStringDistribution(){};

  virtual void Initialize();
  virtual void ReadFile();
  virtual void BuildData();

  virtual void ReadFileText( G4String& fileName );

public:
  virtual G4double GetValueFromTrack(const G4Track* aTrack);
  virtual G4double GetStringValueFromIndex(const G4String& indexVal);

  virtual void Normalize();

protected: 
  GmVData* Build1StringData(const G4String& dataName);

protected:
std::map<G4String,G4double> theStringValues;

  G4bool bAllValuesDefined;
};

#endif
