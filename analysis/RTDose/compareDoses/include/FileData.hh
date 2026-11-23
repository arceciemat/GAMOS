#ifndef FileData__HH
#define FileData__HH
#include <vector>
#include <fstream>
#include "globals.hh"

#include "DoserData.hh"

class FileData 
{
public:
  FileData();
  // FileData(std::vector<G4String> wl);
  ~FileData(){};

public:
  virtual void Add( const FileData* fd2 );

  void AddDoserData(DoserData* sd);

  virtual void Print( std::ofstream& fout ) = 0;
  
  virtual vpsmdpdd GetPlotData( G4bool bLogX );

  G4String GetFileName() const {
    return theFileName;
  }
  void SetFileName( G4String fName ) {
    theFileName = fName;  
  }

  size_t GetNumberOfDosers() const{
    return  theDoserData.size();
  }

  DoserData* GetDoserData(unsigned int ii) const;
  
  std::vector<DoserData*> GetAllDoserData() const {
    return theDoserData;
  }
  
  void SetNEvents( G4double nev ) {
    theNEvents = nev;
  }
  G4double GetNEvents() const {
    return theNEvents;
  }

  void SetInvertX( G4bool binv ) {
    bInvertX = binv; 
  }

private:
  G4bool IsConsistentLogX( mdpdd* plotData );

protected:
  static G4double theNEventsSum;
  
  G4String theFileName;  
  G4double theNEvents;
  std::vector<DoserData*> theDoserData;
  G4bool bInvertX; 
};

#endif
