#ifndef GmFromTextFileSource_H
#define GmFromTextFileSource_H
//
#include "GmParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "GmGenerTextFileData.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class GmFromTextFileSource : public GmParticleSource
{
public:
  GmFromTextFileSource( const G4String& name, const G4String& fileName);
  GmFromTextFileSource( const G4String& name );
  ~GmFromTextFileSource();
  void Initialize();

  virtual std::vector<G4PrimaryVertex*> GenerateVertices( G4double time );
  virtual G4PrimaryVertex* GenerateVertex( G4double time );
  virtual G4double GenerateTime();
  virtual G4String GetParticleName() const {
    return theCurrentData.partName;
  }
  void SetRecycleFile( G4bool brf ) {
    bRecycleFile = brf;
  }

  void SetNLinesToSkip( G4int nLinesToSkip );

  void SetNEventsInFile( G4int nts );
  
  GmFileIn theFileIn;

private:
  G4bool ReadOneParticle();
  
  GenerTextFileData theCurrentData;
  G4int theNLinesRead ;
  G4int theNEventsRead ;
  G4double theLastReadTime;
  GenerTextFileData thePrevData;
  G4double theLastEventTime;
  G4bool bRecycleFile;
  G4int theNEventsToSkip;
  G4int theMaxNVertices;
  G4String theFileName;
  G4bool bInitialised;
  G4bool bRandomNEventsToSkip;
  G4int theNEventsInFile;
  
};

#endif // GmFromTextFileSource_H
