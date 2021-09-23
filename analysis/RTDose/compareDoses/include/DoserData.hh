#ifndef DoserData__HH
#define DoserData__HH
#include "globals.hh"
#include <vector>
#include <map>
#include <fstream>
#include "DoseData.hh"

typedef std::pair<double,double> pdd; // value, error
typedef std::map<double,pdd > mdpdd;  // energy - value, error
typedef std::pair<std::string,mdpdd* > psmdpdd; 
typedef std::vector< psmdpdd > vpsmdpdd; 

class DoserData 
{
public:
  DoserData();
  ~DoserData(){};

public:

  void AddDoseData(DoseData*);
  DoserData operator=( const DoserData& sd2 );
  DoserData operator+=( const DoserData& sd2 );
  DoserData & operator*=( G4double wei );
  bool operator!=( const DoserData& sd2 );

  virtual mdpdd* GetPlotData( G4bool bLogX, G4double multFactor = 1., G4double dispFactor = 0., G4double dispYFactor = 0.);  

  virtual void Print( std::ofstream& ){
    G4cerr << " ERROR Print" << G4endl;
    std::exception();
  };

  DoseData* GetDoseData(unsigned int) const;

  void SetNEvents(G4double nev);
  G4double GetNEvents() const {
    return theNEvents; 
  }

  void SetNDoses(G4double evts) {
    theNDoses = evts;
  }
  G4double GetNDoses() const {
    return theNDoses; // counts all doses except TOTAL
  }
  G4int GetDoseDataSize() const {
    return theDoseData.size(); // counts all doses
   }

  void SetName( G4String name ) {
    theName = name;
  }
  virtual G4String GetName() const{
    return theName;
  }

  G4int GetNumberOfFiles() const { 
    return theNumberOfFiles;
  }
  DoseType GetDoseType() const {
    return theDoseType;
  }
  
  virtual G4double GetIndexVal( G4String, G4bool ){
    G4cerr << "!!! ERROR GetIndexVal() " << G4endl;
    std::exception();
    return 0.;
  } 

  void InvertX();

  /* IN DoseInfo
  G4double GetAddFactor() const {
    return theAddFactor;
  }
  G4double GetMultFactor() const {
    return theMultFactor;
  }
  void SetAddFactor( G4double dispF ){
    theAddFactor = dispF;
  }
  void SetMultFactor( G4double multF ){
    theMultFactor = multF;
  }
  G4int GetIdxColor() const {
    return theIdxColor;
    }*/

protected:
  std::vector<DoseData*> theDoseData;
  G4String theName;
  G4double theNEvents;
  G4double theNDoses; // not counting the 'TOTAL' dose
  G4int theNumberOfFiles;
  DoseType theDoseType;
  //  G4double theAddFactor;
  //  G4double theMultFactor;
  //  G4int theIdxColor;
};

#endif
