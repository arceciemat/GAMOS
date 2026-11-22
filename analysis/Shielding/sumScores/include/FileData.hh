#ifndef FileData__HH
#define FileData__HH
#include <vector>
#include <fstream>
#include "globals.hh"

#include "ScorerData.hh"

class FileData 
{
public:
  FileData();
  // FileData(std::vector<G4String> wl);
  ~FileData(){};

public:
  virtual void Add( const FileData* fd2 );

  void AddScorerData(ScorerData* sd);

  virtual void Print( std::ofstream& fout ) = 0;
  
  virtual msmspdd GetPlotData( G4bool bLogX );

  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fName ) {
    theFileName = fName;  
  }

  size_t GetNumberOfScorers() const{
    return  theScorerData.size();
  }

  ScorerData* GetScorerData(unsigned int ii) const;
  
  std::vector<ScorerData*> GetAllScorerData() const {
    return theScorerData;
  }
  
  void SetNEvents( G4double nev ) {
    theNEvents = nev;
  }
  G4double GetNEvents() const {
    return theNEvents;
  }

private:
  G4bool IsConsistentLogX( mspdd* plotData );

protected:
  static G4double theNEventsSum;
  
protected:
  G4String theFileName;  
  G4double theNEvents;
  std::vector<ScorerData*> theScorerData;
};

#endif
